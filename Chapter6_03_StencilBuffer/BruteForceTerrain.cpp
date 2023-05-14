#include "BruteForceTerrain.h"

BruteForceTerrain::BruteForceTerrain(QOpenGLFunctions_3_3_Core *m_glFuns, int TerrainSize): TerrainBase(m_glFuns, TerrainSize,TerrainSize)
{
    CreateBuffer();
}
///  m_camera = Camera(QVector3D(0.0f, 0.0f, -23.0f));
/// QString fileheightmaps = "D:\\ProjectCDC\\TerrianExample\\Chapter2_02_FaultFormationAlg\\height128.RAW";
/// m_terrain= new BruteForceTerrain(CurrentContex,fileheightmaps);
BruteForceTerrain::BruteForceTerrain(QOpenGLFunctions_3_3_Core *m_glFuns, QString fileHeightmaps): TerrainBase(m_glFuns)
{
    QByteArray ba = fileHeightmaps.toLocal8Bit();
    LoadHeightMap( ba.data(), 128 );
    SetHeightScale( 0.25f );
    int depth = GetSegment();
    SetSizeTerrian(depth,depth);
    // set height các vertices dựa vào file heightmap
    float height=0.0f;
    for (int z = 0; z < depth; z++) {
        for (int x = 0; x < depth; x++) {
            height= GetScaledHeightAtPoint( x, z );
            SetHeight(z,x,height);
        }
    }
    // Tạo buffer
    CreateBuffer();
}

bool BruteForceTerrain::LoadHeightMap(char *szFilename, int iSize)
{
    FILE* pFile;

    //check to see if the data has been set
    if( m_heightData.m_ucpData )
        UnloadHeightMap();

    //open the RAW height map dataset
    pFile= fopen( szFilename, "rb" );
    if( pFile==NULL )
    {
        //bad filename
        qDebug()<<  "LOG_FAILURE, Could not load " <<  szFilename ;
        return false;
    }

    //allocate the memory for our height data
    m_heightData.m_ucpData= new unsigned char [iSize*iSize];

    //check to see if memory was successfully allocated
    if( m_heightData.m_ucpData==NULL )
    {
        //the memory could not be allocated something is seriously wrong here
        qDebug()<< "LOG_FAILURE, Could not allocate memory for " << szFilename ;
        return false;
    }

    //read the heightmap into context
    fread( m_heightData.m_ucpData, 1, iSize*iSize, pFile );

    //Close the file
    fclose( pFile );

    //set the m_iSize data
    m_iSize= iSize;

    //yahoo! The heightmap has been successfully loaded
    qDebug() << "LOG_SUCCESS Loaded " << szFilename ;
    return true;
}

bool BruteForceTerrain::SaveHeightMap(char *szFilename)
{
    FILE* pFile;

    //open a file to write to
    pFile= fopen( szFilename, "wb" );
    if( pFile==NULL )
    {
        //bad filename
        qDebug()<<  "LOG_FAILURE ,Could not create " << szFilename ;
        return false;
    }

    //check to see if our height map actually has data in it
    if( m_heightData.m_ucpData==NULL )
    {
        //something is seriously wrong here
        qDebug()<< "LOG_FAILURE ,The height data buffer for " << szFilename << " is empty";
        return false;
    }

    //write the data to the file
    fwrite( m_heightData.m_ucpData, 1, m_iSize*m_iSize, pFile );

    //Close the file
    fclose( pFile );

    //w00t w00t! The heightmap has been successfully saved
    qDebug() << "LOG_SUCCESS, Saved " <<  szFilename;
    return true;
}

bool BruteForceTerrain::UnloadHeightMap()
{
    //check to see if the data has been set
    if( m_heightData.m_ucpData )
    {
        //delete the data
        delete[] m_heightData.m_ucpData;

        //reset the map dimensions also
        m_iSize= 0;
    }

    //the height map has been unloaded
    qDebug()<< "LOG_SUCCESS, Successfully unloaded the height map" ;
    return true;
}

