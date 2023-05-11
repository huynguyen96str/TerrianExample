#include "Texture.h"

Texture::Texture(std::string path) {

    m_texture = new QOpenGLTexture(QImage(QString::fromStdString(path)).mirrored());
    QOpenGLContext *context = QOpenGLContext::currentContext();
    functions = context->versionFunctions<QOpenGLFunctions_3_3_Core>();
}

void Texture::SetToShader( QOpenGLShaderProgram & shader, std::string uniformValue, unsigned int portNum)
{
    // m_texture->bind();
//     shader.setUniformValue(uniformValue.c_str(),portNum);
//     functions->glActiveTexture(GL_TEXTURE0 + portNum);
//     m_texture->bind(portNum);
    shader.setUniformValue(uniformValue.c_str(),portNum);
    m_texture->bind(portNum);
}

void Texture::bind()
{
    m_texture->bind();
}


void Texture::SetWrapTexture(QOpenGLTexture::CoordinateDirection direction, QOpenGLTexture::WrapMode wrapmode)
{
    m_texture->setWrapMode(direction, wrapmode);
}

void Texture::SetFilter(QOpenGLTexture::Filter minificationF, QOpenGLTexture::Filter magnifitionF)
{
   m_texture->setMinificationFilter(minificationF);
   m_texture->setMagnificationFilter(magnifitionF);
   m_texture->generateMipMaps(0);
}

