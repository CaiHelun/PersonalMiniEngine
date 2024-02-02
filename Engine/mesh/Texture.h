#pragma once 
#include <string>

class Texture
{
public:
	Texture();
	~Texture();

private:
	unsigned int mTextureID = 0;
	std::string mTextureType = "";
};