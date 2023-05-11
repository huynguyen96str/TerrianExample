#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include "Opengl/GlobalOpengl.h"

class ShaderProgram
{
public:
    static void Load(QOpenGLShaderProgram& Shader,std::string fileVertexShader, std::string fileFragmentShader);
};
#endif // SHADERPROGRAM_H
