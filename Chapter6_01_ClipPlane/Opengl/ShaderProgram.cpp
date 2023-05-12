#include "ShaderProgram.h"

void ShaderProgram::Load(QOpenGLShaderProgram& Shader ,std::string fileVertexShader, std::string fileFragmentShader)
{
    bool m_IsSuccess = false;
    Shader.addShaderFromSourceFile(QOpenGLShader::Vertex,QString::fromStdString(fileVertexShader));
    Shader.addShaderFromSourceFile(QOpenGLShader::Fragment, QString::fromStdString(fileFragmentShader));
    Shader.link();
    m_IsSuccess = Shader.link();
    if (!m_IsSuccess)
        qDebug() << "Error shader: " << Shader.log();
}
