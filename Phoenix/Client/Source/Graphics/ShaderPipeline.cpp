// Copyright 2019-20 Genten Studios
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors
// may be used to endorse or promote products derived from this software without
// specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <Client/Graphics/ShaderPipeline.hpp>

#include <Common/FileIO.hpp>

#include <glad/glad.h>

#include <iostream>

using namespace phx::gfx;
using namespace phx;

void ShaderPipeline::prepare(const std::string& vertShaderPath,
                             const std::string& fragShaderPath,
                             const std::vector<ShaderLayout>& layout)
{
	const std::string vertShaderSource = FileIO::readAllFile(vertShaderPath);
	const std::string fragShaderSource = FileIO::readAllFile(fragShaderPath);

	const char* vertShaderSourceRaw = vertShaderSource.c_str();
	const char* fragShaderSourceRaw = fragShaderSource.c_str();

	int  success;
	char infoLog[512];

	//// Vertex Shader
	const int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertShaderSourceRaw, nullptr);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
		          << infoLog << std::endl;
	}
	//// END Vertex Shader

	//// Fragment Shader
	const int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderSourceRaw, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
		          << infoLog << std::endl;
	}
	//// END Fragment Shader

	//// Shader Linking
	m_program = glCreateProgram();
	glAttachShader(m_program, vertexShader);
	glAttachShader(m_program, fragmentShader);
	glLinkProgram(m_program);

	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_program, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
		          << infoLog << std::endl;
	}
	//// END Shader Linking

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	for (auto& attr : layout)
	{
		glBindAttribLocation(m_program, attr.desiredIndex,
		                     attr.attribName.c_str());
	}
}

void ShaderPipeline::activate() { glUseProgram(m_program); }

void ShaderPipeline::setInt(const std::string& location, int value)
{
	glUniform1i(glGetUniformLocation(m_program, location.c_str()), value);
}

void ShaderPipeline::setFloat(const std::string& location, float value)
{
	glUniform1f(glGetUniformLocation(m_program, location.c_str()), value);
}

void ShaderPipeline::setVector2(const std::string& location,
                                const math::vec2&  value)
{
	glUniform2fv(glGetUniformLocation(m_program, location.c_str()), 1,
	             &value.x);
}

void ShaderPipeline::setVector3(const std::string& location,
                                const math::vec3&  value)
{
	glUniform3fv(glGetUniformLocation(m_program, location.c_str()), 1,
	             &value.x);
}

void ShaderPipeline::setMatrix(const std::string& location, const math::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(m_program, location.c_str()), 1,
	                   GL_FALSE, value.elements);
}

int ShaderPipeline::queryLayoutOfAttribute(const std::string& attr)
{
	return glGetAttribLocation(m_program, attr.c_str());
}

