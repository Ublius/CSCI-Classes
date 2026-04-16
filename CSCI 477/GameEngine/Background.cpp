//
//  Background.cpp
//  Game-mac
//
//  Created by Kathleen Kelly on 4/7/23.
//  Copyright © 2023 Sanjay Madhav. All rights reserved.
//

#include "Background.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "ScrollingSpriteComponent.h"

Background::Background(Game* pGame)
:Actor(pGame) {
    SetPosition(Vector2(512.0f, 384.0f));
    
    // Create a background
    // SpriteComponent* bg = new SpriteComponent(this);
    // bg->SetTexture(pGame->GetTexture("Assets/Farback01.png"));

    ScrollingSpriteComponent* bg = new ScrollingSpriteComponent(this);
    std::vector<SDL_Texture*> textures = {
      pGame->GetTexture("Assets/Farback01.png"),
      pGame->GetTexture("Assets/Farback02.png")
    };
    bg->SetTextures(textures);
    bg->SetScrollSpeed(-100);

    ScrollingSpriteComponent* bg2 = new ScrollingSpriteComponent(this);
    std::vector<SDL_Texture*> starTextures = {
      // pGame->GetTexture("Assets/Stars");
    };
}
