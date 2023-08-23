//
// Created by chaseaishang on 23-8-23.
//

#pragma once
#include "Mesh.h"


namespace CsasEngine
{
    struct Particles;
    struct ParticleVertex
    {
        glm::vec3 velocity;
        float     time;
    };
    template<>
    struct MeshComponent<ParticleVertex>
    {
    public:
        friend class Renderer3D;

        enum Primitive m_primitive{Primitive::None};
        uint8_t RenderIndex=0;
        TransformComponent transform{glm::vec3(0)};
        std::vector<ParticleVertex> m_vertices;
        void Update();
        MeshComponent(const MeshComponent&mesh);
        MeshComponent()=default;
        MeshComponent(const Particles&particles,Primitive primitive=Primitive::None,uint8_t RenderIndex=0);

        ~MeshComponent()=default;

    public:
        Ref<VertexArray> m_VAO;
        Ref<VertexBuffer>m_VBO;
    private:
        void CreatParticle(const Particles&particles);
    };
}