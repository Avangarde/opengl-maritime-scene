#include "renderableLoaded.h"
#include <iostream>
//#include <string>
#include <sstream>
#include <fstream>

RenderableLoaded::RenderableLoaded(const char *filePath, const char *texturePath)
{
	mVertices = NULL;
	mNormals = NULL;
	mTextureVertices = NULL;
	mFaces = NULL;
	mTextured = false;
	mTextureData = NULL;
	std::string lName(filePath);
	if(lName.find(".obj") != std::string::npos)
	{
		loadOBJ(filePath);
		return;
	}
	if(lName.find(".xml") != std::string::npos)
	{
		loadXML(filePath, texturePath);
		return;
	}
	else
	{
		std::cerr << "Cannot load " << filePath << " : File type unknown\n";
	}
}

RenderableLoaded::~RenderableLoaded()
{
}

void RenderableLoaded::loadOBJ(const char *filePath)
{
	// Fichier OBJ : bien pour le statique. Prendre OgreXML pour l'animation squelettale. Encore du boulot...
	// http://pastebin.com/2zruHhAn
	std::ifstream in(filePath, std::ifstream::in);
	if(!in){
		std::cerr << "Model \"" << filePath << "\" does not exist\n";
		return;
	}

	std::vector<GLfloat*> lVertTemp;
	std::vector<GLfloat*> lNormTemp;
	std::vector<GLint*> lFaceTemp;
	std::vector<GLint*> lFaceNormTemp;

	std::string line;
	
	while(getline(in, line))
	{
		if(line.substr(0,2) == "v ")
		{
			std::istringstream s(line.substr(2));
			GLfloat *lVert = new GLfloat[3];
			s >> lVert[0] >> lVert[1] >> lVert[2];
			lVertTemp.push_back(lVert);
		}
		else if(line.substr(0,3) == "vn ")
		{
			std::istringstream s(line.substr(3));
			GLfloat *lNorm = new GLfloat[3];
			s >> lNorm[0] >> lNorm[1] >> lNorm[2];
			lNormTemp.push_back(lNorm);
		}
		// Ici, gérer les textures
		else if(line.substr(0,2) == "f ")
		{
			std::istringstream s(line.substr(2));
			char tmp;
			unsigned short a,b,c, d,e,f, g,h,i;
			s >> a >> tmp >> d >> tmp >> g;
			s >> b >> tmp >> e >> tmp >> h;
			s >> c >> tmp >> f >> tmp >> i;
			a--; b--; c--; d--; e--; f--; g--; h--; i--;
//			std::cout << a << ", " << b << ", " << c << " - " << g << ", " << h << ", " << i << "\n";
			GLint *lFace = new GLint[3];
			lFace[0] = a;
			lFace[1] = b;
			lFace[2] = c;
			lFaceTemp.push_back(lFace);
			GLint *lFaceNorm = new GLint[3];
			lFaceNorm[0] = g;
			lFaceNorm[1] = h;
			lFaceNorm[2] = i;
			lFaceNormTemp.push_back(lFaceNorm);
			// Là aussi, gérer les textures
		}
		else
		{
			continue;
		}
	}
	mNbFaces = lFaceTemp.size();
	mVertices = new GLfloat[9*mNbFaces];
	mNormals= new GLfloat[9*mNbFaces];
	for(int i = 0 ; i < mNbFaces ; i++)
	{

		mVertices[9*i+0] = lVertTemp[lFaceTemp[i][0]][0];
		mVertices[9*i+1] = lVertTemp[lFaceTemp[i][0]][1];
		mVertices[9*i+2] = lVertTemp[lFaceTemp[i][0]][2];
		//delete lVertTemp[lFaceTemp[i][0]];

		mVertices[9*i+3] = lVertTemp[lFaceTemp[i][1]][0];
		mVertices[9*i+4] = lVertTemp[lFaceTemp[i][1]][1];
		mVertices[9*i+5] = lVertTemp[lFaceTemp[i][1]][2];
		//delete lVertTemp[lFaceTemp[i][1]];

		mVertices[9*i+6] = lVertTemp[lFaceTemp[i][2]][0];
		mVertices[9*i+7] = lVertTemp[lFaceTemp[i][2]][1];
		mVertices[9*i+8] = lVertTemp[lFaceTemp[i][2]][2];
		//delete lVertTemp[lFaceTemp[i][2]];

		mNormals[9*i+0] = lNormTemp[lFaceNormTemp[i][0]][0];
		mNormals[9*i+1] = lNormTemp[lFaceNormTemp[i][0]][1];
		mNormals[9*i+2] = lNormTemp[lFaceNormTemp[i][0]][2];
		//delete lNormTemp[lFaceNormTemp[i][0]];

		mNormals[9*i+3] = lNormTemp[lFaceNormTemp[i][1]][0];
		mNormals[9*i+4] = lNormTemp[lFaceNormTemp[i][1]][1];
		mNormals[9*i+5] = lNormTemp[lFaceNormTemp[i][1]][2];
		//delete lNormTemp[lFaceNormTemp[i][1]];

		mNormals[9*i+6] = lNormTemp[lFaceNormTemp[i][2]][0];
		mNormals[9*i+7] = lNormTemp[lFaceNormTemp[i][2]][1];
		mNormals[9*i+8] = lNormTemp[lFaceNormTemp[i][2]][2];
		//delete lNormTemp[lFaceNormTemp[i][2]];
	}
}

