#ifndef OGLWIDGETSPC_H
#define OGLWIDGETSPC_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "point2.h"
#include "vec2.h"
#include "oglutilities.h"

// OpenGLWidget to display glyphs in three connected SPC planes
class OGLWidgetSPC: public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    // Constructor
    OGLWidgetSPC(QWidget *parent = nullptr);

protected:
    // Override necessary functions for rendering
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif // OGLWIDGETSPC_H
