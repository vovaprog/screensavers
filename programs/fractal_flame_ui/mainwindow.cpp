#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>
#include <QColorDialog>
#include <QPalette>
#include <QFileDialog>
#include <QProgressDialog>
#include <QKeyEvent>

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

    ui->widgetT0Container->setEnabled(false);
    ui->widgetT1Container->setEnabled(false);
    ui->widgetT2Container->setEnabled(false);
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
    ui->spinT0PreXX->setValue(f->preTransformX_CoefX);
    ui->spinT0PreXY->setValue(f->preTransformX_CoefY);
    ui->spinT0PreXC->setValue(f->preTransformX_CoefC);

    ui->spinT0PreYX->setValue(f->preTransformY_CoefX);
    ui->spinT0PreYY->setValue(f->preTransformY_CoefY);
    ui->spinT0PreYC->setValue(f->preTransformY_CoefC);


    ui->spinT0PostXX->setValue(f->postTransformX_CoefX);
    ui->spinT0PostXY->setValue(f->postTransformX_CoefY);
    ui->spinT0PostXC->setValue(f->postTransformX_CoefC);

    ui->spinT0PostYX->setValue(f->postTransformY_CoefX);
    ui->spinT0PostYY->setValue(f->postTransformY_CoefY);
    ui->spinT0PostYC->setValue(f->postTransformY_CoefC);

    ui->spinT0R->setValue(f->r);
    ui->spinT0G->setValue(f->g);
    ui->spinT0B->setValue(f->b);

    setFunctionsChecked(ui->listT0Functions,f);
}


void MainWindow::FlameTransform1ToControls(Function *f)
{
    ui->spinT1PreXX->setValue(f->preTransformX_CoefX);
    ui->spinT1PreXY->setValue(f->preTransformX_CoefY);
    ui->spinT1PreXC->setValue(f->preTransformX_CoefC);

    ui->spinT1PreYX->setValue(f->preTransformY_CoefX);
    ui->spinT1PreYY->setValue(f->preTransformY_CoefY);
    ui->spinT1PreYC->setValue(f->preTransformY_CoefC);


    ui->spinT1PostXX->setValue(f->postTransformX_CoefX);
    ui->spinT1PostXY->setValue(f->postTransformX_CoefY);
    ui->spinT1PostXC->setValue(f->postTransformX_CoefC);

    ui->spinT1PostYX->setValue(f->postTransformY_CoefX);
    ui->spinT1PostYY->setValue(f->postTransformY_CoefY);
    ui->spinT1PostYC->setValue(f->postTransformY_CoefC);

    ui->spinT1R->setValue(f->r);
    ui->spinT1G->setValue(f->g);
    ui->spinT1B->setValue(f->b);

    setFunctionsChecked(ui->listT1Functions,f);
}

