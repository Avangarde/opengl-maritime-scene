#pragma once
#include "renderable.h"
#include <vector>

#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

class RenderableLoaded : public Renderable
{
	private:
		GLint mNbFaces;
		GLfloat *mVertices;
		GLint **mFaces;
		GLfloat *mNormals;
		GLfloat *mTextureVertices;
		GLuint mTexture[1];
		unsigned char *mTextureData;
		unsigned int mTextureWidth;
		unsigned int mTextureHeight;
		bool mTextured;
		void loadOBJ(const char *filePath);
		void loadXML(const char *filePath, const char *texturePath);
		void loadTexture(const char *texturePath);
	public:
		RenderableLoaded(const char *filePath, const char *texturePath = NULL);
		~RenderableLoaded();
		void draw();
};
