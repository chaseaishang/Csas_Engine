// Basic ADS Light Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;

#include Camera.glsl

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
layout(location = 0)  out vec4 FragColor;
layout(location = 0)  in vec3 v_Position;
layout(location = 1)  in vec3 v_Normal;
layout(location = 2)  in vec2 v_UV;
uniform vec3  albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;
//     albedo 0.5f, 0.0f, 0.0f
//      ao", 1.0
//
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);
#include utils/extension.glsl
#include BRDF.glsl
#include pbr/pbr_uniform.glsl
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
    for(int i = 0; i < Spot_Lights.SpotLightNumber; ++i)
    {
        // calculate per-light radiance
        vec3 spot_position=vec3(Spot_Lights.position[i]);
        vec3 spot_color=vec3(Spot_Lights.color[i]);
        vec3 L = normalize( spot_position- v_Position);
        vec3 H = normalize(V + L);
        float distance = length(spot_position - v_Position);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance =spot_color  * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;

        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }



    // ambient lighting (note that the next IBL tutorial will replace
    // this ambient lighting with environment lighting).

    vec3 kS = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    vec3 irradiance = texture(irradiance_map, N).rgb;
    vec3 diffuse    = irradiance * albedo;

    vec3 R = normalize(reflect(-V, N));
    const float MAX_REFLECTION_LOD = 5.0;
    vec3 prefilteredColor = textureLod(prefilter_map, R,  roughness * MAX_REFLECTION_LOD).rgb;
    vec2 envBRDF  = texture(BRDF_LUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (kS * envBRDF.x + envBRDF.y);

    vec3 ambient = (kD * diffuse + specular) * ao;
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));

    FragColor=vec4(color,1.0);


}