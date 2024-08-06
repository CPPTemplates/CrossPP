#include "settings.h"
#include "math/collisions.h"
#include "overWorldBiomeGenerator.h"
#include "overWorldGenerationData.h"
#include "world.h"
#include "overWorld.h"
#include "chunk.h"
#include "overWorldOreList.h"
#include "textureList.h"
#include "gameControl.h"
#include "math/graphics/brush/brushes/colorMaximizer.h"
#include "math/LayerNoiseSimplex.h"
#include "math/random/shufflerandom.h"
#include "include/math/graphics/brush/brushes/alphaMask.h"

// https://www.researchgate.net/figure/Fitting-effect-of-wind-speed-probability-distribution_fig2_359780398
const transition<fp> noiseValueToWindSpeed = transition<fp>(std::vector<keyFrame<fp>>({
    keyFrame<fp>(
        0,
        0),
    keyFrame<fp>(
        0.5,
        5),
    keyFrame<fp>(
        0.75,
        10),
    keyFrame<fp>(
        0.9,
        20),
    keyFrame<fp>(
        0.95,
        40),
    keyFrame<fp>(
        1,
        100),
}));

constexpr fp averageWindSpeed = 5;

const transition<fp> heightToWindSpeedMultiplier = transition<fp>(std::vector<keyFrame<fp>>({
    keyFrame<fp>(
        minimumHillHeight,
        1),
    keyFrame<fp>(
        maximumHillHeight,
        3),
}));

biomeID overWorld::getBiome(cfp &elevation, cfp &temperature, cfp &humidity) noexcept
{
    if (elevation < 0.35)
    {
        if (humidity < 0.8)
        {
            return biomeID::ocean;
        }
        else
        {
            return biomeID::swamp;
        }
    }
    else if (elevation < 0.65)
    {
        if (humidity < 0.5)
        {
            if (temperature < 0.6)
            {
                return biomeID::plains;
            }
            else
            {
                if (humidity < 0.3)
                {
                    return biomeID::desert;
                }
                else
                {
                    return biomeID::savanna;
                }
            }
        }
        else if (humidity < 0.9)
        {
            if (temperature < 0.4)
            {
                return biomeID::taiga;
            }
            else if (temperature < 0.6)
            {
                return biomeID::forest;
            }
            else if (temperature < 0.8)
            {
                return biomeID::dark_forest;
            }
            else
            {
                return biomeID::jungle;
            }
        }
        else
        {
            if (temperature < 0.2)
            {
                return biomeID::ice_spikes;
            }
            else
            {
                return biomeID::mushroom_fields;
            }
        }
    }
    else
    {
        if ((humidity > 0.4) || (temperature < 0.5))
        {
            return biomeID::mountains;
        }
        else
        {
            return biomeID::badlands;
        }
    }
}

vec2 overWorld::getWindSpeed(cvec2 &position)
{
    // https://sciencing.com/units-anemometer-measure-8146408.html
    if (getHitboxCollisionType(crectangle2(position, cvec2(0))) != collisionTypeID::willCollide)
    {
        cfp &elevation = position.y;

        cvec3 &combinedArguments = cvec3(position, currentWorld->currentTime);

        cfp &windSpeed = noiseValueToWindSpeed.getValue(
            windSpeedNoise->evaluate(combinedArguments));

        vec2 vec = vec2();
        for (int i = 0; i < 2; i++)
        {
            vec[i] = windDirectionNoise[i]->evaluate(combinedArguments);
        }
        vec2 result = heightToWindSpeedMultiplier.getValue(elevation) * (vec * windSpeed);

        constexpr size_t checkDirectionCount = 0x2;
        constexpr size_t testCount[checkDirectionCount]{
            3, // check if can blow from behind
            2  // check if can blow further
        };

        constexpr fp multiplier = 1.0 / ((testCount[0] + testCount[1]) * 2);
        fp successCount = 0;

        cvec2 &normalized = vec.lengthSquared() == 0 ? vec2() : vec.normalized();

        for (size_t checkDirectionIndex = 0;
             checkDirectionIndex < checkDirectionCount; checkDirectionIndex++)
        {
            for (size_t i = 0; i < testCount[checkDirectionIndex]; i++)
            {
                //-4 to 8
                cfp &distance = (((fp)i + randFp(currentRandom)) * 2);

                cvec2 &checkPosition = checkDirectionIndex ? position + (normalized * distance) : position - (normalized * distance);

                const collisionTypeID &type = getHitboxCollisionType(
                    crectangle2(checkPosition, cvec2(0)));

                if (type == collisionTypeID::willNotCollide)
                {
                    successCount += 2;
                }
                else if (type == collisionTypeID::willCollideTop)
                {
                    successCount++;
                }
                else // collisionTypeID::willCollide
                {
                    break;
                }
            }
        }
        // more blockage = less wind speed

        cfp &squared = (successCount * multiplier);
        result *= squared * squared;
        return result;
    }
    return vec2();
}

