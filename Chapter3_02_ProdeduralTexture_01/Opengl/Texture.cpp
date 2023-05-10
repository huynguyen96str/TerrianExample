#include "Texture.h"

Texture::Texture(std::string path) {

    m_texture = new QOpenGLTexture(QImage(QString::fromStdString(path)).mirrored());
    QOpenGLContext *context = QOpenGLContext::currentContext();
    functions = context->versionFunctions<QOpenGLFunctions_3_3_Core>();
}

//void Texture::SetToShader(QOpenGLShaderProgram& shader,std::string uniformValue, int portNum)
//{
//    shader.setUniformValue(uniformValue.c_str(),GL_TEXTURE0 + portNum);
//    //QOpenGLFunctions_3_3_Core *functions = shader..context()->versionFunctions<QOpenGLFunctions_3_3_Core>();
//    QOpenGLContext *context = QOpenGLContext::currentContext();

//    if (context)
//    {
//        QOpenGLFunctions_3_3_Core *functions = context->versionFunctions<QOpenGLFunctions_3_3_Core>();

//        if (functions)
//        {
//            // Đối tượng QOpenGLFunctions_3_3_Core đã được lấy thành công
//            // Bạn có thể sử dụng functions để gọi các phương thức OpenGL 3.3
//             qDebug() << "Đối tượng QOpenGLFunctions_3_3_Core đã được lấy thành công";
//               functions->glActiveTexture(GL_TEXTURE0 + portNum);
//              //m_texture->bind(portNum);
//              m_texture->bind(portNum);
//        }
//        else
//        {
//            // Lỗi khi lấy đối tượng QOpenGLFunctions_3_3_Core
//              qDebug() << "Lỗi khi lấy đối tượng QOpenGLFunctions_3_3_Core";
//        }
//    }
//    else
//    {
//        // Không có OpenGL context đang hoạt động
//        qDebug() << "Không có OpenGL context đang hoạt động";
//    }
//   glActiveTexture(GL_TEXTURE0 + portNum);
//    //    m_texture->bind(portNum);
//}

void Texture::SetToShader( QOpenGLShaderProgram & shader, std::string uniformValue, unsigned int portNum)
{
    //    m_glFuns->glActiveTexture(GL_TEXTURE0+portNum);
    //    shader.setUniformValue(uniformValue.c_str(),portNum);
    //    m_glFuns->glBindTexture(GL_TEXTURE_2D,m_texture->textureId());
    if (functions)
    {
//        GLint programId;
//        functions->glGetIntegerv(GL_CURRENT_PROGRAM, &programId);
//        if(programId!=m_currentProgramId){
//        // Tạo đối tượng QOpenGLShaderProgram và gán ID của shader program
//        QOpenGLShaderProgram activeShaderProgram;
//        activeShaderProgram.create();
//        activeShaderProgram.programId = static_cast<GLuint>(programId);
//        }

        shader.setUniformValue(uniformValue.c_str(),portNum);
        functions->glActiveTexture(GL_TEXTURE0 + portNum);
        m_texture->bind(portNum);
    }
    //m_glFuns->glBindTexture(GL_TEXTURE_2D, m_texture->textureId());

}

QOpenGLTexture *Texture::TextureI()
{
    return  m_texture;
}


