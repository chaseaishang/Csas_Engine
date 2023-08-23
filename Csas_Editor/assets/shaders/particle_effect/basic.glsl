#type vertex
#version 450 core

// Initial velocity and start time
layout (location = 0) in vec3  VertexInitVel;
layout (location = 1) in float StartTime;
out float Transp;
// Transparency of the particle
#include ../Camera.glsl
layout(location = 0) uniform  mat4 model;
uniform float Time; // Animation time
uniform vec3 Gravity = vec3(0.0,-0.05,0.0); // world coords
uniform float ParticleLifetime; // Max particle lifetime
void main()
{
    // Assume the initial position is (0,0,0).
    vec3 pos = vec3(0.0);
    Transp = 0.0;
    // Particle doesn't exist until the start time
    if( Time >StartTime )
    {
        float t = Time - StartTime;
        if( t <ParticleLifetime )
        {
            pos = VertexInitVel * t + Gravity * t * t;
            Transp = 1.0 - t / ParticleLifetime;
        }
    }
    gl_Position = Camera.Projection*Camera.View*model * vec4(pos, 1.0);


}

#type fragment
#version 450 core
in float Transp;
layout ( location = 0 ) out vec4 FragColor;
void main()
{
    FragColor = vec4(Transp);
    FragColor.a *= Transp;

}