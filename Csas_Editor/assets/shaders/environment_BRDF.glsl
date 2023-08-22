// Basic ADS Light Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;

#include Camera.glsl
#include utils/render_input.glsl
layout(location = 0) uniform  mat4 model;
layout(location = 0)  out vec3 v_Position;
layout(location = 1)  out vec3 v_Normal;
layout(location = 2)  out vec2 v_UV;
layout(location = 3)  out vec4 v_FragPosLightSpace;
void getEyeSpace( out vec3 norm, out vec3 position )
{
    norm = normalize(mat3(transpose(inverse(model*Camera.View))) * a_Normal);
    position = vec3(Camera.View*model* vec4(a_Position, 1.0));
}
void main()
{
    getEyeSpace(v_Normal,v_Position);
    v_UV     = a_UV;
    v_FragPosLightSpace=rdr_in.lightSpaceMatrix*model*vec4(a_Position, 1.0);
    mat4 ViewProj= Camera.Projection*Camera.View;
    gl_Position = ViewProj * model * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core
layout(location = 0)  out vec4 FragColor;
layout(location = 0)  in vec3 v_Position;
layout(location = 1)  in vec3 v_Normal;
layout(location = 2)  in vec2 v_UV;
layout(location = 3)  in vec4 v_FragPosLightSpace;
uniform vec3  albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;
//     albedo 0.5f, 0.0f, 0.0f
//      ao", 1.0
//
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGXDirect(float NdotV, float roughness);
float GeometrySchlickGGXIBL(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);

#include utils/render_input.glsl
#include utils/extension.glsl
#include BRDF.glsl
#include pbr/pbr_uniform.glsl
#include pbr/pbr_shading.glsl

void main()
{

    vec3 N = normalize(v_Normal);
    vec3 V = normalize(-v_Position);


    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
    // reflectance equation
    vec3 Lo = vec3(0.0);

    Pixel px;
    px.position=v_Position;
    px.F0=F0;
    px.V=V;
    px.N=N;
    px.roughness=roughness;
    px.albedo=albedo;
    px.metallic=metallic;
    px.ao=ao;
    //v_FragPosLightSpace
    //shadow_map

    for(int i = 0; i < Spot_Lights.SpotLightNumber; ++i)
    {
        // calculate per-light radiance
        vec3 spot_position=vec3(Spot_Lights.position[i]);
        vec3 spot_color=vec3(Spot_Lights.color[i]);


        Lo +=EvaluateAPL(px,spot_position)*spot_color;

        //vec3



    }
    bool DirectLightEnable=bool(Direct_Light.color.w);
    if(DirectLightEnable)
    {
        vec3 L=normalize(Direct_Light.direction.xyz);
        float visibility=PCF(px,L,shadow_map,v_FragPosLightSpace);

        Lo +=EvaluateADL(px,L,visibility)*Direct_Light.color.rgb;
    }



    // ambient lighting (note that the next IBL tutorial will replace
    // this ambient lighting with environment lighting).
    vec3 ambient=EvaluateIBL(px);

    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));

    FragColor=vec4(color,1.0);


}