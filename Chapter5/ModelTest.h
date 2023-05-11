#ifndef MODELTEST_H
#define MODELTEST_H

#include "earcut.hpp"
#include <QOpenGLFunctions_3_3_Core>
#include "Opengl/GlobalOpengl.h"
#include "Opengl/Buffer.h"
#include <QDebug>
#include <array>
class ModelTest
{
public:
    ModelTest();
    void run();
private:
    Buffer* m_buffer;
};

#endif // MODELTEST_H
