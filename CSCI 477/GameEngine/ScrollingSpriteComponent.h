#pragma once
#include "SpriteComponent.h"

class ScrollingSpriteComponent : public SpriteComponent {
  public:
    ScrollingSpriteComponent(class Actor* pActor);
    ~ScrollingSpriteComponent();

    virtual void Update(float deltaTime) override;
    virtual void Draw(SDL_Renderer* pRenderer) override;

    void SetTextures(const std::vector<SDL_Texture*>& vecTexture);
    void SetScrollSpeed(float speed) { _scrollSpeed = speed; }

  private:
    float _scrollSpeed;

    struct BGTexture {
      SDL_Texture* pTexture;
      Vector2 offset;
    };

    std::vector<BGTexture> _vecBGTextures;
};
