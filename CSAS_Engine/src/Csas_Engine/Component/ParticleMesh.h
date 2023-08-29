//
// Created by chaseaishang on 23-8-23.
//

#pragma once
#include "Mesh.h"
#include "Csas_Engine/Core/ObjectPool.h"

namespace CsasEngine
{
    struct Particles;
    struct ParticleVertex
    {
        glm::vec3 velocity;
        float     time;
    };
    struct Particle_Data
    {
        glm::vec3 velocity;
        float     time;
        bool      Active;
    };
    struct MeshComponent_ParticleVertex:public MeshComponent
    {
    public:
        friend class Renderer3D;
        friend class ParticleSystem;
        //return live number
        uint AddParticle(const float now_time,const uint count=10);
        void Update();
        MeshComponent_ParticleVertex(const MeshComponent_ParticleVertex&mesh);
        MeshComponent_ParticleVertex()=default;
        MeshComponent_ParticleVertex(const uint count,const uint max_count,Primitive primitive=Primitive::None,uint8_t RenderIndex=0);

        ~MeshComponent_ParticleVertex()=default;

    public:
        Ref<VertexArray> m_VAO;
        Ref<VertexBuffer>m_VBO;
    private:
        uint max_count;
        uint Live_count=100;
        uint gpu_Live_count=0;
        std::vector<ParticleVertex> m_vertices;
        std::list<std::shared_ptr<Particle_Data>>m_Particle;
        // share ptr
        void CreatParticle(const uint count,uint max_cout);
        void UpdatePool(float time,float live_time);
    };
}