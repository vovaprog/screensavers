#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>
#include <QColorDialog>
#include <QPalette>
#include <QFileDialog>

#include <memory>
#include <vector>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene;
    ui->mainGraphicsView->setScene(scene);

    vector<QString> functionNames;

    functionNames.push_back("sinusoidal");
    functionNames.push_back("eyefish");
    functionNames.push_back("spherical");
    functionNames.push_back("swirl");
    functionNames.push_back("horseshoe");
    functionNames.push_back("polar");
    functionNames.push_back("handkerchief");
    functionNames.push_back("heart");
    functionNames.push_back("disk");
    functionNames.push_back("spiral");
    functionNames.push_back("hyperbolic");
    functionNames.push_back("diamond");
    functionNames.push_back("julia");
    functionNames.push_back("ex");
    functionNames.push_back("bent");
    functionNames.push_back("mirror");
    functionNames.push_back("power");
    functionNames.push_back("bubble");
    functionNames.push_back("cylinder");
    functionNames.push_back("tangent");
    functionNames.push_back("noise");
    functionNames.push_back("blur");
    functionNames.push_back("gaussian");
    functionNames.push_back("exponential");
    functionNames.push_back("cosine");

    FillFunctionList(ui->listT0Functions,functionNames);
    FillFunctionList(ui->listT1Functions,functionNames);
    FillFunctionList(ui->listT2Functions,functionNames);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::FillFunctionList(QListWidget *list,vector<QString> &functionNames)
{
    for(auto& s : functionNames)
    {
        list->addItem(s);
    }

    for(int i=0;i<list->count();i++)
    {
        list->item(i)->setFlags(ui->listT0Functions->item(i)->flags() | Qt::ItemIsUserCheckable);
        list->item(i)->setCheckState(Qt::Unchecked);
    }
}

shared_ptr<RenderParameters> MainWindow::readRenderParameters()
{
    shared_ptr<RenderParameters> rp(new RenderParameters());
    rp->pictureWidth=ui->spinWidth->value();
    rp->pictureHeight=ui->spinHeight->value();
    rp->numberOfIterations=ui->spinIterations->value();
    return rp;
}

void MainWindow::setFunctionChecked(QListWidget *list, QString s)
{
    for(int i=0;i<list->count();i++)
    {
        if(list->item(i)->text() == s)
        {
            list->item(i)->setCheckState(Qt::Checked);
        }
    }
}

void MainWindow::setFunctionsChecked(QListWidget *list, Function *f)
{
    for(int i=0;i<list->count();i++)
    {
        list->item(i)->setCheckState(Qt::Unchecked);
    }

    for(auto variation : f->variations)
    {
        if(variation == variationSin) setFunctionChecked(list, "sinusoidal");
        else if(variation == variationFisheye) setFunctionChecked(list, "eyefish");
        else if(variation == variationSpherical) setFunctionChecked(list, "spherical");
        else if(variation == variationSwirl) setFunctionChecked(list, "swirl");
        else if(variation == variationHorseshoe) setFunctionChecked(list, "horseshoe");
        else if(variation == variationPolar) setFunctionChecked(list, "polar");
        else if(variation == variationHandkerchief) setFunctionChecked(list, "handkerchief");
        else if(variation == variationHeart) setFunctionChecked(list, "heart");
        else if(variation == variationDisk) setFunctionChecked(list, "disk");
        else if(variation == variationSpiral) setFunctionChecked(list, "spiral");
        else if(variation == variationHyperbolic) setFunctionChecked(list, "hyperbolic");
        else if(variation == variationDiamond) setFunctionChecked(list, "diamond");
        else if(variation == variationJulia) setFunctionChecked(list, "julia");
        else if(variation == variationEx) setFunctionChecked(list, "ex");
        else if(variation == variationBent) setFunctionChecked(list, "bent");
        else if(variation == variationMirror) setFunctionChecked(list, "mirror");
        else if(variation == variationPower) setFunctionChecked(list, "power");
        else if(variation == variationBubble) setFunctionChecked(list, "bubble");
        else if(variation == variationCylinder) setFunctionChecked(list, "cylinder");
        else if(variation == variationTangent) setFunctionChecked(list, "tangent");
        else if(variation == variationNoise) setFunctionChecked(list, "noise");
        else if(variation == variationBlur) setFunctionChecked(list, "blur");
        else if(variation == variationGaussian) setFunctionChecked(list, "gaussian");
        else if(variation == variationExponential) setFunctionChecked(list, "exponential");
        else if(variation == variationCosine) setFunctionChecked(list, "cosine");
        else
        {
            throw string("unknown variation!");
        }
    }
}

void MainWindow::FlameTransform0ToControls(Function *f)
{
    ui->spinT0PreXX->setValue(f->preTransformKoef[0][0]);
    ui->spinT0PreXY->setValue(f->preTransformKoef[0][1]);
    ui->spinT0PreXC->setValue(f->preTransformKoef[0][2]);

    ui->spinT0PreYX->setValue(f->preTransformKoef[1][0]);
    ui->spinT0PreYY->setValue(f->preTransformKoef[1][1]);
    ui->spinT0PreYC->setValue(f->preTransformKoef[1][2]);


    ui->spinT0PostXX->setValue(f->postTransformKoef[0][0]);
    ui->spinT0PostXY->setValue(f->postTransformKoef[0][1]);
    ui->spinT0PostXC->setValue(f->postTransformKoef[0][2]);

    ui->spinT0PostYX->setValue(f->postTransformKoef[1][0]);
    ui->spinT0PostYY->setValue(f->postTransformKoef[1][1]);
    ui->spinT0PostYC->setValue(f->postTransformKoef[1][2]);

    ui->spinT0R->setValue(f->r);
    ui->spinT0G->setValue(f->g);
    ui->spinT0B->setValue(f->b);

    setFunctionsChecked(ui->listT0Functions,f);
}


