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

Background::Background(Game* pGame)
:Actor(pGame) {
    SetPosition(Vector2(512.0f, 384.0f));
    
    // Create a background
    SpriteComponent* bg = new SpriteComponent(this);
    bg->SetTexture(pGame->GetTexture("Assets/Farback01.png"));
}