void RenderableLoaded::loadXML(const char *filePath, const char *texturePath)
{
	std::cout << "Loading XML file : " << filePath << "\n";
	FILE *f = fopen(filePath, "rb");
	if(!f)
	{
		std::cerr << "Model \"" << filePath << "\" does not exist\n";
		return;
	}

	char line[1000];
	memset ( line,0,1000 );

	std::vector<GLfloat*> lVertTemp;
	std::vector<GLfloat*> lNormTemp;
	std::vector<GLint*> lFaceTemp;
	std::vector<GLint*> lFaceNormTemp;
	std::vector<GLfloat*> lTexTemp;

	setlocale(LC_ALL, "C");

	while(fgets(line, 1000, f) != NULL)
	{
		GLfloat *lVert = new GLfloat[3];
		GLfloat *lNorm = new GLfloat[3];
		GLfloat *lTex = new GLfloat[2];
		GLint *lFace = new GLint[3];
		int boneindex, vertexindex;
		float weight;

		if(sscanf(line," <face v1=\"%d\" v2=\"%d\" v3=\"%d\"/>",&lFace[0],&lFace[1],&lFace[2])==3)
		{
			lFaceTemp.push_back(lFace);
		}
		if(sscanf(line," <position x=\"%f\" y=\"%f\" z=\"%f\"/>",&lVert[0],&lVert[1],&lVert[2])==3)
		{
			lVertTemp.push_back(lVert);
		}
		if(sscanf(line," <normal x=\"%f\" y=\"%f\" z=\"%f\"/>",&lNorm[0],&lNorm[1],&lNorm[2])==3)
		{
			lNormTemp.push_back(lNorm);
		}
		if(sscanf(line," <texcoord u=\"%f\" v=\"%f\"/>",&lTex[0],&lTex[1])==2)
		{
			if(texturePath)
				mTextured = true;

			lTexTemp.push_back(lTex);
		}
		if(sscanf(line," <vertexboneassignment vertexindex=\"%d\" boneindex=\"%d\" weight=\"%f\" />",&vertexindex,&boneindex,&weight )==3)
		{
			// TODO
		}
	}

	mNbFaces = lFaceTemp.size();
	std::cout << mNbFaces << " Faces\n";
	mVertices = new GLfloat[9*mNbFaces];
	mNormals = new GLfloat[9*mNbFaces];
	mTextureVertices = new GLfloat[6*mNbFaces];
	for(int i = 0 ; i < mNbFaces ; i++)
	{

		mVertices[9*i+0] = lVertTemp[lFaceTemp[i][0]][0];
		mVertices[9*i+1] = lVertTemp[lFaceTemp[i][0]][1];
		mVertices[9*i+2] = lVertTemp[lFaceTemp[i][0]][2];
		//delete lVertTemp[lFaceTemp[i][0]];

		mVertices[9*i+3] = lVertTemp[lFaceTemp[i][1]][0];
		mVertices[9*i+4] = lVertTemp[lFaceTemp[i][1]][1];
		mVertices[9*i+5] = lVertTemp[lFaceTemp[i][1]][2];
		//delete lVertTemp[lFaceTemp[i][1]];

		mVertices[9*i+6] = lVertTemp[lFaceTemp[i][2]][0];
		mVertices[9*i+7] = lVertTemp[lFaceTemp[i][2]][1];
		mVertices[9*i+8] = lVertTemp[lFaceTemp[i][2]][2];
		//delete lVertTemp[lFaceTemp[i][2]];

		mNormals[9*i+0] = lNormTemp[lFaceTemp[i][0]][0];
		mNormals[9*i+1] = lNormTemp[lFaceTemp[i][0]][1];
		mNormals[9*i+2] = lNormTemp[lFaceTemp[i][0]][2];
		//delete lNormTemp[lFaceNormTemp[i][0]];

		mNormals[9*i+3] = lNormTemp[lFaceTemp[i][1]][0];
		mNormals[9*i+4] = lNormTemp[lFaceTemp[i][1]][1];
		mNormals[9*i+5] = lNormTemp[lFaceTemp[i][1]][2];
		//delete lNormTemp[lFaceNormTemp[i][1]];

		mNormals[9*i+6] = lNormTemp[lFaceTemp[i][2]][0];
		mNormals[9*i+7] = lNormTemp[lFaceTemp[i][2]][1];
		mNormals[9*i+8] = lNormTemp[lFaceTemp[i][2]][2];
		//delete lNormTemp[lFaceNormTemp[i][2]];

		if(mTextured)
		{
			mTextureVertices[6*i+0] = lTexTemp[lFaceTemp[i][0]][0];
			mTextureVertices[6*i+1] = lTexTemp[lFaceTemp[i][0]][1];

			mTextureVertices[6*i+2] = lTexTemp[lFaceTemp[i][1]][0];
			mTextureVertices[6*i+3] = lTexTemp[lFaceTemp[i][1]][1];

			mTextureVertices[6*i+4] = lTexTemp[lFaceTemp[i][2]][0];
			mTextureVertices[6*i+5] = lTexTemp[lFaceTemp[i][2]][1];
			//delete lVertTemp[lFaceTemp[i][1]];
		}
	}

	if(mTextured)
		loadTexture(texturePath);
}

