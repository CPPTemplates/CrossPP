#include "videoEncoder.h"
#include "math/algorithm/findInCircles.h"
#include "math/graphics/brush/brushes.h"
#include "filesystem/gzipIncluder.h"
#include "math/rectangle/rectangleBuilder.h"
#include "filesystem/fpng/fpng.h"
#include <unordered_set>

constexpr bool showCommunication = false;
constexpr bool showMotionOnly = false;
constexpr bool changeAllColors = false;

constexpr int motionBlockSize[motionCheckStepCount] = {
    0x400,
    0x100,
    0x10};
constexpr int checkInterval[motionCheckStepCount] = {
    0x100,
    0x20,
    0x4};
constexpr int startingS[motionCheckStepCount] = {
    0x40,
    0x20,
    0x8};

constexpr veci2 relativeCheckPositions[] = {
    veci2(1, 0),
    veci2(0, 1),
    veci2(-1, 0),
    veci2(0, -1),
    veci2(-1, -1),
    veci2(-1, 1),
    veci2(1, 1),
    veci2(1, -1),
};

// brga
constexpr color nothingChangedColor = color(0x79);
constexpr color correctionColor = color(0x79, 0x79, 0x80);

int marginPerColor = 0x10; // the full colors may differ more than

constexpr color motionToColor(const compressedSourceMotionVector &vector)
{
    // return vector.axis[0] > 0 ? vector.axis[1] > 0 ? colorPalette::red : colorPalette::yellow : vector.axis[1] > 0 ? colorPalette::
    return color((colorChannel)(vector.axis[0] + 0x80), (colorChannel)(vector.axis[1] + 0x80), 0);
}

inline constexpr int getByteDifference(cbyte &b1, cbyte &b2)
{
    return b1 > b2 ? b1 - b2 : b2 - b1;
}

constexpr byte channelCompareValue = 0b11111000;
inline constexpr bool isRoughlyTheSameColorValue(cbyte &currentByte, cbyte &targetByte)
{
    // exponential: the higher the byte value, the less one will notice a tad bit difference.
    // our brains measure impulses exponentially
    return !((currentByte ^ targetByte) & channelCompareValue);
    // return getByteDifference(currentByte, targetByte) <= 0x8;
}

constexpr bool isRoughlyTheSameColor(const color &currentColor, const color &targetColor)
{
    // nice for the future: bit comparison for speedup
    constexpr uint colorCompareValue = getUint(color(channelCompareValue, channelCompareValue, channelCompareValue, 0)); // channelCompareValue + channelCompareValue * 0x100 + channelCompareValue * 0x10000;
    return !((currentColor.uintValue ^ targetColor.uintValue) & colorCompareValue);

    // return isRoughlyTheSameColorValue(currentColor[0], targetColor[0]) &&
    //        isRoughlyTheSameColorValue(currentColor[1], targetColor[1]) &&
    //        isRoughlyTheSameColorValue(currentColor[2], targetColor[2]);
}

inline constexpr int getColorDifference(const color &c1, const color &c2)
{
    return getByteDifference(c1[0], c2[0]) +
           getByteDifference(c1[1], c2[1]) +
           getByteDifference(c1[2], c2[2]);
}

inline constexpr int compareColors(const color &c1, const color &c2)
{
    return math::maximum(getByteDifference(c1[0], c2[0]) - 0x4, 0) +
           math::maximum(getByteDifference(c1[1], c2[1]) - 0x4, 0) +
           math::maximum(getByteDifference(c1[2], c2[2]) - 0x4, 0);
}

// crop a rectangle based on direction and the image
// the rect should be inside of the bounds, but the rect + movementvector should also be inside of the bounds
inline bool cropCheckRect(rectanglei2 bounds, rectanglei2 &rect, cveci2 &movementVector)
{
    // bounds.pos0 -= movementVector;
    return crectanglei2(bounds.pos0 - movementVector, bounds.size).cropClientRect(bounds) &&
           bounds.cropClientRect(rect);
}

