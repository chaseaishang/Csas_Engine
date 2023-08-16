#ifndef PBR_SHADING_H
#define PBR_SHADING_H

//#include pbr_uniform.glsl


struct Pbr_Info {
    vec3 position;
    vec3 N;
    vec3 V;
    vec3  albedo;
    float metallic;
    float roughness;
    float ao;
};
// evaluates the contribution of a white analytical light source of unit intensity
vec3 EvaluateAL(const Pbr_Info info,vec3 radiance)
{
    vec3 N = info.N;
    vec3 V = info.V;

    // calculate per-light radiance
    vec3 L = normalize( light_position- info.position);
    vec3 H = normalize(V + L);
    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, info.roughness);
    float G   = GeometrySmith(N, V, L, info.roughness);
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
    kD *= 1.0 - info.metallic;

    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);

    // add to outgoing radiance Lo
    Lo = (kD * info.albedo / PI + specular) * radiance * NdotL;


    return Lo;
}


#endif