#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <GL/glu.h>
#include <vector>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMessageBox>
#include <QOpenGLFunctions_1_0>

#include <QObject>
#include <QFileDialog>
#include <QFile>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>

#include "oglutilities.h"
#include "myglobals.h"
#include "oglwidgetspc.h"
#include "vec2.h"
#include "point2.h"
#include "turtleg.h"
#include "SPC_SF.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Define mainwindow class
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor
    MainWindow(QWidget *parent = nullptr);
    // Destructor
    ~MainWindow();

private slots:
    void on_actionUpload_File_triggered();

    void on_actionSet_width_and_height_triggered();

    void on_verticalSlider_valueChanged(int value);

    void on_openGLWidget_aboutToCompose();

    void on_hyperblock_button_clicked();

    //void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

// Define MyOpenGLWidget Class
class OGLWidgetGrid : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    // Constructor
    OGLWidgetGrid(QWidget *parent = nullptr);

protected:
    // Override necessary functions for rendering
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif // MAINWINDOW_H
