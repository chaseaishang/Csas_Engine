//
// Created by chaseaishang on 23-7-14.
//
#include "Csas_Engine/Csaspch.h"
#include "Entity.h"



namespace CsasEngine {

    Entity::Entity(entt::entity handle, Scene* scene)
            : m_EntityHandle(handle), m_Scene(scene)
    {
    }

}