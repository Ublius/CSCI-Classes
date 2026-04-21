// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"
#include <algorithm>
#include "Background.h"
#include "Asteroid.h"

Game::Game()
:_pWindow(nullptr)
,_pRenderer(nullptr)
,_isRunning(true) {
    
}

bool Game::Initialize() {
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }
    
    // Create generic window.
    _pWindow = SDL_CreateWindow("Window Title", 100, 100, 1024, 768, 0);
    if (!_pWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }
    
    // Create window renderer.
    _pRenderer = SDL_CreateRenderer(_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!_pRenderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }
    
    // Make sure SDL can initialize images to be used.
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
        return false;
    }

    // Game-specific data loading.
    _loadData();

    // Initialize tick count.
    _ticksCount = SDL_GetTicks();
    
    return true;
}

void Game::RunLoop() {
    // Main game loop.
    while (_isRunning) {
        _processInput();
        _updateGame();
        _generateOutput();
    }
}

// Get any input from the keyboard/mouse.
void Game::_processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            // X-button on window.
            case SDL_QUIT:
                _isRunning = false;
                break;
        }
    }
    
    // Get full keyboard state in an array.
    const Uint8* state = SDL_GetKeyboardState(NULL);
    
    // Escape key exits game.
    if (state[SDL_SCANCODE_ESCAPE]) {
        _isRunning = false;
    }

    // TODO: Process any game-specific input.
}

// Update every actor/component in the game to any new
// transforms (position, rotation, scale).
void Game::_updateGame() {
    // Compute delta time
    // Wait until 16ms has elapsed since last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), _ticksCount + 16))
        ;

    // Cap deltaTime to help with smoothness.
    float deltaTime = (SDL_GetTicks() - _ticksCount) / 1000.0f;
    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }
    _ticksCount = SDL_GetTicks();

    // Update all actors in the game.
    for (auto actor : _vecActors) {
        actor->Update(deltaTime);
    }
    
    // Delete any dead actors.
    // Add any dead actors to a temp vector
    std::vector<Actor*> deadActors;
    for (auto actor : _vecActors) {
        if (actor->GetState() == Actor::EDead) {
            deadActors.push_back(actor);
        }
    }

    // Delete dead actors (which removes them from _actors)
    for (auto actor : deadActors) {
        delete actor;
    }
}

// Render output of this frame.
void Game::_generateOutput() {
    // Clear the screen.
    SDL_SetRenderDrawColor(_pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(_pRenderer);
    
    // Draw all sprite components
    for (auto actor : _vecActors) {
        actor->Draw(_pRenderer);
    }

    // Present the screen.
    SDL_RenderPresent(_pRenderer);
}

void Game::_loadData() {
    // TODO: Add game specific loading code here.
    Background* bg = new Background(this);
    
    Asteroid* ast = new Asteroid(this);
}

// Cleanup of any actors and textures.
void Game::_unloadData() {
    // Delete actors
    // Because ~Actor calls RemoveActor, have to use a different style loop
    while (!_vecActors.empty()) {
        delete _vecActors.back();
    }

    // Destroy textures
    for (auto i : _mapTextures) {
        SDL_DestroyTexture(i.second);
    }
    _mapTextures.clear();
}

// Find a texture in the texture map.
SDL_Texture* Game::GetTexture(const std::string& fileName) {
    SDL_Texture* tex = nullptr;
    
    // Is the texture already in the map?
    auto iter = _mapTextures.find(fileName);
    if (iter != _mapTextures.end()) {
        tex = iter->second;
    }
    else {
        // Load from file
        SDL_Surface* surf = IMG_Load(fileName.c_str());
        if (!surf) {
            SDL_Log("Failed to load texture file %s", fileName.c_str());
            return nullptr;
        }

        // Create texture from surface
        tex = SDL_CreateTextureFromSurface(_pRenderer, surf);
        SDL_FreeSurface(surf);
        if (!tex) {
            SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
            return nullptr;
        }

        // Add it to the map.
        _mapTextures.emplace(fileName.c_str(), tex);
    }
    return tex;
}

// Cleanup game and SDL.
void Game::Shutdown() {
    _unloadData();
    IMG_Quit();
    SDL_DestroyRenderer(_pRenderer);
    SDL_DestroyWindow(_pWindow);
    SDL_Quit();
}

// Add an Actor to the game list.
void Game::AddActor(Actor* actor) {
    _vecActors.push_back(actor);
}

// Remove an actor from the game list.
void Game::RemoveActor(Actor* actor) {
    // Is it in actors?
    auto iter = std::find(_vecActors.begin(), _vecActors.end(), actor);
    if (iter != _vecActors.end()) {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, _vecActors.end() - 1);
        _vecActors.pop_back();
    }
}
