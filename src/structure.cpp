#include "structure.h"
#include <filesystem/filemanager.h>
#include "idConverter.h"
#include <filesystem>
#include <numeric>
#include <string>
#include <vector>
#include "arrayDataType.h"
#include "blockData.h"
#include "itemID.h"
#include "chunkLoadLevel.h"
#include "constants.h"
#include "entityID.h"
#include "array/arraynd/arraynd.h"
#include "array/fastlist.h"
#include "globalFunctions.h"
#include "math/mattnxn.h"
#include "math/rectangle/rectangletn.h"
#include "math/vector/vectn.h"
#include "itemID.h"
#include "nbt/nbtCompound.h"
#include "nbt/nbtData.h"
#include "nbt/nbtDataTag.h"
#include "nbt/nbtSerializer.h"
#include "entity.h"
#include "dimension.h"
#include "nbt/serializeColor.h"
#include "include/filesystem/fileFunctions.h"

std::vector<structure*> structureList = std::vector<structure*>();

mat3x3i structure::getBlocksToWorldTransform(cveci2 &pos00, cbool &flipX) const {
    mat3x3i structureToWorldTransform = mat3x3i::translate(pos00);
    if (flipX) {
        //+ blocks->size.x - 1
        structureToWorldTransform = mat3x3i::cross(structureToWorldTransform, mat3x3i::translate(
                cveci2((int) blockIDArray.size.x - 1, 0)));
        //-structurepos.x
        structureToWorldTransform = mat3x3i::cross(structureToWorldTransform,
                                                   mat3x3i::scale(cveci3(-1, 1, 1)));
    }
    //return pos00 + (flipX ? cveci2(blocks->size.x - structurePos.x - 1, structurePos.y) : structurePos);
    return structureToWorldTransform;
}

mat3x3 structure::getEntitiesToWorldTransform(cveci2 &pos00, cbool &flipX) const {
    mat3x3 entitiesToWorldTransform = mat3x3::translate(cvec2(pos00));
    if (flipX) {
        //+ blocks->size.x - 1
        entitiesToWorldTransform = mat3x3::cross(entitiesToWorldTransform, mat3x3::translate(
                cvec2((fp) blockIDArray.size.x, 0)));
        //-structurepos.x
        entitiesToWorldTransform = mat3x3::cross(entitiesToWorldTransform,
                                                 mat3x3::scale(cvec3(-1, 1, 1)));
    }
    //return pos00 + (flipX ? cveci2(blocks->size.x - structurePos.x - 1, structurePos.y) : structurePos);
    return entitiesToWorldTransform;
}

bool structure::serialize(const stdPath &path, cbool &write) {
    nbtCompound *compound = nullptr;

    if (write) {
        createFoldersIfNotExists(stdPath(path).parent_path().wstring());
        compound = new nbtCompound(std::wstring(L"structure"));
    } else {
        if (!nbtCompound::serialize(compound, write, path)) {
            return false;
        }
    }
    nbtSerializer s = nbtSerializer(*compound, write);
    //output file stream
    if (write) {
        serializeNBTValue(s, std::wstring(L"size"), blockIDArray.size);
    } else {
        veci2 size;
        serializeNBTValue(s, std::wstring(L"size"), size);
        blockIDArray = array2d<blockID>(size, false);
        blockDataArray = array2d<blockData *>(size, true);
    }
    if (s.serializeArray(std::wstring(L"blocks"), (int*)blockIDArray.baseArray,
                         (int) blockIDArray.size.volume()) && s.converter) {
        idConverter::convertArray((itemID *&) blockIDArray.baseArray,
                                  (int) blockIDArray.size.volume(), s.converter->itemIDConverter);
    }
    if (!write) {
        //create block data for all blocks
        //necessary so files converted to newer versions with more block data have block data generated
        csize_t &blockCount = (blockIDArray.size.volume());
        for (size_t i = 0; i < blockCount; i++) {
            blockDataArray.baseArray[i] = createBlockData(blockIDArray.baseArray[i]);
        }
    }
    if (s.push<nbtDataTag::tagList>(std::wstring(L"block data"))) {
        if (write) {
            //count blockdata
            for (vect2<fsize_t> relativePosition = vect2<fsize_t>();
                 relativePosition.y < blockIDArray.size.y; relativePosition.y++) {
                for (relativePosition.x = 0;
                     relativePosition.x < blockIDArray.size.x; relativePosition.x++) {
                    blockData *data = blockDataArray.getValueUnsafe(relativePosition);
                    if (data) {
                        if (s.push<nbtDataTag::tagCompound>()) {
                            serializeNBTValue(s, std::wstring(L"relative position"), relativePosition);
                            data->serializeValue(s);
                            s.pop();
                        }
                    }
                }
            }
        } else {

            const std::vector<nbtData *> dataList = s.getChildren();
            for (nbtData *serializedBlockData: dataList) {
                if (s.push(serializedBlockData)) {
                    veci2 position;
                    serializeNBTValue(s, std::wstring(L"relative position"), position);
                    blockData *data = blockDataArray.getValue(position);
                    data->serializeValue(s);
                    s.pop();
                }
            }
        }
        s.pop();
    }
    //entities
    if (s.push<nbtDataTag::tagList>(std::wstring(L"entities"))) {
        if (write) {
            for (entity *e: entities) {
                if (s.push<nbtDataTag::tagCompound>()) {
                    s.serializeValue(std::wstring(L"entity id"), e->entityType);
                    serializeNBTValue(s, std::wstring(L"position"), e->position);
                    e->serializeValue(s);
                    s.pop();
                }
            }
        } else {
            const auto &serializedEntityList = s.getChildren();
            entities = fastList<entity *>();
            for (const nbtData *const &serializedEntity: serializedEntityList) {
                if (s.push(serializedEntity)) {
                    entityID entityType;
                    if (idConverter::serializeID(s, std::wstring(L"entity id"), entityType,
                                                 s.converter ? &s.converter->entityIDConverter
                                                             : nullptr)) {
                        vec2 position;
                        serializeNBTValue(s, std::wstring(L"position"), position);
                        entity *const &e = createEntity(entityType, nullptr, position);
                        e->serializeValue(s);
                        entities.push_back(e);
                    }
                    s.pop();
                }
            }
        }
        s.pop();
    }
    if (write) {
        if (!nbtCompound::serialize(compound, write, path)) {

            delete compound;
            return false;
        }
    }

    delete compound;
    return true;
}

