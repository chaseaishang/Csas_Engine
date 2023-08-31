#type compute
#version 450 core
#define LOCAL_SIZE 32

// ------------------------------------------------------------------
// INPUTS -----------------------------------------------------------
// ------------------------------------------------------------------
// update kickoff
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
layout(std430, binding = 0) buffer ParticleCounters_t
{
    uint dead_count;
    uint alive_count[2];
    uint simulation_count;
    uint emission_count;
}
Counters;
layout(std430, binding = 1) buffer EmissionDispatchArgs_t
{
    uint num_groups_x;
    uint num_groups_y;
    uint num_groups_z;
}
EmissionDispatchArgs;

layout(std430, binding = 2) buffer SimulationDispatchArgs_t
{
    uint num_groups_x;
    uint num_groups_y;
    uint num_groups_z;
}
SimulationDispatchArgs;

uniform uint u_ParticlesPerFrame;
uniform int u_PreSimIdx;
uniform int u_PostSimIdx;

// ------------------------------------------------------------------
// MAIN -------------------------------------------------------------
// ------------------------------------------------------------------

void main()
{


    // We can't emit more particles than we have available
    Counters.emission_count = min(u_ParticlesPerFrame, Counters.dead_count);
    EmissionDispatchArgs.num_groups_x = uint(ceil(float(Counters.emission_count) / float(LOCAL_SIZE)));
    EmissionDispatchArgs.num_groups_y = 1;
    EmissionDispatchArgs.num_groups_z = 1;

    Counters.simulation_count = Counters.alive_count[u_PreSimIdx] + Counters.emission_count;
    SimulationDispatchArgs.num_groups_x = uint(ceil(float(Counters.simulation_count) / float(LOCAL_SIZE)));
    SimulationDispatchArgs.num_groups_y = 1;
    SimulationDispatchArgs.num_groups_z = 1;

    // Reset post sim alive index count
    Counters.alive_count[u_PostSimIdx] = 0;
}