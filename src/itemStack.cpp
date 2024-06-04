#include "potionData.h"
#include "textureList.h"
#include "snowLayerData.h"
#include "idAnalysis.h"
#include "application/control/control.h"
#include "itemStack.h"
#include "itemData.h"
#include "durabilityData.h"
#include "world.h"
#include "statusEffectData.h"
#include "idConverter.h"
#include "renderBlockContainer.h"
#include "minecraftFont.h"
#include "include/math/graphics/brush/brushes/repeatingBrush.h"
#include "include/math/graphics/brush/brushes/alphaMask.h"
#include "include/math/graphics/brush/brushes/colorMultiplier.h"
#include "nbt/nbtSerializer.h"
#include "constants/vectorConstants.h"
#include "gameColors.h"
#include "application/theme.h"

itemStack::itemStack(const itemStack &other)
{
    enchantments = std::vector<enchantment *>();
    for (enchantment *const &otherEnchantment : other.enchantments)
    {
        enchantments.push_back(
            (enchantment *)new enchantment(otherEnchantment->clone<enchantment>()));
    }

    data = other.count ? createItemTag(other.stackItemID) : nullptr;
    if (data)
    {
        other.data->clone(*data);
    }
    stackItemID = other.stackItemID;
    count = other.count;
}

void itemStack::add(cint &count)
{
    this->count += count;
    if (this->count == 0)
    {
        clearData();
    }
    else if (isDebugging && this->count < 0)
    {
        handleError(std::wstring(L"negative stack"));
    }
}

void itemStack::swap(itemStack &with)
{
    std::swap(enchantments, with.enchantments);
    std::swap(count, with.count);
    std::swap(stackItemID, with.stackItemID);
    std::swap(data, with.data);
}

bool itemStack::compare(const itemStack &other) const
{
    if (other.stackItemID != stackItemID)
    {
        return false;
    }
    cbool &emptyData = data == nullptr;
    if ((other.data == nullptr) ^ emptyData)
        return false;
    if (!emptyData && !data->compare(*other.data))
    {
        return false;
    }
    if (other.enchantments.size() != enchantments.size())
    {
        return false;
    }
    for (size_t i = 0; i < other.enchantments.size(); i++)
    {
        if (!other.enchantments[i]->compare(*enchantments[i]))
        {
            return false;
        }
    }
    return true;
}

bool itemStack::addStack(itemStack &s, int amount)
{
    if (s.count < amount)
    {
        // the amount of items to add = the amount this slot has
        amount = s.count;
    }
    if (amount)
    {
        if (count)
        {
            // there are already items in this slot
            // check if they are the same items
            if ((count < itemList[stackItemID]->maxStackSize) && compare(s))
            {
                int room = itemList[stackItemID]->maxStackSize - count;
                if (amount > room)
                {
                    // cant deposit all items
                    // maximize the stack
                    count = itemList[stackItemID]->maxStackSize;
                    // the amount of the items to add = the room there is
                    s.count -= room;
                    return false; // not all items have been added
                }
                else
                {
                    // add all items
                    add(amount);
                    s.add(-amount);
                    return true;
                }
            }
            else
            {
                return false; // can't add, as they're not the same items
            }
        }
        else
        {
            *this = s;
            this->count = amount;
            s.add(-amount);
            return true;
        }
    }
    return true; // this is an empty slot, so what does it matter?
}

bool itemStack::addStack(itemStack &s)
{
    return addStack(s, s.count);
}

void itemStack::clearData()
{
    if (data)
    {
        delete data;
    }
    for (enchantment *e : enchantments)
    {
        delete e;
    }
    stackItemID = (itemID)0;
    enchantments = std::vector<enchantment *>();
    data = nullptr;
    count = 0;
}

itemStack::~itemStack()
{
    clearData();
}

