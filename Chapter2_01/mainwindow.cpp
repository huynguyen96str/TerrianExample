#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //load the height map in
    QString str1 = "C:\\Users\\PC\\Desktop\\TerrianExample\\Chapter2_01\\height128.RAW";
    QByteArray ba = str1.toLocal8Bit();
   // const char *c_str2 = ba.data();
    g_bruteForce.LoadHeightMap( ba.data(), 128 );
    g_bruteForce.SetHeightScale( 0.25f );
}

MainWindow::~MainWindow()
{
    delete ui;
}

