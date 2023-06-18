//
// Created by chaseaishang on 23-6-17.
//


#pragma once
#include "Csas_Engine/Input.h"
namespace CsasEngine
{


class LinuxInput: public Input
{
protected:
    virtual bool IsKeyPressedImpl(int keycode) override;

    virtual bool IsMouseButtonPressedImpl(int button) override;
    virtual std::pair<float, float> GetMousePosImpl() override;
    virtual float GetMouseXImpl() override;
    virtual float GetMouseYImpl() override;
};


}


