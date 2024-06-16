#include "mainwindow.h"
#include "./ui_mainwindow.h"

int DATA_SIZE = 683;
//int NUM_ROWS = 4;
//int NUM_COLS = 4;

std::vector<std::vector<GLfloat>>::iterator it;
std::vector<std::vector<GLfloat>>::iterator hyperIt;

std::vector<std::vector<GLfloat>> allDataPreNormalize(DATA_SIZE);
extern std::vector<std::vector<GLfloat>> allData;
extern std::vector<bool> dataClass;

// Declare the global variable defined in oglutilities.cpp
extern std::vector<std::vector<GLfloat>> hyperblocks;

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
    // Initialize Graphics Tools
    // Construct glyph tool
    SpcSfGlyph glyph = SpcSfGlyph();
    // Construct turtle tool
    TurtleG* turt = new TurtleG();
    Point2* pos2 = new Point2();
    Point2* pos3 = new Point2();

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

                if (!hyperblocks.empty()) {
                    int repCount = 0;
                    bool DYNAMIC_ANGLES = false;
                    bool POS_ANGLE = false;
                    float GLYPH_SCALE_FACTOR = 0.25;
                    float SF_SEGMENT_CONSTANT = 0.5;
                    float SF_ANGLE_SCALE = 2.0;
                    bool ANGLE_FOCUS = true;
                    bool BIRD_FOCUS = true;

                    // Initialize data iterator
                    //std::vector<std::vector<GLfloat>>::iterator it = allData.begin();
                    std::vector<bool>::iterator classIt = dataClass.begin();
                    glPushMatrix();		// Scale and translate glyph

                    // Draw polygon outlines
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

                    // Pointer to current data point
                    std::vector<GLfloat> processedData = *hyperIt;

                    std::vector<GLfloat> axesSPC{};
                    axesSPC.push_back(*(processedData.begin() + 3));	// X1
                    axesSPC.push_back(*(processedData.begin() + 2));	// Y1
                    axesSPC.push_back(*(processedData.begin() + 3));	// X2
                    axesSPC.push_back(*(processedData.begin() + 4));	// Y2
                    axesSPC.push_back(*(processedData.begin() + 8));	// X3
                    axesSPC.push_back(*(processedData.begin() + 7));	// Y3

                    // Encode angles with most meaningful attributes
                    std::vector<GLfloat> stickFig{};
                    // Populate: CL (angle), UC (length), BN (angle), BC (length)
                    stickFig.push_back(*(processedData.begin()));			// Angle 1
                    stickFig.push_back(*(processedData.begin() + 1));	// Length 1
                    stickFig.push_back(*(processedData.begin() + 5));	// Angle 2
                    stickFig.push_back(*(processedData.begin() + 6));	// Length 2

                    float colors[6];
                    colors[0] = *(processedData.begin() + 2);
                    colors[1] = *(processedData.begin() + 1);
                    colors[2] = *(processedData.begin() + 0);
                    colors[3] = *(processedData.begin() + 2);
                    colors[4] = *(processedData.begin() + 7);
                    colors[5] = *(processedData.begin() + 6);

                    // If color values too high, round down so still visible
                    for (unsigned int i = 0; i < 6; i++){
                        if (colors[i] > 0.8){
                            colors[i] = colors[i] - 0.15;
                        }
                    }

                    glPushMatrix();		// Push new matrix
                    glMatrixMode(GL_PROJECTION);
                    glLineWidth(4.0);	// Line width = 4.0

                    GLfloat TRANS_DEF = 0.0;	// Default translation factor
                    GLfloat SCALE_DEF = 1.0;	// Default scaling factor
                    glPushMatrix();	// Push new matrix for glyph transformations
                    // Translate glyph right a based on first SPC shift, and up a set value
                    glScalef(1.7, 1.7, 1.7);
                    glTranslatef(-0.45, 0.35, 0.0);
                    //glTranslatef(glyphTransHort, glyphTransVert - 0.1, TRANS_DEF);
                    GLfloat glyphScaleFactor = 2.0;

                    glEnable(GL_BLEND);		// Enable blending.
                    glDepthMask(GL_FALSE);	// Disable depth masking

                    // *********************** DRAW STICK FIGURE ***********************
                    //A2
                    // Draw representative glyphs in color, Draw other glyphs in grey, if selected

                    glyph.drawGlyphSF(pos2, pos3, stickFig.begin(), *classIt, *turt, DYNAMIC_ANGLES, POS_ANGLE,
                                      GLYPH_SCALE_FACTOR, SF_SEGMENT_CONSTANT, SF_ANGLE_SCALE, ANGLE_FOCUS, BIRD_FOCUS, colors);

                    float AXIS_LENGTH = 0.1;
                    bool DOTTED_AXES = FALSE;

                    glLineWidth(2.0);	// Line width = 2.0
                    glyph.drawAxesSPC(*pos2, *pos3, axesSPC.begin(),
                                      AXIS_LENGTH, 0.25, DOTTED_AXES);

                    // Disable blending and resume depth mask
                    glDepthMask(GL_TRUE);
                    glDisable(GL_BLEND);
                    ++repCount;
                    ++hyperIt;
                    // RESET THE CP AND CD
                    turt->setCP(0.0, 0.0);

                    glPopMatrix();
                    glPopMatrix();
                    glPopMatrix();

                }
                else if (!allData[0].empty()) {
                    int repCount = 0;
                    bool DYNAMIC_ANGLES = false;
                    bool POS_ANGLE = false;
                    float GLYPH_SCALE_FACTOR = 0.25;
                    float SF_SEGMENT_CONSTANT = 0.5;
                    float SF_ANGLE_SCALE = 2.0;
                    bool ANGLE_FOCUS = true;
                    bool BIRD_FOCUS = true;

                    // Initialize data iterator
                    //std::vector<std::vector<GLfloat>>::iterator it = allData.begin();
                    std::vector<bool>::iterator classIt = dataClass.begin();
                    glPushMatrix();		// Scale and translate glyph

                    // Draw polygon outlines
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

                    // Pointer to current data point
                    std::vector<GLfloat> processedData = *it;

                    std::vector<GLfloat> axesSPC{};
                    axesSPC.push_back(*(processedData.begin() + 3));	// X1
                    axesSPC.push_back(*(processedData.begin() + 2));	// Y1
                    axesSPC.push_back(*(processedData.begin() + 3));	// X2
                    axesSPC.push_back(*(processedData.begin() + 4));	// Y2
                    axesSPC.push_back(*(processedData.begin() + 8));	// X3
                    axesSPC.push_back(*(processedData.begin() + 7));	// Y3

                    // Encode angles with most meaningful attributes
                    std::vector<GLfloat> stickFig{};
                    // Populate: CL (angle), UC (length), BN (angle), BC (length)
                    stickFig.push_back(*(processedData.begin()));			// Angle 1
                    stickFig.push_back(*(processedData.begin() + 1));	// Length 1
                    stickFig.push_back(*(processedData.begin() + 5));	// Angle 2
                    stickFig.push_back(*(processedData.begin() + 6));	// Length 2

                    float colors[6];
                    colors[0] = *(processedData.begin() + 2);
                    colors[1] = *(processedData.begin() + 1);
                    colors[2] = *(processedData.begin() + 0);
                    colors[3] = *(processedData.begin() + 2);
                    colors[4] = *(processedData.begin() + 7);
                    colors[5] = *(processedData.begin() + 6);

                    // If color values too high, round down so still visible
                    for (unsigned int i = 0; i < 6; i++){
                        if (colors[i] > 0.8){
                            colors[i] = colors[i] - 0.15;
                        }
                    }

                    glPushMatrix();		// Push new matrix
                    glMatrixMode(GL_PROJECTION);
                    glLineWidth(4.0);	// Line width = 4.0

                    GLfloat TRANS_DEF = 0.0;	// Default translation factor
                    GLfloat SCALE_DEF = 1.0;	// Default scaling factor
                    glPushMatrix();	// Push new matrix for glyph transformations
                    // Translate glyph right a based on first SPC shift, and up a set value
                    glScalef(1.7, 1.7, 1.7);
                    glTranslatef(-0.45, 0.35, 0.0);
                    //glTranslatef(glyphTransHort, glyphTransVert - 0.1, TRANS_DEF);
                    GLfloat glyphScaleFactor = 2.0;

                    glEnable(GL_BLEND);		// Enable blending.
                    glDepthMask(GL_FALSE);	// Disable depth masking

                    // *********************** DRAW STICK FIGURE ***********************
                    // Draw representative glyphs in color, Draw other glyphs in grey, if selected

                    glyph.drawGlyphSF(pos2, pos3, stickFig.begin(), *classIt, *turt, DYNAMIC_ANGLES, POS_ANGLE,
                                      GLYPH_SCALE_FACTOR, SF_SEGMENT_CONSTANT, SF_ANGLE_SCALE, ANGLE_FOCUS, BIRD_FOCUS, colors);

                    float AXIS_LENGTH = 0.1;
                    bool DOTTED_AXES = FALSE;

                    glLineWidth(2.0);	// Line width = 2.0
                    glyph.drawAxesSPC(*pos2, *pos3, axesSPC.begin(),
                                      AXIS_LENGTH, 0.25, DOTTED_AXES);

                    // Disable blending and resume depth mask
                    glDepthMask(GL_TRUE);
                    glDisable(GL_BLEND);
                    ++repCount;
                    ++it;
                    // RESET THE CP AND CD
                    turt->setCP(0.0, 0.0);

                    glPopMatrix();
                    glPopMatrix();
                    glPopMatrix();

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

    // Clean up memory
    delete turt;
    delete pos2;
    delete pos3;

    // Flush the buffer
    glFlush();
}

std::string vectorToString(const std::vector<std::vector<float>>& vec) {
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        ss << "[";
        for (size_t j = 0; j < vec[i].size(); ++j) {
            ss << vec[i][j];
            if (j != vec[i].size() - 1) {
                ss << ", ";
            }
        }
        ss << "]";
        if (i != vec.size() - 1) {
            ss << ", ";
        }
    }
    ss << "]";
    return ss.str();
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
    //std::vector<bool> classify{};	//  vector containing class of data points

    // Import data from csv file
    processData(&dataFile, &allData, &dataClass);

    allDataPreNormalize = allData;

    // Normalize data
    normalizeData(&allData);
    it = allData.begin();
    //std::cout << "\n" << vectorToString(allData) << "\n";

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

void MainWindow::on_hyperblock_button_clicked()
{
    // Run Hyperblock Algorithm On allData
    mergerHyperblock(&allDataPreNormalize, &dataClass);
    // Debug statement

    // Normalize data
    normalizeData(&hyperblocks);

    hyperIt = hyperblocks.begin();


    std::cout << "\n" << hyperblocks.size() <<"\n";
    std::cout << "\nHello there\n";
}
