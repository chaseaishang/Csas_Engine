#include "Csas_Engine/Csaspch.h"
#include "Csas_Engine/Renderer/Buffer.h"
#include "OpenGLShader.h"
#include "Csas_Engine/Core/Timer.h"
#include <filesystem>
#include <glad/glad.h>
#include "ShaderInclude.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace CsasEngine
{
    static uint32_t curr_bound_shader = 0;  // keep track of the current rendering state
    namespace Utils {

        ShaderDataType ShaderDataTypeFromGL(GLint type) {
            switch (type)
            {
//Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool,Sampler2D
                case GL_FLOAT:
                    return ShaderDataType::Float;
                case GL_FLOAT_VEC2:
                    return ShaderDataType::Float2;
                case GL_FLOAT_VEC3:
                    return ShaderDataType::Float3;
                case GL_FLOAT_VEC4:
                    return ShaderDataType::Float4;
                case GL_FLOAT_MAT3:
                    return ShaderDataType::Mat3;
                case GL_FLOAT_MAT4:
                    return ShaderDataType::Mat4;
                case GL_INT:
                    return ShaderDataType::Int;
                case GL_INT_VEC2:
                    return ShaderDataType::Int2;
                case GL_INT_VEC3:
                    return ShaderDataType::Int3;
                case GL_INT_VEC4:
                    return ShaderDataType::Int4;
                case GL_SAMPLER_2D:
                    return ShaderDataType::Sampler2D;
                default:
                    return ShaderDataType::None;
            }
        }
        static GLenum ShaderTypeFromString(const std::string& type)
        {
            if (type == "vertex")
                return GL_VERTEX_SHADER;
            if (type == "fragment" || type == "pixel")
                return GL_FRAGMENT_SHADER;
            if(type=="compute")
                return GL_COMPUTE_SHADER;
            CSAS_CORE_ASSERT(false, "Unknown shader type!");
            return 0;
        }



        static const char* GLShaderStageToString(GLenum stage)
        {
            switch (stage)
            {
                case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
                case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
            }
            CSAS_CORE_ASSERT(false,"error GLShaderStage");
            return nullptr;
        }

        static const char* GetCacheDirectory()
        {
            // TODO: make sure the assets directory is valid
            return "assets/cache/shader/opengl";
        }
        static const char* GetShaderSourceCacheDirectory()
        {
            // TODO: make sure the assets directory is valid
            return "assets/cache/shaderSource/opengl";
        }
        static void CreateCacheDirectoryIfNeeded()
        {
            std::string cacheDirectory = GetCacheDirectory();
            if (!std::filesystem::exists(cacheDirectory))
                std::filesystem::create_directories(cacheDirectory);
        }





    }

    OpenGLShader::OpenGLShader(const std::string& filepath)
            : m_FilePath(filepath)
    {
        CSAS_PROFILE_FUNCTION();

        Utils::CreateCacheDirectoryIfNeeded();

        std::string source = Shadinclude::load(filepath);
        #ifdef ShaderDebug

        std::filesystem::path cacheDirectory = Utils::GetShaderSourceCacheDirectory();

        std::filesystem::path shaderFilePath = filepath;
        std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() +m_Name);
            std::ofstream out(cachedPath,std::ios::out | std::ios::binary);
            if(out.is_open()&&m_write)
            {

                out.write(source.c_str(),source.size());


            }
        out.close();

        #endif
        auto shaderSources = PreProcess(source);

        {
            Timer timer;


            CreateORGetBinaryProgram(shaderSources);
            CSAS_CORE_WARN("Shader creation took {0} ms", timer.ElapsedMillis());
        }

        // Extract name from filepath
        auto lastSlash = filepath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = filepath.rfind('.');
        auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
        m_Name = filepath.substr(lastSlash, count);
        //
        if(!ForRender)
        {
            Reflect();
        }

    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
            : m_Name(name)
    {
        CSAS_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertexSrc;
        sources[GL_FRAGMENT_SHADER] = fragmentSrc;
        {
            Timer timer;
            CreateORGetBinaryProgram(sources);
            CSAS_CORE_WARN("Shader creation took {0} ms", timer.ElapsedMillis());
        }
        Reflect();
    }

    OpenGLShader::~OpenGLShader()
    {
        CSAS_PROFILE_FUNCTION();

        glDeleteProgram(m_RendererID);
    }

    std::string OpenGLShader::ReadFile(const std::string& filepath)
    {
        CSAS_PROFILE_FUNCTION();

        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
        if (in)
        {
            in.seekg(0, std::ios::end);
            size_t size = in.tellg();
            if (size != -1)
            {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(&result[0], size);
            }
            else
            {
                CSAS_CORE_ERROR("Could not read from file '{0}'", filepath);
            }
        }
        else
        {
            CSAS_CORE_ERROR("Could not open file '{0}'", filepath);
        }

        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
    {
        CSAS_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
            CSAS_CORE_ASSERT(eol != std::string::npos, "Syntax error");
            size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
            std::string type = source.substr(begin, eol - begin);
            CSAS_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type specified");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
            CSAS_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
            pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

            shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
        }



        return shaderSources;
    }
    void OpenGLShader::Compile()
    {
        CSAS_PROFILE_FUNCTION();
        auto &shaderSources=m_OpenGLSourceCode;
        GLuint program = glCreateProgram();
        CSAS_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now");
        std::array<GLenum, 2> glShaderIDs;
        int glShaderIDIndex = 0;
        for (auto& kv : shaderSources)
        {
            GLenum type = kv.first;
            const std::string& source = kv.second;

            GLuint shader = glCreateShader(type);

            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);

            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);

                CSAS_CORE_ERROR("{0}", infoLog.data());
                CSAS_CORE_ASSERT(false, "Shader compilation failure!");
                break;
            }

            glAttachShader(program, shader);
            glShaderIDs[glShaderIDIndex++] = shader;
        }

        m_RendererID = program;
        glProgramParameteri(program, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
        // Link our program
        glLinkProgram(program);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(program);
            for(int i=0;i<glShaderIDIndex;i++)
            {
                glDeleteShader(glShaderIDs[i]);
            }



            CSAS_CORE_ERROR("{0}", infoLog.data());
            CSAS_CORE_ASSERT(false, "Shader link failure!");
            return;
        }


        for(int i=0;i<glShaderIDIndex;i++)
        {
            glDetachShader(program, glShaderIDs[i]);
            glDeleteShader(glShaderIDs[i]);
        }

    }

    void OpenGLShader::CreateORGetBinaryProgram(const std::unordered_map<GLenum, std::string>& shaderSources)
    {
        std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

        std::filesystem::path shaderFilePath = m_FilePath;
        std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() +m_Name+".cache");

        std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
        bool NotDebug=true;
        #ifdef ShaderDebug
        NotDebug=false;
        CSAS_CORE_WARN("ShaderDebug ON");
        #endif
        if (in.is_open()&&NotDebug)
        {
            GLuint program = glCreateProgram();

            std::istreambuf_iterator<char> startIt(in), endIt;
            std::vector<char> data(startIt, endIt);  // Load file


            GLint formats = 0;
            glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
            GLint *binaryFormats = new GLint[formats];
            glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, binaryFormats);
            GLint  format=binaryFormats[0];
