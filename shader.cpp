#include "shader.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Shader::Shader(char *FragmentShader, char *VertexShader)
{
	drawShadows = false;

	FragmentShaderFile = new char[strlen(FragmentShader)+1];
	VertexShaderFile = new char[strlen(VertexShader)+1];
	strcpy(FragmentShaderFile, FragmentShader);
	strcpy(VertexShaderFile, VertexShader);

	InitShaders();
}

Shader::~Shader()
{
	deleteShader();
}

void Shader::InitShaders()
{
	 // Lecture des fichiers
    // Certaines personnes aiment avoir le fichier du vertex shader avec l'extension .vert
    // et le fichier du fragment shader avec l'extension .frag
	GLchar* vertexSource = (GLchar*)readFile(VertexShaderFile);
	GLchar* fragmentSource = (GLchar*)readFile(FragmentShaderFile);
    GLint programState = 0;
    GLint vertexSize = 0;
    GLint fragmentSize = 0;
 
    // Création des IDs
    vertexID = glCreateShader(GL_VERTEX_SHADER);
    fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
 
    // Vérification des fichiers
    if ( !vertexSource || !fragmentSource )
    {
        deleteShader();    // Nettoyage
        return;
    }
 
    // Chargement des sources dans OpenGL
    vertexSize = strlen(vertexSource);
    fragmentSize = strlen(fragmentSource);
    glShaderSource(vertexID, 1, (const GLchar**)(&vertexSource), &vertexSize);
    glShaderSource(fragmentID, 1, (const GLchar**)(&fragmentSource), &fragmentSize);
 
    // Compilation du vertex shader
    glCompileShader(vertexID);
    glCompileShader(fragmentID);
 
    // Vérification des erreurs
    if ( !checkShaderCompilation(vertexID) || !checkShaderCompilation(fragmentID))
    {
        deleteShader();
        return;
    }
 
 
    // Creation de l'ID pour le programme
    programID = glCreateProgram();
 
    // On attache les shaders ensemble
    glAttachShader(programID, vertexID);
    glAttachShader(programID, fragmentID);
 
    // On peut enfin passer aux liage.
    glLinkProgram(programID);
	ShadowMapUniform = glGetUniformLocationARB(programID,"ShadowMap");
 
    // Et encore une fois on vérifie si tout se passe bien
    glGetProgramiv(programID , GL_LINK_STATUS  , &programState);
    if ( programState != GL_TRUE)
    {
        // On récupère la taille du log
        GLint logSize = 0;
        GLchar* log = NULL;
 
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logSize);
 
        // On peut allouer la mémoire, une fois que l'on a la taille du log
        log = (GLchar*)malloc(logSize);
        if ( log == NULL )
        {
            fprintf(stderr,"Erreur d'allocation de mémoire pour le log de la compilation du programme\n");
            deleteShader();
            return;
        }
 
        // Et on récupère le log
        glGetProgramInfoLog(programID, logSize, &logSize, log);
 
        // On affiche
        fprintf(stderr,"Erreur lors du liage du shader:\n%s",log);
 
        free(log);
        deleteShader();
        return;
    }
 
    // Voilà, nous sommes prêt
    glUseProgram(programID);
}

bool Shader::GenerateShadowFBO()
{
	GLenum FBOstatus;

	// Try to use a texture depth component
	glGenTextures(1, &DepthTextureId);
	glBindTexture(GL_TEXTURE_2D, DepthTextureId);

	// GL_LINEAR does not make sense for depth texture. However, next tutorial shows usage of GL_LINEAR and PCF
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Remove artifact on the edges of the shadowmap
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	// No need to force GL_DEPTH_COMPONENT24, drivers usually give you the max precision if available
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// create a framebuffer object
	glGenFramebuffersEXT(1, &FBOid);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FBOid);

	// Instruct openGL that we won't bind a color texture with the currently bound FBO
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// attach the texture to FBO depth attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, DepthTextureId, 0);

	// check FBO status
	FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT) {
		printf("GL_FRAMEBUFFER_COMPLETE_EXT failed, CANNOT use FBO\n");
		return false;
	}

	// switch back to window-system-provided framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	return true;
}

GLchar* Shader::readFile(char *fileName)
{
	FILE *pFile = fopen(fileName, "rb");

	if(pFile == NULL)
	{
		std::cerr << "Impossible d'ouvrir le fichier " << fileName << "\n";
		return NULL;
	}

	long length = 0;
 
    fseek(pFile,0,SEEK_END);
 
	length = ftell(pFile);

	// Ne pas oublier de mettre le fichier à son début, sinon on ne peut pas le lire
	fseek(pFile,0,SEEK_SET);

	char* fileContent = (char*)malloc(length+1);
	if ( fileContent == NULL )
	{
		return "";
	}

	fread(fileContent,length,1,pFile);
	// Termine le tableau qui contient le shader
	fileContent[length] = '\0';

	return fileContent;
}

void Shader::deleteShader()
{
	glUseProgram(0);
	glDetachShader(programID, vertexID);
	glDetachShader(programID, fragmentID);
	glDeleteProgram(programID);
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}

bool Shader::checkShaderCompilation(GLuint id)
{
	GLint compilationStatus = 0;
 
    // Vérification de la compilation pour le vertex shader
    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &compilationStatus);
    if ( compilationStatus != GL_TRUE )
    {
        // Nous savons que la compilation a échoué, donc nous devons savoir pourquoi
        // Nous devons récupéré la taille du log
        GLint logLength = 0;
        GLchar* log = NULL;
 
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
 
        // Maintenant que nous avons la taille, nous pouvons alloué la mémoire suffisante pour ce log
        log = (GLchar*)malloc(logLength);
        if ( log == NULL )
        {
            fprintf(stderr,"Erreur d'allocation de mémoire pour le log de la compilation du shader\n");
            return 0;
        }
 
        glGetShaderInfoLog(id, logLength, &logLength, log);
 
        // On peut afficher le message
        fprintf(stderr,"Erreur de compilation:\n%s",log);
 
        // Et on n'oublie pas de libérer la mémoire
        free(log);
 
        return false;
    }
 
    return true; // Pas d'erreur
}
