#pragma once
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

class Texture2D
{
private:
	GLuint Texture;
	
public:
	void LoadTexture(const char* filename);
	void Draw(float, float, float);
};

