#include "SDL.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>
#include "Sprite.h"
#include <algorithm>
#include <vector>

// DONE: Make hitbox smaller of enemies
// DONE: Make hitbox of player smaller (distancePlayer / drawCirclePlayer function - offset by 20 pixels)
// DONE: Randomly spawn enemies around player / off screen?
// TODO: Visible player health and enemy health on collision with bullet
// TODO: Respawning enemies
// TODO: Enemies spawning in more of waves / groups
// TODO: Adding player, enemy, weapon variety (Possibly through arrays or vectors)
// TODO: Add a map
// TODO: Enemies drop experience
// TODO: Items / experience that can be picked up by the player
// TODO: Text rendering
// TODO: Menu

bool running = true;
bool up = false;
bool down = false;
bool left = false;
bool right = false;
double fireTime = 0;
double ATTACKSPEED = .1;
double PROJECTILESPEED = 500;

int main(int argc, char** argv) {

	SDL_Window* window = nullptr;
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Undead Legends",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		RESOLUTION_X, RESOLUTION_Y, SDL_WINDOW_SHOWN);

	// -1 = default gpu
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	Image mapA = loadImage(renderer, "Assets/Map_1.png");
	// Character_Ghoul_5.png
	// Character_Vampire_4.png
	// Character_Vampire_5_ShortHair.png
	// Character_Maiden_1.png
	// Character_FrankensteinCreation_1.png
	// Character_Skeleton_1.png
	Image characterA = loadImage(renderer, "Assets/Character_Maiden_1.png");
	Image mapTextureGrass = loadImage(renderer, "Assets/Map_Grass_1.png");
	// Assets/Enemy_VampireBat_1.png
	// Assets/Enemy_Gargoyle_1.png
	Image enemyA = loadImage(renderer, "Assets/Enemy_VampireBat_1.png");
	Image weaponSpikeImage = loadImage(renderer, "Assets/Weapon_Spike_1.png");

	GameData gameData;

	gameData.renderer = renderer;

	gameData.player = createCharacter(characterA, 100);
	gameData.player.position.x = RESOLUTION_X / 2;
	gameData.player.position.y = RESOLUTION_Y / 2;

	gameData.tileTypeArray[TILE_GRASS] = loadImage(renderer, "Assets/grassTile.png");
	gameData.tileTypeArray[TILE_DIRT] = loadImage(renderer, "Assets/dirtTile.png");
	gameData.tileTypeArray[TILE_ROCK] = loadImage(renderer, "Assets/rockTile.png");

	int spawnAmount = 50;
	const float DELTA = (M_PI * 2) / spawnAmount;
	int distanceR = 300;

	for (int i = 0; i < spawnAmount; i++) {
		double range = randomFloat(RESOLUTION_X / 2, RESOLUTION_X);
		Vector enemyPosition = facingDirection(randomFloat(0, 360));

		enemyPosition.x *= range;
		enemyPosition.y *= range;

		enemyPosition.x += RESOLUTION_X / 2;
		enemyPosition.y += RESOLUTION_Y / 2;

		createEnemy(enemyA, enemyPosition, &gameData, 100, 2);
	}

	// Capping frame rate
	const int FPS = 60;
	// The max time between each frame
	const int frameDelay = 1000 / FPS;
	// Massive integer
	Uint32 frameStart;
	int frameTime;

	double lastFrameTime = getTime();

	int wCameraSpace = 0;
	int hCameraSpace = 0;

 	while (running) {
		// How many miliseconds it's been since we first
		frameStart = SDL_GetTicks();
		SDL_Event event = {};
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
				case SDLK_w:
					up = false;
					break;
				case SDLK_a:
					left = false;
					break;
				case SDLK_d:
					right = false;
					break;
				case SDLK_s:
					down = false;
					break;
				}
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_w:
					up = true;
					break;
				case SDLK_a:
					left = true;
					break;
				case SDLK_d:
					right = true;
					break;
				case SDLK_s:
					down = true;
					break;
				}
				break;
			}
		}
	
		double currentTime = getTime();

		double deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		fireTime -= deltaTime;
		
		double speed = 2;

		if (left) {
			gameData.player.position.x -= speed;
		}
		if (right) {
			gameData.player.position.x += speed;
		}
		if (down) {
			gameData.player.position.y += speed;
		}
		if (up) {
			gameData.player.position.y -= speed;
		}

		// 1: Check to see if the enemies are colliding with eachother
		if (gameData.player.hp > 0) {
			for (int i = 0; i < gameData.enemies.size(); i++) {			
				updateEnemyPosition(&gameData.player, &gameData.enemies[i], deltaTime);
				for (int j = 0; j < gameData.enemies.size(); j++) {
					if (j == i) {
						continue;
					}
					else {
						// Length
						double distanceBetween = distance(gameData.enemies[i].position, gameData.enemies[j].position);
						double radiusSum = gameData.enemies[i].radius + gameData.enemies[j].radius;
						if (distanceBetween < radiusSum) {
							Vector offset = gameData.enemies[j].position - gameData.enemies[i].position;
							// This is equivalent to offset / length
							offset *= 1 / distanceBetween;							
							offset *= radiusSum;
							gameData.enemies[j].position = offset + gameData.enemies[i].position;
						}
						// float randomNumber = randomFloat(-80, 80);
					}
				}
			}
		}

		// Update weapon position
		for (int i = 0; i < gameData.weaponSpike.size(); i++) {
			updateEntityPosition(&gameData.weaponSpike[i], deltaTime);
		}

		// Weapon spike firing at nearest enemies
		// Check if the player is existing
		if (gameData.player.hp > 0) {
			if (fireTime <= 0) {
				// Find the closest enemy
				int nearestEnemy = closestEnemy(gameData.player, &gameData);
				if (nearestEnemy >= 0) {
					Weapon weaponSpike = createWeapon(weaponSpikeImage, 50);
					weaponSpike.position = gameData.player.position;
					// Vector spikeDirection = facingDirection(weaponSpikeArray[i].sprite.angle);
					Vector offset = {};
					// Calculates the vector from the player to the enemy (enemy <--- player)
					offset = gameData.enemies[nearestEnemy].position - gameData.player.position;
					weaponSpike.velocity = normalize(offset);
					weaponSpike.velocity *= PROJECTILESPEED;
					fireTime = ATTACKSPEED;
					weaponSpike.angle = angleFromDirection(weaponSpike.velocity);
					gameData.weaponSpike.push_back(weaponSpike);
				}
			}
		}

		// Weapon collision with enemy
		for (int i = 0; i < gameData.weaponSpike.size(); i++) {
			for (int j = 0; j < gameData.enemies.size(); j++) {
				double distanceBetween = distance(gameData.weaponSpike[i].position, gameData.enemies[j].position);
				double radiusSum = gameData.weaponSpike[i].radius + gameData.enemies[j].radius;
				if (distanceBetween < radiusSum) {
					if (gameData.enemies[j].hp > 0) {
						gameData.enemies[j].hp -= gameData.weaponSpike[i].damage;
						gameData.weaponSpike[i].lifeTime = 0;
						if (gameData.enemies[j].hp <= 0) {
							gameData.enemies[j].destroyed = true;						
						}

						// Knock back enemies
						gameData.enemies[j].velocity = gameData.weaponSpike[i].velocity;
						double length = sqrt(gameData.enemies[j].velocity.x * gameData.enemies[j].velocity.x + gameData.enemies[j].velocity.y * gameData.enemies[j].velocity.y);
						gameData.enemies[j].velocity.x /= length;
						gameData.enemies[j].velocity.y /= length;
						gameData.enemies[j].velocity = gameData.enemies[j].velocity * 600;
						break;
					}
				}
			}
		}

		bool playerTakingDamage = false;
		
		for (int i = 0; i < gameData.enemies.size(); i++) {
			if (gameData.enemies[i].timeUntilDamage > 0) {
				gameData.enemies[i].timeUntilDamage -= deltaTime;
			}
		}

		// Player collision with enemy
		if (gameData.player.hp > 0) {
			for (int i = 0; i < gameData.enemies.size(); i++) {
				if (gameData.enemies[i].timeUntilDamage <= 0) {
					double distanceBetween = distancePlayer(gameData.player.position, gameData.enemies[i].position);
					double radiusSum = gameData.player.radius + gameData.enemies[i].radius;
					if (distanceBetween < radiusSum) {
						gameData.player.hp -= gameData.enemies[i].damage;
						playerTakingDamage = true;
						gameData.enemies[i].timeUntilDamage = .1;
					}
				}
			}
		}

		// updateTilePosition

		gameData.camera.position = gameData.player.position;

		// SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		// Clear what we are drawing to
		// Anything done before render clear gets erased
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, mapA.texture, NULL, NULL);

		int playerPositionX = gameData.player.position.x;
		int playerPositionY = gameData.player.position.y;

		for (int w = 0; w < (RESOLUTION_X / TILE_SIZE) + 2; w++) {
			for (int h = 0; h < (RESOLUTION_Y / TILE_SIZE) + 2; h++) {
				Tile tile = {};
				double offsetX = gameData.camera.position.x - (RESOLUTION_X / 2);
				double offsetY = gameData.camera.position.y - (RESOLUTION_Y / 2);
				tile.position.x = (w * (double) TILE_SIZE) + (floor(offsetX / TILE_SIZE) * TILE_SIZE);
				tile.position.y = (h * (double) TILE_SIZE) + (floor(offsetY / TILE_SIZE) * TILE_SIZE);
				tile.tileType = (TileType)abs((int)(tile.position.x / TILE_SIZE) % 2);
				drawTile(gameData, tile);
			}
		}

		if (gameData.player.hp > 0) {
			drawEntity(gameData, gameData.player);
			if (playerTakingDamage) {
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				drawCircle(gameData, gameData.player.position, gameData.player.radius, -20);
			}
		}

		for (int i = 0; i < gameData.enemies.size(); i++) {
			// Check to see if the boolean value is true when the enemy was created. If it was, draw it.
			drawEntity(gameData, gameData.enemies[i]);
			// SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			// drawCircle(renderer, enemy[i].sprite.position, enemy[i].radius);
		}

		for (int i = 0; i < gameData.weaponSpike.size(); i++) {
			drawEntity(gameData, gameData.weaponSpike[i]);
			gameData.weaponSpike[i].lifeTime -= deltaTime;
			//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			//drawCircle(renderer, weaponSpikeArray[i].sprite.position, weaponSpikeArray[i].radius);
		}

		// After renderPresent, the frame is over
		SDL_RenderPresent(renderer);

		// [] syntax for lambda
		// erase_if is going over every element and asking you if 
		// you want it to be deleted
		std::erase_if(gameData.enemies, [](const Enemy& enemy) {
			return enemy.destroyed;
			}
		);

		// If lifeTime <= 0, get rid of it
		std::erase_if(gameData.weaponSpike, [](const Weapon& weapon) {
			return weapon.lifeTime <= 0;
			}
		);

		// Calculate the frame time (Home much time it's 
		// taken to get through the loop and update game
		// objects...etc.
		frameTime = SDL_GetTicks() - frameStart;

		// This will delay our frames
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	return 0;
}