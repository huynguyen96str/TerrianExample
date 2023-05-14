#ifndef VIEW3D_H
#define VIEW3D_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QElapsedTimer>
#include <QOpenGLBuffer>
#include "Opengl/GlobalOpengl.h"
#include "Opengl/Camera.h"

#include "Opengl/ShaderProgram.h"
#include "Opengl/Buffer.h"

#include "BruteForceTerrain.h"
#include "FaultFormationTerrain.h"
#include "MidpointDispTerrain.h"
#include "Opengl/Texture.h"
#include "Opengl/ProceduralTexture.h"
#include <array>
#include <iostream>
#include <fstream>
#define MU_PREC 1E-10 // treshhold prectision
class Vector3
{
public:
  union
  {
    struct
    {
      float x;
      float y;
      float z;    // x,y,z coordinates
    };
    float v[3];
  };

  // constructors
  Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
  Vector3(const Vector3 &vec) : x(vec.x), y(vec.y), z(vec.z) {}
  Vector3(const Vector3 &a, const Vector3 &b) : x(b.x - a.x), y(b.y - a.y), z(b.z - a.z) {}
  Vector3(const float *v) : x(v[0]), y(v[1]), z(v[2]) {}

  // vector set
  void Set(float nx, float ny, float nz)
  {
    x = nx; y = ny; z = nz;
  }

  // vector index
  float &operator[](const long idx)
  {
    return *((&x)+idx);
  }

  // vector assignment
  const Vector3 &operator=(const Vector3 &vec)
  {
    x = vec.x;
    y = vec.y;
    z = vec.z;
    return *this;
  }

  // vector equality
  const bool operator==(const Vector3 &vec) const
  {
    return ((x == vec.x) && (y == vec.y) && (z == vec.z));
  }

  // vector inequality
  const bool operator!=(const Vector3 &vec) const
  {
    return !(*this == vec);
  }

  // vector add
  const Vector3 operator+(const Vector3 &vec) const
  {
    return Vector3(x + vec.x, y + vec.y, z + vec.z);
  }

  // vector add (opposite of negation)
  const Vector3 operator+() const
  {
    return Vector3(*this);
  }

  // vector increment
  const Vector3& operator+=(const Vector3& vec)
  {
    x += vec.x;
    y += vec.y;
    z += vec.z;
    return *this;
  }

  // vector subtraction
  const Vector3 operator-(const Vector3& vec) const
  {
    return Vector3(x - vec.x, y - vec.y, z - vec.z);
  }

  // vector negation
  const Vector3 operator-() const
  {
    return Vector3(-x, -y, -z);
  }

  // vector decrement
  const Vector3 &operator-=(const Vector3& vec)
  {
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
    return *this;
  }

  // scalar self-multiply
  const Vector3 &operator*=(const float s)
  {
    x *= s;
    y *= s;
    z *= s;
    return *this;
  }

  // scalar self-divide
  const Vector3 &operator/=(const float s)
  {
    const float recip = 1.0f/s; // for speed, one division
    x *= recip;
    y *= recip;
    z *= recip;
    return *this;
  }

  // post multiply by scalar
  const Vector3 operator*(const float s) const
  {
    return Vector3(x*s, y*s, z*s);
  }

  // pre multiply by scalar
  friend const Vector3 operator*(const float s, const Vector3& vec)
  {
    return vec*s;
  }

  // divide by scalar
  const Vector3 operator/(const float s) const
  {
    const float recip = 1/s;
    return Vector3(recip*x, recip*y, recip*z);
  }

  // vector multiply
  const Vector3 operator*(const Vector3& vec) const
  {
    return Vector3(x*vec.x, y*vec.y, z*vec.z);
  }

  // cross product
  const Vector3 CrossProduct(const Vector3 &vec) const
  {
    return Vector3(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
  }

  // dot product
  const float DotProduct(const Vector3 &vec) const
  {
    return x*vec.x + y*vec.y + z*vec.z;
  }

  // length of vector
  const float Length() const
  {
    return sqrtf(x*x + y*y + z*z);
  }

  // return the unit vector
  const Vector3 UnitVector() const
  {
    return (*this) / Length();
  }

  // normalize this vector
  void Normalize()
  {
    (*this) /= Length();
  }

  // return angle between two vectors
  const float inline Angle(const Vector3& vec) const
  {
    return acosf(DotProduct(vec));
  }

  // reflect this vector off surface with normal vector
  const Vector3 Reflection(const Vector3& normal) const
  {
    return (*this - normal * 2.0 * DotProduct(normal));
  }

  // rotate angle degrees about a normal
  const Vector3 Rotate(const float angle, const Vector3& normal) const
  {
    const float cosine = cosf(angle);
    const float sine = sinf(angle);

    return Vector3(*this * cosine + ((normal * *this) * (1.0f - cosine)) *
      normal + (CrossProduct(normal)) * sine);
  }

};
class View3D: public QOpenGLWidget,public QOpenGLFunctions_3_3_Core {
    Q_OBJECT
public:
    explicit View3D(QWidget* parent = nullptr);
    void SetRenderType(RenderType newRenderType);
private:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    // User interact interface
    void mousePressEvent(QMouseEvent* ev) override;
    void mouseMoveEvent(QMouseEvent* ev) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    QOpenGLFunctions_3_3_Core* CurrentContex;
    void paintGL() override;
private slots:
    void on_TimeOut();
    virtual  void MousePress(QMouseEvent* ev);
private:
    QPoint lastPos;
    int perFrame;
    Camera m_camera;
    QTimer m_timer;
    QMatrix4x4 m_projection, m_view, m_model;
    float m_near =0.1f;
    float m_far=10000.0f;
    QOpenGLShaderProgram shaderProgram;
        QOpenGLShaderProgram shaderProgramInv;
        QOpenGLShaderProgram shaderProgram2;
             QOpenGLShaderProgram shaderProgramNoSection;
    Buffer* m_buffer;
    MidpointDispTerrain* m_terrain;
    Texture* m_texture1;
    Texture* m_texture2;
    Texture* m_texture3;
    Texture* m_texture4;
    GLdouble eq[3][4]; // 3 plane equations
    bool get_plane_equation(Vector3 p0, Vector3 p1, Vector3 p2, double* eq);
    void ReadStencilBuffer(int width, int height);
    void ReadColorBuffer(int width, int height);
    void ReadDepthBuffer(int width, int height);
    bool isWriteStencilValue0=true;
    bool isWriteStencilValue1=true;
    bool isWriteStencilValue2=true;
    bool isWriteStencilValue3=true;

     bool isWriteDepthValue=true;
    int m_w;
    int m_h;
};

#endif // VIEW3D_H
