#include "Buffer.h"

Buffer::Buffer() {
    this->m_indexVBO = 0;
}
Buffer::Buffer(QOpenGLFunctions_3_3_Core* m_glFuns) {
    this->m_indexVBO = 0;
    this->glFuns = m_glFuns;
    glFuns->glGenBuffers(1, &m_vertexVBO);
    glFuns->glGenBuffers(1, &m_colorVBO);
    glFuns->glGenBuffers(1, &m_textureVBO);
    glFuns->glGenBuffers(1, &m_normalVBO);
    glFuns->glGenVertexArrays(1, &m_VAO);
}

void Buffer::FillVBO(VBOType vboType, const void* data, GLsizeiptr bufferSize, FillType fillType) {
    ActiveVAOAndVBO(vboType);
    m_checkChangedVertices = false;
    m_verticies = GetVerticalCount(vboType, bufferSize, m_checkChangedVertices);
    if (m_checkChangedVertices) m_totalVertices = m_verticies;
    glFuns->glBufferData(vboType == VBOType::IndexBuffer ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER, bufferSize, data, static_cast<GLenum>(fillType));
    glFuns->glBindVertexArray(0);
}

void Buffer::AppendVBO(VBOType vboType, const void* data, GLsizeiptr bufferSize, GLuint offset) {
    ActiveVAOAndVBO(vboType);
    m_checkChangedVertices = false;
    m_verticies = GetVerticalCount(vboType, bufferSize, m_checkChangedVertices);
    if (m_checkChangedVertices) {
        m_totalVertices = offset == 0 ? m_verticies : m_totalVertices + m_verticies;
    }
    glFuns->glBufferSubData(vboType == VBOType::IndexBuffer ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER, offset, bufferSize, data);
    glFuns->glBindVertexArray(0);
}
void Buffer::LinkBuffer(GLuint IDAtribute, GLint size, VBOType vboType)
{
    ActiveVAOAndVBO(vboType);
    glFuns->glVertexAttribPointer(IDAtribute, size, GL_FLOAT, GL_FALSE, 0, nullptr);
    glFuns->glEnableVertexAttribArray(IDAtribute);
}
void Buffer::UnActiveVAO() {
    glFuns->glBindVertexArray(0);
}
void Buffer::DestroyBuffer()
{
    glFuns->glDeleteBuffers(1, &m_vertexVBO);
    glFuns->glDeleteBuffers(1, &m_colorVBO);
    glFuns->glDeleteBuffers(1, &m_textureVBO);
    if (m_indexVBO != 0) glFuns->glDeleteBuffers(1, &m_indexVBO);
    glFuns->glDeleteVertexArrays(1, &m_VAO);
}
void Buffer::Render(DrawType drawType, bool isWireFrame) {
    glFuns->glBindVertexArray(m_VAO);
    if (isWireFrame)
        glFuns->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glFuns->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (m_indexVBO != 0)
    {
        glFuns->glDrawElements(static_cast<GLenum>(drawType), m_totalVertices, GL_UNSIGNED_INT, nullptr);
    }
    else {
        glFuns->glDrawArrays(static_cast<GLenum>(drawType), 0, m_totalVertices);
    }
    UnActiveVAO();
}

void Buffer::ActiveVAOAndVBO(VBOType vboType) {
    glFuns->glBindVertexArray(m_VAO);
    if (vboType == VBOType::VertexBuffer)
    {
        glFuns->glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
    }
    else if (vboType == VBOType::ColorBuffer)
    {
        glFuns->glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
    }

    else if (vboType == VBOType::TextureBuffer)
    {
        glFuns->glBindBuffer(GL_ARRAY_BUFFER, m_textureVBO);
    }
    else if (vboType == VBOType::IndexBuffer) {
        if (m_indexVBO <= 0) glFuns->glGenBuffers(1, &m_indexVBO);
        glFuns->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexVBO);
    }
    else
    {
        glFuns->glBindBuffer(GL_ARRAY_BUFFER, m_normalVBO);
    }
}
int Buffer::GetVerticalCount(VBOType vboType, GLsizeiptr bufferSize, bool& ischeckChanged) {
    ischeckChanged = false;
    if (vboType == VBOType::VertexBuffer && m_indexVBO <= 0)
    {
        auto sizeitem = 3 * sizeof(float);
        ischeckChanged = true;
        return  bufferSize / sizeitem;
    }
    else if (vboType == VBOType::IndexBuffer && m_indexVBO > 0) {
        auto sizeitem = sizeof(int);
        ischeckChanged = true;
        return  bufferSize / sizeitem;
    }
    return 0;
}