biomeID getBiomeByColor(const color &c)
{
    for (size_t i = 0; i < biomeDataList.size; i++)
    {
        if (biomeDataList[i]->biomeColor == c)
            return (biomeID)i;
    }
    return (biomeID)0;
}

biomeID overWorld::getBiome(cvec2 &position) const
{
    const color &biomeColor = biomeTexture->scaledTextures[0]->getValue(vect2<fsize_t>(getBiomeTextureCoords(position.x)));
    return getBiomeByColor(biomeColor);
}

fp overWorld::getTemperature(cvec2 &position) const
{
    // above 64 = above 0
    static const transition<fp> texToTemp = transition<fp>({keyFrame<fp>((fp)0, (fp)-100),
                                                            keyFrame<fp>((fp)0x40, (fp)0),
                                                            keyFrame<fp>((fp)0xff, (fp)50)});

    cfp &texValue = (fp)tempMapTexture->scaledTextures[0]->getValue(vect2<fsize_t>(getBiomeTextureCoords(position.x))).r();
    return texToTemp.getValue(texValue);
}

vec2 overWorld::getBiomeTextureCoords(cfp &x) const
{
    // after how many meters will we choose new coordinates?
    constexpr fp metersPerCoord = 0x400;
    vec2 relativePos[2];
    cfp &divided = x / metersPerCoord;
    cint &previousIndex = math::floor(divided);
    cint &nextIndex = previousIndex + 1;
    for (int posIndex = 0; posIndex < 2; posIndex++)
    {
        uint currentIndex = posIndex ? nextIndex : previousIndex;
        for (int dimensionIndex = 0; dimensionIndex < 2; dimensionIndex++)
        {
            relativePos[posIndex][dimensionIndex] = (fp)(biomeCoordinatesRandom[dimensionIndex]->shuffle((uint)currentIndex) / (fp)uintShuffleRandom::maxPlus1);
        }
    }
    // calculate the shortest distance
    for (int dimensionIndex = 0; dimensionIndex < 2; dimensionIndex++)
    {
        cfp &diff = relativePos[1][dimensionIndex] - relativePos[0][dimensionIndex];
        if (diff > 0.5)
        {
            // add 1 to pos[0]
            relativePos[0][dimensionIndex]++;
        }
        else if (diff < -0.5)
        {
            relativePos[1][dimensionIndex]++;
        }
    }

    // now lerp between the two using the remainder
    vec2 interpolated = math::lerp(relativePos[0], relativePos[1], divided - previousIndex);
    interpolated.x = math::Remainder1(interpolated.x);
    interpolated.y = math::Remainder1(interpolated.y);
    // return getBiome(
    //         biomeElevationNoise->evaluate(position),
    //         biomeTemperatureNoise->evaluate(position),
    //         biomeHumidityNoise->evaluate(position)
    //);
    return interpolated * vec2(biomeTexture->scaledTextures[0]->size);
}

overWorld::~overWorld()
{
    // we can safely delete null pointers. it won't do anything
    delete biomeCoordinatesRandom[0];
    delete biomeCoordinatesRandom[1];

    delete windSpeedNoise;
    delete caveNoise;
}

