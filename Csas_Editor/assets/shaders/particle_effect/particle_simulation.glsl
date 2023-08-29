#type compute
#version 450 core
// ------------------------------------------------------------------
// CONSTANTS ---------------------------------------------------------
// ------------------------------------------------------------------

#define LOCAL_SIZE 32

// ------------------------------------------------------------------
// INPUTS -----------------------------------------------------------
// ------------------------------------------------------------------

layout(local_size_x = LOCAL_SIZE, local_size_y = 1, local_size_z = 1) in;

// ------------------------------------------------------------------
// UNIFORMS ---------------------------------------------------------
// ------------------------------------------------------------------
#include ../utils/render_input.glsl
struct Particle
{
    vec4 lifetime;
    vec4 velocity;
    vec4 position;
    vec4 color;
};

layout(std430, binding = 0) buffer ParticleData_t
{
    Particle particles[];
}
ParticleData;

layout(std430, binding = 1) buffer ParticleDeadIndices_t
{
    uint indices[];
}
DeadIndices;

layout(std430, binding = 2) buffer ParticleAlivePreSimIndices_t
{
    uint indices[];
}
AliveIndicesPreSim;

layout(std430, binding = 3) buffer ParticleAlivePostSimIndices_t
{
    uint indices[];
}
AliveIndicesPostSim;


layout(std430, binding = 4) buffer ParticleCounters_t
{
    uint dead_count;
    uint alive_count[2];
    uint simulation_count;
    uint emission_count;
}
Counters;

//time
uniform int   u_PreSimIdx;
uniform int   u_PostSimIdx;

// ------------------------------------------------------------------
// FUNCTIONS --------------------------------------------------------
// ------------------------------------------------------------------

void push_dead_index(uint index)
{
    uint insert_idx                 = atomicAdd(Counters.dead_count, 1);
    DeadIndices.indices[insert_idx] = index;
}

uint pop_dead_index()
{
    uint index = atomicAdd(Counters.dead_count, -1);
    return DeadIndices.indices[index - 1];
}

void push_alive_index(uint index)
{
    uint insert_idx                         = atomicAdd(Counters.alive_count[u_PostSimIdx], 1);
    AliveIndicesPostSim.indices[insert_idx] = index;
}

uint pop_alive_index()
{
    uint index = atomicAdd(Counters.alive_count[u_PreSimIdx], -1);
    return AliveIndicesPreSim.indices[index - 1];
}

// ------------------------------------------------------------------
// MAIN -------------------------------------------------------------
// ------------------------------------------------------------------

void main()
{
    uint index = gl_GlobalInvocationID.x;

    if (index < Counters.simulation_count)
    {
        // Consume an Alive particle index
        uint particle_index = pop_alive_index();

        Particle particle = ParticleData.particles[particle_index];

        // Is it dead?
        if (particle.lifetime.x >= particle.lifetime.y)
        {
            // If dead, just append into the DeadIndices list
            push_dead_index(particle_index);
        }
        else
        {
            // If still alive, increment lifetime and run simulation
            particle.lifetime.x += rdr_in.delta_time;

            particle.position.xyz += particle.velocity.xyz * rdr_in.delta_time;

            ParticleData.particles[particle_index] = particle;

            // Append index back into AliveIndices list
            push_alive_index(particle_index);


        }
    }
}