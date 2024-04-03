#pragma once
#include "SDL.h"
/*
class RenderingSystem {
public: 
void renderMenu() {
}
void renderPlaying() {
}
void renderGameOver() {
}
};

class InputHandler {
private:
    std::unordered_map<SDL_Keycode, bool> keys;
    bool running = true; // assuming we start running
public:
    void update() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYUP:
                keys[event.key.keysym.sym] = false;
                break;
            case SDL_KEYDOWN:
                keys[event.key.keysym.sym] = true;
                break;
            }
        }
    }

    bool isKeyPressed(SDL_Keycode key) {
        return keys[key];
    }
};
*/