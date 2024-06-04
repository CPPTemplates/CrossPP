#include <memory>
#include "math/graphics/resolutiontexture.h"
#include "itemID.h"
#include "armorTier.h"
#include "dimensionID.h"
#include "itemID.h"
#include "textureList.h"
#include "minecraftFont.h"

resolutionTexture* mainMenuBackgroundTexture = nullptr;
resolutionTexture* widgetsTexture = nullptr;
resolutionTexture* iconsTexture = nullptr;
resolutionTexture* experienceTexture = nullptr;
resolutionTexture* grassOverlay = nullptr;
resolutionTexture* woolOverlay = nullptr;
resolutionTexture* endPortalFrameEyeTexture = nullptr;
resolutionTexture* endSkyTexture = nullptr;
resolutionTexture* endCrystalTexture = nullptr;
resolutionTexture* endCrystalBeamTexture = nullptr;
resolutionTexture* fireChargeTexture = nullptr;
resolutionTexture* dirtTexture = nullptr;
resolutionTexture* barsTexture = nullptr;
resolutionTexture* rainTexture = nullptr;
resolutionTexture* snowTexture = nullptr;
resolutionTexture* moonPhasesTexture = nullptr;
resolutionTexture* sunTexture = nullptr;
resolutionTexture* potionOverlayTexture = nullptr;
resolutionTexture* minecartTexture = nullptr;
resolutionTexture* brewingStandBaseTexture = nullptr;
resolutionTexture* brewingStandTopTexture = nullptr;
resolutionTexture* enchantedItemTexture = nullptr;
resolutionTexture* unLitRedstoneTorchTexture = nullptr;
resolutionTexture* attachedStemTextures[stemPlantTypeCount]{};
resolutionTexture* unAttachedStemTextures[stemPlantTypeCount]{};
resolutionTexture* redstoneLampOnTexture = nullptr;
resolutionTexture* furnaceOnTextures[furnaceTypeCount]{};
resolutionTexture* verticalDispenserTextures[2]{};
resolutionTexture* lightMapTextures[(int)dimensionID::count]{};
resolutionTexture* armorTextures[armorTierCount]{};
resolutionTexture* armorLegTextures[armorTierCount]{};
resolutionTexture* chatButtonTexture = nullptr;
resolutionTexture* settingsButtonTexture = nullptr;
resolutionTexture* inventoryButtonTexture = nullptr;
resolutionTexture* biomeTexture = nullptr;
resolutionTexture* tempMapTexture = nullptr;
resolutionTexture* snowyGrassBlockTexture = nullptr;

resolutionTexture* boatTextures[normalTreeTypeCount];
std::vector<resolutionTexture* > destroyStageTextures = std::vector<resolutionTexture* >();

resolutionTexture* missingTexture = nullptr;

resolutionTexture* respawnAnchorTextures[respawnAnchorMaximumCharge + 1];


resolutionTexture* anvilTextures[anvilDamageLevelCount];


resolutionTexture* frostedIceAgeTextures[frostedIceMeltTreshold]{};
