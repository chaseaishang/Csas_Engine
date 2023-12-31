

#type compute
#version 450 core
// compute specular environment BRDF (multi-scattering LUT with energy compensation)




#include extension.glsl
#include ../BRDF.glsl
#include projection.glsl
#include sampling.glsl

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
layout(binding = 2, rgba16f) restrict writeonly uniform image2D BRDF_LUT;

/* the approximated 2D LUT only depends on two parameters: roughness and the cosine term.
   that is, given a roughness level, we only care about the cosine's value, or the angle
   between N and L, rather than the exact vectors. So, for convenience, we can assume N
   to be a constant vector `vec3(0, 0, 1)`, and confine V to the X-Z plane (V.y = 0), so
   for each input cosine, we can find a unique V on the half circle `x^2 + z^2 = 1` that
   satisfies `dot(N, V) = NoV`, this will also give us L once we have the half vector H.
   the idea of this assumption is to keep all vectors in tangent space, since there's no
   texture lookup operation involved, there's no need to convert them to world space, as
   long as all angles are measured in the same reference frame, the result won't change.

   note that we need to properly handle the edge cases when cosine or roughness is close
   to zero so as to reduce artifacts on the borders: if cosine = 0, vectors N and V will
   be orthogonal to each other which could cause problems, so we clamp `NoV` to 0.0001.
   on the other hand, when roughness is 0, GGX importance sampling will return a halfway
   vector H that overlaps with N, this can lead to precision errors when we calculate L,
   to solve this issue, we must either clamp roughness, or use the `precise` qualifier.

   also note that we are using `NoV` instead of `NoL` as the cosine term in the integral
   seeing L needs to be derived from V and H. In practice, `NoV, NoL, HoV` and `HoL` are
   all very close angles so can be used interchangeably w/o adding noticeable errors.
   see also: https://knarkowicz.wordpress.com/2014/12/27/analytical-dfg-term-for-ibl/
*/
vec3 IntegrateBRDF(float NoV, float roughness, uint n_samples) {
    float alpha = roughness * roughness;
    float inv_ns = 1.0 / float(n_samples);

    NoV = max(NoV, 1e-4);  // reduce artifact on the border
    vec3 N = vec3(0.0, 0.0, 1.0);
    vec3 V = vec3(sqrt(1.0 - NoV * NoV), 0.0, NoV);  // vec3(sin, 0, cos)

    float scale = 0.0;
    float bias = 0.0;

    for (uint i = 0; i < n_samples; i++) {
        vec2 Xi = Hammersley2D(i, n_samples);

        vec3 H = Tangent2World(N, ImportanceSampleGGX(Xi.x, Xi.y, alpha));
        vec3 L = normalize(2.0 * dot(V, H) * H - V);  // need the precise qualifier

        // implicitly assume that all vectors lie in the X-Z plane
        float NoL = max(L.z, 0.0);
        float NoH = max(H.z, 0.0);
        float VoH = max(dot(V, H), 0.0);

        if (NoL > 0.0)
        {
            float G =GeometrySmithIBL(N,V,L,roughness);
            float G_Vis = (G * VoH) / (NoH * NoV);
            float Fc = pow5(1.0 - VoH);  // Fresnel F has been factorized out of the integral

             scale += G_Vis * (1.0 - Fc);  // this only considers single bounce
             bias += G_Vis * Fc;           // this only considers single bounce

//            scale += V * Fc;  // take account of multi-scattering energy compensation
//            bias += V;        // take account of multi-scattering energy compensation
        }
    }

    scale *=  inv_ns;
    bias  *=  inv_ns;



    return vec3(scale, bias, 1.0);
}

void main() {
    vec2 resolution = vec2(imageSize(BRDF_LUT));
    float cosine    = float(gl_GlobalInvocationID.x) / resolution.x;
    float roughness = float(gl_GlobalInvocationID.y) / resolution.y;

    vec3 texel = IntegrateBRDF(cosine, roughness, 1024);
    imageStore(BRDF_LUT, ivec2(gl_GlobalInvocationID.xy), vec4(texel, 0.0));
}


