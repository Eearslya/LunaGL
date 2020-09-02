#pragma once

#include <unordered_map>

#include "Mesh.h"
#include "OpenGL/Shader.h"

struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiScene;

enum aiTextureType;

class Model {
 public:
  Model(const char* filePath);

  void Draw(const Ref<Shader>& shader);

 private:
  void ProcessNode(aiNode* node, const aiScene* scene);
  Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
  void LoadMaterialTextures(aiMaterial* mat, std::vector<TextureBinding>& bindings,
                            aiTextureType type);

  std::vector<Mesh> m_Meshes;
  std::string m_Directory;
  std::unordered_map<std::string, Ref<Texture>> m_LoadedTextures;
  unsigned long m_MeshCount;
  unsigned long m_FaceCount;
  unsigned long long m_VertexCount;
  unsigned long long m_IndexCount;
};