void MainWindow::FlameTransform1ToControls(Function *f)
{
    ui->spinT1PreXX->setValue(f->preTransformKoef[0][0]);
    ui->spinT1PreXY->setValue(f->preTransformKoef[0][1]);
    ui->spinT1PreXC->setValue(f->preTransformKoef[0][2]);

    ui->spinT1PreYX->setValue(f->preTransformKoef[1][0]);
    ui->spinT1PreYY->setValue(f->preTransformKoef[1][1]);
    ui->spinT1PreYC->setValue(f->preTransformKoef[1][2]);


    ui->spinT1PostXX->setValue(f->postTransformKoef[0][0]);
    ui->spinT1PostXY->setValue(f->postTransformKoef[0][1]);
    ui->spinT1PostXC->setValue(f->postTransformKoef[0][2]);

    ui->spinT1PostYX->setValue(f->postTransformKoef[1][0]);
    ui->spinT1PostYY->setValue(f->postTransformKoef[1][1]);
    ui->spinT1PostYC->setValue(f->postTransformKoef[1][2]);

    ui->spinT1R->setValue(f->r);
    ui->spinT1G->setValue(f->g);
    ui->spinT1B->setValue(f->b);

    setFunctionsChecked(ui->listT1Functions,f);
}

void MainWindow::FlameTransform2ToControls(Function *f)
{
    ui->spinT2PreXX->setValue(f->preTransformKoef[0][0]);
    ui->spinT2PreXY->setValue(f->preTransformKoef[0][1]);
    ui->spinT2PreXC->setValue(f->preTransformKoef[0][2]);

    ui->spinT2PreYX->setValue(f->preTransformKoef[1][0]);
    ui->spinT2PreYY->setValue(f->preTransformKoef[1][1]);
    ui->spinT2PreYC->setValue(f->preTransformKoef[1][2]);


    ui->spinT2PostXX->setValue(f->postTransformKoef[0][0]);
    ui->spinT2PostXY->setValue(f->postTransformKoef[0][1]);
    ui->spinT2PostXC->setValue(f->postTransformKoef[0][2]);

    ui->spinT2PostYX->setValue(f->postTransformKoef[1][0]);
    ui->spinT2PostYY->setValue(f->postTransformKoef[1][1]);
    ui->spinT2PostYC->setValue(f->postTransformKoef[1][2]);

    ui->spinT2R->setValue(f->r);
    ui->spinT2G->setValue(f->g);
    ui->spinT2B->setValue(f->b);

    setFunctionsChecked(ui->listT2Functions,f);
}



void MainWindow::FlameParametersToControls(shared_ptr<FlameParameters> fp)
{
    ui->spinXLowerBound->setValue(fp->xLowerBound);
    ui->spinXUpperBound->setValue(fp->xUpperBound);
    ui->spinYLowerBound->setValue(fp->yLowerBound);
    ui->spinYUpperBound->setValue(fp->yUpperBound);

    ui->checkSetBoundsByX->setChecked(fp->setViewBoundsByX);
    ui->checkSetBoundsByY->setChecked(fp->setViewBoundsByY);

    ui->spinSetBoundsRatio->setValue(fp->viewBoundsRatio);
    ui->spinSetBoundsCenter->setValue(fp->viewBoundsCenter);

    if(fp->functions.size()>0)
    {
        FlameTransform0ToControls(fp->functions[0].get());
    }
    if(fp->functions.size()>1)
    {
        FlameTransform1ToControls(fp->functions[1].get());
    }
    if(fp->functions.size()>2)
    {
        FlameTransform2ToControls(fp->functions[2].get());
    }
}

void MainWindow::on_butCalculateFlame_clicked()
{
    shared_ptr<RenderParameters> rp=readRenderParameters();
    rp->pictureWidth = ui->mainGraphicsView->size().width();
    rp->pictureHeight = ui->mainGraphicsView->size().height();

    shared_ptr<FlameParameters> fp(new FlameParameters());
    fp->initRandom();

    FlameParametersToControls(fp);


    fractalAlgo.setRenderParameters(rp);
    unsigned int *output = fractalAlgo.calculate(fp);

    if(output!=nullptr)
    {
        QImage img((uchar*)output, rp->pictureWidth, rp->pictureHeight, QImage::Format_RGB32);

        scene->clear();
        scene->addPixmap(QPixmap::fromImage(img));
        ui->mainGraphicsView->update();
    }
}

void MainWindow::on_butTransform0Color_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white);
    QString style = "background-color: rgb(%1, %2, %3);";
    ui->labelT0Color->setStyleSheet(style.arg(color.red()).arg(color.green()).arg(color.blue()));
}

void MainWindow::on_butOpen_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"select file");

    if(fileName.isNull())
    {
        return;
    }

    shared_ptr<RenderParameters> rp=readRenderParameters();
    rp->pictureWidth = ui->mainGraphicsView->size().width();
    rp->pictureHeight = ui->mainGraphicsView->size().height();

    shared_ptr<FlameParameters> fp(new FlameParameters());

    QByteArray ba = fileName.toLatin1();
    const char *c_str = ba.data();
    fp->load(c_str);

    FlameParametersToControls(fp);


    fractalAlgo.setRenderParameters(rp);
    unsigned int *output = fractalAlgo.calculate(fp);

    if(output!=nullptr)
    {
        QImage img((uchar*)output, rp->pictureWidth, rp->pictureHeight, QImage::Format_RGB32);

        scene->clear();
        scene->addPixmap(QPixmap::fromImage(img));
        ui->mainGraphicsView->update();
    }
}