void RenderableLoaded::loadTexture(const char *texturePath)
{
	std::cout << "Loading texture " << texturePath << "\n";
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;

	FILE *file = fopen(texturePath, "rb");
	if(!file)
	{
		mTextured = false;
		std::cerr << "Cannot load " << texturePath << "\n";
		fclose(file);
		return;
	}

	if(fread(header, 1, 54, file) != 54)
	{
		mTextured = false;
		std::cerr  << "Error while loading " << texturePath << ", not a bmp file\n";
		fclose(file);
		return;
	}

	if(header[0] != 'B' || header[1] != 'M')
	{
		mTextured = false;
		std::cerr  << "Error while loading " << texturePath << ", not a bmp file\n";
		fclose(file);
		return;
	}

	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	mTextureWidth      = *(int*)&(header[0x12]);
	mTextureHeight     = *(int*)&(header[0x16]);

	if(imageSize == 0)
		imageSize = mTextureWidth * mTextureHeight * 3;
	if(dataPos == 0)
		dataPos = 54;

	mTextureData = new unsigned char[imageSize];
	fread(mTextureData, 1, imageSize, file);
	fclose(file);

	glGenTextures(1, mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture[1]);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, mTextureWidth, mTextureHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, mTextureData); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void RenderableLoaded::draw()
{
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	if(mTextured)
	{
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, mTexture[1]);
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, mTextureWidth, mTextureHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, mTextureData); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	glNormalPointer(GL_FLOAT, 0, mNormals);
	glVertexPointer(3, GL_FLOAT, 0 , mVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, mTextureVertices);

	glDrawArrays(GL_TRIANGLES, 0, 3*mNbFaces);

	// disable the use of arrays (do not forget!)
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	if(mTextured)
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
