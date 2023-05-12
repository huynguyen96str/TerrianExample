#include "View3D.h"
View3D::View3D(QWidget* parent) : QOpenGLWidget(parent)
{
    lastPos = QPoint(0, 0);
    perFrame = 100;// delta time / frame
    setFocusPolicy(Qt::StrongFocus);
    m_camera = Camera(QVector3D(-10.0f, 0.0f, -25.0f));//m_camera = Camera(QVector3D(1000.0f, 500.0f, -4000.0f));
    m_timer.setInterval(perFrame);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(on_TimeOut()));
    m_timer.start();
}


bool View3D::get_plane_equation(Vector3 p0, Vector3 p1, Vector3 p2,double* eq)
{
    // following two expressions order of arguments is very important
    // still we need to keep plane's positive normal in plane's equation
    Vector3 v0(p0, p1), v1(p1, p2), n;
    n = v0.CrossProduct(v1);

    // this handles cases, when two or more points are too close to each other
    if(n.Length() <= MU_PREC) return false;

    // just for our convenience
    double &A = eq[0], &B = eq[1], &C = eq[2], &D = eq[3];

    // positive normal
    A = n.x; B = n.y; C = n.z;

    // Here instead of p0, p1 and p2 can be used as well since they all belong to this plane
    D = - (A*p0.x + B*p0.y + C*p0.z);
    return true;
}
void View3D::initializeGL() {
    initializeOpenGLFunctions();
    CurrentContex = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    ShaderProgram::Load(shaderProgram,":/Shaders/Resources/obj.vert", ":/Shaders/Resources/obj.frag");
    ShaderProgram::Load(shaderProgram2,":/Shaders/Resources/obj2.vert", ":/Shaders/Resources/obj2.frag");
    //ShaderProgram::Load(shaderProgram,":/Shaders/Resources/single_tex_terrain.vert", ":/Shaders/Resources/single_tex_terrain.frag");
    float TextureScale = 4.0f;
    int Size = 256;
    float Roughness = 1.0f;
    float MinHeight =-50.0f;
    float MaxHeight = 50.f;
    // Texture single

    m_texture1=new Texture(":/textures/Resources/textures/IMGP5525_seamless.jpg");
    m_texture1->bind();
    m_texture1->SetWrapTexture(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    m_texture1->SetWrapTexture(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
    m_texture1->SetFilter(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
    m_texture2=new Texture(":/textures/Resources/textures/IMGP5487_seamless.jpg");
    m_texture2->bind();
    m_texture2->SetWrapTexture(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    m_texture2->SetWrapTexture(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
    m_texture2->SetFilter(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
    m_texture3=new Texture(":/textures/Resources/textures/tilable-IMG_0044-verydark.png");
    m_texture3->bind();
    m_texture3->SetWrapTexture(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    m_texture3->SetWrapTexture(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
    m_texture3->SetFilter(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
    m_texture4=new Texture(":/textures/Resources/textures/water.png");
    m_texture4->bind();
    m_texture4->SetWrapTexture(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    m_texture4->SetWrapTexture(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
    m_texture4->SetFilter(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);

    int TextureSize = 1024;
    m_terrain= new MidpointDispTerrain(CurrentContex,Size,Roughness,MinHeight, MaxHeight);
    m_terrain->SetTextureScale(TextureScale);


    // setup clip planes
    Vector3 verts[3][4] = { // 3 sets of 4 points. Each set defines a plane.
                            Vector3(0,0,1), Vector3(0,1,0), Vector3(0,0,-1), Vector3(0,-1,0),
                            Vector3(1,0,0), Vector3(0,0,1), Vector3(-1,0,0), Vector3(0,0,-1),
                            Vector3(1,0,0), Vector3(0,-1,0), Vector3(-1,0,0), Vector3(0,1,0)
                          };
    for(int k=0; k<3; k++) get_plane_equation(verts[k][0], verts[k][1], verts[k][2], eq[k]);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT);
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

    m_texture1->SetToShader(shaderProgram,"gTextureHeight0",0);
    m_texture2->SetToShader(shaderProgram,"gTextureHeight1",1);
    m_texture3->SetToShader(shaderProgram,"gTextureHeight2",2);
    m_texture4->SetToShader(shaderProgram,"gTextureHeight3",3);
   glEnable(GL_CLIP_DISTANCE0); // Kích hoạt mặt phẳng cắt 0, nếu có nhiều mặt cắt thì thay GL_CLIP_DISTANCE1,...
   m_terrain->Render(DrawType::Triangles, false);


}
