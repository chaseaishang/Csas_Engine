//
// Created by chaseaishang on 23-8-23.
//

#include "ParticleMesh.h"
#include "Particle.h"
#include <random>
#include "Csas_Engine/Renderer/VertexArray.h"
#include "Csas_Engine/Renderer/Buffer.h"
#include "Csas_Engine/Renderer/Shader.h"
#include "Csas_Engine/Utils/math.h"
namespace CsasEngine
{



    void ParticleInitializer(Particle_Data *object)
    {
        object->Active= true;
    }
    void ParticleReleaser(Particle_Data *object)
    {
        object->Active= false;
    }

    auto& PaticlePool
    =SharedObjectPool<Particle_Data,100>::GetInstance(ParticleInitializer,ParticleReleaser);
    MeshComponent_ParticleVertex::MeshComponent_ParticleVertex(const uint count,const uint max_count,Primitive primitive,uint8_t RenderIndex)
    {
        m_primitive=primitive;
        this->RenderIndex=RenderIndex;
        switch (primitive)
        {
            case Primitive::Particle:CreatParticle(count,max_count);break;

            default:CSAS_ASSERT(false,"error primitive!");

        }
    }
    void MeshComponent_ParticleVertex::CreatParticle(const uint count,uint max_count)
    {
        this->max_count=max_count;
        using namespace Utils::math;
        std::vector<ParticleVertex> &vertices=m_vertices;
        vertices.reserve(count);
        int number=count;
        glm::vec3 v(0.0f);
        float velocity, theta, phi;
        float time = 0.0f, rate = 0.00075f;
        for( uint32_t i = 0; i<number; i++ )
        {
// Pick the direction of the velocity
            theta = glm::mix(0.0f, glm::pi<float>()/ 6.0f, RandomGenerator<float>());
            phi = glm::mix(0.0f, glm::pi<float>(), RandomGenerator<float>());
            v.x = sinf(theta) * cosf(phi);
            v.y = cosf(theta);
            v.z = sinf(theta) * sinf(phi);
// Scale to set the magnitude of the velocity (speed)
            velocity = glm::mix(1.25f,1.5f,RandomGenerator<float>());
            v = v * velocity;
            {
                vertices.emplace_back(ParticleVertex{v,time});
                auto particle=PaticlePool.Get();
                *particle={v,time, true};
                m_Particle.push_back(particle);
                time+=rate;
            }
        }

        BufferLayout layout=
                {
                        {ShaderDataType::Float3, "a_Velocity;"},
                        {ShaderDataType::Float, "a_Time"}

                };
        m_VAO=VertexArray::Create();
        this->Live_count=vertices.size();
        m_VBO=VertexBuffer::Create(vertices.size()*sizeof(ParticleVertex));
        m_VBO->SetLayout(layout);
        m_VAO->AddVertexBuffer(m_VBO);
        m_VBO->SetData(vertices.data(),sizeof(ParticleVertex)*vertices.size());

        vertices.clear();


    }
    MeshComponent_ParticleVertex::MeshComponent_ParticleVertex(const MeshComponent_ParticleVertex &mesh)
    {
        this->m_VAO=mesh.m_VAO;
        this->m_VBO=mesh.m_VBO;
        m_primitive=mesh.m_primitive;
        transform=mesh.transform;
        m_vertices=mesh.m_vertices;
        this->RenderIndex=mesh.RenderIndex;

    }

    void MeshComponent_ParticleVertex::Update()
    {

    }
    void MeshComponent_ParticleVertex::AddParticle(const float now_time,const uint count)
    {
        using namespace Utils::math;
        uint can_add=max_count-Live_count;
        uint add_count=0;
        if(can_add<count)
        {
            add_count=can_add;
        }
        else
        {
            add_count=count;
        }
        std::vector<ParticleVertex> &vertices=m_vertices;
        vertices.clear();
        vertices.reserve(m_Particle.size()+add_count);
        if(m_Particle.size()>0)
        {
            for(auto&particle:m_Particle)
            {
                vertices.emplace_back(ParticleVertex{particle->velocity,particle->time});
            }
        }


        glm::vec3 v(0.0f);
        float velocity, theta, phi;
        float time = now_time, rate = 0.00075f;
        for( uint32_t i = 0; i<add_count; i++ )
        {
// Pick the direction of the velocity
            theta = glm::mix(0.0f, glm::pi<float>()/ 6.0f, RandomGenerator<float>());
            phi = glm::mix(0.0f, glm::pi<float>(), RandomGenerator<float>());
            v.x = sinf(theta) * cosf(phi);
            v.y = cosf(theta);
            v.z = sinf(theta) * sinf(phi);
// Scale to set the magnitude of the velocity (speed)
            velocity = glm::mix(1.25f,1.5f,RandomGenerator<float>());
            v = v * velocity;
            {
                vertices.emplace_back(ParticleVertex{v,time});
                auto particle=PaticlePool.Get();
                *particle={v,time, true};
                m_Particle.push_back(particle);
                time+=rate;
            }
        }
        m_VBO->SetData(m_vertices.data(),sizeof(ParticleVertex)*m_vertices.size());
    }
    void MeshComponent_ParticleVertex::UpdatePool(float now_time,float Particle_livetime)
    {
        float can_live=now_time-Particle_livetime;
        m_Particle.remove_if([&](std::shared_ptr<Particle_Data> data)->bool
         {
            if(data->Active)
            {
                //return data->time+Particle_livetime>now_time;
                return data->time<can_live;
            }
             return false;
         }
        );
//        for(auto&particle:m_Particle)
//        {
//            if(particle->Active)
//            {
//                //return data->time+Particle_livetime>now_time;
//                bool true_dele= particle->time+Particle_livetime<now_time;
//            }
//        }
        Live_count=m_Particle.size();


    }
}