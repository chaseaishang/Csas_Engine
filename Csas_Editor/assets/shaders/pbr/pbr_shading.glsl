#ifndef PBR_SHADING_H
#define PBR_SHADING_H

//#include pbr_uniform.glsl


struct Pixel {
    vec3 position;
    vec3 F0;
    vec3 N;
    vec3 V;
    vec3  albedo;
    float metallic;
    float roughness;
    float ao;
};

// evaluates base material's diffuse BRDF lobe
vec3 EvalDiffuseLobe(const Pixel px,vec3 kD)
{
    return kD*px.albedo*INV_PI;
}

// evaluates base material's specular BRDF lobe
vec3 EvalSpecularLobe(const Pixel px, vec3 H,const vec3 L,out vec3 F)
{
    float NDF = DistributionGGX(px.N, H, px.roughness);
    float G   = GeometrySmith(px.N, px.V, L, px.roughness);
    F    = fresnelSchlick(clamp(dot(H, px.V), 0.0, 1.0), px.F0);

    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(px.N, px.V), 0.0) * max(dot(px.N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;

    return specular;
}

// evaluates the contribution of a white analytical light source of unit intensity
vec3 EvaluateAL(const Pixel px,const vec3 L,const vec3 radiance)
{
    vec3 H = normalize(px.V + L);
    float NdotL = max(dot(px.N, L), 0.0);
    // kS is equal to Fresnel
    vec3 F;
    vec3 Fr = EvalSpecularLobe(px,H,L,F);  // compensate energy

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    vec3 Fd = EvalDiffuseLobe(px,kD);
    vec3 Lo = (Fd + Fr) * NdotL*radiance;
    return Lo;
}
// evaluates the contribution of a white point light of unit intensity
vec3 EvaluateAPL(const Pixel px,const vec3 spot_position,const vec3 spot_color)
{
    vec3 L = normalize( spot_position- px.position);
    float distance = length(spot_position - px.position);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance =spot_color  * attenuation;

    return EvaluateAL(px,L,radiance);
}

#endif