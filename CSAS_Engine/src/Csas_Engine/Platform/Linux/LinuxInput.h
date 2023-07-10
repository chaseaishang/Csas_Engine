//
// Created by chaseaishang on 23-6-17.
//


#pragma once
#include "Csas_Engine/Core/Input.h"
namespace CsasEngine
{


class LinuxInput: public Input
{
public:
    LinuxInput();
protected:
    virtual bool IsKeyPressedImpl(KeyCode key) override;

    virtual bool IsMouseButtonPressedImpl(MouseCode button) override;
    virtual std::pair<float, float> GetMousePositionImpl() override;
    virtual float GetMouseXImpl() override;
    virtual float GetMouseYImpl() override;


};


}


