#pragma once
#define NOMINMAX
#include <soloud.h>
#include <soloud_wav.h>
#include <soloud_audiosource.h>
#include <unordered_map>

#include "Entity.h"

enum TileType {
	TILE_ROCK,
	TILE_DIRT,
	TILE_GRASS,

	TILE_COUNT
};

enum EntityType {
	ENTITY_PLAYER,
	ENTITY_ENEMY,

	ENTITY_COUNT
};

enum DamageNumberType {
	DN_FALLING,
	DN_ASCENDING,

	DN_COUNT
};

struct DamageNumber {
	std::string				damageString;
	int						damageDealt;
	Vector					position;
	Vector					velocity;
	int						textSize = 0;
	double					lifeTime = 0;
	EntityType				entityType;
	DamageNumberType		damageNumberT;
	double					numberDelay = 0.0;
	bool					soundPlayed = false;
};

class Staff;
class Spell;

struct Camera {
	Vector					position;
	Vector					size;
	// Viewing region (16:9) size;
	// Figure out the window size every frame using the aspect ratio
};

struct Tile {
	TileType				tileType;
	Vector					position;
};

class GameData {
	public:
		Character*										player;
		Camera											camera;
		Image											currentEnemy;
		std::vector<Enemy>								enemies;
		std::vector<DeathAnimation>						deathAnimations;
		std::vector<uint32_t>							magicSwordEnemyIds;
		std::vector<Spell*>								spells;
		std::vector<DamageNumber>						damageNumbers;
		std::vector<ExperienceOrb>						experienceOrbs;
		Image											tileTypeArray[TILE_COUNT];
		SoLoud::Soloud									soloud;
		std::unordered_map<std::string, SoLoud::Wav>	soundFileUMap;
		std::unordered_map<std::string, Image>			entityImageFileUMap;
};

void myMemcpy(void* destination, void const* source, size_t size);

double distancePlayer(Vector a, Vector b);

Image loadImage(const char* fileName, int frames);

Image loadFont(const char* fileName);

float randomFloat(float min, float max);

double dotProduct(Vector a, Vector b);

double ClosestRotation(double current, double target);

double sign(double value);

void drawTile(GameData& gameData, Tile tile, float perlin);

void drawCircle(GameData& gameData, Vector position, double radius, int circleOffsetY);

void drawString(Color color, Image* textImage, int size, std::string string, double x, double y);

void drawStringWorldSpace(Color color, GameData& gameData, Image* textImage, int size, std::string string, int x, int y);

DamageNumber createDamageNumber(EntityType type, DamageNumberType damageNumberT, double numberDelay, int damageNumber, Vector position, Vector velocity, int textSize, double lifeTime);

std::string loadDamageNumberHitSound(GameData& gameData);

void playEnemyHitSound(GameData& gameData);

void drawDamageNumber(GameData& gameData, DamageNumber& damageNumber, Image* textImage, double deltaTime);

void drawFilledRectangle(R_Rect* rect, int red, int green, int blue, int alpha);

void drawNonFilledRectangle(R_Rect* rect, int red, int green, int blue, int alpha);

void addImage(GameData& gameData, std::string name, const char* fileName, int frames);

void loadImages(GameData& gameData);