#ifndef TEXTURE_H
#define TEXTURE_H

#include <qopenglfunctions_3_3_core.h>
#include "Buffer.h"
#include <QOpenGLTexture>
#include <qimage.h>
#include <QOpenGLShaderProgram>

class Texture
{
public:
    Texture(std::string path);
    void SetToShader(QOpenGLShaderProgram& shader, std::string uniformValue, int portNum=0);
    void SetToShader(QOpenGLFunctions_3_3_Core* m_glfunc, QOpenGLShaderProgram& shader, std::string uniformValue, GLint portNum=0);
   // color3f GetColor(int x, int y) const;
    QOpenGLTexture * TextureI();
private:
    QOpenGLTexture * m_texture;
};
#endif // TEXTURE_H
