#type vertex
#version 450 core

out float Transp;
// Transparency of the particle
#include ../Camera.glsl
#include ../utils/render_input.glsl
layout(location = 0) uniform  mat4 model;
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

layout(std430, binding = 3) buffer ParticleIndices_t
{
    uint indices[];
}
AliveIndicesPostSim;


void main()
{



    Transp=0.0f;
    Particle particle = ParticleData.particles[AliveIndicesPostSim.indices[gl_VertexID]];
    Transp=particle.lifetime.x/particle.lifetime.y;

    gl_Position = Camera.Projection*Camera.View*model * vec4(particle.position.xyz, 1.0);


}

#type fragment
#version 450 core

in float Transp;
layout ( location = 0 ) out vec4 FragColor;
void main()
{

    FragColor=vec4(1,1,0,1);


}