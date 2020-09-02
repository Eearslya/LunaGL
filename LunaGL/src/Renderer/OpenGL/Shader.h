#pragma once

#include <glm/glm.hpp>
#include <string>

class Shader {
 public:
  Shader(const std::string& vertexFile, const std::string& fragmentFile,
         const std::string& geometryFile = "");
  ~Shader();

  void Bind() const;
  void Unbind() const;

  void SetBool(const std::string& name, bool value);
  void SetInt(const std::string& name, int value);
  void SetInt2(const std::string& name, glm::ivec2 value);
  void SetInt3(const std::string& name, glm::ivec3 value);
  void SetInt4(const std::string& name, glm::ivec4 value);
  void SetFloat(const std::string& name, float value);
  void SetFloat2(const std::string& name, glm::vec2 value);
  void SetFloat3(const std::string& name, glm::vec3 value);
  void SetFloat4(const std::string& name, glm::vec4 value);
  void SetMat4(const std::string& name, glm::mat4 value);

  static Ref<Shader> Create(const std::string& vertexFile, const std::string& fragmentFile,
                            const std::string& geometryFile = "");

 private:
  int32_t GetUniformLocation(const std::string& name);
  U32 Compile(const std::string& sourceFile, U32 shaderType);
  std::string ReadFile(const std::string& filepath);

  U32 m_RendererID;
};