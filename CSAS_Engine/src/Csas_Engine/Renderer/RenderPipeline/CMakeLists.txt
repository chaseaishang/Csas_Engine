
add_subdirectory(RenderData/)
aux_source_directory(. RENDERER_PIPELINE_DIR_LIB_SRCS)
target_sources(CSAS_Renderer
        PRIVATE
        PassData.cpp
        PassNode.cpp
        RenderPass.cpp
        RenderPipeline.cpp
        )
#${RENDERER_PIPELINE_DIR_LIB_SRCS}