void itemStack::render(crectangle2 &rect, const gameRenderData &targetData) const
{
    if (count)
    {
        const resolutionTexture &textureToUse = replaceIfMissing(itemList[stackItemID]->tex);
        cmat3x3 &matrix = mat3x3::fromRectToRect(crectangle2(textureToUse.getClientRect()), rect);
        renderSingleItem(targetData.clone(matrix));
        if (count > 1)
        {
            // draw text
            std::wstring s = std::to_wstring(count);
            cveci2 size = cveci2((int)((fp)s.size() * defaultTheme().font->fontSize),
                                 (int)defaultTheme().font->fontSize);
            cvec2 topRight = rect.pos1();
            currentMinecraftFont->DrawString(s, rectangle2(topRight.x - size.x, rect.pos0.y, size.x, size.y), targetData.renderTarget);
        }

        if (hasDurability(stackItemID))
        {
            durabilityData *toDurabilityData = (durabilityData *)data;

            if (toDurabilityData->durability < 1)
            {
                cfp &margin = 0.1;
                crectangle2 &durabilityBarRect = getAbsoluteRect(rect, crectangle2(margin, margin,
                                                                                   1 - (margin * 2),
                                                                                   0.15));

                fillRectangle(targetData.renderTarget, durabilityBarRect, brushes::black);

                crectangle2 &durabilityRect = getAbsoluteRect(durabilityBarRect, crectangle2(0, 0.5,
                                                                                             toDurabilityData->durability,
                                                                                             0.5));

                ccolor &durabilityColor = lerpColor(colorPalette::red, colorPalette::green,
                                                    toDurabilityData->durability);
                fillRectangle(targetData.renderTarget, durabilityRect,
                              solidColorBrush(durabilityColor));
            }
        }
    }
}

#pragma optimize("", off)

void itemStack::renderSingleItem(const gameRenderData &targetData) const
{
    if (count)
    {
        const resolutionTexture &textureToUse = replaceIfMissing(itemList[stackItemID]->tex);

        crectangle2 textureRect = crectangle2(textureToUse.getClientRect());

        if (enchantments.size() || (stackItemID == itemID::enchanted_golden_apple))
        {
            // repeat the enchanted item texture
            const auto &repeater = repeatingBrush<resolutionTexture>(*enchantedItemTexture);
            // move the enchantment glint over time; one texture size up every 5 seconds
            const auto &transformer = transformBrush<repeatingBrush<resolutionTexture>>(
                mat3x3::translate(cvec2(0, (currentWorld->ticksSinceStart /
                                            ((fp)ticksPerRealLifeSecond * 5)) *
                                               enchantedItemTexture->defaultSize.y)),
                repeater);
            // keep this apart, so the reference is not destroyed

            // make the glint transparent (set alpha channel)
            const auto &transparencyBrush = solidColorBrush(
                color((colorChannel)(color::maxValue * 0.4), 0));
            const auto &glintMask = alphaMask(
                transparencyBrush, transformer);

            // now mix this color with the item texture
            const auto &mixer = colorMixer(
                glintMask, textureToUse);

            //'cut out' the final product based on the original item texture
            const auto &textureMask = alphaMask(textureToUse,
                                                mixer);

            fillTransparentRectangle(textureRect, targetData.worldToRenderTargetTransform,
                                     textureMask, targetData.renderTarget);
        }
        else
        {
            if (renderAsBlock(stackItemID))
            {
                // transform from block container to texture
                cmat3x3 &matrix = mat3x3::cross(targetData.worldToRenderTargetTransform,
                                                mat3x3::fromRectToRect(crectangle2(0, 0, 1, 1),
                                                                       textureRect));
                const blockID &itemBlock = (blockID)stackItemID;
                cveci2 &blockPosition = cveci2();
                const gameRenderData &transformedData = gameRenderData(matrix,
                                                                       targetData.renderTarget,
                                                                       targetData.screen);
                renderBlockContainer container = renderBlockContainer(cveci2(1));
                container.setBlockID(cveci2(), (blockID)stackItemID);
                blockList[itemBlock]->render(transformedData, container.getBlockData(blockPosition),
                                             &container, blockPosition);
            }
            else
            {
                fillTransparentRectangle(textureRect, targetData.worldToRenderTargetTransform,
                                         textureToUse, targetData.renderTarget);
            }
        }
        if (isPotion(stackItemID))
        {
            color potionColor;
            if (canAddEffects(stackItemID) && ((potionData *)data)->effectsToAdd.size())
            {
                potionData *toPotionData = (potionData *)data;
                // average out
                colorf totalColor = colorf();
                for (const statusEffect &effectToAdd : toPotionData->effectsToAdd)
                {
                    totalColor += colorf(
                        statusEffectDataList[(int)effectToAdd.identifier]->particleColor);
                }
                totalColor /= (fp)toPotionData->effectsToAdd.size();
                potionColor = color(totalColor);
            }
            else
            {
                potionColor = waterColor;
            }
            const solidColorBrush &potionColorBrush = solidColorBrush(potionColor);
            mat3x3 mipmapTransform = targetData.worldToRenderTargetTransform;
            rectangle2 rect = crectangle2(blockTextureRect);
            const auto &mult = colorMultiplier<texture, solidColorBrush>(
                potionOverlayTexture->mipmap(mipmapTransform, rect), potionColorBrush);
            fillTransparentRectangle(rect, mipmapTransform, mult, targetData.renderTarget);
        }
    }
}

