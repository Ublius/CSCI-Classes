// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once

class Component {
public:
    // Constructor
    Component(class Actor* pActor, int updateOrder = 100);
    
    // Destructor
    virtual ~Component();
    
    // Update this component by delta time
    virtual void Update(float deltaTime);

    int GetUpdateOrder() const { return _updateOrder; }

protected:
    // Owning actor
    class Actor* _pActor;
    
    int _updateOrder;
};
