// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Component.h"
#include <vector>
#include "Math.h"

class MoveComponent : public Component {
public:
	// Lower update order to update first
	MoveComponent(class Actor* pActor, int updateOrder = 10);

	virtual void Update(float deltaTime) override;
	
	float GetAngularSpeed() const { return _angularSpeed; }
	float GetForwardSpeed() const { return _forwardSpeed; }
	void SetAngularSpeed(float speed) { _angularSpeed = speed; }
	void SetForwardSpeed(float speed) { _forwardSpeed = speed; }
    
private:
	// Controls rotation (radians/second)
	float _angularSpeed;
    
	// Controls forward movement (units/second)
	float _forwardSpeed;
};
