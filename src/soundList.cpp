#include <memory>
#include "soundCollection.h"
#include "armorTier.h"
std::shared_ptr<soundCollection> popSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> experienceSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> levelUpSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> chestOpenSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> chestCloseSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> enderChestOpenSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> enderChestCloseSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> barrelOpenSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> barrelCloseSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> tillSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> stripLogSound = std::shared_ptr<soundCollection>();

std::shared_ptr<soundCollection> eatSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> burpSound = std::shared_ptr<soundCollection>();

std::shared_ptr<soundCollection> drinkingSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> honeyDrinkingSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> bottleFillSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> bottleEmptySound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> bottleFillDragonBreathSound = std::shared_ptr<soundCollection>();

std::shared_ptr<soundCollection> waterBucketFillSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> lavaBucketFillSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> waterBucketEmptySound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> lavaBucketEmptySound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> milkSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> waterPaddleSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> weakAttackSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> strongAttackSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> criticalAttackSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> enchantSound = std::shared_ptr<soundCollection>();

std::shared_ptr<soundCollection> woodenDoorOpenSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> woodenDoorCloseSound = std::shared_ptr<soundCollection>();

std::shared_ptr<soundCollection> ironDoorOpenSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> ironDoorCloseSound = std::shared_ptr<soundCollection>();

std::shared_ptr<soundCollection> woodenTrapDoorOpenSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> woodenTrapDoorCloseSound = std::shared_ptr<soundCollection>();

std::shared_ptr<soundCollection> ironTrapDoorOpenSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> ironTrapDoorCloseSound = std::shared_ptr<soundCollection>();

std::shared_ptr<soundCollection> fenceGateOpenSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> fenceGateCloseSound = std::shared_ptr<soundCollection>();

std::shared_ptr<soundCollection> flintAndSteelSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> extinguishSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> sheepShearingSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> burningDamageSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> portalDepartSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> portalArriveSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> teleportFromSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> teleportToSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> shootSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> eyeOfEnderLaunchSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> eyeOfEnderDropSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> eyeOfEnderPlaceSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> endPortalOpenSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> fuseSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> explosionSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> enderDragonWingsSound = std::shared_ptr<soundCollection>();

std::shared_ptr<soundCollection> ghastWarningSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> fireBallShootSound = std::shared_ptr<soundCollection>();

std::shared_ptr<soundCollection> endermanStareSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> endermanScreamSound = std::shared_ptr<soundCollection>();

std::shared_ptr<soundCollection> fireSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> enderDragonDeathSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> bowHitSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> brewingFinishSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> clickSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> pistonExtendSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> pistonRetractSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> noteSounds = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> pumpkinCarveSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> toolBreakSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> smithingTableUseSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> anvilUseSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> anvilLandSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> anvilBreakSound = std::shared_ptr<soundCollection>();

std::shared_ptr<soundCollection> smallSlimeSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> bigSlimeSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> slimeAttackSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> windSound = std::shared_ptr<soundCollection>();

std::shared_ptr<soundCollection> equipSound = std::shared_ptr<soundCollection>();
std::shared_ptr<soundCollection> armorEquipSound[armorTierCount];