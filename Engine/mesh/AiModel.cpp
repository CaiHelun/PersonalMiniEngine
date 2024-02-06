#include"AiModel.h"
#include "../Image/stb_image.h"
#include <iostream>

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

AiModel::AiModel(const char* path)
{
	_LoadModel(path);
}

AiModel::~AiModel()
{
}

void AiModel::Render(Shader& shader)
{
	for (auto mesh : mMeshes)
		mesh.Render(shader);
}

void AiModel::_LoadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	mDirectory = path.substr(0, path.find_last_of("/"));

	_ProcessNode(scene->mRootNode, scene);

}

void AiModel::_ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		mMeshes.emplace_back(_ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		_ProcessNode(node->mChildren[i], scene);
	}
}

Mesh AiModel::_ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<MeshVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		MeshVertex vertex;
		glm::vec3 float3;
		float3.x = mesh->mVertices[i].x;
		float3.y = mesh->mVertices[i].y;
		float3.z = mesh->mVertices[i].z;
		vertex.mPosition = float3;

		float3.x = mesh->mNormals[i].x;
		float3.y = mesh->mNormals[i].y;
		float3.z = mesh->mNormals[i].z;
		vertex.mNormal = float3;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 texC;
			texC.x = mesh->mTextureCoords[0][i].x;
			texC.y = mesh->mTextureCoords[0][i].y;
			vertex.mTexCoord = texC;
		}
		else
			vertex.mTexCoord = glm::vec2(.0f);

		for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; ++j)
				indices.emplace_back(face.mIndices[j]);
		}

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<Texture> diffTex = _LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffTex.begin(), diffTex.end());

			std::vector<Texture> specTex = _LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specTex.begin(), specTex.end());
		}
	}
}

std::vector<Texture> AiModel::_LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString path;
		mat->GetTexture(type, i, &path);
		bool alreadyLoad = false;
		for (unsigned int j = 0; j < mTextureLoaded.size(); ++j)
		{
			if (std::strcmp(mTextureLoaded[i].mPath.c_str(), path.C_Str()) == 0)
			{
				textures.emplace_back(mTextureLoaded[i]);
				alreadyLoad = true;
				break;
			}
		}

		if (!alreadyLoad)
		{
			Texture texture;
			texture.mTextureID = TextureFromFile(path.C_Str());
			texture.mTextureType = typeName;
			texture.mPath = path.C_Str();
			textures.emplace_back(texture);
			mTextureLoaded.emplace_back(std::move(texture));
		}
	}
	
	return textures;
}


unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false)
{
	std::string fileName = std::string(path);

	fileName = directory + "/" + fileName;

	unsigned int textureId;
	glGenTextures(1, &textureId);

	int width, height, channel;
	unsigned char* data= stbi_load(fileName.c_str(), &width, &width, &channel, 0);
	if (data)
	{
		GLenum format;
		if (channel == 1)
			format = GL_RED;
		else if (channel == 3)
			format = GL_RGB;
		else if (channel == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture Load Failed!Path = " << path << std::endl;
		stbi_image_free(data);
	}

	return textureId;
}
