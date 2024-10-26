#pragma once
#include <memory>
#include "math/graphics/resolutiontexture.h"
#include "itemID.h"
#include "dimensionID.h"
#include "armorTier.h"
#include "itemID.h"
extern resolutionTexture* brewingStandBaseTexture;
extern resolutionTexture* brewingStandTopTexture;
extern resolutionTexture* mainMenuBackgroundTexture;
extern resolutionTexture* widgetsTexture;
extern resolutionTexture* iconsTexture;
extern resolutionTexture* experienceTexture;
extern resolutionTexture* grassOverlay;
extern resolutionTexture* woolOverlay;
extern resolutionTexture* endPortalFrameEyeTexture;
extern resolutionTexture* endSkyTexture;
extern resolutionTexture* endCrystalTexture;
extern resolutionTexture* endCrystalBeamTexture;
extern resolutionTexture* fireChargeTexture;
extern resolutionTexture* dirtTexture;
extern resolutionTexture* barsTexture;
extern resolutionTexture* rainTexture;
extern resolutionTexture* snowTexture;
extern resolutionTexture* moonPhasesTexture;
extern resolutionTexture* sunTexture;
extern resolutionTexture* potionOverlayTexture;
extern resolutionTexture* minecartTexture;
extern resolutionTexture* brewingStandBaseTexture;
extern resolutionTexture* brewingStandTopTexture;
extern resolutionTexture* enchantedItemTexture;
extern resolutionTexture* unLitRedstoneTorchTexture;
extern resolutionTexture* attachedStemTextures[stemPlantTypeCount];
extern resolutionTexture* unAttachedStemTextures[stemPlantTypeCount];
extern resolutionTexture* redstoneLampOnTexture;
extern resolutionTexture* furnaceOnTextures[furnaceTypeCount];
extern resolutionTexture* verticalDispenserTextures[2];
extern resolutionTexture* lightMapTextures[(int)dimensionID::count];
extern resolutionTexture* armorTextures[armorTierCount];
extern resolutionTexture* armorLegTextures[armorTierCount];
extern resolutionTexture* chatButtonTexture;
extern resolutionTexture* settingsButtonTexture;
extern resolutionTexture* inventoryButtonTexture;
extern resolutionTexture* biomeTexture;
extern resolutionTexture* tempMapTexture;
extern resolutionTexture* snowyGrassBlockTexture;

extern resolutionTexture* boatTextures[normalTreeTypeCount];
extern std::vector<resolutionTexture* > destroyStageTextures;

extern resolutionTexture* missingTexture;

constexpr int respawnAnchorMaximumCharge = 4;
extern resolutionTexture* respawnAnchorTextures[respawnAnchorMaximumCharge + 1];

constexpr int anvilDamageLevelCount = 0x3;

extern resolutionTexture* anvilTextures[anvilDamageLevelCount];



extern resolutionTexture* frostedIceAgeTextures[frostedIceMeltTreshold];