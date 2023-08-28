#type vertex
#version 450 core

// Initial velocity and start time
layout (location = 0) in vec3  VertexInitVel;
layout (location = 1) in float StartTime;
out float Transp;
// Transparency of the particle
#include ../Camera.glsl
#include ../utils/render_input.glsl
layout(location = 0) uniform  mat4 model;

uniform vec3 Gravity = vec3(0.0,-0.05,0.0); // world coords
uniform float ParticleLifetime; // Max particle lifetime
layout(std430, binding = 0) buffer ParticleData_t
{
    vec4 color;
}
ParticleData;
void main()
{
    // Assume the initial position is (0,0,0).
    vec3 pos = vec3(0.0);
    Transp = 0.0;
    float Time=rdr_in.now_time;
    // Particle doesn't exist until the start time
    if(Time>StartTime)
    {
        float t = Time - StartTime;

        if( t <ParticleLifetime )
        {
            Transp = 1.0 - t/ParticleLifetime;
            Transp=min(max(0,Transp),1);
            t*=0.2;
            pos = VertexInitVel*t+0.5*Gravity*t*t;

        }

    }
    ParticleData.color=vec4(1,Transp,Transp,Transp*Transp);
    gl_Position = Camera.Projection*Camera.View*model * vec4(pos, 1.0);


}

#type fragment
#version 450 core
layout(std430, binding = 0) buffer ParticleData_t
{
    vec4 color;
}
ParticleData;
in float Transp;
layout ( location = 0 ) out vec4 FragColor;
void main()
{
    vec3 color=vec3(1,Transp,Transp);

    FragColor = ParticleData.color;
    //FragColor.a *= Transp;

}