#ifndef RENDER_INPUT_H
#define RENDER_INPUT_H


// uniform blocks >= 10 are reserved for internal use only
layout(std140, binding = 10) uniform RendererInput {
    mat4 lightSpaceMatrix;
} rdr_in;


#endif