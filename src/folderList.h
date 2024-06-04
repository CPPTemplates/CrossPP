#pragma once
#include <string>
#include "filesystem/filemanager.h"
#include "constants.h"

//folders
const stdPath dataFolder = stdPath(L"data");

const stdPath miscellaneousDataFolder = dataFolder / L"miscellaneous";
const stdPath miscellaneousTexturesFolder = miscellaneousDataFolder / L"textures";
const stdPath miscellaneousMusicFolder = miscellaneousDataFolder / L"music";
const stdPath miscellaneousNetherMusicFolder = miscellaneousMusicFolder / L"nether";
const stdPath fileVersionsFolder = miscellaneousDataFolder / L"fileversions";
const stdPath playerDataFolder = dataFolder / L"player";
const stdPath skinFolder = playerDataFolder / L"skins";
const stdPath screenshotsFolder = playerDataFolder / L"screenshots";

const stdPath resourcePackFolder = dataFolder / L"resourcepacks";

const stdPath assetsFolder = stdPath(L"assets") / L"minecraft";

//optifine
const stdPath optifineFolder = assetsFolder / L"optifine";
const stdPath lightMapFolder = optifineFolder / L"lightmap";

//texts
const stdPath textFolder = assetsFolder / L"texts";

//textures
const stdPath particleDataFolder = assetsFolder / L"particles";
const stdPath generalTextureFolder = assetsFolder / L"textures";
const stdPath particleTexturesFolder = generalTextureFolder / L"particle";
const stdPath guiTextureFolder = generalTextureFolder / L"gui";
const stdPath buttonTextureFolder = guiTextureFolder / L"button";
const stdPath blockTextureFolder = generalTextureFolder / L"block";
const stdPath environmentTextureFolder = generalTextureFolder / L"environment";
const stdPath containerTextureFolder = generalTextureFolder / L"gui" / "container";
const stdPath entityTextureFolder = generalTextureFolder / L"entity";
const stdPath itemTextureFolder = generalTextureFolder / L"item";
const stdPath armorTextureFolder = generalTextureFolder / L"models" / L"armor";
const stdPath miscellaneousTextureFolder = generalTextureFolder / L"misc";
const stdPath particleTextureFolder = generalTextureFolder / L"particle";

//sounds
const stdPath generalSoundFolder = assetsFolder / L"sounds";
const stdPath blockSoundFolder = generalSoundFolder / L"block";
const stdPath ambientSoundFolder = generalSoundFolder / L"ambient";
const stdPath weatherSoundFolder = ambientSoundFolder / L"weather";
const stdPath tileSoundFolder = generalSoundFolder / L"tile";
const stdPath itemSoundFolder = generalSoundFolder / L"item";
const stdPath entitySoundFolder = generalSoundFolder / L"entity";
const stdPath mobSoundFolder = generalSoundFolder / L"mob";
const stdPath miscellaneousSoundFolder = generalSoundFolder / L"random";
const stdPath generalMusicFolder = generalSoundFolder / L"music";
const stdPath gameMusicFolder = generalMusicFolder / L"game";
const stdPath menuMusicFolder = generalMusicFolder / L"menu";
const stdPath waterMusicFolder = gameMusicFolder / L"water";
const stdPath creativeModeMusicFolder = gameMusicFolder / L"creative";
const stdPath netherMusicFolder = gameMusicFolder / L"nether";
const stdPath endMusicFolder = gameMusicFolder / L"end";
const stdPath recordsMusicFolder = generalSoundFolder / L"records";
const stdPath noteSoundFolder = generalSoundFolder / L"note";

//'data packs'
const stdPath resourcePackDataFolder = stdPath(L"data");

//minecraft namespace datapack folders
const stdPath minecraftDataFolder = resourcePackDataFolder / L"minecraft";
const stdPath mainTagFolder = minecraftDataFolder / L"tags";
const stdPath recipeFolder = minecraftDataFolder / L"recipes";

//medieval survival namespace datapack folders
const stdPath msNameSpaceDataFolder = resourcePackDataFolder / L"medieval survival";
const stdPath structureFolder = msNameSpaceDataFolder / L"structures";
const stdPath overworldDataFolder = msNameSpaceDataFolder / L"overworld";

const stdPath lootTablesFolder = minecraftDataFolder / L"loot_tables";
const stdPath blockLootTablesFolder = lootTablesFolder / L"blocks";
const stdPath chestLootTablesFolder = lootTablesFolder / L"chests";
const stdPath entityLootTablesFolder = lootTablesFolder / L"entities";

const stdPath savesFolder = dataFolder / L"saves";
const stdPath serversFolder = dataFolder / L"servers";
const stdPath saveFolderDimensions = stdPath(L"dimensions");
const stdPath saveFolderChunks = stdPath(L"chunks");
const stdPath saveFolderScreenShot = stdPath(L"screenshot.png");
const stdPath saveFolderWorldData = stdPath(L"world data" + nbtFileExtension);

const stdPath minecraftVersionFolder = dataFolder / L"version" / L"1.16.2";
//const stdPath dataPackFolder = dataFolder / L"datapacks";
extern stdPath workingDirectory;