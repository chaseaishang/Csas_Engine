//
// Created by chaseaishang on 23-8-25.
//

#pragma once
#include "Csas_Engine/Component/Entity.h"
namespace CsasEngine {
    class BaseSystem
    {
    public:
        const std::string &getName() const{return m_Name;};
        BaseSystem(std::string name):m_Name(name){};
        BaseSystem()=default;
        void Mount(Entity&entity);
        const std::vector<Entity> getEntities()const{return m_entity;}
        virtual void Update()=0;
    protected:
        std::vector<Entity>m_entity;
        std::string m_Name;
    };
}