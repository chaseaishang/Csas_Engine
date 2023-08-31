#pragma once

#include "Csas_Engine/Renderer/Shader.h"
#include <glm/glm.hpp>
enum class ShaderDataType;
// TODO: REMOVE!
typedef unsigned int GLenum;

namespace CsasEngine {

    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~OpenGLShader();
        std::unordered_map<std::string,ShaderDataType>getUniform();
        void Bind() const override;
        void Unbind() const override;
        void UseSubroutines(const std::string& name, Shader_Type type)override;
        void SetInt(const std::string& name, int value) override;
        void SetuInt(const std::string& name, uint value) override;
        void SetIntArray(const std::string& name, int* values, uint32_t count) override;
        void SetBoolean(const std::string& name, bool value) override;
        void SetFloat(const std::string& name, float value) override;
        void SetFloat2(const std::string& name, const glm::vec2& value) override;
        void SetFloat3(const std::string& name, const glm::vec3& value) override;
        void SetFloat4(const std::string& name, const glm::vec4& value) override;
        void SetMat4(const std::string& name, const glm::mat4& value) override;

        virtual const std::string& GetName() const override { return m_Name; }

        void UploadUniformInt(const std::string& name, int value);
        void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);

        void UploadUniformBoolean(const std::string& name, float value);
        void UploadUniformFloat(const std::string& name, float value);
        void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
        void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
        void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);


    protected:
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);


        void Compile();
        void CreateORGetBinaryProgram(const std::unordered_map<GLenum, std::string>& shaderSources);
        void Reflect();

    protected:
        uint32_t m_RendererID;
        std::string m_FilePath;
        std::string m_Name;


        std::unordered_map<GLenum, std::string> m_OpenGLSourceCode;

        std::unordered_map<std::string,ShaderDataType> m_Uniform;
        bool ForRender=true;
        bool m_write= false;
    };
    class OpenGLComputeShader : public OpenGLShader ,public ComputeShader
    {
    public:
        OpenGLComputeShader(const std::string& filepath);
        virtual ~OpenGLComputeShader()=default;
        void Dispatch(uint nx, uint ny, uint nz = 1) const override;
        void SyncWait(ComputeSync barriers) const override;
        void DispatchComputeIndirect(uint indirect) const override;
        virtual const std::string& GetName() const override { return m_Name; }
        void Bind() const override;
        void Unbind() const override;
        void UseSubroutines(const std::string& name, Shader_Type type)override;
        void SetInt(const std::string& name, int value) override;
        void SetuInt(const std::string& name, uint value) override;
        void SetIntArray(const std::string& name, int* values, uint32_t count) override;
        void SetBoolean(const std::string& name, bool value) override;
        void SetFloat(const std::string& name, float value) override;
        void SetFloat2(const std::string& name, const glm::vec2& value) override;
        void SetFloat3(const std::string& name, const glm::vec3& value) override;
        void SetFloat4(const std::string& name, const glm::vec4& value) override;
        void SetMat4(const std::string& name, const glm::mat4& value) override;
    };

}