void overWorld::initialize()
{
    std::vector<fp> caveOctaveWeights{
        1, 0.9, 0.8, 0.7, 0.6};
    caveNoise = new layerNoiseSimplex(worldRandom, caveOctaveWeights, 0x40,
                                      crectangle1::fromOppositeCorners(cvec1(-caveCornerSharpness),
                                                                       cvec1(caveCornerSharpness)));

    // generate cloud thickness noise
    std::vector<fp> cloudThicknessWeights{
        0.5,
        0.2,
        0.2,
        0.1};

    std::vector<fp> weatherInfluenceScales{
        ticksPerHour,
        ticksPerDay,
        ticksPerMonth,
        ticksPerYear};

    // month based

    cloudThicknessNoise = new layerNoiseSimplex(worldRandom, cloudThicknessWeights,
                                                weatherInfluenceScales,
                                                crectangle1::fromOppositeCorners(cvec1(0),
                                                                                 cvec1(1)));

    std::vector<vec3> directionInfluenceScales{
        vec3(vec2(0x10), ticksPerRealLifeSecond * 5),
        vec3(vec2(0x40, 0x20), ticksPerHour),
        vec3(vec2(0x80, 0x40), ticksPerDay)};

    std::vector<fp> directionNoiseWeights{
        0.2,
        0.3,
        0.5};
    for (int i = 0; i < 2; i++)
    {
        windDirectionNoise[i] = new layerNoiseSimplex(worldRandom, directionNoiseWeights,
                                                      directionInfluenceScales,
                                                      crectangle1::fromOppositeCorners(cvec1(-1),
                                                                                       cvec1(1)));
    }
    windSpeedNoise = new layerNoiseSimplex(worldRandom, directionNoiseWeights,
                                           directionInfluenceScales,
                                           crectangle1::fromOppositeCorners(cvec1(0), cvec1(1)));

    biomeCoordinatesRandom[0] = new uintShuffleRandom(rand<uint>(worldRandom));
    biomeCoordinatesRandom[1] = new uintShuffleRandom(rand<uint>(worldRandom));

    //  //generate hills
    //  cint biomeElevationOctaveCount = 1;
    //  const std::vector<fp> &biomeElevationWeights
    //          {
    //                  //big scale noise: 100%
    //                  1
    //                  //small scale noise: 0%
    //          };
    //  cfp elevationNoiseScale = 0x400;
    //  cfp humidityNoiseScale = 0x800;
    //  cfp temperatureNoiseScale = 0x1000;
    //
    //
    //  biomeElevationNoise = new layerNoiseSimplex(worldRandom, biomeElevationWeights,
    //                                              elevationNoiseScale,
    //                                              crectangle1::fromOppositeCorners(cvec1(0),
    //                                                                               cvec1(1)));
    //
    //  //generate temperatures
    //  cint temperatureOctaveCount = 1;
    //  const std::vector<fp> &temperatureOctaveWeights
    //          {
    //                  //big scale noise: 100%
    //                  1
    //                  //small scale noise: 0%
    //          };
    //  biomeTemperatureNoise = new layerNoiseSimplex(worldRandom, temperatureOctaveWeights,
    //                                                temperatureNoiseScale,
    //                                                crectangle1::fromOppositeCorners(cvec1(0),
    //                                                                                 cvec1(1)));
    //
    //  //generate humidities
    //  cint humidityOctaveCount = 1;
    //  const std::vector<fp> &humidityOctaveWeights
    //          {
    //                  //big scale noise: 100%
    //                  1
    //                  //small scale noise: 0%
    //          };
    //  biomeHumidityNoise = new layerNoiseSimplex(worldRandom, humidityOctaveWeights,
    //                                             humidityNoiseScale,
    //                                             crectangle1::fromOppositeCorners(cvec1(0),
    //                                                                              cvec1(1)));
}

overWorld::overWorld() : dimension(dimensionID::overworld)
{
}

