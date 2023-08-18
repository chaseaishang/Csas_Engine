#ifndef RENDER_INPUT_H
#define RENDER_INPUT_H


// uniform blocks >= 10 are reserved for internal use only
layout(std140, binding = 10) uniform RendererInput {
    ivec2 resolution;     // viewport size in pixels
    ivec2 cursor_pos;     // cursor position relative to viewport's upper-left corner
    float near_clip;      // frustum near clip distance
    float far_clip;       // frustum far clip distance
    float time;           // number of seconds since window created
    float delta_time;     // number of seconds since the last frame
    bool  depth_prepass;  // early z test
    uint  shadow_index;   // index of the shadow map render pass (one index per light source)
} rdr_in;


#endif