#pragma once 

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class AiModel
{
public:
	AiModel(const char* path);
	~AiModel();
	
	void Render(Shader& shader);

protected:
	void _LoadModel(const std::string& path);
	void _ProcessNode(aiNode* node, const aiScene* scene);
	Mesh _ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> _LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);

private:
	std::string mDirectory;
	std::vector<Texture> mTextureLoaded;
	std::vector<Mesh> mMeshes;

};