generationData *overWorld::generateTerrain(chunk &generateIn)
{

    overworldGenerationData *data = new overworldGenerationData();

    cint interpolationRadius = 0x20; // 0x10;
    cint interpolationSize = interpolationRadius * 2;
    fastArray<biomeID> biomesToInterpolate = fastArray<biomeID>(
        (int)chunkSize.x + interpolationSize);
    // get heights to interpolate
    for (int x = -interpolationRadius; x < ((int)chunkSize.x + interpolationRadius); x++)
    {
        biomesToInterpolate[x + interpolationRadius] = getBiome(
            cvec2(x + generateIn.worldPos.x, 0));
    }
    // initialize biome list
    for (int relativeX = 0; relativeX < (int)chunkSize.x; relativeX++)
    {
        data->biomes[relativeX] = getBiome(cvec2(relativeX + generateIn.worldPos.x, 0));
        fp height = 0;
        fp totalWeight = 0;
        std::vector<biomeID> currentBiomes;
        std::vector<fp> currentBiomeWeights;

        // find biome interpolation values
        for (int interpolationIndex = relativeX;
             interpolationIndex <= relativeX + interpolationSize; interpolationIndex++)
        {
            // simulate sinus hill
            cfp weight = interpolationSize == 0 ? 1 : sin(((interpolationIndex - relativeX) / (fp)interpolationSize) * math::PI);
            totalWeight += weight;
            auto it = std::find(currentBiomes.begin(), currentBiomes.end(),
                                biomesToInterpolate[interpolationIndex]);
            if (it == currentBiomes.end())
            {
                currentBiomeWeights.push_back(weight);
                currentBiomes.push_back(biomesToInterpolate[interpolationIndex]);
            }
            else
            {
                size_t index = std::distance(currentBiomes.begin(), it);
                currentBiomeWeights[index] += weight;
            }
        }
        for (size_t biomeIndex = 0; biomeIndex < currentBiomes.size(); biomeIndex++)
        {
            height +=
                ((overWorldBiomeGenerator *)currentWorld->biomeList[(int)currentBiomes[biomeIndex]])->getHeight(generateIn.worldPos.x + relativeX) * currentBiomeWeights[biomeIndex];
        }
        height /= totalWeight;
        data->heights[relativeX] = (int)floor(height);

        if (data->heights[relativeX] > generateIn.worldPos.y)
        {
            cint &relativeHeight = data->heights[relativeX] - generateIn.worldPos.y;
            cint &cappedHeight = math::minimum(relativeHeight, (int)chunkSize.y);
            setBlockRange(generateIn.worldPos + cveci2(relativeX, 0),
                          generateIn.worldPos + cveci2(relativeX, cappedHeight - 1),
                          blockID::stone);
        }
    }
    return data;
}

