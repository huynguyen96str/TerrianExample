#include "View3D.h"
View3D::View3D(QWidget* parent) : QOpenGLWidget(parent)
{
    lastPos = QPoint(0, 0);
    perFrame = 100;// delta time / frame
    setFocusPolicy(Qt::StrongFocus);
    m_camera = Camera(QVector3D(0.0f, 10.0f, -50.0f));
    m_timer.setInterval(perFrame);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(on_TimeOut()));
    m_timer.start();
}
void View3D::initializeGL() {
    initializeOpenGLFunctions();
    CurrentContex = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    ShaderProgram::Load(shaderProgram,":/Shaders/Resources/obj.vert", ":/Shaders/Resources/obj.frag");
    int size = 4;
    float minHeight = 0.0f;
    float maxHeight = 1.0f;
    float roughness = 1.5f;
    m_terrain= new MidpointDispTerrain(CurrentContex,size,roughness,minHeight,maxHeight);
}
void View3D::resizeGL(int w, int h) {
    glViewport(0, 0, (GLsizei)(w), (GLsizei)(h));
}

// User interact interface
void View3D::mousePressEvent(QMouseEvent* ev) {
    if (ev->button() == Qt::RightButton)
    {
        lastPos = ev->pos();
    }
    if (ev->button() == Qt::LeftButton)
    {
        MousePress(ev);
    }
}

void View3D::mouseMoveEvent(QMouseEvent* ev) {
    if (ev->buttons() == Qt::RightButton)
    {
        auto posIn = ev->pos();
        // reversed since y-coordinates go from bottom to top
        int xoffset = posIn.x() - lastPos.x();
        int yoffset = lastPos.y() - posIn.y();

        lastPos = posIn;
        m_camera.ProcessMouseMovement(xoffset, yoffset);
        update();
    }
}
void View3D::wheelEvent(QWheelEvent* event) {
    m_camera.ProcessMouseScroll(event->angleDelta().y() / 120);
    update();
}
void View3D::keyPressEvent(QKeyEvent* event) {
    float detatime2 = 2.5 * perFrame / 1000;
    switch (event->key()) {
    case Qt::Key_W:m_camera.ProcessKeyboard(Camera_Movement::FORWARD, detatime2); break;
    case Qt::Key_S:m_camera.ProcessKeyboard(Camera_Movement::BACKWARD, detatime2); break;
    case Qt::Key_D:m_camera.ProcessKeyboard(Camera_Movement::RIGHT, detatime2); break;
    case Qt::Key_A:m_camera.ProcessKeyboard(Camera_Movement::LEFT, detatime2); break;
    }
    update();
}


void View3D::on_TimeOut() {
    update();
}


void View3D::MousePress(QMouseEvent *ev)
{
    Q_UNUSED(ev)
}

void View3D::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_projection.setToIdentity();
    m_projection.perspective(45, (float)width() / height(),m_near, m_far);
    m_view  = m_camera.GetViewMatrix();

    shaderProgram.bind();
    m_model.setToIdentity();
    shaderProgram.setUniformValue("projection", m_projection);
    shaderProgram.setUniformValue("view", m_view);
    shaderProgram.setUniformValue("model", m_model);

    m_terrain->Render(DrawType::Triangles, true);
}
