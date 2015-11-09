#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QGraphicsScene>
#include <QListWidget>

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
    void on_butCalculateFlame_clicked();

    void on_butTransform0Color_clicked();

    void on_butOpenFlame_clicked();

    void on_butRandomFlame_clicked();

private:
    Ui::MainWindow *ui;

    shared_ptr<RenderParameters> readRenderParameters();
    void FlameParametersToControls(shared_ptr<FlameParameters> fp);
    void FlameTransform0ToControls(Function *f);
    void FlameTransform1ToControls(Function *f);
    void FlameTransform2ToControls(Function *f);
    void setFunctionChecked(QListWidget *list, QString s);
    void setFunctionsChecked(QListWidget *list, Function *f);
    void FillFunctionList(QListWidget *list,vector<QString> &functionNames);

    void ReadFlameParametersFromControls(shared_ptr<FlameParameters> fp);
    void ReadTransform0FromControls(Function *f);
    void ReadTransform1FromControls(Function *f);
    void ReadTransform2FromControls(Function *f);
    void getFunctionsChecked(QListWidget *list, Function *f);

    QGraphicsScene * scene;
    FractalFlameAlgorithm fractalAlgo;
};

#endif // MAINWINDOW_H
