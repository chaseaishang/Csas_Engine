//
// Created by chaseaishang on 23-7-23.
//
#pragma once
#include "Csas_Engine/Core/Core.h"
namespace CsasEngine {


    class UniformBuffer
    {
    public:
        virtual ~UniformBuffer() {}

        virtual void SetData(const void *data, uint32_t size, uint32_t offset = 0) = 0;

        static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding);
    };



}