void
structure::createStructure(dimension &dimensionIn, crectanglei2 &rect, cbool &includeEntities) {
    blockIDArray = array2d<blockID>(rect.size, false);
    blockDataArray = array2d<blockData *>(rect.size, false);
    //cveci2 pos11 = rect.pos1();
    for (veci2 pos = veci2(); pos.y <= rect.size.y; pos.y++) {
        for (pos.x = 0; pos.x <= rect.size.x; pos.x++) {
            const blockID &blockToClone = dimensionIn.getBlockID(pos + rect.pos0,
                                                                 chunkLoadLevel::updateLoaded);
            blockIDArray.setValue(pos, blockToClone);

            blockData *const &cloneTo = createBlockData(blockToClone);
            if (cloneTo) {
                blockData *const &cloneFrom = dimensionIn.getBlockData(pos + rect.pos0,
                                                                       chunkLoadLevel::updateLoaded);
                if (cloneFrom) {
                    cloneFrom->clone(*cloneTo);
                }
            }
            blockDataArray.setValue(pos, cloneTo);
        }
    }
    if (includeEntities) {
        const std::vector<entity *> entitiesInBox = dimensionIn.findNearEntities(crectangle2(rect));
        for (entity *e: entitiesInBox) {
            if (e->entityType != entityID::particle && e->entityType != entityID::item) {
                //summon a new entity in no dimension
                entity *clonedEntity = createEntity(e->entityType, nullptr,
                                                    e->position - rect.pos0);
                e->clone(*clonedEntity);
                clonedEntity->newPosition -= cvec2(rect.pos0);
                entities.push_back(clonedEntity);
            }
        }
        entities.update();
    }
}

std::vector<veci2> structure::findBlocks(const blockID &id) {
    return getAffectedPositions(crectanglei2(blockIDArray.getClientRect()),
                                [this, &id](cveci2 &pos) {
                                    return getBlockID(pos) == id;
                                });
}

structure::~structure() {
    for (entity *const &e: entities) {

        delete e;
    }
    for (blockData *const &b: blockDataArray) {
        delete b;
    }
}

void *structure::getArrayValuePointerUnsafe(cveci2 &position, const arrayDataType &dataType,
                                            const chunkLoadLevel &minimalLoadLevel) {
    if (blockIDArray.inBounds(position)) {
        csize_t &index = position.x + (position.y * blockIDArray.size.x);
        switch (dataType) {
            case arrayDataType::blockIDType:
                return blockIDArray.baseArray + index;
            case arrayDataType::blockDataType:
                return blockDataArray.baseArray + index;
            default:
                break;
        }
    }
    return nullptr;
}

bool structure::inBounds(cveci2 &position) const {
    return blockIDArray.inBounds(position);
}

structure *getStructureByPath(const stdPath &path) {

    const auto i = std::find_if(structureList.begin(), structureList.end(), [&path](const auto &a) {
        return a->pathWithoutExtension.compare(path) == 0;
    });
    return i == structureList.end() ? nullptr : *i;
}

std::vector<structure *> getStructuresByName(const stdPath &seekFolder, const std::wstring &name) {
    std::vector<structure *> foundStructures = std::vector<structure *>();
    for (size_t i = 0; i < structureList.size(); i++) {
        structure *s = structureList[i];

        stdPath currentFolder = s->pathWithoutExtension;
        currentFolder.remove_filename();

        if (currentFolder.compare(seekFolder) == 0) {
            const std::wstring &currentFileName = stdPath(
                    s->pathWithoutExtension).filename().wstring();
            if (name == currentFileName) {
                foundStructures.push_back(s);
            } else {
                if (currentFileName.length() > name.length()) {
                    int fileNumber;
                    if ((!convertToInt(std::wstring(1, currentFileName[name.length() - 1]),
                                       fileNumber)) &&
                        convertToInt(currentFileName.substr(name.length()), fileNumber)) {
                        foundStructures.push_back(s);
                    }
                }
            }
        }
    }
    return foundStructures;
}