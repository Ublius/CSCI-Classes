// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "MoveComponent.h"
#include <cstdint>

class InputComponent : public MoveComponent {
public:
	// Lower update order to update first
	InputComponent(class Actor* owner);

	virtual void ProcessInput(const uint8_t* keyState) override;
	
	// Getters/setters for private variables
	float GetMaxForward() const { return _maxForwardSpeed; }
	float GetMaxAngular() const { return _maxAngularSpeed; }
	int GetForwardKey() const { return _forwardKey; }
	int GetBackKey() const { return _backKey; }
	int GetClockwiseKey() const { return _clockwiseKey; }
	int GetCounterClockwiseKey() const { return _counterClockwiseKey; }

	void SetMaxForwardSpeed(float speed) { _maxForwardSpeed = speed; }
	void SetMaxAngularSpeed(float speed) { _maxAngularSpeed = speed; }
	void SetForwardKey(int key) { _forwardKey = key; }
	void SetBackKey(int key) { _backKey = key; }
	void SetClockwiseKey(int key) { _clockwiseKey = key; }
	void SetCounterClockwiseKey(int key) { _counterClockwiseKey = key; }

private:
	// The maximum forward/angular speeds
	float _maxForwardSpeed;
	float _maxAngularSpeed;
    
	// Keys for forward/back movement
	int _forwardKey;
	int _backKey;
	
    // Keys for angular movement
	int _clockwiseKey;
	int _counterClockwiseKey;
};
