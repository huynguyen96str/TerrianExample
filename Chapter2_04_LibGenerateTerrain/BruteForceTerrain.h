#ifndef BRUTEFORCETERRAIN_H
#define BRUTEFORCETERRAIN_H
#include "TerrainBase.h"

struct SHEIGHT_DATA
{
    unsigned char* m_ucpData=nullptr;	//the height data
    int m_iSize;				//the height size (must be a power of 2)
};
class BruteForceTerrain:public TerrainBase
{
public:
    BruteForceTerrain(QOpenGLFunctions_3_3_Core* m_glFuns, int TerrainSize);
    BruteForceTerrain(QOpenGLFunctions_3_3_Core* m_glFuns, QString fileHeightmaps);
private:
    SHEIGHT_DATA m_heightData;	//the height data
    float m_fHeightScale;		//scaling variable
    int m_iVertsPerFrame;		//stat variables
    int m_iTrisPerFrame;
    int m_iSize;	//the size of the heightmap, must be a power of two
    bool LoadHeightMap(char* szFilename, int iSize);
    bool SaveHeightMap(char* szFilename );
    bool UnloadHeightMap( void );

    inline int GetSegment()
    {
        return m_iSize-1;
    }
    //--------------------------------------------------------------
    // Description:		Get the number of vertices being sent to the API every frame
    //--------------------------------------------------------------
    inline int GetNumVertsPerFrame( void )
    {
        return m_iVertsPerFrame;
    }

    //--------------------------------------------------------------
    // Description:		Get the number of triangles being rendered every frame
    //--------------------------------------------------------------
    inline int GetNumTrisPerFrame( void )
    {	return m_iTrisPerFrame;	}

    //--------------------------------------------------------------
    // Description:		Set the height scaling variable
    // Arguments:		-fScale: how much to scale the terrain
    //--------------------------------------------------------------
    inline void SetHeightScale( float fScale )
    {
        m_fHeightScale= fScale;
    }

    //--------------------------------------------------------------
    // Description:		Set the true height value at the given point
    // Arguments:		-ucHeight: the new height value for the point
    //					-iX, iZ: which height value to retrieve
    //--------------------------------------------------------------
    inline void SetHeightAtPoint( unsigned char ucHeight, int iX, int iZ)
    {
        m_heightData.m_ucpData[( iZ*m_iSize )+iX]= ucHeight;
    }

    //--------------------------------------------------------------
    // Description:		A function to get the true height value (0-255) at a point
    // Arguments:		-iX, iZ: which height value to retrieve
    // Return Value:	An float value: the true height at the given point
    //--------------------------------------------------------------
    inline unsigned char GetTrueHeightAtPoint( int iX, int iZ )
    {
        return ( m_heightData.m_ucpData[( iZ*m_iSize )+iX] );
    }

    //--------------------------------------------------------------
    // Description:		Retrieve the scaled height at a given point
    // Arguments:		-iX, iZ: which height value to retrieve
    // Return Value:	A float value: the scaled height at the given point.
    //--------------------------------------------------------------
    inline float GetScaledHeightAtPoint( int iX, int iZ )
    {
        return ( ( float )( m_heightData.m_ucpData[( iZ*m_iSize )+iX] )*m_fHeightScale );
    }
};

#endif // BRUTEFORCETERRAIN_H