// Install shader binary
            // format=36385,size=10793
            CSAS_CORE_WARN("format={0},size={1}",(int)format,(int)data.size());

            glProgramBinary(program, format, data.data(), data.size() );

// Check for success/failure
            GLint isLinked = 0;
            glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
            if (isLinked == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
                // The maxLength includes the NULL character
                std::vector<GLchar> infoLog(maxLength);
                glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
                // We don't need the program anymore.
                glDeleteProgram(program);
                printf("%s",infoLog.data());

                CSAS_CORE_ERROR("isLinked == GL_FALSE {0}", infoLog.data());
                CSAS_CORE_ASSERT(false, "Shader link failure!");
                return;
            }
            m_RendererID=program;
        }
        else {
            m_OpenGLSourceCode = shaderSources;

            GLint formats = 0;
            glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
            if (formats < 1) {
                CSAS_CORE_ERROR("{0}", "Driver does not support any binary formats.");

                exit(EXIT_FAILURE);
            }
            Compile();
            if (NotDebug)
            {


            std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
            if (out.is_open()) {

                // Get the binary length
                GLint length = 0;//10793
                glGetProgramiv(m_RendererID, GL_PROGRAM_BINARY_LENGTH, &length);
                std::vector<GLubyte> buffer(length);
                GLenum format = 0;
                glGetProgramBinary(m_RendererID, length, NULL, &format, buffer.data());
                CSAS_CORE_WARN("format={0},size={1}", (int) format, (int) length);
// Write the binary to a file.
                out.write((char *) buffer.data(), length);
                out.flush();
                out.close();
                }
            }
        }
    }



    void OpenGLShader::Reflect()
    {
        GLint maxUniformLen;
        GLint numUniforms;


        glGetProgramiv ( m_RendererID, GL_ACTIVE_UNIFORMS, &numUniforms );
        glGetProgramiv ( m_RendererID, GL_ACTIVE_UNIFORM_MAX_LENGTH,
                         &maxUniformLen );
        std::string uniformName ;
        uniformName.resize(maxUniformLen);
        for ( int index = 0; index < numUniforms; index++ )
        {
            GLint size;
            GLenum type;
            GLint location;
            // Get the uniform info
            glGetActiveUniform (m_RendererID, index, maxUniformLen, NULL,
                                &size, &type,
                                const_cast<GLchar *>(uniformName.c_str()));
            // Get the uniform location
            location = glGetUniformLocation ( m_RendererID, const_cast<GLchar *>(uniformName.c_str()) );
            CSAS_CORE_TRACE("  name= {0}", uniformName);
            CSAS_CORE_TRACE("    size = {0}", size);
            CSAS_CORE_TRACE("    location = {0}", location);
            ShaderDataType shadertype=Utils::ShaderDataTypeFromGL(type);

            m_Uniform[uniformName]=shadertype;
            switch (type)
            {
                case GL_SAMPLER_2D:
                {
                    CSAS_CORE_TRACE("    type = {0}", "GL_SAMPLER_2D");
                    break;
                }
                case GL_FLOAT_MAT4:
                {
                    CSAS_CORE_TRACE("    type = {0}", "GL_FLOAT_MAT4");
                    break;
                }

            }


        }

    }
    std::unordered_map<std::string,ShaderDataType> OpenGLShader::getUniform()
    {
        return std::unordered_map<std::string,ShaderDataType>();
    }

    void OpenGLShader::Bind() const
    {
        CSAS_PROFILE_FUNCTION();
        //glActiveShaderProgram()
        if (m_RendererID != curr_bound_shader) {
            glUseProgram(m_RendererID);
            curr_bound_shader = m_RendererID;
        }


    }

    void OpenGLShader::Unbind() const
    {
        CSAS_PROFILE_FUNCTION();
        if (m_RendererID == curr_bound_shader)
        {
            curr_bound_shader = 0;
            glUseProgram(0);
        }

    }

    void OpenGLShader::SetInt(const std::string& name, int value)
    {
        CSAS_PROFILE_FUNCTION();

        UploadUniformInt(name, value);
    }
    void OpenGLShader::SetuInt(const std::string &name, uint value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glProgramUniform1ui(m_RendererID,location,value);
    }
    void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
    {
        UploadUniformIntArray(name, values, count);
    }
    void OpenGLShader::SetBoolean(const std::string& name, bool value)
    {
        UploadUniformBoolean(name,value);
    }
    void OpenGLShader::SetFloat(const std::string& name, float value)
    {
        CSAS_PROFILE_FUNCTION();

        UploadUniformFloat(name, value);
    }

    void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
    {
        CSAS_PROFILE_FUNCTION();

        UploadUniformFloat2(name, value);
    }

    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
    {
        CSAS_PROFILE_FUNCTION();

        UploadUniformFloat3(name, value);
    }

    void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
    {
        CSAS_PROFILE_FUNCTION();

        UploadUniformFloat4(name, value);
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
    {
        CSAS_PROFILE_FUNCTION();

        UploadUniformMat4(name, value);
    }

    void OpenGLShader::UploadUniformInt(const std::string& name, int value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glProgramUniform1i(m_RendererID,location,value);

    }

    void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glProgramUniform1iv(m_RendererID,location, count, values);
    }
    void OpenGLShader::UploadUniformBoolean(const std::string& name, float value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glProgramUniform1i(m_RendererID,location,value);
    }
    void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glProgramUniform1f(m_RendererID,location,value);
    }

    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glProgramUniform2f(m_RendererID,location,value.x, value.y);
    }

    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glProgramUniform3f(m_RendererID,location, value.x, value.y, value.z);
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glProgramUniform4f(m_RendererID,location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glProgramUniformMatrix3fv(m_RendererID,location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
    {


        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glProgramUniformMatrix4fv(m_RendererID,location, 1, GL_FALSE, glm::value_ptr(matrix));
        //glProgramUniform4fv(m_RendererID,location,1,glm::value_ptr(matrix));
    }

    void OpenGLShader::UseSubroutines(const std::string &name, Shader::Shader_Type type)
    {
        if(type==Shader_Type::Vertex)
        {
             GLuint loc=glGetSubroutineIndex( m_RendererID,
                                  GL_VERTEX_SHADER,name.c_str());
            glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &loc);

        }
        else
        {
            GLuint loc=glGetSubroutineIndex( m_RendererID,
                                             GL_FRAGMENT_SHADER,name.c_str());
            glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &loc);
        }
    }

    void OpenGLShader::Dispatch(uint nx, uint ny, uint nz) const
    {

        glDispatchCompute(nx, ny, nz);
    }

    void OpenGLShader::SyncWait(ComputeShader::ComputeSync barriers) const
    {
        GLbitfield flag;
        switch (barriers)
        {
            case ComputeShader::ComputeSync::SSBO_FIN:flag=GL_SHADER_STORAGE_BARRIER_BIT;
            default:flag= GL_ALL_BARRIER_BITS;
        }
        glMemoryBarrier(flag);  // sync to ensure all writes are complete

    }




}