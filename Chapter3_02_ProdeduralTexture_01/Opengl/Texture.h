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
    //void SetToShader(QOpenGLShaderProgram& shader, std::string uniformValue, int portNum=0);
    void SetToShader(QOpenGLShaderProgram& shader, std::string uniformValue, unsigned int portNum=0);
   // color3f GetColor(int x, int y) const;
    QOpenGLTexture * TextureI();
private:
    QOpenGLTexture * m_texture;
    QOpenGLFunctions_3_3_Core *functions=nullptr;
    QOpenGLShaderProgram shaderProgram;
    // Lấy ID của shader program đang active
    GLint m_currentProgramId;

};
#endif // TEXTURE_H
