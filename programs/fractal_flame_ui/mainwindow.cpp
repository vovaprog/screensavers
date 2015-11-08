#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>

#include <memory>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene;
    ui->mainGraphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_butCalculateFlame_clicked()
{
    shared_ptr<RenderParameters> rp(new RenderParameters());
    rp->pictureWidth=800;
    rp->pictureHeight=800;
    rp->numberOfIterations=1000000;

    shared_ptr<FlameParameters> fp(new FlameParameters());
    //fp->load("../fractal_flame/fractals/wing.xml");
    fp->initRandom();

    fractalAlgo.setRenderParameters(rp);
    unsigned int *output = fractalAlgo.calculate(fp);

    if(output!=nullptr)
    {
        QImage img((uchar*)output, 800, 800, QImage::Format_RGB32);

        scene->clear();
        scene->addPixmap(QPixmap::fromImage(img));
        ui->mainGraphicsView->update();
    }
}
