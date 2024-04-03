#pragma once
#include "Game.h"
#include "InputAndRendering.h"
/*
class BaseState {
private:
	GameData& mGameData;
public:
	BaseState(GameData& gameData) : mGameData(gameData) {}
	virtual ~BaseState() {}

	virtual void enter(GameData& gameData) = 0;
	virtual void update() = 0;
	virtual void exit() = 0;

	GameData& getGameData() {
		return mGameData;
	}
};

class MenuState : public BaseState {
	
};

class PlayingState : public BaseState {
public:
	PlayingState(GameData& gameData) : BaseState(gameData) {}
		
	// Call a separate renderer system inside the update method for renderering
	void enter(GameData& gameData) override;
	void update() override;
	void exit() override;
};

class GameOverState : public BaseState {

};

class GameManager {
private:
	RenderingSystem		mRenderer;
	InputHandler		mInputHandler;
	BaseState*			mCurrentState;
	GameData&			mGameData;
public:
	GameManager(GameData& gameData) : mCurrentState(nullptr), mGameData(gameData){}

	void changeState(BaseState* newState) {
		if (mCurrentState) {
			mCurrentState->exit();
			delete mCurrentState;
		}
		mCurrentState = newState;
		mCurrentState->enter(getGameData());
	}

	GameData& getGameData() {
		return mGameData;
	}

	void render();
};
;
*/