// compares the pixels of the moved location in the old frame to
// the pixels of the current location in the new frame
// (moves the old frame 'over' the new frame)
inline fp getMovementCostUnsafe(const texture &oldFrame, const texture &newFrame, rectanglei2 rectToCheck, cveci2 &sourceMovementVector, cint &checkInterval, cfp &maxError)
{
    cfp &originalRectPixelCount = rectToCheck.size.volume();
    // check one pixel of every #x# where # = checkInterval
    if (cropCheckRect(newFrame.getClientRect(), rectToCheck, sourceMovementVector))
    {
        fp total = 0;
        cfp &rectToPixelMultiplier = 1 / originalRectPixelCount;
        // each pixel outside counts as full error
        total += 0xff * 3 * (originalRectPixelCount - rectToCheck.size.volume()) * rectToPixelMultiplier;
        crectanglei2 &baseRect = rectanglei2(veci2(), rectToCheck.size / checkInterval);
        for (cveci2 &baseCheckPos : baseRect)
        {
            cveci2 &checkPos = rectToCheck.pos0 + (baseCheckPos * checkInterval);
            const color &sourceColor = oldFrame.getValueUnsafe(checkPos + sourceMovementVector);
            const color &targetColor = newFrame.getValueUnsafe(checkPos);
            total += compareColors(sourceColor, targetColor) * rectToPixelMultiplier;
            // if (sourceColor != targetColor)
            //{
            //     // 1 added for each wrong color
            //     total += multiplier; // total += 1 * multiplier
            if (total >= maxError)
            {
                return total;
            }
            //}
        }
        return total;
    }
    return INFINITY;
}

inline void moveArrayPart(const texture &tex, rectanglei2 rect, cveci2 &direction)
{
    if (direction != veci2())
    {
        if (cropCheckRect(tex.getClientRect(), rect, direction))
        {
            // copy in the reverse order of moving
            cint &yStep = tex.size.x;
            //cveci2 &end = rect.pos1();
            if (direction.y > 0)
            {
                // copy from top to bottom
                color *const &srcYEndPtr = &tex.getValueReferenceUnsafe(rect.pos0);
                color *srcPtr = srcYEndPtr + yStep * rect.size.y;
                color *destPtr = srcPtr + direction.x + direction.y * yStep;
                for (; srcPtr > srcYEndPtr;)
                {
                    srcPtr -= yStep;
                    destPtr -= yStep;
                    std::copy(srcPtr, srcPtr + rect.size.x, destPtr);
                }
            }
            else
            {
                const color *srcPtr = &tex.getValueReferenceUnsafe(rect.pos0);
                const color *const &srcYEndPtr = srcPtr + yStep * rect.size.y;
                color *destPtr = &tex.getValueReferenceUnsafe(rect.pos0 + direction);
                cbool &copyForward = direction.y < 0 || direction.x < 0;
                // when copyForward is not satisfied then we copy backwards, because we know that direction.y == 0 && direction.x > 0
                if (!copyForward)
                {
                    destPtr += rect.size.x;
                }
                for (; srcPtr < srcYEndPtr; srcPtr += yStep, destPtr += yStep)
                {
                    if (copyForward)
                    {
                        // copy from left to right, from bottom to top
                        std::copy(srcPtr, srcPtr + rect.size.x, destPtr);
                    }
                    else
                    {
                        // copy from right to left
                        std::copy_backward(srcPtr, srcPtr + rect.size.x, destPtr);
                    }
                }
            }
        }
    }
}

void videoEncoder::addFrameDiff(streamSerializer& serializer)
{
    serializeScreen(serializer);
    cbool &shouldResize = diffTex.size != totalTexture.size;
    //  temporary, for visualisation
    if (shouldResize || !shouldCompress || showCommunication)
    {
        // copy difftex to totaltexture
        totalTexture = diffTex;
        if (shouldResize)
            resizeGrid();
        if constexpr (showCommunication)
        {
            serializeMotionVectors(serializer);
        }
    }
    else
    {
        serializeMotionVectors(serializer);
        addMotionVectors();

        const color *const &totalTexEndPtr = (color *)totalTexture.end();
        color *diffPtr = (color *)diffTex.baseArray;
        for (color *totalTexPtr = (color *)totalTexture.baseArray; totalTexPtr < totalTexEndPtr; diffPtr++, totalTexPtr++)
        {
            if (diffPtr->uintValue != getUint(nothingChangedColor))
            {
                *totalTexPtr = *diffPtr;
            }
        }
    }
    // tex will safely go out of scope, as it doesn't hurt to delete a null pointer
    // this can't be in the destructor, as that is also used by the encoder
    diffTex.baseArray = nullptr;
}

