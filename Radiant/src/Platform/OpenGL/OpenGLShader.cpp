#include "rdpch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Radiant {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		RD_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& shader_filepath)
	{
		RD_PROFILE_FUNCTION();

		std::string file_source = ReadFile(shader_filepath);
		auto shader_sources = PreProcess(file_source);

		Compile(shader_sources);

		// asset/shaders/texture.glsl
		auto last_slash = shader_filepath.find_last_of("/\\");
		last_slash = last_slash == std::string::npos ? 0 : last_slash + 1;

		auto last_dot = shader_filepath.rfind(".");

		auto count = last_dot == std::string::npos ? shader_filepath.size() - last_slash : last_dot - last_slash;

		m_name = shader_filepath.substr(last_slash, count);		
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertex_shader_file, const std::string& pixel_shader_file)
		: m_name(name)
	{
		RD_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertex_shader_file;
		sources[GL_FRAGMENT_SHADER] = pixel_shader_file;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		RD_PROFILE_FUNCTION();

		glDeleteProgram(m_renderer_id);
	}

	void OpenGLShader::Bind() const
	{
		RD_PROFILE_FUNCTION();

		glUseProgram(m_renderer_id);
	}

	void OpenGLShader::Unbind() const
	{
		RD_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix_data)
	{
		RD_PROFILE_FUNCTION();

		auto location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix_data));
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& value)
	{
		RD_PROFILE_FUNCTION();

		auto location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		RD_PROFILE_FUNCTION();

		auto location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniform4fv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		RD_PROFILE_FUNCTION();

		auto location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniform3fv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		RD_PROFILE_FUNCTION();

		auto location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniform2fv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		RD_PROFILE_FUNCTION();

		auto location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		RD_PROFILE_FUNCTION();

		auto location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		RD_PROFILE_FUNCTION();

		auto location = glGetUniformLocation(m_renderer_id, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		UploadUniformMat4(name, value);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& value)
	{
		UploadUniformMat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		UploadUniformFloat2(name, value);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		UploadUniformIntArray(name, values, count);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		RD_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in_sh(filepath, std::ios::in | std::ios::binary);
		if (in_sh)
		{
			// file size
			in_sh.seekg(0, std::ios::end);

			// string that size
			result.resize(in_sh.tellg());

			// back to beginning to read actual data into newly resize string
			in_sh.seekg(0, std::ios::beg);
			in_sh.read(&result[0], result.size());
			in_sh.close();
		}
		else
		{
			RD_CORE_ASSERT(false, "Could not open shader file: {0}", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		RD_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shader_sources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			RD_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			RD_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			RD_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shader_sources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shader_sources;

	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& sources)
	{
		RD_PROFILE_FUNCTION();

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		GLuint program = glCreateProgram();
		std::array<GLenum, MAX_SHADERS> shader_ids;
		uint32_t shader_index = 0;

		for (auto src : sources)
		{
			GLenum shader_type = src.first;
			std::string& raw_source = src.second;

			GLuint shader = glCreateShader(shader_type);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* source = (const GLchar*)raw_source.c_str();
			glShaderSource(shader, 1, &source, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint is_compiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
			if (is_compiled == GL_FALSE)
			{
				GLint max_length = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

				// The maxLength includes the NULL character
				std::vector<GLchar> info_log(max_length);
				glGetShaderInfoLog(shader, max_length, &max_length, &info_log[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.
				RD_CORE_ERROR("{0}", info_log.data());
				RD_CORE_ASSERT(false, "Shader compilation failed!");
				break;
			}

			glAttachShader(program, shader);
			shader_ids[shader_index++] = shader;
		}

		// Link our program
		glLinkProgram(program);

			// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint is_linked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&is_linked);
		if (is_linked == GL_FALSE)
		{
			GLint max_length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

			// The maxLength includes the NULL character
			std::vector<GLchar> info_log(max_length);
			glGetProgramInfoLog(program, max_length, &max_length, &info_log[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto id : shader_ids)
			{
				glDeleteShader(id);
			}

			// Use the infoLog as you see fit.
			RD_CORE_ERROR("{0}", info_log.data());
			RD_CORE_ASSERT(false, "Shader Link compilation failed!");

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : shader_ids)
		{
			glDetachShader(m_renderer_id, id);
		}

		m_renderer_id = program;
	}
}
