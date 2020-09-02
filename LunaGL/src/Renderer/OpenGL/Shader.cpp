#include "pch.h"

#include "Shader.h"

#include "Log.h"

#include <glad/glad.h>

#include <fstream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile,
               const std::string& geometryFile)
    : m_RendererID(0) {
  U32 program = glCreateProgram();
  U32 vertexShader = Compile(vertexFile, GL_VERTEX_SHADER);
  U32 fragmentShader = Compile(fragmentFile, GL_FRAGMENT_SHADER);
  U32 geometryShader = 0;
  if (geometryFile.length() > 0) {
    geometryShader = Compile(geometryFile, GL_GEOMETRY_SHADER);
  }

  if (vertexShader != 0) {
    glAttachShader(program, vertexShader);
  }
  if (fragmentShader != 0) {
    glAttachShader(program, fragmentShader);
  }
  if (geometryShader != 0) {
    glAttachShader(program, geometryShader);
  }

  glLinkProgram(program);

  GLint isLinked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());

    if (vertexShader != 0) {
      glDeleteShader(vertexShader);
    }
    if (fragmentShader != 0) {
      glDeleteShader(fragmentShader);
    }
    if (geometryShader != 0) {
      glDeleteShader(geometryShader);
    }

    glDeleteProgram(program);
    LOGE("OpenGL Linker Error: {0}", infoLog.data());

    return;
  }

  if (vertexShader != 0) {
    glDetachShader(program, vertexShader);
  }
  if (fragmentShader != 0) {
    glDetachShader(program, fragmentShader);
  }
  if (geometryShader != 0) {
    glDetachShader(program, geometryShader);
  }

  m_RendererID = program;
}

Shader::~Shader() {
  if (m_RendererID != 0) {
    glDeleteProgram(m_RendererID);
  }
}

void Shader::Bind() const { glUseProgram(m_RendererID); }

void Shader::Unbind() const { glUseProgram(0); }

void Shader::SetBool(const std::string& name, bool value) {
  glUniform1i(GetUniformLocation(name), (int)value);
}

void Shader::SetInt(const std::string& name, int value) {
  glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetInt2(const std::string& name, glm::ivec2 value) {
  glUniform2i(GetUniformLocation(name), value.x, value.y);
}

void Shader::SetInt3(const std::string& name, glm::ivec3 value) {
  glUniform3i(GetUniformLocation(name), value.x, value.y, value.z);
}

void Shader::SetInt4(const std::string& name, glm::ivec4 value) {
  glUniform4i(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Shader::SetFloat(const std::string& name, float value) {
  glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetFloat2(const std::string& name, glm::vec2 value) {
  glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void Shader::SetFloat3(const std::string& name, glm::vec3 value) {
  glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void Shader::SetFloat4(const std::string& name, glm::vec4 value) {
  glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Shader::SetMat4(const std::string& name, glm::mat4 value) {
  glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

Ref<Shader> Shader::Create(const std::string& vertexFile, const std::string& fragmentFile,
                           const std::string& geometryFile) {
  return CreateRef<Shader>(vertexFile, fragmentFile, geometryFile);
}

int32_t Shader::GetUniformLocation(const std::string& name) {
  return glGetUniformLocation(m_RendererID, name.c_str());
}

U32 Shader::Compile(const std::string& sourceFile, U32 shaderType) {
  U32 shader = glCreateShader(shaderType);
  std::string source = ReadFile(sourceFile);
  const GLchar* sourceStr = source.c_str();

  glShaderSource(shader, 1, &sourceStr, nullptr);
  glCompileShader(shader);

  GLint isCompiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    std::vector<GLchar> infoLog(maxLength);
    glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());

    glDeleteShader(shader);
    LOGE("OpenGL Shader Error: {0}", infoLog.data());

    return 0;
  }

  return shader;
}

std::string Shader::ReadFile(const std::string& filepath) {
  std::ifstream in(filepath, std::ios::in | std::ios::binary);
  if (!in) {
    LOGE("Unable to open shader source file!");
    return std::string();
  }

  std::string contents;
  in.seekg(0, std::ios::end);
  contents.resize(in.tellg());
  in.seekg(0, std::ios::beg);
  in.read(&contents[0], contents.size());
  in.close();

  return contents;
}
