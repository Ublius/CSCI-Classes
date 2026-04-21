// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "CircleComponent.h"
#include "Actor.h"

CircleComponent::CircleComponent(class Actor* pActor)
: Component(pActor)
,_radius(0.0f) {
	
}

const Vector2& CircleComponent::GetCenter() const {
	return _pActor->GetPosition();
}

float CircleComponent::GetRadius() const {
	return _pActor->GetScale() * _radius;
}

bool CircleComponent::Intersect(const CircleComponent& other) {
    // Calculate distance squared between this CircleComponet
    // and other.
    Vector2 diff = GetCenter() - other.GetCenter();
    float distSq = diff.LengthSq();

	// Calculate the sum of the radii squared.
    float radiiSq = GetRadius() + other.GetRadius();
    radiiSq *= radiiSq;

    // Return whether or not the distance squared is less
    // than the radii squared - if it is, Intersect of these
    // two CircleComponets is true.
    return distSq <= radiiSq;;
}
