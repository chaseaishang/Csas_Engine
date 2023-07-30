//
// Created by chaseaishang on 23-7-28.
//

#pragma once
#include "glm/glm.hpp"
class aiNode;
class aiScene;
#include "Mesh.h"
#include "Material.h"
namespace CsasEngine
{
    struct MeshData
    {
        std::vector<Vertex> m_vertices;
        std::vector<uint32_t> m_indices;
        MaterialType materialType;
    };



    struct ModelComponent
    {
    public:
        ModelComponent()=default;
        ~ModelComponent()=default;
        ModelComponent(std::string path);

        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
    public:
        std::vector<MeshData>meshes;
        std::vector<MeshComponent>meshComponents;
        void processMeshComponent();
    };
}