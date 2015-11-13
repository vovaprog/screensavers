#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QGraphicsScene>
#include <QListWidget>
#include <QSpinBox>

#include <FractalFlame2.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_butOpenFlame_clicked();

    void on_butRandomFlame_clicked();

    void on_butCalculateFast_clicked();

    void on_butCalculateMore_clicked();

    void on_butSaveFlame_clicked();

    void on_butT0Color_clicked();

    void on_butT1Color_clicked();

    void on_spinT0R_valueChanged(int arg1);

    void on_spinT0G_valueChanged(int arg1);

    void on_spinT0B_valueChanged(int arg1);

    void on_spinT1R_valueChanged(int arg1);

    void on_spinT1G_valueChanged(int arg1);

    void on_spinT1B_valueChanged(int arg1);

    void on_butT2Color_clicked();

    void on_spinT2R_valueChanged(int arg1);

    void on_spinT2G_valueChanged(int arg1);

    void on_spinT2B_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;

    void FlameParametersToControls(shared_ptr<FlameParameters> fp);
    void FlameTransform0ToControls(Function *f);
    void FlameTransform1ToControls(Function *f);
    void FlameTransform2ToControls(Function *f);
    void setFunctionChecked(QListWidget *list, QString s);
    void setFunctionsChecked(QListWidget *list, Function *f);
    void FillFunctionList(QListWidget *list);

    void ReadFlameParametersFromControls(shared_ptr<FlameParameters> fp);
    void ReadTransform0FromControls(Function *f);
    void ReadTransform1FromControls(Function *f);
    void ReadTransform2FromControls(Function *f);
    void getFunctionsChecked(QListWidget *list, Function *f);

    bool calculateFlame(unsigned int iterations);
    void selectColor(QSpinBox *spinR,QSpinBox *spinG,QSpinBox *spinB,QLineEdit *colorDisplay);
    void displayColor(QSpinBox *spinR,QSpinBox *spinG,QSpinBox *spinB,QLineEdit *colorDisplay);

    QGraphicsScene * scene;
    FractalFlameAlgorithm fractalAlgo;
};

#endif // MAINWINDOW_H
