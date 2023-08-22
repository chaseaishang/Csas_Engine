#ifndef PBR_SHADING_H
#define PBR_SHADING_H

#include pbr_uniform.glsl
#include ../utils/sampling.glsl

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
vec3 EvaluateAL(const Pixel px,const vec3 L)
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
    vec3 Lo = (Fd + Fr) * NdotL;
    return Lo;
}

// evaluates the contribution of environment IBL at the pixel
vec3 EvaluateIBL(const Pixel px)
{

    // ambient lighting (note that the next IBL tutorial will replace
    // this ambient lighting with environment lighting).

    vec3 kS = fresnelSchlickRoughness(max(dot(px.N, px.V), 0.0), px.F0, px.roughness);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - px.metallic;

    vec3 irradiance = texture(irradiance_map, px.N).rgb;
    vec3 diffuse    = irradiance * px.albedo;

    vec3 R = normalize(reflect(-px.V, px.N));
    const float MAX_REFLECTION_LOD = 5.0;
    vec3 prefilteredColor = textureLod(prefilter_map, R,  px.roughness * MAX_REFLECTION_LOD).rgb;
    vec2 envBRDF  = texture(BRDF_LUT, vec2(max(dot(px.N, px.V), 0.0), px.roughness)).rg;
    vec3 specular = prefilteredColor * (kS * envBRDF.x + envBRDF.y);

    vec3 ambient = (kD * diffuse + specular) * px.ao;
    return ambient;
}
// evaluates the contribution of a white point light of unit intensity
vec3 EvaluateAPL(const Pixel px,const vec3 spot_position)
{
    vec3 L = normalize( spot_position- px.position);
    float distance = length(spot_position - px.position);
    float attenuation = 1.0 / (distance * distance);
    return EvaluateAL(px,L)*attenuation;
}
// evaluates the contribution of a white directional light of unit intensity
vec3 EvaluateADL(const Pixel px, const vec3 L,float visibility)
{

    return visibility*EvaluateAL(px,L);
}

////////////////////////////////////// shadow map


/* computes the bias term that is used to offset the shadow map and remove shadow acne
   we need more bias when NoL is small, less when NoL is large (perpendicular surfaces)
   if the resolution of the shadow map is high, the min/max bias values can be reduced
   N and L must be normalized, and N must be geometric normal, not from the normal map
*/
float ComputeDepthBias(const vec3 L, const vec3 N)
{
    const float max_bias = 0.05;
    const float min_bias = 0.005;
    return max(max_bias * (1.0 - dot(N, L)), min_bias);
}


float PCF(const Pixel px,const vec3 L,in sampler2D shadowMap,vec4 coords )
{

    // 执行透视除法
    vec3 projCoords = coords.xyz / coords.w;
    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;
    // 采样
    poissonDiskSamples(projCoords.xy);
    // uniformDiskSamples(coords.xy);

    // shadow map 的大小, 越大滤波的范围越小
    float textureSize = 1024.0;
    // 滤波的步长
    float filterStride = 5.0;
    // 滤波窗口的范围
    float filterRange = 1.0 / textureSize * filterStride;
    // 有多少点不在阴影里
    int noShadowCount = 0;
    //float bias =0.5*(1.0 - dot(px.N, L))*(1+ceil(filterRange))*20/2048;
    float bias=ComputeDepthBias(L,px.N);
    for( int i = 0; i < NUM_SAMPLES; i ++ )
    {
        vec2 sampleCoord = poissonDisk[i] * filterRange + projCoords.xy;
        float closestDepth = texture( shadow_map,sampleCoord).r;
        float currentDepth = projCoords.z;
        if(currentDepth < closestDepth + bias)
        {
            noShadowCount += 1;
        }
    }

    float shadow = float(noShadowCount) / float(NUM_SAMPLES);
    return shadow;
}

#endif