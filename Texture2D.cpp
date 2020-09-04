#include "Texture2D.h"

void Texture2D::LoadTexture(const char* filename)
{
	ILuint ImageName;

	ilutRenderer(ILUT_OPENGL);
	ilGenImages(1, &ImageName);
	ilBindImage(ImageName);
	ilLoadImage((ILstring)filename);
	Texture = ilutGLBindTexImage();
	
}

//draws image in square. (x,y) is the bottom left corner
void Texture2D::Draw(float x, float y, float width)
{
	glBindTexture(GL_TEXTURE_2D, Texture);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2f(x, y);
	glTexCoord2i(0, 1); glVertex2f(x, y + width);
	glTexCoord2i(1, 1); glVertex2f(x + width, y + width);
	glTexCoord2i(1, 0); glVertex2f(x+width, y);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
