#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QGraphicsScene>

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

private:
    Ui::MainWindow *ui;

    QGraphicsScene * scene;
    FractalFlameAlgorithm fractalAlgo;
};

#endif // MAINWINDOW_H
