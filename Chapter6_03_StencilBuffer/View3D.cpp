#include "View3D.h"
View3D::View3D(QWidget* parent) : QOpenGLWidget(parent)
{
    lastPos = QPoint(0, 0);
    perFrame = 100;// delta time / frame
    setFocusPolicy(Qt::StrongFocus);
    m_camera = Camera(QVector3D(0.0f, 0.0f, -5.0f));//m_camera = Camera(QVector3D(1000.0f, 500.0f, -4000.0f));
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
    ShaderProgram::Load(shaderProgramInv,":/Shaders/Resources/objSection.vert", ":/Shaders/Resources/objSection.frag");
    ShaderProgram::Load(shaderProgramNoSection,":/Shaders/Resources/objNoSection.vert", ":/Shaders/Resources/objNoSection.frag");

    //ShaderProgram::Load(shaderProgram,":/Shaders/Resources/single_tex_terrain.vert", ":/Shaders/Resources/single_tex_terrain.frag");
    float TextureScale = 1.0f;
    int Size = 8;
    float Size2 = 8;
    float Roughness = 1.0f;
    float MinHeight =-1.0f;
    float MaxHeight = 1.f;
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

    m_buffer = new Buffer(CurrentContex);
    float vertices[]={
        -Size2, 0.0f, -Size2,
        Size2, 0.0f,- Size2,
        Size2, 0.0f, Size2,
        -Size2, 0.0f, Size2,
    };
    int indexces[] ={
        0,1,2,
        0,2,3
    };
    m_buffer->FillVBO( VBOType::VertexBuffer, vertices, sizeof (vertices), FillType::Static_draw);
    m_buffer->FillVBO( VBOType::IndexBuffer, indexces, sizeof (indexces), FillType::Static_draw);
    m_buffer->LinkBuffer(0,3, VBOType::VertexBuffer);

}
void View3D::resizeGL(int w, int h) {
    m_w= w;
    m_h = h;
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
    case Qt::Key_0:
        isWriteStencilValue0=false;
        update();
        break;
    case Qt::Key_1:
        isWriteStencilValue1=false;
        update();
        break;
    case Qt::Key_2:
        isWriteStencilValue2=false;
        update();
        break;
    case Qt::Key_3:
        isWriteStencilValue3=false;
        update();
        break;
    case Qt::Key_O:
        isWriteDepthValue=false;
        update();
        break;
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


    shaderProgramInv.bind();
    m_model.setToIdentity();
    shaderProgramInv.setUniformValue("projection", m_projection);
    shaderProgramInv.setUniformValue("view", m_view);
    shaderProgramInv.setUniformValue("model", m_model);

    shaderProgram2.bind();
    m_model.setToIdentity();
    shaderProgram2.setUniformValue("projection", m_projection);
    shaderProgram2.setUniformValue("view", m_view);
    shaderProgram2.setUniformValue("model", m_model);

    shaderProgramNoSection.bind();
    m_model.setToIdentity();
    shaderProgramNoSection.setUniformValue("projection", m_projection);
    shaderProgramNoSection.setUniformValue("view", m_view);
    shaderProgramNoSection.setUniformValue("model", m_model);


    glEnable(GL_CLIP_DISTANCE0); // Kích hoạt mặt phẳng cắt 0, nếu có nhiều mặt cắt thì thay GL_CLIP_DISTANCE1,...
    shaderProgram.bind();
    m_terrain->Render(DrawType::Triangles, false);
    glDisable(GL_CLIP_DISTANCE0);




    glEnable(GL_STENCIL_TEST);

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    glCullFace(GL_FRONT_AND_BACK);
    glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
    glStencilMask(0xFF);
    shaderProgramNoSection.bind();
    m_terrain->Render(DrawType::Triangles, false);
    if(isWriteStencilValue0==false){
        ReadStencilBuffer(m_w,m_h);
        isWriteStencilValue0=true;
    }

    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_ZERO,GL_INCR , GL_ZERO);
    shaderProgram2.bind();
    m_buffer->Render(DrawType::Triangles, false);
    if(isWriteStencilValue1==false){
        ReadStencilBuffer(m_w,m_h);
        isWriteStencilValue1=true;
    }
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP,GL_ZERO , GL_ZERO);
    shaderProgramNoSection.bind();
    m_terrain->Render(DrawType::Triangles, false);
    if(isWriteStencilValue2==false){
        ReadStencilBuffer(m_w,m_h);
        isWriteStencilValue2=true;
    }

    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
    glStencilFunc(GL_EQUAL, 2, 0xFF);
    glStencilOp(GL_KEEP,GL_KEEP , GL_KEEP);
      glStencilMask(0xFF);
      glDisable(GL_DEPTH_TEST);
    shaderProgram2.bind();
    m_buffer->Render(DrawType::Triangles, false);
    if(isWriteStencilValue3==false){
        ReadStencilBuffer(m_w,m_h);
        isWriteStencilValue3=true;
    }
    glDisable(GL_STENCIL_TEST);

 glEnable(GL_DEPTH_TEST);

}
void View3D::ReadStencilBuffer(int w, int h)
{
    qDebug() << "------------------ "  ;
    unsigned char *stencilData = new unsigned char[w*h];
    glReadPixels(0, 0, w, h, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, stencilData);
    QString dt="";
    QString value="";
    for (int y = h-1; y >0; y--) {
        dt="";
        for (int x = 0; x < w; x++) {
            value= QString::number(static_cast<int>(stencilData[y * w + x]));
            dt+=value ;
        }
        qDebug() << dt;
    }
    delete []  stencilData;
    qDebug() << "Data written to file: "  << QString::number( w) << QString::number( h);
}
void View3D::ReadColorBuffer(int w, int h)
{
    qDebug() << "------------------ "  ;
    unsigned char *stencilData = new unsigned char[w*h*4];
    glReadPixels(0, 0, w, h, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, stencilData);
    QString dt="";
    QString value="";
    int index=0;
    for (int y = h-1; y >0; y--) {
        dt="";
        for (int x = 0; x < w; x++) {
            index= (y * w + x)*4;
            dt+=QString("(") + QString::number(static_cast<int>(stencilData[index])) +","+
                    QString::number(static_cast<int>(stencilData[index+1]))+ "," +
                    QString::number(static_cast<int>(stencilData[index+2])) +","+
                    QString::number(static_cast<int>(stencilData[index+2]))+ ")";
        }
        qDebug() << dt;
    }
    delete []  stencilData;
    qDebug() << "Data written to file: "  << QString::number( w) << QString::number( h);
}

void View3D::ReadDepthBuffer(int w, int h)
{
    // Read the depth buffer values
    GLfloat* depthValues = new GLfloat[w * h];
    glReadPixels(0, 0, w, h, GL_DEPTH_COMPONENT, GL_FLOAT, depthValues);
    // Output the depth values
    QString dt="";
    QString value="";
    int index=0;
    for (int y = h-1; y >0; y--) {
        dt="";
        for (int x = 0; x < w; x++) {
            GLfloat depth = depthValues[y * w + x];
            dt+=" "+ QString::number(depth );
        }
        qDebug() << dt;
    }
    // Clean up
    delete[] depthValues;
}
