#ifndef CBRUTE_FORCE_H
#define CBRUTE_FORCE_H

#include "CTERRAIN.h"
#include <QOpenGLFunctions_3_3_Core>
#include "Opengl/GlobalOpengl.h"
#include "Opengl/Buffer.h"

class CBRUTE_FORCE
{
public:
    CBRUTE_FORCE(QOpenGLFunctions_3_3_Core* m_glFuns, int count);
    CBRUTE_FORCE(QOpenGLFunctions_3_3_Core* m_glFuns, QString fileHeightmaps);
private:
    Buffer* m_buffer;
    int m_depth;
    int m_width;
     void initial();
     CTERRAIN m_cterrain;
     bool m_isLoadHeightMaps;
public:
    void Render(DrawType type, bool isWireFrame);

};

#endif // CBRUTE_FORCE_H
