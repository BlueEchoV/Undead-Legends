#pragma once
#include <vector>
#include <string>
#include "Game.h"
#include "Constants.h"

class GameData;

class Spell {
	protected:
		GameData&				mGameData;
		Vector					mPosition;
		Vector					mVelocity;

		int						mDamage = 0;
		int						mNumberOfHits = 1;
		int						mPiercingLayers = 0;
		int						mAmmo = 1;
		int						mCurrentFrameSelected = 0;

		double					mAngle = 0.0;
		double					mLifeTime = 0.0;
		double					mRadius = 0.0;
		double					mKnockbackDistance = 0.0;
		double					mAOEAttackDelay = 0.0;
		double					mCastSound = 0.0;
		double					mSpellAttackDelay = 0.0;
		double					mAmmoAttackSpeed = 0.0;
		double					mTimeUntilDamageDealt = 0.0;
		double					mDelayUntilAttack = 0.0;

		bool					mImageLoaded = false;
		Image					mSpellImage;

		int						mSpellSpeed = 0;

		std::string				mSpellKey;
		std::string				mEnemyHitKey;
		std::vector<uint32_t>	mEnemyIds;
		uint32_t				mSingleEnemyId;

		std::string				mSpellType = "";

	public:
		Spell(GameData& gameData) : mGameData(gameData) {}

		virtual ~Spell() = default;

		void setVelocity(Vector velocity) {
			mVelocity = velocity;
		}
		Vector getVelocity() {
			return mVelocity;
		}
		virtual void setSpellAttackDelay(double spellAttackDelay) {
			mSpellAttackDelay = spellAttackDelay;
		}
		virtual double getSpellAttackDelay() {
			return mSpellAttackDelay;
		}
		virtual void setPosition(Vector position) = 0;
		virtual void setTarget(int spellSpeed);
		virtual void setDamage(int damage) {
			mDamage = damage;
		}
		virtual void setPiercingLayers(int piercingLayers) {
			mPiercingLayers = piercingLayers;
		}
		virtual void setLifeTime(double lifeTime) {
			mLifeTime = lifeTime;
		}
		virtual void setKnockBackDistance(double knockbackDistance) {
			mKnockbackDistance = knockbackDistance;
		}
		virtual void setAOEAttackDelay(double aOEAttackDelay) {
			mAOEAttackDelay = aOEAttackDelay;
		}
		virtual void setNumberOfHits(int numberOfHits) {
			mNumberOfHits = numberOfHits;
		}
		void setDelayUntilAttack(double delayUntilAttack) {
			mDelayUntilAttack = delayUntilAttack;
		}

		virtual double getAOEAttackDelay() {
			return mAOEAttackDelay;
		}

		virtual int getPiercingLayers() {
			return mPiercingLayers;
		}
		virtual double getLifeTime() {
			return mLifeTime;
		}
		virtual double getKnockbackDistance() {
			return mKnockbackDistance;
		}
		virtual Vector getPosition() {
			return mPosition;
		}

		void setSpellType(std::string newSpellType) {
			mSpellType = newSpellType;
		}
		std::string getSpellType() {
			return mSpellType;
		}

		virtual void activateKnockback(Enemy* enemyTargeted);

		virtual Image* getImage(GameData& gameData) = 0;
		virtual void draw(GameData& gameData);
		// TODO: Make pure virtual function
		virtual void loadCastSound();
		virtual void playCastSound();
		virtual void loadEnemyHitSound();
		virtual void playEnemyHitSound();

		virtual double currentLifeTime(double deltaTime) {
			return mLifeTime -= deltaTime;
		}
		virtual void updatePosition(double deltaTime) {
			REF(deltaTime);
		}

		virtual bool canDamage(Enemy* enemyTargeted) {
			if (enemyTargeted->hp <= 0) {
				return false;
			}
			for (uint32_t id : mEnemyIds) {
				if (id == enemyTargeted->mId) {
					return false;
				}
			}
			return true;
		}
		virtual int applyDamage(Enemy* enemyTargeted) {
			if (getPiercingLayers() == 0) {
				mLifeTime = 0;
			}
			else {
				--mPiercingLayers;
			}

			enemyTargeted->hp -= mDamage;
			return mDamage;
		}

		virtual void collision(Enemy* enemyTargeted);


};

class SpikeSpell : public Spell {
	public:
		SpikeSpell(GameData& gameData) : Spell(gameData) {}
		Image* getImage(GameData& gameData) override;
		void setPosition(Vector position) override;
		void updatePosition(double deltaTime) override {
			if (currentLifeTime(deltaTime) > 0) {
				mPosition = mPosition + (mVelocity * deltaTime);
			}
		}
		void loadCastSound() override;
		void playCastSound() override;
};

class ShadowOrbSpell : public Spell {
	public:
		ShadowOrbSpell(GameData& gameData) : Spell(gameData) {}
		void setPosition(Vector position) override;
		Image* getImage(GameData& gameData) override;
		void updatePosition(double deltaTime) override {
			if (currentLifeTime(deltaTime) > 0) {
				mPosition = mPosition + (mVelocity * deltaTime);
			}
		}
};

