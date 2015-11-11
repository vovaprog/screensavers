#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>
#include <QColorDialog>
#include <QPalette>
#include <QFileDialog>
#include <QProgressDialog>

#include <memory>
#include <vector>

#include <filesystem_utils.h>
#include <ImageUtils.h>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene;
    ui->mainGraphicsView->setScene(scene);

    FillFunctionList(ui->listT0Functions);
    FillFunctionList(ui->listT1Functions);
    FillFunctionList(ui->listT2Functions);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::FillFunctionList(QListWidget *list)
{
    for(auto& v : getVariations())
    {
        list->addItem(v.name);
    }

    for(int i=0;i<list->count();i++)
    {
        list->item(i)->setFlags(ui->listT0Functions->item(i)->flags() | Qt::ItemIsUserCheckable);
        list->item(i)->setCheckState(Qt::Unchecked);
    }
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
        setFunctionChecked(list, variation.name);
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
        ui->pageT0->setHidden(false);
    }
    else
    {
        ui->pageT0->setHidden(true);
    }

    if(fp->functions.size()>1)
    {
        FlameTransform1ToControls(fp->functions[1].get());
        ui->pageT1->setHidden(false);
    }
    else
    {
        ui->pageT1->setHidden(true);
    }

    if(fp->functions.size()>2)
    {
        FlameTransform2ToControls(fp->functions[2].get());
        ui->pageT2->setHidden(false);
    }
    else
    {
        ui->pageT2->setHidden(true);
    }
}

void MainWindow::getFunctionsChecked(QListWidget *list, Function *f)
{
    for(int i=0;i<list->count();i++)
    {
        if(list->item(i)->checkState()==Qt::Checked)
        {
            QString t = list->item(i)->text();

            QByteArray ba = t.toLatin1();
            const char *c_str = ba.data();


            Variation v = getVariationByName(c_str);

            if(v.f!=nullptr)
            {
                f->variations.push_back(v);
            }
            else
            {
                throw string("invalid variation name!");
            }
        }
    }
}


void MainWindow::ReadTransform0FromControls(Function *f)
{
    f->preTransformKoef[0][0]=ui->spinT0PreXX->value();
    f->preTransformKoef[0][1]=ui->spinT0PreXY->value();
    f->preTransformKoef[0][2]=ui->spinT0PreXC->value();

    f->preTransformKoef[1][0]=ui->spinT0PreYX->value();
    f->preTransformKoef[1][1]=ui->spinT0PreYY->value();
    f->preTransformKoef[1][2]=ui->spinT0PreYC->value();


    f->postTransformKoef[0][0]=ui->spinT0PostXX->value();
    f->postTransformKoef[0][1]=ui->spinT0PostXY->value();
    f->postTransformKoef[0][2]=ui->spinT0PostXC->value();

    f->postTransformKoef[1][0]=ui->spinT0PostYX->value();
    f->postTransformKoef[1][1]=ui->spinT0PostYY->value();
    f->postTransformKoef[1][2]=ui->spinT0PostYC->value();

    f->r=ui->spinT0R->value();
    f->g=ui->spinT0G->value();
    f->b=ui->spinT0B->value();

    getFunctionsChecked(ui->listT0Functions,f);
}

void MainWindow::ReadTransform1FromControls(Function *f)
{
    f->preTransformKoef[0][0]=ui->spinT1PreXX->value();
    f->preTransformKoef[0][1]=ui->spinT1PreXY->value();
    f->preTransformKoef[0][2]=ui->spinT1PreXC->value();

    f->preTransformKoef[1][0]=ui->spinT1PreYX->value();
    f->preTransformKoef[1][1]=ui->spinT1PreYY->value();
    f->preTransformKoef[1][2]=ui->spinT1PreYC->value();


    f->postTransformKoef[0][0]=ui->spinT1PostXX->value();
    f->postTransformKoef[0][1]=ui->spinT1PostXY->value();
    f->postTransformKoef[0][2]=ui->spinT1PostXC->value();

    f->postTransformKoef[1][0]=ui->spinT1PostYX->value();
    f->postTransformKoef[1][1]=ui->spinT1PostYY->value();
    f->postTransformKoef[1][2]=ui->spinT1PostYC->value();

    f->r=ui->spinT1R->value();
    f->g=ui->spinT1G->value();
    f->b=ui->spinT1B->value();

    getFunctionsChecked(ui->listT1Functions,f);
}

