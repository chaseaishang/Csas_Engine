//
// Created by chaseaishang on 23-8-23.
//

#include "ParticleMesh.h"
#include "Particle.h"
#include <random>
#include "Csas_Engine/Renderer/VertexArray.h"
#include "Csas_Engine/Renderer/Buffer.h"
#include "Csas_Engine/Renderer/Shader.h"
namespace CsasEngine
{
    namespace Utils::math {
        // create random number generator engines and set engine state
        static std::random_device device;               // acquire seed from device()
        static std::mt19937 engine_32(device());  // seed only once at startup
        static std::mt19937_64 engine_64(device());  // seed only once at startup

        // construct static uniform distributions
        static std::uniform_int_distribution<uint32_t> uint32_t_distributor(0, std::numeric_limits<uint32_t>::max());
        static std::uniform_int_distribution<uint64_t> uint64_t_distributor(0, std::numeric_limits<uint64_t>::max());
        static std::uniform_real_distribution<double> double_t_distributor(0.0, 1.0);

        template<typename T>
        T RandomGenerator() {
            if constexpr (std::is_same_v<T, uint64_t>) {
                return uint64_t_distributor(engine_64);
            } else if constexpr (std::is_same_v<T, uint32_t>) {
                return uint32_t_distributor(engine_32);
            } else if constexpr (std::is_same_v<T, double>) {
                return double_t_distributor(engine_32);
            } else if constexpr (std::is_same_v<T, float>) {
                return static_cast<float>(double_t_distributor(engine_32));
            } else {
                CSAS_CORE_ASSERT(true, "Unsupported integral type  ...");
            }

        }

        // explicit template function instantiation
        template uint64_t RandomGenerator();

        template uint32_t RandomGenerator();

        template double RandomGenerator();

        template float RandomGenerator();
    }
    MeshComponent_ParticleVertex::MeshComponent_ParticleVertex(const Particles&particles,Primitive primitive,uint8_t RenderIndex)
    {
        m_primitive=primitive;
        this->RenderIndex=RenderIndex;
        switch (primitive)
        {
            case Primitive::Particle:CreatParticle(particles);break;

            default:CSAS_ASSERT(false,"error primitive!");

        }
    }
    void MeshComponent_ParticleVertex::CreatParticle(const Particles&particles)
    {
        using namespace Utils::math;
        std::vector<ParticleVertex> &vertices=m_vertices;
        vertices.resize(particles.n_particles);
        int number=particles.n_particles;
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
                time+=rate;
            }
        }

        BufferLayout layout=
                {
                        {ShaderDataType::Float3, "a_Velocity;"},
                        {ShaderDataType::Float, "a_Time"}

                };
        m_VAO=VertexArray::Create();

        m_VBO=VertexBuffer::Create(vertices.size()*sizeof(ParticleVertex));
        m_VBO->SetLayout(layout);
        m_VAO->AddVertexBuffer(m_VBO);
        m_VBO->SetData(vertices.data(),sizeof(ParticleVertex)*vertices.size());




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
}