void videoEncoder::addFrame(const texture &frame, streamSerializer& serializer)
{
    if (frame.size != totalTexture.size)
    {
        totalTexture = frame;
        resizeGrid();
        // copy is necessary, we're setting up a buffer for the next frames too
        diffTex = totalTexture;
    }
    else if (!shouldCompress)
    {
        diffTex = frame;
    }
    else
    {
        // TODO: remove unnecessary copy of frame to totaltexture
        // const textureRGB &diffTex = textureRGB(frame.size, false);

        // optimize frame further: detect general image motion

        // block matching algorithms don't seem to help much:
        // https://en.wikipedia.org/wiki/Block-matching_algorithm
        // we'll store the motion of each pixel and make an image of this motion. the PNG compressor will create a PNG from the motion.

        // now compare the error of different parts of the image and 'vote' which way to move
        // std::unordered_map<veci2, int> motionWeights = std::unordered_map<veci2, int>();

        crectanglei2 &imageRect = frame.getClientRect();

        // find rectangles that are moving

        // std::vector<rectanglei2, veci2> motionRects = std::vector<rectanglei2, veci2>(frame.size);

        // get motion vectors for the whole image

        for (byte motionStepIndex = 0; motionStepIndex < motionCheckStepCount; motionStepIndex++)
        {
            const array2d<veci2> &currentVectorArray = motionBlockVectors[motionStepIndex];
            cint &currentMotionBlockSize = motionBlockSize[motionStepIndex];

            // the outer borders of the frame will not be checked. too bad
            for (cveci2 &blockPos : currentVectorArray.getClientRect())
            {
                //  we extend to the right, but that doesn't really matter
                // crectanglei2 &rectToCheck = crectanglei2(posToCheck - cveci2(0x4), cveci2(0x8));
                crectanglei2 &rectToCheck = crectanglei2(blockPos * currentMotionBlockSize, cveci2(currentMotionBlockSize));
                // ccolor &colorToCompare = totalTexture.getValue(posToCheck);

                // constexpr int searchRadius = 0x8;
                veci2 &leastErrorPos = currentVectorArray.getValueReferenceUnsafe(blockPos);

                // copy
                cveci2 lastMotion = leastErrorPos;

                // std::unordered_set<veci2> checkedVectors = std::unordered_set<veci2>();
                //  first check 00, then check around the old movement, to create constant motion. constant motion is not appreciated when there's a still part of an image.

                leastErrorPos = veci2();

                fp leastError = INFINITY;

                const auto &checkFunction = [this, &rectToCheck, &leastError, &leastErrorPos, &motionStepIndex, &frame](cveci2 &relativePosition)
                {
                    // if (checkedVectors.contains(relativePosition))
                    //{
                    //     return false;
                    // }
                    cfp &error = getMovementCostUnsafe(totalTexture, frame, rectToCheck, relativePosition, checkInterval[motionStepIndex], leastError);
                    // checkedVectors.insert(relativePosition);
                    if (error < leastError)
                    {
                        leastErrorPos = relativePosition;
                        leastError = error;
                    }
                    return error == 0;
                };

                // leastError = getMovementCostUnsafe(totalTexture, frame, rectToCheck, leastErrorPos, checkInterval[motionStepIndex], leastError);
                // checkedVectors[leastErrorPos] = leastError;

                // first things to check:
                if (checkFunction(veci2()) ||                           // zero motion
                    (lastMotion != veci2() && checkFunction(lastMotion))) // old motion
                    continue;

                // copy
                // we will be zooming in on this position (TSS)
                // get the motion from a higher level or from memory
                veci2 zoomInOn;
                if (motionStepIndex)
                {
                    zoomInOn = motionBlockVectors[motionStepIndex - 1].getValueUnsafe(rectToCheck.pos0 / motionBlockSize[motionStepIndex - 1]);
                    rectanglei2 testRect = rectToCheck;
                    if (!cropCheckRect(imageRect, testRect, zoomInOn))
                    {
                        // the parent rect isn't fully out of bounds but this subrect is
                        // look around the last motion, maybe we can copy something from there
                        zoomInOn = lastMotion;
                    }
                }
                else
                {
                    zoomInOn = lastMotion;
                }

                if (zoomInOn != lastMotion && checkFunction(zoomInOn))
                    continue;

                for (int stepSize = startingS[motionStepIndex]; stepSize; stepSize /= 2, zoomInOn = leastErrorPos)
                {
                    veci2 finalCoords;
                    for (cveci2 &checkPos : relativeCheckPositions)
                    {
                        if (checkFunction(zoomInOn + checkPos * stepSize))
                        {
                            goto found;
                        }
                    }
                    // if (findInCircles(2, checkFunction, finalCoords))
                    //{
                    //     // exact match
                    //     break;
                    // }
                }
            found:
                // old: // finally, check 00 if it hasn't been checked yet
                //      if (leastError && !blockMotionWeight.contains(veci2()))
                //      {
                //          cint &error = getMovementCostUnsafe(totalTexture, frame, rectToCheck, leastErrorPos, leastError);
                //          if (error < leastError)
                //          {
                //              leastErrorPos = veci2();
                //              leastError = error;
                //          }
                //      }

                // motionWeights[leastErrorPos]++; //= leastError;
            }
        }

        const auto &smallestMotionBlockVectorArray = motionBlockVectors[motionCheckStepCount - 1];
        constexpr int smallestMotionBlockSize = motionBlockSize[motionCheckStepCount - 1];
        for (cveci2 &pixelPos : imageRect)
        {
            const color &currentSearchColor = frame.getValueUnsafe(pixelPos);
            compressedSourceMotionVector &leastErrorPos = sourceMotionVectors.getValueReferenceUnsafe(pixelPos);
            int leastError = INT_MAX;
            const auto &pixelCheckFunction = [&frame, &pixelPos, &leastErrorPos, this, &currentSearchColor, &leastError](cveci2 &relativePosition)
            {
                cveci2 &checkPos = pixelPos + relativePosition;
                if (frame.inBounds(checkPos))
                {
                    cint &error = compareColors(totalTexture.getValueUnsafe(checkPos), currentSearchColor);
                    if (error < leastError)
                    {
                        leastError = error;
                        leastErrorPos = (compressedSourceMotionVector)relativePosition;
                        return !error;
                    }
                }
                return false;
            };
            // first check 0 0
            if (pixelCheckFunction(cveci2()))
                continue;

            // check last motion vector
            compressedSourceMotionVector lastSourceMotionVector = leastErrorPos;
            if (lastSourceMotionVector == compressedSourceMotionVector() || !pixelCheckFunction(lastSourceMotionVector))
            {
                cveci2 &blockMovementPos = smallestMotionBlockVectorArray.getValueUnsafe(pixelPos / smallestMotionBlockSize);
                // cveci2 &offsetPos = blockMovementPos + pixelPos;

                if (pixelCheckFunction(blockMovementPos))
                    continue;

                // check pixels left or right for the same pixel (did it move or did it disappear?)
                const auto &checkFunction = [&blockMovementPos, &pixelCheckFunction](cveci2 &relativePosition)
                {
                    return pixelCheckFunction(blockMovementPos + relativePosition);
                };

                for (cveci2 &checkPos : relativeCheckPositions)
                {
                    if (checkFunction(checkPos))
                    {
                        break;
                    }
                }
                // veci2 finalPos;
                // findInCircles(2, checkFunction, finalPos);
                //{
                //     sourceMotionVectors.setValueUnsafe(pixelPos, compressedSourceMotionVector(blockMovementPos + finalPos));
                // }
            }
        }

        // apply source motion vectors
        addMotionVectors();

        // if (totalMovementVector != veci2())
        //{
        //     textureRGB texCopy = totalTexture;
        //     // move the whole texture
        //     auto movedRect = texCopy.getClientRect();
        //     movedRect.pos0 += totalMovementVector;
        //     fillTransformedBrushRectangle(rectangle2(texCopy.getClientRect()), vec2(totalMovementVector), texCopy, totalTexture);
        // }

        const color *const &totalTexEndPtr = (color *)totalTexture.end();
        color *diffPtr = (color *)diffTex.baseArray;
        color *framePtr = (color *)frame.baseArray;
        for (color *totalTexPtr = (color *)totalTexture.baseArray; totalTexPtr < totalTexEndPtr; framePtr++, diffPtr++, totalTexPtr++)
        {
            if (!changeAllColors && (!compareColors(*framePtr, *totalTexPtr) || showMotionOnly))
            {
                *diffPtr = nothingChangedColor;
            }
            else
            {
                // we
                if (*framePtr == nothingChangedColor)
                {
                    *diffPtr = color(0x79, 0x79, 0x80);
                }
                else
                {
                    *diffPtr = *framePtr;
                }
                *totalTexPtr = *diffPtr;
            }
        }
        serializeMotionVectors(serializer);
    }
    serializeScreen(serializer);
}

