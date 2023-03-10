#include"tgepch.h"
#include"OpenGLShader.h"
#include<glad/glad.h>
#include"TurboGE/Logger.h"
#include"glm/gtc/type_ptr.hpp"

namespace TurboGE
{
	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{

		CompileProgram(vertexSrc, fragmentSrc);

	}

	OpenGLShader::OpenGLShader(const std::string& path)
	{
		
		std::string vertexSrc, fragmentSrc;
		parseFile(path, vertexSrc, fragmentSrc);
		CompileProgram(vertexSrc, fragmentSrc);
	}

	void OpenGLShader::parseFile(const std::string& path, std::string& vertexSrc, std::string& fragmentSrc)
	{
		bool shaderFlag = false;
		bool vertexFlag = false, fragmentFlag = false;
		std::ifstream inf{ path };
		if (!inf)
		{
			TURBO_CORE_ERR("No file found");
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


		std::cout << "V " << vertexSrc << std::endl;
		std::cout << "f " << fragmentSrc << std::endl;
	}

	void OpenGLShader::CompileProgram(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			TURBO_CORE_ERR(" Vertex Shader {0}", infoLog.data());
			// In this simple program, we'll just leave
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.
			TURBO_CORE_ERR(" Fragment shader {0}", infoLog.data());
			// In this simple program, we'll just leave
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		GLuint program = glCreateProgram();
		m_RendererID = program;

		// Attach our shaders to our program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

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
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.
			TURBO_CORE_ERR("{0}", infoLog.data());
			// In this simple program, we'll just leave
			return;
		}

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