#ifndef PCSS_H
#define PCSS_H

#define BLOCKER_SEARCH_NUM_SAMPLES 16
#define PCF_NUM_SAMPLES 16
// Assuming that LIGHT_FRUSTUM_WIDTH == LIGHT_FRUSTUM_HEIGHT
#define LIGHT_RADIUS    .5

vec2 poissonDisk[16] = {
vec2( -0.94201624, -0.39906216 ),
vec2( 0.94558609, -0.76890725 ),
vec2( -0.094184101, -0.92938870 ),
vec2( 0.34495938, 0.29387760 ),
vec2( -0.91588581, 0.45771432 ),
vec2( -0.81544232, -0.87912464 ),
vec2( -0.38277543, 0.27676845 ),
vec2( 0.97484398, 0.75648379 ),
vec2( 0.44323325, -0.97511554 ),
vec2( 0.53742981, -0.47373420 ),
vec2( -0.26496911, -0.41893023 ),
vec2( 0.79197514, 0.19090188 ),
vec2( -0.24188840, 0.99706507 ),
vec2( -0.81409955, 0.91437590 ),
vec2( 0.19984126, 0.78641367 ),
vec2( 0.14383161, -0.14100790 )
};


void FindBlocker(out float avgBlockerDepth,
out float numBlockers,
vec2 uv, float zReceiver, in sampler2D shadowMap
)
{
    const float near_clip=0.1f;
    //This uses similar triangles to compute what
    //area of the shadow map we should search
    float searchWidth =zReceiver/1024 ;
    float blockerSum = 0;
    numBlockers = 0;
    for( int i = 0; i < BLOCKER_SEARCH_NUM_SAMPLES; ++i )
    {
        float shadowMapDepth = texture( shadowMap,uv + poissonDisk[i] * searchWidth).r;


        if ( shadowMapDepth < zReceiver )
        {
            blockerSum += shadowMapDepth;
            numBlockers++;
        }
    }
    avgBlockerDepth = blockerSum / numBlockers;
}
float ComputeDepthBias(const vec3 L, const vec3 N)
{
    const float max_bias = 0.05;
    const float min_bias = 0.005;
    return max(max_bias * (1.0 - dot(N, L)), min_bias);
}
float PCF_Filter( vec2 uv, float zReceiver, float filterRadiusUV , in sampler2D shadowMap,float bias )
{
    float sum = 0.0f;
    for ( int i = 0; i < PCF_NUM_SAMPLES; ++i )
    {
        vec2 offset =filterRadiusUV* poissonDisk[i] /1024*20;
        float shadowMapDepth = texture( shadowMap,uv + offset).r;
        if (zReceiver - bias < shadowMapDepth)
        {
            sum += 1.0;
        }

    }
    return sum / PCF_NUM_SAMPLES;
}
float PCSS ( const Pixel px,vec3 L,in sampler2D shadowMapTex, vec4 coords )
{
    vec3 projCoords = coords.xyz / coords.w;
    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;

    vec2 uv = projCoords.xy;
    float zReceiver = projCoords.z; // Assumed to be eye-space z in this code

    // STEP 1: blocker search
    float avgBlockerDepth = 0;
    float numBlockers = 0;
    FindBlocker( avgBlockerDepth, numBlockers, uv, zReceiver,shadowMapTex );
    if( numBlockers < 1 )
    {
        //There are no occluders so early out (this saves filtering)
        return 1.0f;
    }
    // STEP 2: penumbra size
    float penumbraRatio = (zReceiver- avgBlockerDepth)/avgBlockerDepth;
    //(zReceiver - zBlocker) / zBlocker
    float filterRadiusUV = penumbraRatio*LIGHT_RADIUS;
    float bias = ComputeDepthBias(L, px.N);
    // STEP 3: filtering
    return PCF_Filter( uv, zReceiver, filterRadiusUV,shadowMapTex,bias);
}
#endif