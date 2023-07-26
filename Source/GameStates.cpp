#include "GameStates.h"
#include "Constants.h"

void PlayingState::enter(GameData& gameData) {
	setGameData(gameData);

	createCharacter(gameData, "characterDemon", 100, false, 300);
	
	gameData.player->position.x = Constants::RESOLUTION_X / 2;
	gameData.player->position.y = Constants::RESOLUTION_Y / 2;
	
	// Set starting enemy to spawn
	gameData.currentEnemy = gameData.entityImageFileUMap["enemyBatAnimated"];

	// Map Tiles
	gameData.tileTypeArray[TILE_GRASS] = loadImage("Assets/grassTile.png", 1);
	gameData.tileTypeArray[TILE_DIRT] = loadImage("Assets/dirtTile.png", 1);
	gameData.tileTypeArray[TILE_ROCK] = loadImage("Assets/rockTile.png", 1);

	createCharacter(gameData, "characterDemon", 100, false, 300);
	
	gameData.player->position.x = Constants::RESOLUTION_X / 2;
	gameData.player->position.y = Constants::RESOLUTION_Y / 2;

	gameData.soloud.init();
}

void PlayingState::update() {

}

void PlayingState::exit() {
}