void videoEncoder::serializeMotionVectors(streamSerializer& serializer)
{
    byte *unCompressedRectBytes = nullptr;
    size_t rectCount;

    // serialize the motion vectors as one big char array

    // static rectangleBuilder<compressedSourceMotionVector> builder;
    std::string unCompressedArray;
    typedef ushort posType;
    typedef ushort sizeType;
    constexpr size_t stride = (sizeof(vect2<posType>) + sizeof(vect2<sizeType>) + sizeof(compressedSourceMotionVector));
    rectangleBuilder<compressedSourceMotionVector> builder;
    if (serializer.write)
    {
        builder = rectangleBuilder<compressedSourceMotionVector>();
        for (cveci2 &pos : sourceMotionVectors.getClientRect())
        {
            const auto &value = sourceMotionVectors.getValueUnsafe(pos);
            if (value != compressedSourceMotionVector())
            {
                builder.addNextPosition(pos, value);
            }
            else
            {
                builder.discardNextPosition(pos);
            }
        }
        builder.finish();
        rectCount = builder.finishedRectangles.size();
        unCompressedRectBytes = new byte[rectCount * stride];
    }
    else
    {
        //size_t compressedDataSize = 0;
        //sbyte *compressedRectBytes = nullptr; 
        std::string compressedRectBytes;
        // initialize with nullptr so the serializer will allocate a new array

        // when no packet with motion vectors is sent, return. for example when showCommunication is true and the screen is resized
        if (!serializer.serializeString(compressedRectBytes))
            return;

        unCompressedArray = gzip::decompress(&*compressedRectBytes.begin(), compressedRectBytes.size());
        unCompressedRectBytes = (byte *)&*unCompressedArray.begin();
        rectCount = unCompressedArray.size() / stride;
    }
    // static videoEncoder *otherEncoder = this;

    // save the (x), (y), (w), (h) and (sourcex, sourcey) in separate rows to make them more optimized
    posType *rectXPtr = (posType *)unCompressedRectBytes;
    posType *rectYPtr = rectXPtr + rectCount;
    sizeType *rectWPtr = (sizeType *)(rectYPtr + rectCount);
    sizeType *rectHPtr = rectWPtr + rectCount;
    compressedSourceMotionVector *sourcePtr = (compressedSourceMotionVector *)(rectHPtr + rectCount);
    if (serializer.write)
    {
        for (const auto &rect : builder.finishedRectangles)
        {
            *rectXPtr++ = (posType)rect->rect.x;
            *rectYPtr++ = (posType)rect->rect.y;
            *rectWPtr++ = (sizeType)rect->rect.w;
            *rectHPtr++ = (sizeType)rect->rect.h;
            *sourcePtr++ = rect->value;
        }
        std::string compressedVectors = gzip::compress((char *)unCompressedRectBytes, rectCount * stride, Z_HUFFMAN_ONLY);
        delete[] unCompressedRectBytes;
        serializer.serializeString(compressedVectors);
    }
    else
    {
        // copy pointer
        const posType *const endPtr = rectYPtr;
        //csize_t &count = endPtr - rectXPtr;
        sourceMotionVectors.fill(compressedSourceMotionVector());
        // totalTexture.fill(color(0x80, 0xff, 0xff));
        auto it = builder.finishedRectangles.begin();
        while (rectXPtr < endPtr)
        {
            crectanglet2<fsize_t> &rect = rectanglet2<fsize_t>(*rectXPtr++, *rectYPtr++, *rectWPtr++, *rectHPtr++);
            // if((*it)->rect != rectanglei2(rect)){
            //     throw;
            // }
            const compressedSourceMotionVector &vec = *sourcePtr++;
            for (cvect2<fsize_t> &pos : rect)
            {
                // if (otherEncoder->sourceMotionVectors.getValueUnsafe(pos) != vec)
                //     throw;
                sourceMotionVectors.setValueUnsafe(pos, vec);
                // totalTexture.setValue(pos, color(0xff, 0xff, 0));
            }
            it++;
        }
    }

    // this code is working:
    // sbyte *ptr = nullptr;
    // int count;
    //
    //// declare the string outside so it doesn't go out of scope
    // std::string compressedVectors;
    // if (serializer.write)
    //{
    //     compressedVectors = gzip::compress((const char *)sourceMotionVectors.baseArray, sourceMotionVectors.size.volume() * sizeof(compressedSourceMotionVector));
    //     count = compressedVectors.size();
    //     ptr = (sbyte *)&compressedVectors[0];
    // }
    //// serialize the motion vectors as one big char array
    // serializer.serializeVariableArray<sbyte>(std::wstring(L"motion vectors"), ptr, count);
    // if (!serializer.write)
    //{
    //     std::string decomp = gzip::decompress((char *)ptr, count);
    //     std::copy(decomp.begin(), decomp.end(), (char *)sourceMotionVectors.baseArray);
    // }
}

