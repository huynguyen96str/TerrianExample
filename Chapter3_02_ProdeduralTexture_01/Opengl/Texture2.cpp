#include "Texture2.h"


Texture2::Texture2(std::string path, int port)
{
    TextureData dt;
    dt.Texture = new QOpenGLTexture(QImage(QString::fromStdString(path)).mirrored());
    dt.Port = port;
    m_data.push_back(dt);
}

void Texture2::AddTexture(std::string path, int port)
{
    TextureData dt;
    dt.Texture = new QOpenGLTexture(QImage(QString::fromStdString(path)).mirrored());
    dt.Port = port;
    m_data.push_back(dt);
}

void Texture2::SetToShader(QOpenGLFunctions_3_3_Core* m_glfunc,QOpenGLShaderProgram &shader)
{
    for(int index=0; index < m_data.size(); index++){
        shader.setUniformValue("gTextureHeight" + index ,GL_TEXTURE0 + index);
    }
    for(int index=0; index < m_data.size(); index++){
        m_data.at(index).Texture->bind(index);
    }
//    for(int index=0; index < m_data.size(); index++){
//        if(m_data.at(index).Port == portNum){
//           shader.setUniformValue(uniformValue.c_str(),GL_TEXTURE0 + portNum);
////            m_glfunc->glActiveTexture(GL_TEXTURE0 + portNum);
//            m_data.at(index).Texture->bind(portNum);
//        }
//    }
}