void MainWindow::FlameTransform2ToControls(Function *f)
{
    ui->spinT2PreXX->setValue(f->preTransformX_CoefX);
    ui->spinT2PreXY->setValue(f->preTransformX_CoefY);
    ui->spinT2PreXC->setValue(f->preTransformX_CoefC);

    ui->spinT2PreYX->setValue(f->preTransformY_CoefX);
    ui->spinT2PreYY->setValue(f->preTransformY_CoefY);
    ui->spinT2PreYC->setValue(f->preTransformY_CoefC);


    ui->spinT2PostXX->setValue(f->postTransformX_CoefX);
    ui->spinT2PostXY->setValue(f->postTransformX_CoefY);
    ui->spinT2PostXC->setValue(f->postTransformX_CoefC);

    ui->spinT2PostYX->setValue(f->postTransformY_CoefX);
    ui->spinT2PostYY->setValue(f->postTransformY_CoefY);
    ui->spinT2PostYC->setValue(f->postTransformY_CoefC);

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

    if(fp->setBoundsAxis==FlameParameters::Axis::x)
    {
        ui->comboSetBoundsAxis->setCurrentIndex(1);
    }
    else if(fp->setBoundsAxis==FlameParameters::Axis::y)
    {
        ui->comboSetBoundsAxis->setCurrentIndex(2);
    }
    else
    {
        ui->comboSetBoundsAxis->setCurrentIndex(0);
    }

    ui->spinSetBoundsRatio->setValue(fp->setBoundsRatio);
    ui->spinSetBoundsCenter->setValue(fp->setBoundsCenter);

    if(fp->functions.size()>0)
    {
        FlameTransform0ToControls(fp->functions[0].get());
        ui->checkUseT0->setChecked(true);
    }
    else
    {
        ui->checkUseT0->setChecked(false);
    }

    if(fp->functions.size()>1)
    {
        FlameTransform1ToControls(fp->functions[1].get());
        ui->checkUseT1->setChecked(true);
    }
    else
    {
        ui->checkUseT1->setChecked(false);
    }

    if(fp->functions.size()>2)
    {
        FlameTransform2ToControls(fp->functions[2].get());
        ui->checkUseT2->setChecked(true);
    }
    else
    {
        ui->checkUseT2->setChecked(false);
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

            if(v.isValid())
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
    f->preTransformX_CoefX=ui->spinT0PreXX->value();
    f->preTransformX_CoefY=ui->spinT0PreXY->value();
    f->preTransformX_CoefC=ui->spinT0PreXC->value();

    f->preTransformY_CoefX=ui->spinT0PreYX->value();
    f->preTransformY_CoefY=ui->spinT0PreYY->value();
    f->preTransformY_CoefC=ui->spinT0PreYC->value();


    f->postTransformX_CoefX=ui->spinT0PostXX->value();
    f->postTransformX_CoefY=ui->spinT0PostXY->value();
    f->postTransformX_CoefC=ui->spinT0PostXC->value();

    f->postTransformY_CoefX=ui->spinT0PostYX->value();
    f->postTransformY_CoefY=ui->spinT0PostYY->value();
    f->postTransformY_CoefC=ui->spinT0PostYC->value();

    f->r=ui->spinT0R->value();
    f->g=ui->spinT0G->value();
    f->b=ui->spinT0B->value();

    getFunctionsChecked(ui->listT0Functions,f);
}

void MainWindow::ReadTransform1FromControls(Function *f)
{
    f->preTransformX_CoefX=ui->spinT1PreXX->value();
    f->preTransformX_CoefY=ui->spinT1PreXY->value();
    f->preTransformX_CoefC=ui->spinT1PreXC->value();

    f->preTransformY_CoefX=ui->spinT1PreYX->value();
    f->preTransformY_CoefY=ui->spinT1PreYY->value();
    f->preTransformY_CoefC=ui->spinT1PreYC->value();


    f->postTransformX_CoefX=ui->spinT1PostXX->value();
    f->postTransformX_CoefY=ui->spinT1PostXY->value();
    f->postTransformX_CoefC=ui->spinT1PostXC->value();

    f->postTransformY_CoefX=ui->spinT1PostYX->value();
    f->postTransformY_CoefY=ui->spinT1PostYY->value();
    f->postTransformY_CoefC=ui->spinT1PostYC->value();

    f->r=ui->spinT1R->value();
    f->g=ui->spinT1G->value();
    f->b=ui->spinT1B->value();

    getFunctionsChecked(ui->listT1Functions,f);
}

void MainWindow::ReadTransform2FromControls(Function *f)
{
    f->preTransformX_CoefX=ui->spinT2PreXX->value();
    f->preTransformX_CoefY=ui->spinT2PreXY->value();
    f->preTransformX_CoefC=ui->spinT2PreXC->value();

    f->preTransformY_CoefX=ui->spinT2PreYX->value();
    f->preTransformY_CoefY=ui->spinT2PreYY->value();
    f->preTransformY_CoefC=ui->spinT2PreYC->value();


    f->postTransformX_CoefX=ui->spinT2PostXX->value();
    f->postTransformX_CoefY=ui->spinT2PostXY->value();
    f->postTransformX_CoefC=ui->spinT2PostXC->value();

    f->postTransformY_CoefX=ui->spinT2PostYX->value();
    f->postTransformY_CoefY=ui->spinT2PostYY->value();
    f->postTransformY_CoefC=ui->spinT2PostYC->value();

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

    if(ui->comboSetBoundsAxis->currentText()=="x")
    {
        fp->setBoundsAxis=FlameParameters::Axis::x;
    }
    else if(ui->comboSetBoundsAxis->currentText()=="y")
    {
        fp->setBoundsAxis=FlameParameters::Axis::y;
    }
    else
    {
        fp->setBoundsAxis=FlameParameters::Axis::none;
    }

    fp->setBoundsRatio = ui->spinSetBoundsRatio->value();
    fp->setBoundsCenter = ui->spinSetBoundsCenter->value();

    if(ui->checkUseT0->isChecked())
    {
        Function *f = new Function();
        ReadTransform0FromControls(f);
        fp->functions.push_back(unique_ptr<Function>(f));
    }

    if(ui->checkUseT1->isChecked())
    {
        Function *f = new Function();
        ReadTransform1FromControls(f);
        fp->functions.push_back(unique_ptr<Function>(f));
    }

    if(ui->checkUseT2->isChecked())
    {
        Function *f = new Function();
        ReadTransform2FromControls(f);
        fp->functions.push_back(unique_ptr<Function>(f));
    }

    fp->prepare();
}

bool MainWindow::calculateFlame(unsigned int iterations)
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

        return true;
    }
    else
    {
        return false;
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

    for(int i=0;i<10;i++)
    {
        fp->initRandom();

        FlameParametersToControls(fp);

        if(calculateFlame(ui->spinIterationsFast->value()))
        {
            break;
        }
    }
}

