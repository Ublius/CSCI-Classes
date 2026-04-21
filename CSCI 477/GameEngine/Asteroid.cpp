#include "Asteroid.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "Random.h"
#include "MoveComponent.h"

Asteroid::Asteroid(Game* pGame) : Actor(pGame) {
  SpriteComponent* sc = new SpriteComponent(this);
  sc->SetTexture(pGame->GetTexture("Assets/Asteroid.png"));

  Vector2 randPos = Random::GetVector(Vector2::Zero, Vector2(1024, 768));
  SetPosition(randPos);

  // Move on its own
  MoveComponent* mc = new MoveComponent(this);
  mc->SetForwardSpeed(150);
  mc->SetAngularSpeed(1);
} 

Asteroid::~Asteroid() {

}
