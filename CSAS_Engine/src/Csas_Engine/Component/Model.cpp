//
// Created by chaseaishang on 23-7-28.
//

#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Csas_Engine/Renderer/Buffer.h>
#include "Csas_Engine/Renderer/Texture.h"

namespace CsasEngine {




    MeshData processMesh(aiMesh *mesh, const aiScene *scene);



    void ModelComponent::loadModel(std::string path) {

        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(path,
                                               aiProcess_Triangulate |
                                               aiProcess_FlipUVs|
                                               aiProcess_GenSmoothNormals
                                               );

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            CSAS_CORE_ERROR("ERROR::ASSIMP::{0}", import.GetErrorString());

            return;
        }
        auto directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
        //
        processMeshComponent();
    }

    void ModelComponent::processNode(aiNode *node, const aiScene *scene) {
// 处理节点所有的网格（如果有的话）
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // 接下来对它的子节点重复这一过程
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    void ModelComponent::processMeshComponent()
    {
        for(auto&mesh:meshes)
        {
            BufferLayout layout;
            switch (mesh.materialType)
            {
                case MaterialType::BasePBR:
                {
                    layout={
                            {ShaderDataType::Float3, "a_Position"},
                            {ShaderDataType::Float3, "a_Normal"},
                            {ShaderDataType::Float2, "a_UV"}
                            };
                    break;
                };
                default:
                {
                    CSAS_CORE_ERROR("Unknown Material Type");
                }


            }
            meshComponents.push_back(MeshComponent<Vertex>(mesh.m_vertices,mesh.m_indices,layout,this->RenderIndex));

        }


    }

    ModelComponent::ModelComponent(std::string path,uint8_t RenderIndex)
    {
        this->RenderIndex=RenderIndex;
        loadModel(path);

    }

    MeshData processMesh(aiMesh *mesh, const aiScene *scene)
    {
// data to fill
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<Texture> textures;

        // walk through each of the mesh's vertices
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.UV = vec;
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                //vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                //vertex.Bitangent = vector;
            }
            else
                vertex.UV = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        MaterialType type;
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        if(material->GetTextureCount(aiTextureType_DIFFUSE)>0)
        {

            aiString str;
            material->GetTexture(aiTextureType_DIFFUSE, 0, &str);

            CSAS_CORE_ERROR("Now we don't have other material");

        }
        else
        {
            type=MaterialType::BasePBR;

        }







        // return a mesh object created from the extracted mesh data
        return MeshData{vertices, indices,type};
    }
}