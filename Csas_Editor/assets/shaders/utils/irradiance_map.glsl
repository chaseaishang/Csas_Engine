
#type compute
#version 450 core
// compute diffuse irradiance map from an HDR environment cubemap texture
layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
layout(binding = 0) uniform samplerCube environment_map;
layout(binding = 0, rgba16f) restrict writeonly uniform imageCube irradiance_map;


#include projection.glsl
// this way of sampling over the hemisphere is simple but biased and slow.
vec3 NaiveConvolution(vec3 N, float h_step, float v_step) {
    vec3 irradiance = vec3(0.0);
    uint n_samples = 0;

    // tweak the step size based on the environment map resolution, 1K needs
    // a step size of ~ 0.025, 2K/4K requires 0.01 or even lower, very large
    // step size can lead to noticeable artifacts due to undersampling.
    // h_step: increment on the azimuth angle (longitude)
    // v_step: increment on the polar angle (latitude)

    for (float phi = 0.0; phi < PI2; phi += h_step) {
        for (float theta = 0.0; theta < 0.5 * PI; theta += v_step) {
            vec3 L = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            L = Tangent2World(N, L);
            irradiance += texture(environment_map, L).rgb * cos(theta) * sin(theta);
            n_samples++;
        }
    }

    /* this expression already takes into account the Lambertian diffuse factor `INV_PI`
       o/w we should have multiplied by `PI squared` instead of `PI`, check the formula:
       http://www.codinglabs.net/article_physically_based_rendering.aspx

       if you still wonder why we multiply by `PI`, here's another perspective:
       every texture lookup is weighted by `sin(theta) * cos(theta) = sin(2 * theta) / 2`
       where the polar angle `theta` on the hemisphere ranges from 0 to `PI / 2`. Hence,
       the expected value of all weights is `1 / PI` if you solve the integral, that is,
       the PDF is 1 over PI, therefore we multiply `PI` back to compensate for that.
    */

    return PI * irradiance / float(n_samples);
}

void main()
{
    ivec3 ils_coordinate = ivec3(gl_GlobalInvocationID);
    vec2 resolution = vec2(imageSize(irradiance_map));
    vec3 N = ILS2Cartesian(ils_coordinate, resolution);
    vec3 irradiance = NaiveConvolution(N, 0.01, 0.01);
    imageStore(irradiance_map, ils_coordinate, vec4(irradiance, 1.0));

}

