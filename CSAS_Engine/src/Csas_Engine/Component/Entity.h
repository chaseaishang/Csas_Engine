//
// Created by chaseaishang on 23-7-14.
//

#pragma once


#include "Csas_Engine/Scene/Scene.h"

#include "entt/include/entt.hpp"

namespace CsasEngine {

    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity& other) = default;


        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            CSAS_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
            return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& GetComponent()
        {
            CSAS_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        template<typename T>
        bool HasComponent()
        {
            return m_Scene->m_Registry.has<T>(m_EntityHandle);
        }
        template<typename... Comp>
        decltype(auto) GetComponents()const
        {
            if constexpr(sizeof...(Comp) == 0)
            {
                CSAS_ASSERT(true,"More argument");

            }
            else if constexpr(sizeof...(Comp) == 1)
            {
                CSAS_WARN("Why don't you just use GetComponent");
                return GetComponent<Comp...>();
            }
            CSAS_ASSERT(m_Scene->m_Registry.has<Comp...>(m_EntityHandle),"");

            return  m_Scene->m_Registry.get<Comp...>(m_EntityHandle);;
        }
        template<typename T>
        void RemoveComponent()
        {
            HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
            m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }

        operator bool() const { return m_EntityHandle != entt::null; }
        operator uint32_t() const { return (uint32_t)m_EntityHandle; }

        bool operator==(const Entity& other) const
        {
            return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
        }

        bool operator!=(const Entity& other) const
        {
            return !(*this == other);
        }

    protected:
        entt::entity       m_EntityHandle{ entt::null };
        Scene*             m_Scene = nullptr;

    };



}



