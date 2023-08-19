#ifndef PBR_UNIFORM_H
#define PBR_UNIFORM_H

const int SpotLightNumber=4;
layout(std140, binding = 1) uniform SL {
    vec4  color[4];
    vec4  position[4];
    int SpotLightNumber;
    int ext0;
    int ext1;
    int ext2;
} Spot_Lights;
layout(std140, binding = 2) uniform DL
{
    vec4  color;
    vec4  direction;
} Direct_Light;

// sampler binding points (texture units) 17-19 are reserved for PBR IBL
layout(binding = 16) uniform sampler2D   shadow_map;
layout(binding = 17) uniform samplerCube irradiance_map;
layout(binding = 18) uniform samplerCube prefilter_map;
layout(binding = 19) uniform sampler2D   BRDF_LUT;











#endif