void videoEncoder::visualize(const texture &screen)
{
    // visualize the moved rects
    for (int i = 0; i < motionCheckStepCount; i++)
    {
        const array2d<veci2> &arrayToVisualize = motionBlockVectors[i];
        cint &currentMotionBlockSize = motionBlockSize[i];
        crectanglei2 &blocksRect = arrayToVisualize.getClientRect();
        for (cveci2 &blockPos : blocksRect)
        {
            crectanglei2 &rectToMove = rectanglei2(blockPos * currentMotionBlockSize, cveci2(currentMotionBlockSize));
            cveci2 &vec = arrayToVisualize.getValueUnsafe(blockPos);
            if (vec != veci2())
            {
                const color &motionColor = motionToColor((compressedSourceMotionVector)vec);
                fillRectangleBorders(screen, rectToMove, 1, solidColorBrush(motionColor));
                constexpr int motionMult = 30;
                fillLine(screen, rectToMove.pos0, rectToMove.pos0 + vec * motionMult, brushes::green);
            }
        }
    }

    for (const compressedSourceMotionVector &vec : sourceMotionVectors)
    {
        if (vec != compressedSourceMotionVector())
        {
            screen.baseArray[&vec - sourceMotionVectors.baseArray] = motionToColor(vec);
        }
    }
}