void MainWindow::ReadTransform2FromControls(Function *f)
{
    f->preTransformKoef[0][0]=ui->spinT2PreXX->value();
    f->preTransformKoef[0][1]=ui->spinT2PreXY->value();
    f->preTransformKoef[0][2]=ui->spinT2PreXC->value();

    f->preTransformKoef[1][0]=ui->spinT2PreYX->value();
    f->preTransformKoef[1][1]=ui->spinT2PreYY->value();
    f->preTransformKoef[1][2]=ui->spinT2PreYC->value();


    f->postTransformKoef[0][0]=ui->spinT2PostXX->value();
    f->postTransformKoef[0][1]=ui->spinT2PostXY->value();
    f->postTransformKoef[0][2]=ui->spinT2PostXC->value();

    f->postTransformKoef[1][0]=ui->spinT2PostYX->value();
    f->postTransformKoef[1][1]=ui->spinT2PostYY->value();
    f->postTransformKoef[1][2]=ui->spinT2PostYC->value();

    f->r=ui->spinT2R->value();
    f->g=ui->spinT2G->value();
    f->b=ui->spinT2B->value();

    getFunctionsChecked(ui->listT2Functions,f);
}


void MainWindow::ReadFlameParametersFromControls(shared_ptr<FlameParameters> fp)
{
    fp->xLowerBound=ui->spinXLowerBound->value();
    fp->xUpperBound=ui->spinXUpperBound->value();
    fp->yLowerBound=ui->spinYLowerBound->value();
    fp->yUpperBound=ui->spinYUpperBound->value();

    fp->setViewBoundsByX = ui->checkSetBoundsByX->isChecked();
    fp->setViewBoundsByY = ui->checkSetBoundsByY->isChecked();

    fp->viewBoundsRatio = ui->spinSetBoundsRatio->value();
    fp->viewBoundsCenter = ui->spinSetBoundsCenter->value();

    if(!ui->pageT0->isHidden())
    {
        Function *f = new Function();
        ReadTransform0FromControls(f);
        fp->functions.push_back(unique_ptr<Function>(f));
    }

    if(!ui->pageT1->isHidden())
    {
        Function *f = new Function();
        ReadTransform1FromControls(f);
        fp->functions.push_back(unique_ptr<Function>(f));
    }

    if(!ui->pageT2->isHidden())
    {
        Function *f = new Function();
        ReadTransform2FromControls(f);
        fp->functions.push_back(unique_ptr<Function>(f));
    }

    fp->prepare();
}

void MainWindow::calculateFlame(unsigned int iterations)
{
    shared_ptr<RenderParameters> rp(new RenderParameters());
    rp->pictureWidth = ui->mainGraphicsView->size().width()-4;
    rp->pictureHeight = ui->mainGraphicsView->size().height()-4;
    rp->numberOfIterations=iterations;

    shared_ptr<FlameParameters> fp(new FlameParameters());
    ReadFlameParametersFromControls(fp);

    fractalAlgo.setRenderParameters(rp);
    unsigned int *output = fractalAlgo.calculate(fp);

    if(output!=nullptr)
    {
        QImage img((uchar*)output, rp->pictureWidth, rp->pictureHeight, QImage::Format_RGB32);

        img = img.mirrored();

        scene->clear();
        scene->addPixmap(QPixmap::fromImage(img));
        ui->mainGraphicsView->update();
    }
}

void MainWindow::on_butOpenFlame_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"select file");

    if(fileName.isNull())
    {
        return;
    }

    shared_ptr<FlameParameters> fp(new FlameParameters());

    QByteArray ba = fileName.toLatin1();
    const char *c_str = ba.data();
    fp->load(c_str);

    FlameParametersToControls(fp);

    calculateFlame(ui->spinIterationsFast->value());
}

void MainWindow::on_butRandomFlame_clicked()
{
    shared_ptr<FlameParameters> fp(new FlameParameters());
    fp->initRandom();

    FlameParametersToControls(fp);

    calculateFlame(ui->spinIterationsFast->value());
}

void MainWindow::on_butCalculateFast_clicked()
{
    calculateFlame(ui->spinIterationsFast->value());
}

