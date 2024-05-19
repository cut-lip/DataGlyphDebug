#include "oglwidgetspc.h"


OGLWidgetSPC::OGLWidgetSPC(QWidget *parent)
    : QOpenGLWidget(parent)
{
    // Additional initialization code if needed
}


void OGLWidgetSPC::initializeGL() {
    initializeOpenGLFunctions();

    // Set the viewport to the entire widget
    glViewport(0, 0, width(), height());

    // Clear the screen (typically, this should be done in the rendering loop, not here)
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Set the projection matrix (this should come before setting up the viewport)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width(), 0, height(), -1, 1);

    // Check if any errors were thrown by OpenGL (this is okay to do here)
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        // Handle the error (e.g., print an error message)
        qDebug() << "OpenGL error:" << error;
    }

    // Flush the buffer (this is typically not necessary in modern OpenGL)
    glFlush();
}

void OGLWidgetSPC::resizeGL(int w, int h)
{
    // Handle viewport resizing and projection matrix updates
}

void OGLWidgetSPC::paintGL() {
    // Switch to modelview matrix mode
    initializeOpenGLFunctions();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // DEBUG STATEMENT
    /*
    for (const auto& number : allData[0]) {
        std::cout << number << " ";
    }
    */

    glColor3f(1.0, 0.0, 0.0);

    // Call this to enable Qt to work with vertex arrays
    glEnableClientState(GL_VERTEX_ARRAY);

    // Set the viewport to the entire widget
    glViewport(0, 0, width(), height()/2);

    auto numSPC = 3;
    auto margin = 10;
    auto gridWidth = (width() / numSPC) - 2 * margin;

    // DRAW 1st SPC AXES
    glLineWidth(4.0);
    drawArrow(Point2(margin, margin), Point2(margin, height() - margin), width());
    drawArrow(Point2(margin, margin), Point2((width() / 4) + (width() / 24), margin), height());

    glLineWidth(1.0);
    drawGridSPC(0, 0,
                (width() / 4) + (width() / 24), height() - 10, 5);

    // Draw 2nd SPC axes
    glLineWidth(4.0);
    drawArrow(Point2((width() / numSPC), margin),
              Point2((width() / numSPC), height() - margin), height());

    drawArrow(Point2((width() / numSPC), margin),
              Point2(((7 * width()) / 12) + (width() / 24), margin), height());

    glLineWidth(1.0);
    drawGridSPC((width() / numSPC) + margin, 0,
                (width() / 4) + (width() / 24), height() - margin, 5);

    /*
    // Draw 3rd SPC axes
    glLineWidth(4.0);
    drawArrow(Point2(((2 * width()) / numSPC), margin),
              Point2(((2 * width()) / numSPC), height() - margin), height());
    drawArrow(Point2(((2 * width()) / numSPC), margin),
              Point2(((11 * width()) / 12) + (width() / 24), margin), height());

    glLineWidth(1.0);
    drawGridSPC(((2 * width()) / numSPC) + margin, 0,
                (width() / 4) + (width() / 24), height() - margin, 5);
*/
}
