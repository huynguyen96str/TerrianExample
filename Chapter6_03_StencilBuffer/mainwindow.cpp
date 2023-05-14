#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "View3D.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    View3D *w = new View3D(parent);
    setMinimumSize(300,200);
    setMaximumSize(300,200);
    setCentralWidget(w);
}

MainWindow::~MainWindow()
{
    delete ui;
}