void MainWindow::on_butCalculateMore_clicked()
{
    calculateFlame(ui->spinIterationsMore->value());
}


void MainWindow::on_butSaveFlame_clicked()
{
    string dirName="./fractals";

    if(!directoryExists(dirName.c_str()))
    {
        createDirectory(dirName.c_str());
    }


    string fileName;

    for(int i=0;i<1000;i++)
    {
        fileName = dirName+"/fractal_"+to_string(i);

        if(!fileExists((fileName+".xml").c_str()) && !fileExists((fileName+".png").c_str()))
        {
            break;
        }
    }


    shared_ptr<FlameParameters> fp(new FlameParameters());
    ReadFlameParametersFromControls(fp);
    fp->save((fileName+".xml").c_str());


    shared_ptr<RenderParameters> rp(new RenderParameters());
    rp->pictureWidth = ui->mainGraphicsView->size().width();
    rp->pictureHeight = ui->mainGraphicsView->size().height();
    rp->numberOfIterations=ui->spinIterationsFast->value();

    fractalAlgo.setRenderParameters(rp);
    unsigned int *output = fractalAlgo.calculate(fp);

    if(output!=nullptr)
    {
        saveImage((fileName+".png").c_str(), "png", output, rp->pictureWidth, rp->pictureHeight);
    }
}

void MainWindow::displayColor(QSpinBox *spinR,QSpinBox *spinG,QSpinBox *spinB,QLineEdit *colorDisplay)
{
    QColor color(spinR->value(),spinG->value(),spinB->value());
    QString style = "background-color: rgb(%1, %2, %3);";
    colorDisplay->setStyleSheet(style.arg(color.red()).arg(color.green()).arg(color.blue()));
}

void MainWindow::selectColor(QSpinBox *spinR,QSpinBox *spinG,QSpinBox *spinB,QLineEdit *colorDisplay)
{
    QColor color(spinR->value(),spinG->value(),spinB->value());

    color = QColorDialog::getColor(color);

    spinR->setValue(color.red());
    spinG->setValue(color.green());
    spinB->setValue(color.blue());

    displayColor(spinR,spinG,spinB,colorDisplay);
}

void MainWindow::on_butT0Color_clicked()
{
    selectColor(ui->spinT0R,ui->spinT0G,ui->spinT0B,ui->colorT0Display);
}

void MainWindow::on_butT1Color_clicked()
{
    selectColor(ui->spinT1R,ui->spinT1G,ui->spinT1B,ui->colorT1Display);
}

void MainWindow::on_spinT0R_valueChanged(int arg1)
{
    displayColor(ui->spinT0R,ui->spinT0G,ui->spinT0B,ui->colorT0Display);
}

void MainWindow::on_spinT0G_valueChanged(int arg1)
{
    displayColor(ui->spinT0R,ui->spinT0G,ui->spinT0B,ui->colorT0Display);
}

void MainWindow::on_spinT0B_valueChanged(int arg1)
{
    displayColor(ui->spinT0R,ui->spinT0G,ui->spinT0B,ui->colorT0Display);
}

void MainWindow::on_spinT1R_valueChanged(int arg1)
{
    displayColor(ui->spinT1R,ui->spinT1G,ui->spinT1B,ui->colorT1Display);
}

void MainWindow::on_spinT1G_valueChanged(int arg1)
{
    displayColor(ui->spinT1R,ui->spinT1G,ui->spinT1B,ui->colorT1Display);
}

void MainWindow::on_spinT1B_valueChanged(int arg1)
{
    displayColor(ui->spinT1R,ui->spinT1G,ui->spinT1B,ui->colorT1Display);
}

void MainWindow::on_butT2Color_clicked()
{
    selectColor(ui->spinT2R,ui->spinT2G,ui->spinT2B,ui->colorT2Display);
}

void MainWindow::on_spinT2R_valueChanged(int arg1)
{
    displayColor(ui->spinT2R,ui->spinT2G,ui->spinT2B,ui->colorT2Display);
}

void MainWindow::on_spinT2G_valueChanged(int arg1)
{
    displayColor(ui->spinT2R,ui->spinT2G,ui->spinT2B,ui->colorT2Display);
}

void MainWindow::on_spinT2B_valueChanged(int arg1)
{
    displayColor(ui->spinT2R,ui->spinT2G,ui->spinT2B,ui->colorT2Display);
}
