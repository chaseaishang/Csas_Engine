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

    struct MeshComponent_ParticleVertex:public MeshComponent
    {
    public:
        friend class Renderer3D;

        std::vector<ParticleVertex> m_vertices;
        void Update();
        MeshComponent_ParticleVertex(const MeshComponent_ParticleVertex&mesh);
        MeshComponent_ParticleVertex()=default;
        MeshComponent_ParticleVertex(const Particles&particles,Primitive primitive=Primitive::None,uint8_t RenderIndex=0);

        ~MeshComponent_ParticleVertex()=default;

    public:
        Ref<VertexArray> m_VAO;
        Ref<VertexBuffer>m_VBO;
    private:
        void CreatParticle(const Particles&particles);
    };
}