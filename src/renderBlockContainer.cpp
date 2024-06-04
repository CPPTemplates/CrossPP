#include "renderBlockContainer.h"

bool renderBlockContainer::inBounds(cveci2 &position) const {
    return blockIDArray.inBounds(position);
}

void *
renderBlockContainer::getArrayValuePointerUnsafe(cveci2 &position, const arrayDataType &dataType,
                                                 const chunkLoadLevel &minimalLoadLevel) {
    switch ((int)dataType) {
        case (int)arrayDataType::blockIDType: {
            return (void *) &blockIDArray.getValueReferenceUnsafe(position);
        }

        case (int)arrayDataType::blockDataType: {
            return (void *) &blockDataArray.getValueReferenceUnsafe(position);
        }
        case ((int) arrayDataType::levelType + (int) levelID::powerLevel) : {
            return (void *) &powerLevelArray.getValueReferenceUnsafe(position);
        }
        default: {
            break;
        }
    }
    return nullptr;
}
