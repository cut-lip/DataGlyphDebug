#include "oglwidgetspc.h"
#include <QMatrix4x4>


extern std::vector<std::vector<GLfloat>> allData;;

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
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1.0, 0.0, 0.0);

    // Call this to enable Qt to work with vertex arrays
    glEnableClientState(GL_VERTEX_ARRAY);

    // Set the viewport to the entire widget
    glViewport(0, 0, width(), height()/2);

    auto numSPC = 3;
    auto margin = 10;
    //glTranslatef(0.0, 20.0, 0.0);

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


    // Draw 3rd SPC axes
    glLineWidth(4.0);
    drawArrow(Point2(((2 * width()) / numSPC), margin),
              Point2(((2 * width()) / numSPC), height() - margin), height());
    drawArrow(Point2(((2 * width()) / numSPC), margin),
              Point2(((11 * width()) / 12) + (width() / 24), margin), height());

    glLineWidth(1.0);
    drawGridSPC(((2 * width()) / numSPC) + margin, 0,
                (width() / 4) + (width() / 24), height() - margin, 5);


    if (!allData[0].empty()) {

        int iteration = 1;
        // Display points within threshold
        // Pass through twice: Draw classes sequentially depending on CLASS_SEPERATION_MODE flag
        for (std::vector<std::vector<GLfloat>>::iterator it = (allData.begin());
             it < allData.end(); ++it)
        {
            // Set the viewport to the entire widget
            //glViewport(0, 0, width(), height()/2);

            //DEBUGG
            std::cout << "Hello STPPPPPSJNFES" << std::endl;

            glBegin(GL_LINE_LOOP);
            glVertex2f(0, 0);
            glVertex2f(200, 200);
            glEnd();


            //glMatrixMode(GL_PROJECTION);
            //glLoadIdentity();
            //QMatrix4x4 projection;
            //projection.perspective(45.0f, float(width()) / height(), 0.1f, 100.0f);

            drawLocatedGLyphs(&(*it), this->width(), this->height());
            ++iteration;

            // don't iterate past end of vectors
            if (it != allData.end())
            {
                //++classVec;
            }
        }

        // Reset iterators for second pass through data
        // A1
/*
        maxY = 0.0;
        maxX = 0.0;
        minX = 10.0;
        minY = 10.0;
*/
        //glFlush();
    }

    glPopMatrix();
}