void overWorld::generateStructures(chunk &generateIn)
{
    overworldGenerationData *data = (overworldGenerationData *)generateIn.terrainData;
    // generate caves

    cfp caveChance = 0.05;

    transition<fp> caveSizeDistribution = transition<fp>(std::vector<keyFrame<fp>>(
        {
            keyFrame<fp>(0, 2),   // really small
            keyFrame<fp>(0.1, 4), // small
            keyFrame<fp>(0.9, 8), // big
            keyFrame<fp>(1, 0x10) // huge
        }));

    std::vector<blockID> caveReplaceList = std::vector<blockID>(overWorldOreList, overWorldOreList +
                                                                                      overWorldOreTypeCount);
    caveReplaceList.push_back(blockID::stone);

    cint caveStep = 0x20;

    cfp caveSteepness = 0.4;
    veci2 caveSquare = veci2();
    for (caveSquare.y = 0; caveSquare.y < (int)chunkSize.y; caveSquare.y += caveStep)
    {
        for (caveSquare.x = 0; caveSquare.x < (int)chunkSize.x; caveSquare.x += caveStep)
        {
            veci2 caveSquareIndex = veci2(randIndex(generateIn.chunkRandom, caveStep),
                                          randIndex(generateIn.chunkRandom, caveStep)) +
                                    caveSquare;
            if (caveSquareIndex.x < (int)chunkSize.x && caveSquareIndex.y < (int)chunkSize.y &&
                (randFp(generateIn.chunkRandom) < caveChance))
            {
                vec2 caveCentre = caveSquareIndex + generateIn.worldPos;
                std::vector<vec2> caveSegments = std::vector<vec2>();
                cfp caveHeightMultiplier = sqrt(
                    randFp(generateIn.chunkRandom)); // deeper are more caves
                if (caveCentre.y < (data->heights[(int)caveCentre.x - generateIn.worldPos.x] *
                                    caveHeightMultiplier))
                {
                    fp size = caveSizeDistribution.getValue(randFp(generateIn.chunkRandom));
                    fp rot = randFp(generateIn.chunkRandom, math::PI2);
                    fp speed = size * 0.2;
                    int maxCaveLength = 0x100;
                    // iterate
                    for (int j = 0; j < maxCaveLength; j++)
                    {
                        // check if the cave intersects itself
                        if (j > 0)
                        {
                            cvec2 caveNewStart = caveSegments[j - 1];
                            for (int index = 0; index < j - 2; index++)
                            {
                                cvec2 caveOldStart = caveSegments[index];
                                cvec2 caveOldEnd = caveSegments[index + 1];
                                if (collides2d(caveOldStart, caveOldEnd, caveNewStart,
                                               caveCentre))
                                {
                                    // this cave intersects with itself
                                    goto nextCave;
                                }
                            }
                        }
                        caveSegments.push_back(caveCentre);
                        // check rotation
                        replaceCircleCentered(caveCentre, vec2(size), blockID::air,
                                              caveReplaceList);
                        fp rotationPlus = caveNoise->evaluate(caveCentre);
                        rot += rotationPlus;
                        caveCentre += vec2::getrotatedvector(rot) * speed * vec2(1, caveSteepness);
                    }
                }
            }
        nextCave:;
        }
    }

    // add pools
    cfp poolAttemptsPerChunk = chunkSize.volume() / (fp)0x100;
    cint count = roundRandom(generateIn.chunkRandom, poolAttemptsPerChunk);
    for (int attempt = 0; attempt < count; attempt++)
    {
        // more chance on pools the lower you get
        cfp &heightPart = math::squared(randFp(generateIn.chunkRandom));
        cveci2 &pos = generateIn.worldPos +
                      cveci2(randIndex(generateIn.chunkRandom, (int)chunkSize.x),
                             randIndex(generateIn.chunkRandom, (int)chunkSize.y));

        // more lava at the bottom at warmer biomes
        cfp &temperature = ((1 - heightPart) + 0.5) * 0.5; // biomeTemperatureNoise->evaluate(vec1(pos.x))) * 0.5;

        const blockID poolBlock = (temperature > randFp()) ? blockID::lava : blockID::water;

        // place pool
        addFluidPool(pos, poolBlock, 0x20);
    }

    // add structures
    for (size_t i = 0; i < chunkSize.x; i++)
    {
        if ((data->heights[i] >= generateIn.worldPos.y) &&
            (data->heights[i] < (generateIn.worldPos.y + (int)chunkSize.y)))
        {
            biomeID biomeToUse = data->biomes[i];

            if ((data->heights[i] < seaLevel) && (biomeToUse != biomeID::swamp))
            {
                biomeToUse = biomeID::ocean;
            }
            currentWorld->biomeList[(int)biomeToUse]->attemptgenerateStructures(this,
                                                                                cveci2((int)i +
                                                                                           generateIn.worldPos.x,
                                                                                       data->heights[i]),
                                                                                generateIn.chunkRandom);
        }
    }
    // generate ores
    // attempts per 256 blocks

    csize_t minecraftSpawnAttemptsPerChunk[overWorldOreTypeCount]{
        10,
        8,
        10,
        10,
        10,
        20,
        20,
        1,
        1,
        8,
        2,
        1,
    };
    cint maxVeinSize[overWorldOreTypeCount]{
        33,
        33,
        33,
        33,
        33,
        17,
        9,
        1,
        7,
        8,
        9,
        8,
    };

    cint &compressionLevel = math::maximum(-generateIn.worldPos.y, 0);

    // coal has more chance to convert to diamond when deeper
    // gold is weighty, so it has more chance to be lower

    cint &halfTreshold = 0x1000;

    // the amount of coal that converts to diamond
    cfp &diamondConversionMultiplier = (1 -
                                        (halfTreshold / (fp)(compressionLevel + halfTreshold)));

    cint &summedSpawnAttempts =
        (int)minecraftSpawnAttemptsPerChunk[std::find(overWorldOreList, overWorldOreList + overWorldOreTypeCount,
                                                      blockID::coal_ore) -
                                            overWorldOreList] +
        (int)minecraftSpawnAttemptsPerChunk[std::find(overWorldOreList, overWorldOreList + overWorldOreTypeCount,
                                                      blockID::diamond_ore) -
                                            overWorldOreList];

    for (size_t i = 0; i < overWorldOreTypeCount; i++)
    {
        const blockID &currentOre = overWorldOreList[i];

        fp currentOreRarity;

        if (currentOre == blockID::coal_ore)
        {
            currentOreRarity = summedSpawnAttempts * (1 - diamondConversionMultiplier);
        }
        else if (currentOre == blockID::diamond_ore)
        {
            currentOreRarity = summedSpawnAttempts * diamondConversionMultiplier;
        }
        else
        {
            currentOreRarity = (fp)minecraftSpawnAttemptsPerChunk[i];
        }

        currentOreRarity *= chunkOreMultiplier;

        csize_t &count = roundRandom(generateIn.chunkRandom, currentOreRarity);

        for (size_t j = 0; j < count; j++)
        {
            veci2 depositPosition = generateIn.worldPos +
                                    veci2(rand(generateIn.chunkRandom, (int)chunkSize.x - 1),
                                          rand(generateIn.chunkRandom, (int)chunkSize.x - 1));

            addOres(depositPosition, currentOre, rand(generateIn.chunkRandom, maxVeinSize[i]),
                    generateIn.chunkRandom, {blockID::stone});
        }
    }
}

