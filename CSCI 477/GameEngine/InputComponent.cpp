// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "InputComponent.h"
#include "Actor.h"

InputComponent::InputComponent(class Actor* owner)
: MoveComponent(owner)
,_forwardKey(0)
,_backKey(0)
,_clockwiseKey(0)
,_counterClockwiseKey(0) {
	
}

void InputComponent::ProcessInput(const uint8_t* keyState) {
	// Calculate forward/backward speed for MoveComponent
    // based on forward/backward custom keys.
	float forwardSpeed = 0.0f;
	if (keyState[_forwardKey]) {
		forwardSpeed += _maxForwardSpeed;
	}
	if (keyState[_backKey]) {
		forwardSpeed -= _maxForwardSpeed;
	}
	SetForwardSpeed(forwardSpeed);

    // Calculate angular speed for MoveComponent
    // based on clockwise/counterClockwise custom keys.
    float angularSpeed = 0.0f;
    if (keyState[_clockwiseKey]) {
        angularSpeed += _maxAngularSpeed;
    }
    if (keyState[_counterClockwiseKey]) {
        angularSpeed -= _maxAngularSpeed;
    }
    SetAngularSpeed(angularSpeed);
}
