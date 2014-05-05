#ifndef __SHADER__
#define __SHADER__

#include "GL/glew.h"

class Shader {
private:
// Je n'active pas les ombres pour l'instant, mais on na sait jamais
static const int SHADOWMAP_WIDTH = 8192;
static const int SHADOWMAP_HEIGHT = 8192;
GLuint vertexID;
GLuint fragmentID;
GLuint programID;
char *FragmentShaderFile;
char *VertexShaderFile;
GLuint silhouetteVertexID;
GLuint silhouetteFragmentID;
GLuint silhouetteProgramID;

// ID du framebuffer object pour le rendu du point de vue de la lumiere
GLuint FBOid;
// ID de la texture générée
GLuint DepthTextureId;
GLuint ShadowMapUniform;
bool drawShadows;

GLchar *readFile(char *fileName);
bool checkShaderCompilation(GLuint id);

public:
Shader(char *FragmentShader, char *VertexShader);
~Shader();
bool GenerateShadowFBO();
void InitShaders();
void deleteShader();
};

#endif
