//
// Created by chaseaishang on 23-7-24.
//

#pragma once

namespace CsasEngine {
    struct Material_BasePBR;
    class EditorUI
    {
    public:
        EditorUI()=default;
        static void MenuSetting();
        static void DrawMaterial_PBR(Material_BasePBR&material);
        static void DrawSpotLight(SpotLightComponent&spot);
    };
}


