#pragma once

#include "Texture.h"
#include "MeshData.h"
#include <vector>
#include "../Shader.h"

class Mesh
{
public:
	Mesh(const std::vector<MeshVertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);
	~Mesh();

	void Render(Shader& shader);
private:
	void _SetupMeshs();

private:
	std::vector<MeshVertex> mVertices;
	std::vector<unsigned int> mIndices;
	std::vector<Texture> mTextures;
	std::vector<int>     mTextureUniformLocation;

	unsigned int VAO, VBO, EBO;

};