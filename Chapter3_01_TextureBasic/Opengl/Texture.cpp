#include "Texture.h"

Texture::Texture(std::string path) {
    m_texture = new QOpenGLTexture(QImage(QString::fromStdString(path)).mirrored());
}

void Texture::SetToShader(QOpenGLShaderProgram& shader,std::string uniformValue, int portNum)
{
    shader.setUniformValue(uniformValue.c_str(),GL_TEXTURE0 + portNum);
    m_texture->bind(portNum);
}


