#include "pch.h"

#include "Model.h"

#include <glad/glad.h>

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

Model::Model(const char* filePath)
    : m_MeshCount(0), m_VertexCount(0), m_FaceCount(0), m_IndexCount(0) {
  LOGI("Loading mesh from \"{}\"...", filePath);

  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(
      filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices |
                    aiProcess_GenNormals | aiProcess_OptimizeMeshes);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    abort();
    return;
  }

  std::string_view file(filePath);
  m_Directory = file.substr(0, file.find_last_of('/') + 1);

  ProcessNode(scene->mRootNode, scene);
  LOGI("Model Statistics:");
  LOGI("- {} meshes", m_MeshCount);
  LOGI("- {} faces", m_FaceCount);
  LOGI("- {} vertices", m_VertexCount);
  LOGI("- {} indices", m_IndexCount);
}

void Model::Draw(const Ref<Shader>& shader) {
  for (const auto& mesh : m_Meshes) {
    mesh.Draw(shader);
  }
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
  m_MeshCount += node->mNumMeshes;

  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    m_Meshes.push_back(ProcessMesh(mesh, scene));
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    ProcessNode(node->mChildren[i], scene);
  }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<TextureBinding> textures;

  m_VertexCount += mesh->mNumVertices;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex v{{mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z},
             {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z}};

    if (mesh->mTextureCoords[0]) {
      v.TexCoords = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
    }

    vertices.push_back(v);
  }

  m_FaceCount += mesh->mNumFaces;
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    m_IndexCount += face.mNumIndices;
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  if (mesh->mMaterialIndex >= 0) {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    LoadMaterialTextures(material, textures, aiTextureType_DIFFUSE);
    LoadMaterialTextures(material, textures, aiTextureType_SPECULAR);
  }

  return Mesh(vertices, indices, textures);
}

void Model::LoadMaterialTextures(aiMaterial* mat, std::vector<TextureBinding>& bindings,
                                 aiTextureType type) {
  unsigned int count = mat->GetTextureCount(type);

  for (unsigned int i = 0; i < count; i++) {
    aiString str;
    mat->GetTexture(type, i, &str);

    TextureBinding binding;
    if (type == aiTextureType_DIFFUSE) {
      binding.Type = TexType::Diffuse;
    } else if (type == aiTextureType_SPECULAR) {
      binding.Type = TexType::Specular;
    }

    auto cache = m_LoadedTextures.find(str.C_Str());
    if (cache == m_LoadedTextures.end()) {
      LOGI("- Loading texture \"{}\"", m_Directory + str.C_Str());
      binding.Texture = CreateRef<Texture>(m_Directory + str.C_Str());
      m_LoadedTextures.insert_or_assign(str.C_Str(), binding.Texture);
    } else {
      binding.Texture = cache->second;
    }

    bindings.push_back(binding);
  }
}