void MainWindow::on_butCalculateFast_clicked()
{
    calculateFlame(ui->spinIterationsFast->value());
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

    if(color.isValid())
    {
        spinR->setValue(color.red());
        spinG->setValue(color.green());
        spinB->setValue(color.blue());

        displayColor(spinR,spinG,spinB,colorDisplay);
    }
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

void MainWindow::on_butCalculateGood_clicked()
{
    calculateFlame(ui->spinIterationsGood->value());
}

void MainWindow::setBoundsValue(double value)
{
    ui->spinXLowerBound->setValue(-value);
    ui->spinXUpperBound->setValue(value);
    ui->spinYLowerBound->setValue(-value);
    ui->spinYUpperBound->setValue(value);
}

void MainWindow::on_butSetBounds1_clicked()
{
    setBoundsValue(1.0);
}

void MainWindow::on_butSetBounds2_clicked()
{
    setBoundsValue(2.0);
}

void MainWindow::on_butSetBounds3_clicked()
{
    setBoundsValue(3.0);
}

void MainWindow::on_butSetBounds4_clicked()
{
    setBoundsValue(4.0);
}

void MainWindow::on_comboSetBoundsAxis_currentIndexChanged(const QString &arg1)
{
    if(arg1=="x")
    {
        ui->spinXLowerBound->setEnabled(false);
        ui->spinXUpperBound->setEnabled(false);
        ui->spinYLowerBound->setEnabled(true);
        ui->spinYUpperBound->setEnabled(true);
    }
    else if(arg1=="y")
    {
        ui->spinXLowerBound->setEnabled(true);
        ui->spinXUpperBound->setEnabled(true);
        ui->spinYLowerBound->setEnabled(false);
        ui->spinYUpperBound->setEnabled(false);
    }
    else
    {
        ui->spinXLowerBound->setEnabled(true);
        ui->spinXUpperBound->setEnabled(true);
        ui->spinYLowerBound->setEnabled(true);
        ui->spinYUpperBound->setEnabled(true);
    }
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Q)
    {
        calculateFlame(ui->spinIterationsFast->value());
    }
    else if(event->key() == Qt::Key_W)
    {
        calculateFlame(ui->spinIterationsGood->value());
    }
}


