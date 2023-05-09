#ifndef BUFFER_H
#define BUFFER_H
#include <QOpenGLFunctions_3_3_Core>
#include "Opengl/GlobalOpengl.h"

class Buffer {
public:
    Buffer();
    Buffer(QOpenGLFunctions_3_3_Core* glFuns);
    void FillVBO(VBOType vboType, const void* data, GLsizeiptr bufferSize, FillType fillType);
    void AppendVBO(VBOType vboType, const void* data, GLsizeiptr bufferSize, GLuint offset);
    void LinkBuffer(GLuint IDAtribute, GLint size, VBOType vboType);
    void UnActiveVAO();
    void DestroyBuffer();
    void Render(DrawType drawType, bool isWireFrame);
private:
    unsigned int m_VAO;
    unsigned int m_vertexVBO;
    unsigned int m_colorVBO;
    unsigned int m_textureVBO;
    unsigned int m_normalVBO;
    unsigned int m_indexVBO = 0;
    unsigned int m_totalVertices;
    bool m_checkChangedVertices;
    int m_verticies;
    QOpenGLFunctions_3_3_Core* glFuns;
    void ActiveVAOAndVBO(VBOType vboType);
    int GetVerticalCount(VBOType vboType, GLsizeiptr bufferSize, bool& ischeckChanged);
};


#endif // BUFFER_H
