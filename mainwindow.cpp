#include "mainwindow.h"
#include "./ui_mainwindow.h"

int DATA_SIZE = 683;
//int NUM_ROWS = 4;
//int NUM_COLS = 4;

std::vector<std::vector<GLfloat>> allData(DATA_SIZE);

// MainWindow constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    OGLWidgetGrid* oglWidget = new OGLWidgetGrid(this); // Create a dynamic instance of MyOpenGLWidget

    mainLayout->addWidget(oglWidget);
    //w.setCentralWidget(oglWidget); // Set MyOpenGLWidget as the central widget of MainWindow

    QLabel *nativeLabel = new QLabel(tr("Native"));
    nativeLabel->setAlignment(Qt::AlignHCenter);
    mainLayout->addWidget(nativeLabel);
    ui->setupUi(this);
}

// MainWindow destructor
MainWindow::~MainWindow()
{
    delete ui;
}

// OpenGL Widget Constructor
OGLWidgetGrid::OGLWidgetGrid(QWidget *parent)
    : QOpenGLWidget(parent)
{
    // Additional initialization code if needed
}

// OpenGLWidget Override Functions
void OGLWidgetGrid::initializeGL()
{
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

void OGLWidgetGrid::resizeGL(int w, int h)
{
    // Handle viewport resizing and projection matrix updates
}

void OGLWidgetGrid::paintGL()
{
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
    glViewport(0, 0, width(), height());
    drawGrid(width(), height());

    // Check if any errors were thrown by OpenGL
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        // Handle the error (e.g., print an error message)
        qDebug() << "OpenGL error:" << error;
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(0.0, 0.0, 1.0);
    int dataCount = 0;
    // Calculate the viewport dimensions based on the window size
    // Calculate the viewport dimensions based on the window size
    int viewportWidth = width() / NUM_COLUMNS;
    int viewportHeight = height() / NUM_ROWS;

    // Iterate through the entire screen
    for (int j = 0; j < NUM_ROWS; j++) {
        for (int i = 0; i < NUM_COLUMNS; i++) {
            // Set the viewport for the current cell
            int viewportX = i * viewportWidth;
            int viewportY = j * viewportHeight;
            glViewport(viewportX, viewportY, viewportWidth, viewportHeight);


            if (!allData[0].empty()) {
                // Example, draw line strip
                glLineWidth(4.0);
                glBegin(GL_LINE_STRIP);
                glVertex2f(allData[dataCount][1]*2, allData[dataCount][2]*2);  // Bottom left vertex
                glVertex2f(allData[dataCount][3]*2, allData[dataCount][4]*2);  // Bottom right vertex
                glVertex2f(allData[dataCount][5]*2, allData[dataCount][6]*2);  // Top vertex
                glEnd();
            } else {
                // Example, draw line strip
                glLineWidth(4.0);
                glBegin(GL_LINE_STRIP);
                glVertex2f(1, 1);           // Bottom left vertex
                glVertex2f(0.3, 0.5);       // Bottom right vertex
                glVertex2f(0, 0.8);         // Top vertex
                glEnd();
            }

            dataCount++;
        }
    }

    glViewport(0, 0, width()/4, height()/4);
    glColor3f(0.0, 0.0, 1.0);

    // Flush the buffer
    glFlush();
}

// OnClick funtion for upload file menu option
void MainWindow::on_actionUpload_File_triggered()
{
    // Import file with QFileDialog
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C://", tr("Text files (*.txt *.csv *.DATA)"));
    QFile dataFile(fileName);

    // Check if file was opened correctly
    if (!dataFile.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "title", "file not open");
    }

    //std::vector<std::vector<GLfloat>> allData(DATA_SIZE);
    std::vector<bool> classify{};	//  vector containing class of data points

    // Import data from csv file
    processData(&dataFile, &allData, &classify);

    // Normalize data
    normalizeData(&allData);

    // DEBUG STATEMENT

    for (const auto& number : allData[10]) {
        std::cout << number << " ";
    }

    // Close data file
    dataFile.close();
}


void MainWindow::on_actionSet_width_and_height_triggered()
{
    QDialog dialog(this); // Create a local instance of the dialog

    // Set up the dialog's layout and widgets
    QLabel widthLabel("Width:", &dialog);
    QLineEdit widthLineEdit(&dialog);
    QLabel heightLabel("Height:", &dialog);
    QLineEdit heightLineEdit(&dialog);
    QPushButton okButton("OK", &dialog);

    // Create a layout for organizing the widgets
    QFormLayout layout(&dialog);
    layout.addRow(&widthLabel, &widthLineEdit);
    layout.addRow(&heightLabel, &heightLineEdit);
    layout.addRow(&okButton);

    // Connect the OK button to accept the dialog
    QObject::connect(&okButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    // Show the dialog modally
    if (dialog.exec() == QDialog::Accepted) {
        // Retrieve and process the user's input
        QString widthText = widthLineEdit.text();
        QString heightText = heightLineEdit.text();

        // Convert the input to integers (you may want to add error handling here)
        NUM_COLUMNS = widthText.toInt();
        NUM_ROWS = heightText.toInt();

        // Now, NUM_ROWS and NUM_COLS hold the user's input
        qDebug() << "NUM_ROWS: " << NUM_ROWS;
        qDebug() << "NUM_COLS: " << NUM_COLUMNS;

        // You can use NUM_ROWS and NUM_COLS in your application as needed
    }
}

void MainWindow::on_verticalSlider_valueChanged(int value)
{

}


void MainWindow::on_openGLWidget_aboutToCompose()
{

}


//
void MainWindow::on_hyperblock_button_clicked()
{
    // Run Hyperblock Algorithm On allData
}

