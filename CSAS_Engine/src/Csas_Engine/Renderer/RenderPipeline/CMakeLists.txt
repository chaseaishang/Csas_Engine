
add_subdirectory(RenderData/)
aux_source_directory(. RENDERER_PIPELINE_DIR_LIB_SRCS)
target_sources(CSAS_Renderer
        PRIVATE
        ${RENDERER_PIPELINE_DIR_LIB_SRCS}
        )