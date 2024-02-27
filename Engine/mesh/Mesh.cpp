#include "Mesh.h"

Mesh::Mesh(const std::vector<MeshVertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
	:mVertices(vertices),mIndices(indices),mTextures(textures)
{
	mTextureUniformLocation = std::vector<int>(mTextures.size(), -1);
	_SetupMeshs();
}

Mesh::~Mesh()
{

}

void Mesh::Render(Shader& shader)
{
	unsigned int diffuseNum = 1;
	unsigned int specularNum = 1;
	bool initUniformLocation = false;
	if (!mTextureUniformLocation.empty() && mTextureUniformLocation[0] != -1)
		initUniformLocation = true;

	for (size_t i = 0; i < mTextures.size(); ++i)
	{
		glActiveTexture(GLenum(GL_TEXTURE0 + i));
		if (!initUniformLocation)
		{
			std::string number;
			std::string name = mTextures[i].mTextureType;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNum++);
			else if (name == "texture_specular")
				number = std::to_string(specularNum++);
			name += number;
			mTextureUniformLocation[i] = glGetUniformLocation(shader.ShaderProgramID, name.c_str());
		}
		if (mTextureUniformLocation[i] > 0)
			glUniform1i(mTextureUniformLocation[i], i);
		glBindTexture(GL_TEXTURE_2D, mTextures[i].mTextureID);
	}
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)mIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
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
