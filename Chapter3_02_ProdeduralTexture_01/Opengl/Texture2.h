#ifndef TEXTURE2_H
#define TEXTURE2_H

#include <qopenglfunctions_3_3_core.h>
#include "Buffer.h"
#include <QOpenGLTexture>
#include <qimage.h>
#include <QOpenGLShaderProgram>
#include<QList>
struct TextureData{
    QOpenGLTexture* Texture;
    int Port;
};

class Texture2
{
public:
    Texture2(std::string path, int port=0);
    void AddTexture(std::string path, int port=0);
    void SetToShader(QOpenGLFunctions_3_3_Core* m_glfunc,QOpenGLShaderProgram& shader);
private:
    QList<TextureData> m_data={};
};

#endif // TEXTURE2_H
