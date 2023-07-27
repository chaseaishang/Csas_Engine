// Basic ADS Light Shader

#type vertex
#version 450 core


layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec2 a_UV;
layout (std140, binding = 0) uniform Matrices
{
    mat4 View;
    mat4 Projection;
}Camera;

layout(location = 0) uniform  mat4 model;
layout(location = 0)  out vec3 v_Position;
layout(location = 1)  out vec4 v_Color;
layout(location = 2)  out vec3 v_Normal;
layout(location = 3)  out vec2 v_UV;
void getEyeSpace( out vec3 norm, out vec3 position )
{
    norm = normalize(mat3(transpose(inverse(model*Camera.View))) * a_Normal);
    position = vec3(model *Camera.View* vec4(a_Position, 1.0));
}
void main()
{
    getEyeSpace(v_Normal,v_Position);
    v_Color  = a_Color;
    v_UV     = a_UV;



    mat4 ViewProj= Camera.Projection*Camera.View;
    gl_Position = ViewProj * model * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core
layout(location = 0)  out vec4 color;
layout(location = 0)  in vec3 v_Position;
layout(location = 1)  in vec4 v_Color;
layout(location = 2)  in vec3 v_Normal;
layout(location = 3)  in vec2 v_UV;
struct LightInfo {
    vec3 Position;
    vec3 La;
    vec3 Ld;
    vec3 Ls;
};
struct MaterialInfo {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Shininess;
};
uniform LightInfo Light;
uniform MaterialInfo Material;

vec3 phongModel( vec3 position, vec3 norm )
{
    vec3 s = normalize(vec3(Light.Position - position));
    vec3 v = normalize(-position.xyz);
    vec3 r = reflect( -s, norm );
    vec3 ambient = Light.La * Material.Ka;
    float sDotN = max( dot(s,norm), 0.0 );
    vec3 diffuse = Light.Ld * Material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    if( sDotN > 0.0 )
    {
        spec = Light.Ls * Material.Ks *
        pow( max( dot(r,v), 0.0 ), Material.Shininess );
    }
    return ambient + diffuse + spec;
}
void main()
{
    const float scale = 15.0;
    bvec2 toDiscard = greaterThan( fract(v_UV * scale), vec2(0.5,0.5) );
    //
    if(all(toDiscard))
    {
        discard;
    }
    vec3 phone_re=phongModel(v_Position,v_Normal);

    color = vec4(phone_re,1.0)*v_Color;


    //
}