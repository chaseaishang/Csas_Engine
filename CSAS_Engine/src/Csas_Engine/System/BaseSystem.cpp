//
// Created by chaseaishang on 23-8-25.
//

#include "BaseSystem.h"

namespace CsasEngine {

    void BaseSystem::Mount(Entity &entity)
    {
        this->m_entity.push_back(entity);
    }
}