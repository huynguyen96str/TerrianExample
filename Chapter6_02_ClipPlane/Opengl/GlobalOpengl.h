#ifndef GLOBALOPENGL_H
#define GLOBALOPENGL_H
#include <QOpenGLFunctions_3_3_Core>
#include <corecrt_math_defines.h>

enum class DrawType
{
    Points = GL_POINTS,
    Lines = GL_LINES,
    Line_strip= GL_LINE_STRIP,
    Line_loop = GL_LINE_LOOP,
    Polygon = GL_POLYGON,
    Quads= GL_QUADS,
    Quad_strip = GL_QUAD_STRIP,
    Triangles = GL_TRIANGLES,
    Triangle_strip = GL_TRIANGLE_STRIP,
    Triangle_fan = GL_TRIANGLE_FAN
};
enum class VBOType
{
    VertexBuffer,
    ColorBuffer,
    TextureBuffer,
    NormalBuffer,
    IndexBuffer
};
enum class FillType
{
    Static_draw = GL_STATIC_DRAW,
    Dynamic_draw = GL_DYNAMIC_DRAW
};
enum class DataType
{
    IntData = GL_INT,
    FloatData = GL_FLOAT,
    UnsignedInt = GL_UNSIGNED_INT
};
enum class RenderType {
    TRIANGLES = GL_TRIANGLES,
    LINES = GL_LINES
};
enum class Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};


#endif // GLOBALOPENGL_H
