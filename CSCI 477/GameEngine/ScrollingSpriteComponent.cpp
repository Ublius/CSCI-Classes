#include "ScrollingSpriteComponent.h"

ScrollingSpriteComponent::ScrollingSpriteComponent(Actor* pActor) 
  : SpriteComponent(pActor)
{

}

ScrollingSpriteComponent::~ScrollingSpriteComponent() {

}

void ScrollingSpriteComponent::SetTextures(const std::vector<SDL_Texture*>& vecTextures) {
  int count = 0;

  for (SDL_Texture* tex: vecTextures) {
    BGTexture bg;
    bg.pTexture = tex;
    bg.offset.x = count * 1024;
    bg.offset.y = 0;

    count++;
    _vecBGTextures.push_back(bg);
  }
}

void ScrollingSpriteComponent::Update(float deltaTime) {
  SpriteComponent::Update(deltaTime);

  for (BGTexture& bg : _vecBGTextures) {
    bg.offset.x += _scrollSpeed * deltaTime;
    if (bg.offset.x < -1024) {
      bg.offset.x = (_vecBGTextures.size() - 1 ) * 1024 - 1;
    }
  }
}

void ScrollingSpriteComponent::Draw(SDL_Renderer* pRenderer) {
    for (BGTexture bg : _vecBGTextures) {
        SDL_Rect r;
        // Scale the width/height by owner's scale
        r.w = static_cast<int>(1024 * _pActor->GetScale());
        r.h = static_cast<int>(768 * _pActor->GetScale());
        
        // Center the rectangle around the position of the owner
        r.x = static_cast<int>(bg.offset.x);
        r.y = static_cast<int>(bg.offset.y);

        // Draw (have to convert angle from radians to degrees, and clockwise to counter)
        SDL_RenderCopyEx(pRenderer,
            bg.pTexture,
            nullptr,
            &r,
            -Math::ToDegrees(_pActor->GetRotation()),
            nullptr,
            SDL_FLIP_NONE);
    }
}
