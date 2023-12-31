#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
/*
 *
   > compute_shader.Bind();
   > compute_shader.Dispatch(nx, ny, nz);
   > compute_shader.SyncWait(GL_ALL_BARRIER_BITS);  // safest but wasteful and slow!
   > compute_shader.Unbind();

 *
 *
 *
 *
 * */
namespace CsasEngine
{
    class Shader
    {
    public:

        enum class Shader_Type{Vertex,Fragment,Computer};
        virtual ~Shader() = default;
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual void UseSubroutines(const std::string& name, Shader_Type type) = 0;
        virtual void SetInt(const std::string& name, int value) = 0;
        virtual void SetuInt(const std::string& name, uint value) = 0;
        virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
        virtual void SetBoolean(const std::string& name, bool value) = 0;
        virtual void SetFloat(const std::string& name, float value) = 0;
        virtual void SetFloat2(const std::string& name, const glm::vec2& value) =0;
        virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
        virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

        virtual const std::string& GetName() const = 0;

        static Ref<Shader> Create(const std::string& filepath);
        static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);


    };
    class ComputeShader:public Shader
    {
    public:
        enum class ComputeSync:uint
        {
            SSBO_FIN,ALL_FIN
        };
        static Ref<ComputeShader> Create(const std::string& filepath);
        virtual void Dispatch(uint nx, uint ny, uint nz = 1) const=0;
        virtual void DispatchComputeIndirect(uint indirect) const=0;

        virtual void SyncWait(ComputeSync barriers) const=0;
    };

    class ShaderLibrary
    {
    public:
        void Add(const std::string& name, const Ref<Shader>& shader);
        void Add(const Ref<Shader>& shader);
        Ref<Shader> Load(const std::string& filepath);
        Ref<Shader> Load(const std::string& name, const std::string& filepath);

        Ref<Shader> Get(const std::string& name);

        bool Exists(const std::string& name) const;
    private:

        std::unordered_map<std::string, Ref<Shader>> m_Shaders;
    };



}