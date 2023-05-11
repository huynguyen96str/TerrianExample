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
#include "ModelTest.h"
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
    // Buffer* m_buffer;
    MidpointDispTerrain* m_terrain;
    Texture* m_texture1;
    Texture* m_texture2;
    Texture* m_texture3;
    Texture* m_texture4;
    ModelTest m_md;
};

#endif // VIEW3D_H
