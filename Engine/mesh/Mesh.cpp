#include "Mesh.h"

Mesh::Mesh(const std::vector<MeshVertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
	:mVertices(vertices),mIndices(indices),mTextures(textures)
{
	_SetupMeshs();
}

Mesh::~Mesh()
{

}

void Mesh::Render(Shader& shader)
{
	unsigned int diffuseNum = 1;
	unsigned int specularNum = 1;
	for (size_t i = 0; i < mTextures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		if (mTextures[i].mTextureType == "diffuse_texture")
			number = std::to_string(diffuseNum++);
		else if (mTextures[i].mTextureType == "specular_texture")
			number = std::to_string(specularNum++);

		shader.SetUniformInt(std::string("material." + mTextures[i].mTextureType + number), i);
		glBindTexture(GL_TEXTURE_2D, mTextures[i].mTextureID);
	}

	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::_SetupMeshs()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(MeshVertex), &mVertices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, mNormal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, mTexCoord));

	glBindVertexArray(0);
}
