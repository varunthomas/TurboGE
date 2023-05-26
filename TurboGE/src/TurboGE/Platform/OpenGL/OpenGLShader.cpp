#include"tgepch.h"
#include"OpenGLShader.h"
#include<glad/glad.h>
#include"TurboGE/Logger.h"
#include"glm/gtc/type_ptr.hpp"
#include<shaderc/shaderc.hpp>
#include<spirv_cross/spirv_cross.hpp>
#include<spirv_cross/spirv_glsl.hpp>
namespace TurboGE
{

	OpenGLShader::OpenGLShader(const std::string& path)
		:m_Path{path}
	{
		
		std::string vertexSrc, fragmentSrc;
		parseFile(path, vertexSrc, fragmentSrc);
		std::vector<std::string> source{ vertexSrc, fragmentSrc };

		if (!isBinariesPresent())
		{
			std::cout << "Compiling " << m_Path.c_str() << std::endl;
			//ADDING SUPPORT FOR VULKAN. GLSL FILE IS MODIFIED TO HELP CONVERT TO SPIRV BY SHADERC
			CompileVulkan(source); // THE SHADERC CONVERTS THE GLSL TO SPIRV AS VULKAN AS ARGUMENT AND GETS A SPIRV WITH VULKAN COMPATIBLE BIN
			CompileOpenGL(); // THIS VULKAN BIN IS FED TO SPIRV TO GET A STRING GLSL FOR OPENGL. THIS STRING IS PASSED TO SHADERC THAT CONVERTS TO SPIRV AS OPENGL AS ARGUMENT AND GETS A SPIRV WITH OPENGL COMPATIBLE BIN.
		}

		CreateProgram(); //THIS OPENGL BIN IS FED TO SHADER

	}

	/* 1. Check if glsl file is present in assets/temp.
	*  1.a. If file exists, check if file content is same as the actual shader file
	*		1.a.a. If file content is not same, copy the actual shader to temp and return false
	*		1.a.b. If file content is same, do not compile
	*  1.b. If file does not exist, copy file to temp and return false
	* WE CAN SAVE ~2 SECONDS BY NOT COMPILING ALWAYS
	* */
	bool OpenGLShader::isBinariesPresent()
	{
		if (!std::filesystem::exists("assets/shaders/temp"))
			std::filesystem::create_directories("assets/shaders/temp");

		std::string originalFileName = m_Path.substr(m_Path.find_last_of('/') + 1);
		std::filesystem::path cachedShaderFile= "assets/shaders/temp/" + originalFileName;
		std::ifstream inCache(cachedShaderFile, std::ios::in | std::ios::binary);

		std::filesystem::path actualShaderFile = m_Path;
		std::ifstream inActual(actualShaderFile, std::ios::in | std::ios::binary);

		if (inCache.is_open())
		{
			if (isContentSame(inCache, inActual))
			{
				for (int i = 0; i < shaderNum; i++)
				{
					std::filesystem::path binPath = "assets/shaders/bin/" + std::filesystem::path(m_Path).filename().string() + binFileOpenGL[i];
					std::ifstream in(binPath, std::ios::in | std::ios::binary);

					in.seekg(0, std::ios::end);
					auto size = in.tellg();
					in.seekg(0, std::ios::beg);
					openGLShaderBin[i].resize(size / sizeof(uint32_t));
					in.read((char*)openGLShaderBin[i].data(), size);
				}
				return true;
			}
			else
			{
				std::filesystem::copy(actualShaderFile, cachedShaderFile, std::filesystem::copy_options::update_existing);
				return false;
			}
		}
		else
		{
			std::filesystem::copy(actualShaderFile, cachedShaderFile);
			return false;
		}
	}