void overWorld::renderSky(crectangle2 &blockRect, crectangle2 &drawRect,
                          const gameRenderData &targetData) const
{
    cvec2 &position = blockRect.getCenter();
    cfp height = position.y;
    cfp temperature = getTemperature(position); // biomeTemperatureNoise->evaluate(vec1(position.x));
    // cfp humidity = 0.5;                         // biomeHumidityNoise->evaluate(vec1(position.x));
    cfp cloudThickness = cloudThicknessNoise->evaluate(cvec1(currentWorld->currentTime));

    // color(40, 94, 181);
    const colorf &airColorHSV = colorf(
        // math::lerp((fp)215, (fp)230, math::clamp((fp)(2 - temperature * 3), (fp)0, (fp)1)),
        math::mapValue(temperature, (fp)-20, (fp)40, (fp)215, (fp)230),
        math::clamp(
            math::mapValue(height, (fp)seaLevel, (fp)maximumHillHeight, (fp)0.8, (fp)0),
            (fp)0, (fp)1),
        math::clamp(math::minimum(
                        math::mapValue(height, (fp)seaLevel, (fp)maximumHillHeight, (fp)1, (fp)0.4),
                        math::lerp((fp)1, (fp)0.7, math::squared(cloudThickness))),
                    (fp)0, (fp)1));
    const color airColor = color(hsv2rgb(airColorHSV));

    if (settings::videoSettings::currentGraphicsMode != graphicsMode::fast)
    {
        // draw celestial bodies
        // render sun
        cfp &timeOfDay = currentWorld->getTimeOfDay();
        cfp &sunAngle = ((timeOfDay - noon) / (fp)ticksPerDay) * math::PI2;

        cvec2 &sunPosition = cvec2(targetData.renderTarget.size.x * 0.5, 0) +
                             vec2::getrotatedvector(sunAngle) * targetData.renderTarget.size.x *
                                 0.5;
        cfp &sunSize = targetData.renderTarget.size.x * 0.2;
        crectangle2 &sunRect = crectangle2(sunPosition, cvec2()).expanded(sunSize * 0.5);

        rectangle2 croppedSunRect = sunRect;
        if (drawRect.cropClientRect(croppedSunRect))
        {
            const auto &backGroundToSun = transformBrush(
                mat3x3::fromRectToRect(sunRect, crectangle2(0, 0, sunTexture->defaultSize.x,
                                                            sunTexture->defaultSize.y)),
                *sunTexture);

            const auto& airBrush = solidColorBrush(airColor);
            const auto &maximizer = colorMaximizer(
                airBrush, backGroundToSun);
            fillRectangle(targetData.renderTarget, ceilRectangle(croppedSunRect), colorMixer(targetData.renderTarget, maximizer));

            if (cloudThickness > 0.7) // raining
            {
                /*cfp velocity = targetData.worldToRenderTargetTransform.multSizeMatrix(cvec1(TerminalGravityVelocityAirPerTick)).x;

                const auto& weatherBrush = repeatingBrush<resolutionTexture>(*rainTexture);
                const auto& movingWeatherBrush = transformBrush<repeatingBrush<resolutionTexture>>(mat3x3::cross(mat3x3::translate(cvec2(0, currentWorld->ticksSinceStart * velocity)), mat3x3::scale(cvec2(0.25))), weatherBrush);
                fillTransparentRectangle(rect, movingWeatherBrush, targetData.renderTarget);
                */
            }
        }
    }

    fillRectangle(targetData.renderTarget, ceilRectangle(drawRect), colorMixer(targetData.renderTarget, solidColorBrush(airColor)));
}