videoEncoder::~videoEncoder()
{
}

void videoEncoder::addMotionVectors() const
{
    // copy texture
    // const textureRGB oldTexture = totalTexture;
    // for (cveci2 &blockPos : sourceMotionVectors.getClientRect())
    //{
    //    veci2 movement = sourceMotionVectors.getValueUnsafe(blockPos);
    //    if(movement != veci2()){
    //        totalTexture.setValueUnsafe(blockPos + movement, oldTexture.getValue(blockPos));
    //    }
    //    //crectanglei2 &rectToMove = rectanglei2(blockPos * gridStepSize, cveci2(gridStepSize));
    //    //totalTexture.copyArray(rectToMove, rectToMove.pos0 + sourceMotionVectors.getValueUnsafe(blockPos), oldTexture);
    //    // moveArrayPart(totalTexture, rectToMove, sourceMotionVectors.getValueUnsafe(blockPos));
    //}
    // copy
    texture oldTexture = totalTexture;

    for (const compressedSourceMotionVector &vector : sourceMotionVectors)
    {
        if (vector != compressedSourceMotionVector())
        {
            csize_t &pixelIndex = (&vector - sourceMotionVectors.begin());
            totalTexture.baseArray[pixelIndex] = oldTexture.baseArray[pixelIndex + (vector.x + (vector.y * oldTexture.size.x))];
        }
    }
}

void videoEncoder::serializeScreen(streamSerializer& serializer)
{
    constexpr int compressionColorChannelCount = bgraColorChannelCount;
    if (serializer.write)
    {
        std::vector<byte> compressedScreen;
        // std::copy(socket->lastRenderResult->baseArray, socket->lastRenderResult->baseArray + socket->lastRenderResult->size.volume(), colorsWithoutAlpha);
        fpng::fpng_encode_image_to_memory(diffTex.baseArray, diffTex.size.x, diffTex.size.y, compressionColorChannelCount, compressedScreen);

        serializer.serialize(compressedScreen);
    }
    else
    {
        std::vector<byte> compressedScreen;
        if (serializer.serialize(compressedScreen))
        {
            vectn<fsize_t, 2> size;
            uint32_t channelCount;
            fpng::fpng_decode_memory((const char *)&(*compressedScreen.begin()),
                                     (uint32_t)compressedScreen.size(), decompressedScreen, size.x,
                                     size.y, channelCount, compressionColorChannelCount);

            // a dangerous trick: we don't copy the difftex, but use the vector pointer!
            diffTex = texture(size, (color *)&*decompressedScreen.begin());
        }
    }
    serializer.serialize(shouldCompress);
}

void videoEncoder::resizeGrid()
{
    // we can't resize difftex here; it might be pointing to the vector
    sourceMotionVectors = array2d<compressedSourceMotionVector>(totalTexture.size);
    // resize arrays
    for (byte motionStepIndex = 0; motionStepIndex < motionCheckStepCount; motionStepIndex++)
    {
        motionBlockVectors[motionStepIndex] = array2d<veci2>(ceilVector(vec2(totalTexture.size) / (fp)motionBlockSize[motionStepIndex]));
    }
}