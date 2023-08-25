//
// Created by chaseaishang on 23-8-17.
//

#include "Node.h"
#include "Csas_Engine/Component/Entity.h"
namespace CsasEngine
{
    void CsasEngine::Node::SetParent(Node *node)
    {
        this->m_parent=node;

    }

    const std::vector<Node> &Node::GetChildren() const
    {
        return m_children;
    }

    void Node::AddEntity(Entity &entity)
    {
        this->m_entities.push_back(entity);
    }

    Node::Node(std::string name)
        :m_name(name)
    {

    }

    const std::string &Node::getName() const {
        return m_name;
    }

    void Node::AddChild(Node &node)
    {
        m_children.push_back(node);

    }


}