	bool OpenGLShader::isContentSame(std::ifstream& f1, std::ifstream& f2)
	{
		if (f1.tellg() != f2.tellg()) {
			return false; //size mismatch
		}

		//seek back to beginning and use std::equal to compare contents
		f1.seekg(0, std::ifstream::beg);
		f2.seekg(0, std::ifstream::beg);
		return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
			std::istreambuf_iterator<char>(),
			std::istreambuf_iterator<char>(f2.rdbuf()));
	}

	void OpenGLShader::parseFile(const std::string& path, std::string& vertexSrc, std::string& fragmentSrc)
	{
		bool shaderFlag = false;
		bool vertexFlag = false, fragmentFlag = false;
		std::ifstream inf{ path };
		if (!inf)
		{
			TURBO_ASSERT("No file found", 0);
		}

		while (inf)
		{
			std::string text;
			std::getline(inf, text);
			if (text.find("#text") == std::string::npos)
			{
				if (vertexFlag == true)
				{
					vertexSrc.append(text);
					vertexSrc.append("\n");
				}
				else if (fragmentFlag == true)
				{
					fragmentSrc.append(text);
					fragmentSrc.append("\n");
				}
			}
			else
			{
				shaderFlag = true;
				vertexFlag = false;
				fragmentFlag = false;
				if (text.find("vertex") != std::string::npos)

				{
					vertexFlag = true;
				}
				else if (text.find("fragment") != std::string::npos)
				{
					fragmentFlag = true;
				}
			}
		}
	}

	void OpenGLShader::CompileVulkan(const std::vector<std::string>& src)
	{
		GLuint program = glCreateProgram();
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;

		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
		options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path dir = "assets/shaders/bin";
		if (!std::filesystem::exists(dir))
			std::filesystem::create_directories(dir);
		std::filesystem::path srcPath = m_Path;

		for(int i = 0; i < src.size(); i++)
		{
			std::filesystem::path dstPath = dir / (srcPath.filename().string() + binFileVulkan[i]);
			shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(src.at(i), (shaderc_shader_kind)i, m_Path.c_str(), options);

			if (result.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				std::cout << result.GetErrorMessage() << '\n';
				TURBO_ASSERT("Failed to compile", 0);
			}

			vulkanShaderBin[i] = std::vector<uint32_t>(result.cbegin(), result.cend());

			std::ofstream out(dstPath, std::ios::out | std::ios::binary);
			if (out.is_open())
			{
				const auto& data = std::vector<uint32_t>(result.cbegin(), result.cend());
				out.write((char*)data.data(), data.size() * sizeof(uint32_t));
				out.flush();
				out.close();
			}
		}
	}

	void OpenGLShader::CompileOpenGL()
	{

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = false;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path dir = "assets/shaders/bin";
		std::filesystem::path srcPath = m_Path;

		for(int i =0; i < shaderNum; i++)
		{
			std::filesystem::path dstPath = dir / (srcPath.filename().string() + binFileOpenGL[i]);

			spirv_cross::CompilerGLSL glslCompiler(vulkanShaderBin[i]);
			openGLText[i] = glslCompiler.compile();
			auto& source = openGLText[i];

			shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, (shaderc_shader_kind)i, m_Path.c_str());
			if (module.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				TURBO_ASSERT("Failed to compile", 0);
			}

			openGLShaderBin[i] = std::vector<uint32_t>(module.cbegin(), module.cend());

			std::ofstream out(dstPath, std::ios::out | std::ios::binary);
			if (out.is_open())
			{
				auto& data = openGLShaderBin[i];
				out.write((char*)data.data(), data.size() * sizeof(uint32_t));
				out.flush();
				out.close();
			}
		}
	}

	void OpenGLShader::CreateProgram()
	{
		GLuint program = glCreateProgram();
		std::vector<GLuint> shaderIDs;
		GLuint shaderID;

		for (int i = 0; i < shaderNum; i++)
		{
			shaderID = shaderIDs.emplace_back(glCreateShader(i==0? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, openGLShaderBin[i].data(), openGLShaderBin[i].size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			std::cout << "Shader linking failed " <<  m_Path << " data:: " << infoLog.data();

			glDeleteProgram(program);

			for (auto id : shaderIDs)
				glDeleteShader(id);
		}

		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererID = program;
	}

	void OpenGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::uploadUniformFloat3(const std::string& name, const glm::vec3& vec)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, vec.x, vec.y, vec.z);
	}

	void OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& vec)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* value, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, glm::mat4 matrix)
	{
		uploadUniformMat4(name, matrix);
	}
	void OpenGLShader::SetFloat4(const std::string& name, glm::vec4 vector)
	{
		uploadUniformFloat4(name, vector);
	}
	void OpenGLShader::SetFloat3(const std::string& name, glm::vec3 vector)
	{
		uploadUniformFloat3(name, vector);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* value, uint32_t count)
	{
		UploadUniformIntArray(name, value, count);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind()
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind()
	{
		glUseProgram(0);
	}
}