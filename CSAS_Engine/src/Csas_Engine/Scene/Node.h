//
// Created by chaseaishang on 23-8-17.
//

#pragma once
#include <vector>

// now it just serve for Imgui to show
namespace CsasEngine
{
    class Entity;
    class Node {
    public:
        Node(std::string name="");
        void SetParent(Node* node);
        const std::string &getName() const;
        const std::vector<Node>& GetChildren()const;
        const std::vector<Entity> &getEntities() const {
            return m_entities;
        }
        void AddEntity(Entity&entity);
        void AddChild(Node&node);


    private:
        Node*m_parent= nullptr;
        std::vector<Node>m_children;
        std::vector<Entity>m_entities;

    private:
        std::string m_name;
    public:

    };
}