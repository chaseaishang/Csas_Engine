#type compute
#version 450 core
#define LOCAL_SIZE 32

// ------------------------------------------------------------------
// INPUTS -----------------------------------------------------------
// ------------------------------------------------------------------

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

// ------------------------------------------------------------------
// UNIFORMS ---------------------------------------------------------
// ------------------------------------------------------------------
struct Particle
{
    vec4 lifetime;
    vec4 velocity;
    vec4 position;
    vec4 color;
};
layout(std430, binding = 1) buffer ParticleCounters_t
{
    uint dead_count;
    uint alive_count[2];
    uint simulation_count;
    uint emission_count;
}
Counters;

uniform int u_ParticlesPerFrame;
uniform int u_PreSimIdx;
uniform int u_PostSimIdx;

// ------------------------------------------------------------------
// MAIN -------------------------------------------------------------
// ------------------------------------------------------------------

void main()
{


    // We can't emit more particles than we have available
    Counters.emission_count = min(uint(u_ParticlesPerFrame), Counters.dead_count);

    Counters.simulation_count = Counters.alive_count[u_PreSimIdx] + Counters.emission_count;

    // Reset post sim alive index count
    Counters.alive_count[u_PostSimIdx] = 0;
}