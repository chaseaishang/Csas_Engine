

aux_source_directory(. RENDERER_DATA_DIR_LIB_SRCS)
target_sources(CSAS_Renderer
        PRIVATE
        RenderData.cpp

        )
#${RENDERER_DATA_DIR_LIB_SRCS}