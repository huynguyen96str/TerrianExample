#ifndef TEXTURE_H
#define TEXTURE_H

#include "Buffer.h"
#include <QOpenGLTexture>
#include <qimage.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
class Texture
{
public:
    Texture(std::string path);
    void SetToShader(QOpenGLShaderProgram& shader, std::string uniformValue, unsigned int portNum=0);
private:
    QOpenGLTexture * m_texture;
    QOpenGLFunctions_3_3_Core *functions=nullptr;


};
#endif // TEXTURE_H
