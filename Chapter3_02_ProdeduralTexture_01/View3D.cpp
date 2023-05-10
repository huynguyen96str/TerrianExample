#include "View3D.h"
View3D::View3D(QWidget* parent) : QOpenGLWidget(parent)
{
    lastPos = QPoint(0, 0);
    perFrame = 100;// delta time / frame
    setFocusPolicy(Qt::StrongFocus);
    m_camera = Camera(QVector3D(0.0f, 200.0f, -120.0f));
    m_timer.setInterval(perFrame);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(on_TimeOut()));
    m_timer.start();
}

void View3D::initializeGL() {
    initializeOpenGLFunctions();
    CurrentContex = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
     ShaderProgram::Load(shaderProgram,":/Shaders/Resources/obj.vert", ":/Shaders/Resources/obj.frag");
    // ShaderProgram::Load(shaderProgram,":/Shaders/Resources/single_tex_terrain.vert", ":/Shaders/Resources/single_tex_terrain.frag");
    float TextureScale = 4.0f;
    int Size = 512;
    float Roughness = 1.0f;
    float MinHeight = 0.f;
    float MaxHeight = 256.f;
   // Texture single

    m_texture= new ProceduralTexture();
//    m_texture->LoadTile(":/textures/Resources/textures/rock02_2.jpg");
//    m_texture->LoadTile(":/textures/Resources/textures/rock01.jpg");
//    m_texture->LoadTile(":/textures/Resources/textures/tilable-IMG_0044-verydark.png");
//    m_texture->LoadTile(":/textures/Resources/textures/water.png");
//    TextureFilenames.push_back("../Content/textures/IMGP5525_seamless.jpg");
//         TextureFilenames.push_back("../Content/textures/IMGP5487_seamless.jpg");
//         TextureFilenames.push_back("../Content/textures/tilable-IMG_0044-verydark.png");
//         TextureFilenames.push_back("../Content/textures/water.png");

//  m_textures =new Texture2(":/textures/Resources/textures/IMGP5525_seamless.jpg");
//  m_textures->AddTexture(":/textures/Resources/textures/IMGP5487_seamless.jpg",1);
//  m_textures->AddTexture(":/textures/Resources/textures/tilable-IMG_0044-verydark.png",2);
//  m_textures->AddTexture(":/textures/Resources/textures/water.png",3);
    m_texture1=new Texture(":/textures/Resources/textures/IMGP5525_seamless.jpg");
  m_texture2=new Texture(":/textures/Resources/textures/IMGP5487_seamless.jpg");
  m_texture3=new Texture(":/textures/Resources/textures/tilable-IMG_0044-verydark.png");
  m_texture4=new Texture(":/textures/Resources/textures/water.png");

    int TextureSize = 1024;
    m_terrain= new MidpointDispTerrain(CurrentContex,Size,Roughness,MinHeight, MaxHeight);
    m_terrain->SetTextureScale(TextureScale);
    //m_texture->GenerateTexture(TextureSize, MinHeight, MaxHeight,m_terrain);
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
    //m_texture->SetToShader(shaderProgram,"texturew");
     //m_texture->SetToShader(shaderProgram,"gTerrainTexture",0);


    // Kích hoạt texture và kết nối với texture unit
    //m_textures->SetToShader(CurrentContex,shaderProgram);
    //CurrentContex = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>()
    m_texture1->SetToShader(shaderProgram,"gTextureHeight0",0);
    m_texture2->SetToShader(shaderProgram,"gTextureHeight1",1);
    m_texture3->SetToShader(shaderProgram,"gTextureHeight2",2);
    m_texture4->SetToShader(shaderProgram,"gTextureHeight3",3);




// shaderProgram.setUniformValue("gTextureHeight0",0);
// glActiveTexture(GL_TEXTURE0+0);
// m_texture1->TextureI()->bind(0);

//  shaderProgram.setUniformValue("gTextureHeight1",1);
  // glActiveTexture(GL_TEXTURE0+1);
 //m_texture2->TextureI()->bind(1);

// shaderProgram.setUniformValue("gTextureHeight2",2);
//   glActiveTexture(GL_TEXTURE0+2);
//   m_texture3->TextureI()->bind(2);

//shaderProgram.setUniformValue("gTextureHeight3",3);
//   glActiveTexture(GL_TEXTURE0+3);
// m_texture4->TextureI()->bind(3);

    m_terrain->Render(DrawType::Triangles, false);
    m_texture1->TextureI()->release();
     m_texture2->TextureI()->release();
      m_texture3->TextureI()->release();
       m_texture4->TextureI()->release();
}