#pragma optimize("", on)

int itemStack::getEnchantmentLevel(const enchantmentID &identifier) const
{
    for (enchantment *e : enchantments)
    {
        if (e->identifier == identifier)
        {
            return e->power;
        }
    }
    return 0;
}

void itemStack::drawToolTips(cveci2 &position, const texture &renderTarget) const
{
    const std::wstring &nameText = replace(itemList[stackItemID]->name, std::wstring(L"_"),
                                           std::wstring(L" "));
    std::wstring stringToDraw = nameText;
    std::wstring enchantmentsString = std::wstring();
    for (size_t i = 0; i < enchantments.size(); i++)
    {
        if (i)
        {
            enchantmentsString += std::wstring(L"\n");
        }
        else
        {
            stringToDraw += std::wstring(L"\n");
        }
        enchantment *e = enchantments[i];
        enchantmentsString += e->toWString();
    }
    stringToDraw += enchantmentsString;

    cvec2 maximumSize = cvec2(0x200, 0x300);
    cvec2 rectangleSize = defaultTheme().font->measureStringSize(maximumSize, stringToDraw);
    crectangle2 stringRect = crectangle2(position.x, position.y - rectangleSize.y, rectangleSize.x,
                                         rectangleSize.y);
    fillRectangle(renderTarget, stringRect.expanded(defaultTheme().borderSize), brushes::black);
    const minecraftFont nameFont = minecraftFont();
    nameFont.DrawString(nameText, stringRect, renderTarget);
    crectangle2 enchantmentsStringrect = crectangle2(stringRect.x, stringRect.y, stringRect.w,
                                                     stringRect.h - defaultTheme().font->fontSize);
    defaultTheme().font->DrawString(enchantmentsString, enchantmentsStringrect, renderTarget);
}

void itemStack::serializeValue(nbtSerializer &s)
{
    if (s.write)
    {
        s.serializeValue(std::wstring(L"item id"), stackItemID);
    }
    else
    {
        if (idConverter::serializeID(s, std::wstring(L"item id"), stackItemID,
                                     s.converter ? &s.converter->itemIDConverter : nullptr))
        {
        }
    }
    if ((int)stackItemID) // only store additional data of filled slots
    {
        s.serializeValue(std::wstring(L"amount"), count);
        if (s.push<nbtDataTag::tagList>(std::wstring(L"enchantments")))
        {
            if (s.write)
            {
                for (enchantment *e : enchantments)
                {
                    if (s.push<nbtDataTag::tagCompound>())
                    {
                        e->serializeValue(s);
                        s.pop();
                    }
                }
            }
            else
            {
                std::vector<nbtData *> serializedEnchantmentData = s.getChildren();
                enchantments = std::vector<enchantment *>(serializedEnchantmentData.size());
                for (size_t i = 0; i < enchantments.size(); i++)
                {
                    if (s.push(serializedEnchantmentData[i]))
                    {
                        enchantments[i] = new enchantment();
                        enchantments[i]->serializeValue(s);
                        s.pop();
                    }
                }
            }
            s.pop();
        }
        if (s.write)
        {
            if (data)
            {
                data->serialize(s, std::wstring(L"data"));
            }
        }
        else
        {
            data = createItemTag(stackItemID);
            if (data)
            {
                data->serialize(s, std::wstring(L"data"));
            }
        }
    }
}
