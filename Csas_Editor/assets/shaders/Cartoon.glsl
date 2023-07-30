// Basic ADS Light Shader

#type vertex
#version 450 core


layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;
layout (std140, binding = 0) uniform Matrices
{
    mat4 View;
    mat4 Projection;
}Camera;

layout(location = 0) uniform  mat4 model;
layout(location = 0)  out vec3 v_Position;
layout(location = 1)  out vec3 v_Normal;
layout(location = 2)  out vec2 v_UV;
void getEyeSpace( out vec3 norm, out vec3 position )
{
    norm = normalize(mat3(transpose(inverse(model*Camera.View))) * a_Normal);
    position = vec3(model *Camera.View* vec4(a_Position, 1.0));
}
void main()
{
    getEyeSpace(v_Normal,v_Position);
    v_UV     = a_UV;



    mat4 ViewProj= Camera.Projection*Camera.View;
    gl_Position = ViewProj * model * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core
layout(location = 0)  out vec4 color;
layout(location = 0)  in vec3 v_Position;
layout(location = 1)  in vec3 v_Normal;
layout(location = 2)  in vec2 v_UV;
struct LightInfo {
    vec3 position;
    vec3 intensity;
};
struct MaterialInfo {
    vec3 Ka;
    vec3 Kd;
};
uniform LightInfo Light;
uniform MaterialInfo Material;
const int levels = 3;
const float scaleFactor = 1.0 / levels;
vec3 toonShade( )
{
    // eye space
    vec3 s = normalize( Light.position - v_Position );
    float cosine = max( 0.0, dot( s, v_Normal ) );
    //                           ceil
    vec3 diffuse = Material.Kd * ceil( cosine * levels ) *
    scaleFactor;
    return Light.intensity * (Material.Ka + diffuse);
}

void main()
{


    vec3 toon_re=toonShade();

    color = vec4(toon_re,1.0);



}