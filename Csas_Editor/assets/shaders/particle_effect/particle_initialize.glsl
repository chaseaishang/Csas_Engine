#type compute
#version 450 core
// initialize the particle system




layout(local_size_x = 32, local_size_y = 1, local_size_z = 1) in;
// ------------------------------------------------------------------
// UNIFORMS ---------------------------------------------------------
// ------------------------------------------------------------------

layout(std430, binding = 0) buffer ParticleDeadIndices
{
    uint indices[];
}
DeadIndices;

layout(std430, binding = 1) buffer ParticleCounters
{
    uint dead_count;
    uint alive_count[2];
    uint simulation_count;
    uint emission_count;
}
Counters;

uniform uint u_MaxParticles;//1000


void main()
{
    uint index = uint(gl_GlobalInvocationID.x);

    if (index == 0)
    {
        // Initialize counts
        Counters.dead_count     = u_MaxParticles;
        Counters.alive_count[0] = 0;
        Counters.alive_count[1] = 0;
    }

    if (index < u_MaxParticles)
    {
        DeadIndices.indices[index] = index;
    }

}