class ConsecratedGroundSpell : public Spell {
	public:
		ConsecratedGroundSpell(GameData& gameData) : Spell(gameData) {}
		void setPosition(Vector position) override;
		Image* getImage(GameData& gameData) override;
		void updatePosition(double deltaTime) override {
			REF(deltaTime);
			mPosition = mGameData.player->position;
		}
		virtual bool canDamage(Enemy* enemyTargeted) override {
			if (enemyTargeted->hp > 0) {
				if (enemyTargeted->timeUntilDamageTaken <= 0) {
					return true;
				}
			}
			return false;
		}
		virtual int applyDamage(Enemy* enemyTargeted) override {
			enemyTargeted->timeUntilDamageTaken = getAOEAttackDelay();
			setPiercingLayers(-1);
			return Spell::applyDamage(enemyTargeted);
		}
};

class FireballSpell : public Spell {
	public:
		// Constructor
		FireballSpell(GameData& gameData) : Spell(gameData) {}
		Image* getImage(GameData& gameData) override;
		void setPosition(Vector position) override;
		void updatePosition(double deltaTime) override {
			if (currentLifeTime(deltaTime) > 0) {
				mPosition = mPosition + (mVelocity * deltaTime);
			}
		}
		virtual int applyDamage(Enemy* enemyTargeted) override;
};

class FireAOESpell : public Spell {
	public:
		bool firstHit = true;
		// Constructor
		FireAOESpell(GameData& gameData) : Spell(gameData) {
			mSpellAttackDelay = 0.5;
		}
		Image* getImage(GameData& gameData) override;
		void setPosition(Vector position) override;
		void updatePosition(double deltaTime) override {
			currentLifeTime(deltaTime);
		}
		virtual bool canDamage(Enemy* enemyTargeted) override {
			if (enemyTargeted->hp > 0) {
				if (enemyTargeted->timeUntilDamageTaken <= 0) {
					return true;
				}
			}
			return false;
		}
		virtual int applyDamage(Enemy* enemyTargeted) override {
			// So the aoe doesn't attack right away
			if (firstHit) {
				firstHit = false;
				enemyTargeted->timeUntilDamageTaken = getAOEAttackDelay();
				// Return zero damage taken
				return 0;
			}
			else {
				enemyTargeted->timeUntilDamageTaken = getAOEAttackDelay();
				setPiercingLayers(-1);
				return Spell::applyDamage(enemyTargeted);
			}
		}
};

class MagicSwordSpell : public Spell {
	public:
		double			mTurnSpeed = 0.0;
		bool			mFindNewTarget = false;

		// Constructor
		MagicSwordSpell(GameData& gameData) : Spell(gameData) {}
		Image* getImage(GameData& gameData) override;
		void setPosition(Vector position) override;

		void setTarget(int spellSpeed) override;
		void setTurnSpeed(double turnSpeed) {
			mTurnSpeed = turnSpeed;
		}

		void updatePosition(double deltaTime) override;

		virtual bool canDamage(Enemy* enemyTargeted) {
			if (enemyTargeted->hp <= 0) {
				return false;
			}
			if (mTimeUntilDamageDealt <= 0) {
				if (mSingleEnemyId == enemyTargeted->mId) {
					return true;
				}
			}
			return false;
		}

		virtual int applyDamage(Enemy* enemyTargeted) override {
			enemyTargeted->hp -= mDamage;
			if (enemyTargeted->hp <= 0) {
				mFindNewTarget = true;
			}
			else {
				mTimeUntilDamageDealt = mDelayUntilAttack;
			}
			return mDamage;
		}

};

class GreenDiamondSpell : public Spell {
	public:
		bool				mExplosionPushedBack = false;

		GreenDiamondSpell(GameData& gameData) : Spell(gameData) {}
		void setPosition(Vector position) override;
		Image* getImage(GameData& gameData) override;
		void updatePosition(double deltaTime) override;
		virtual bool canDamage(Enemy* enemyTargeted) override;
		virtual int applyDamage(Enemy* enemyTargeted) override {
			REF(enemyTargeted);
			return 0;
		}
};

class GreenDiamondExplosionSpell : public Spell {
	public:
		GreenDiamondExplosionSpell(GameData& gameData) : Spell(gameData) {
			mSpellAttackDelay = 1;
		}
		void setPosition(Vector position) override;
		Image* getImage(GameData& gameData) override;
		void updatePosition(double deltaTime) override;
		virtual bool canDamage(Enemy* enemyTargeted) {
			if (enemyTargeted->hp > 0) {
				if (enemyTargeted->timeUntilDamageTaken <= 0) {
					return true;
				}
			}
			/*
			if (mTimeUntilDamageDealt <= 0) {
				return true;
			}
			*/
			return false;
		}
		virtual int applyDamage(Enemy* enemyTargeted) override {
			enemyTargeted->timeUntilDamageTaken = getAOEAttackDelay();
			setPiercingLayers(-1);
			return Spell::applyDamage(enemyTargeted);
		}
};