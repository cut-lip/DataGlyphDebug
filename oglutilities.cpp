#include "oglutilities.h"

//int DATA_SIZE = 683;
unsigned int NUM_ROWS = 4;
unsigned int NUM_COLUMNS = 4;
float THRESHOLD_VALUE = 5.0;			/* threshold calculation value */
float MIN_THRESHOLD =2.0;

int VIEWPORT_SCALE = 8;
const unsigned int HEIGHT_SCALE = 10;	/* height scaling constant */
const unsigned int WIDTH_SCALE = 4;		/* width scaling constant */
bool DYNAMIC_ANGLES = false;
bool DOTTED_AXES = false;
float GLYPH_SCALE_FACTOR = 0.25;
bool ANGLE_FOCUS = true;
bool BIRD_FOCUS = true;
bool DRAW_AXES = false;
float AXIS_LENGTH = 1.0;
bool POS_ANGLE = false;

bool HYPERBLOCKS_COLLECTED;
std::vector< std::vector<GLfloat>> hyperblocks{};
std::vector<std::string> hyperblockLabels{};

std::vector<std::vector<GLfloat>> allData(683);
std::vector<bool> dataClass(683);

extern std::vector<std::vector<GLfloat>> allData;

/*
isClose
This function is a helper function for the isClose function.
It is used to determine whether two given data points are within
the given threshold of each other, and thus are in the same
neighborhood.
@param		vec1		the first data point to be compared
            vec2		the second data point to be compared
@return					boolean value, is vec2 within threshold
                        of vec1
*/
bool isClose(std::vector<float>* vec1, std::vector<float>* vec2)
{
    // modified HyClu hypercube clustering algorithm. algorithm now forms Hyperblocks.
    // Threshold value for attributes used in SPC shifts is MIN_THRESHOLD
    // Threshold value for attributes allowed to expand is THRESHOLD

    // Initialize iterators for vectors to be compared
    std::vector<float>::iterator it1 = vec1->begin();
    std::vector<float>::iterator it2 = vec2->begin();

    int count = 0;
    for (it1; it1 != vec1->end(); ++it1)
    {
        if (count == 0 || count == 1 || count == 2 || count == 5 || count == 6)
        {
            if (abs(*it1 - *it2) > MIN_THRESHOLD)
            {
                return false;
            }
        }
        else if (count == 3 || count == 4 || count == 7 || count == 8)
        {
            if (abs(*it1 - *it2) > THRESHOLD_VALUE)
            {
                return false;
            }
        }

        ++count;
        ++it2;
    }
    // Return true if all attributes are within threshold
    return true;
}

std::vector<bool> computeAllDistances(std::vector<GLfloat>* curr, std::vector<std::vector<GLfloat>>* data)
{
    // This variable is for temp extracted data to compare
    //std::vector<float> data0();
    std::vector<bool> close{};

    // Populate boolean vector to determine
    // which data points are within threshold of current point
    for (std::vector<std::vector<GLfloat>>::iterator iter = data->begin(); iter != data->end(); ++iter)
    {
        close.push_back(isClose(curr, &(*iter)));
    }

    // Close file streams
    //output.close();

    return close;
}

void mergerHyperblock(std::vector<std::vector<GLfloat>>* all_Data, std::vector<bool>* dataClass)
{

    // Initialize new vector copy of data
    std::vector<std::vector<GLfloat>> allData(*all_Data);
    std::vector<bool> dataClassCopy(*dataClass);
    std::vector<bool> thresholds{};	// track hyperblock membership

    // Debug
    std::cout << "\nallData size = " << allData.size();

    int classACount = 0;
    int classBCount = 0;

    while (allData.size() != 0)
    {
        // Compute points within threshold
        thresholds = computeAllDistances(&allData[0], &allData);

        std::vector< std::vector<GLfloat>> tempData{};		// temp vec for average glyph

        // Initialize vector iterators
        std::vector<std::vector<GLfloat>>::iterator dataIt = allData.begin();
        std::vector<bool>::iterator classIt = dataClassCopy.begin();
        std::vector<bool>::iterator threshIt = thresholds.begin();

        // Use threshold values to remove points
        while (dataIt != allData.end())
        {
            // If current point is in current neighborhood
            if (*threshIt)
            {
                // Add class to class count
                if (*classIt)
                {
                    ++classACount;
                }
                else if (!*classIt)
                {
                    ++classBCount;
                }

                // Add point to temp vector
                tempData.push_back(*dataIt);

                // Remove current point from vectors
                dataIt = allData.erase(dataIt);
                classIt = dataClassCopy.erase(classIt);
                threshIt = thresholds.erase(threshIt);
                std::cout << "\nHello";
            }
            else
            {   // Increment iterators
                ++dataIt;
                ++classIt;
                ++threshIt;
            }

            if (dataIt == allData.end())
            {	// Exit loop
                break;
            }
        }

        hyperblockLabels.push_back(std::to_string(classACount) + " Class A, " +
                                std::to_string(classBCount) + " Class B");

        // Initialize new rep vector
        std::vector<GLfloat> tempVec{};

        /* Compute average glyph of neighborhood by taking tempData,
        *  the set of vectors saved during clustering, and computing
        *  the average value for each attribute from the data points
        *  in the set.
        */
            // CHECK INDEX VARIABLE RELEVANCY!!
        for (unsigned int index = 0; index < 9; ++index)
        {	// For the current index of each vector
            GLfloat tempAttr = 0.0;
            for (auto& vec : tempData)
            {	// For each vector
                tempAttr += vec[index];
            }
            // Compute average value from sum
            tempAttr /= tempData.size();
            // Add to representative point
            tempVec.push_back(tempAttr);
        }

        // Add representative vector to set
        hyperblocks.push_back(tempVec);

        // Reset class counters
        classACount = 0;
        classBCount = 0;
    }

    // Debug
    std::cout << "\nhyperblocks size = " << hyperblocks.size();
        std::cout << "\nhyperblockLabels size = " << hyperblockLabels.size();

    HYPERBLOCKS_COLLECTED = true;	// set collected flag
}

/* drawGridSPC - draw a grid for each SPC axis
*  @params -	originX - x coord of plane origin
*				originY - y coord of plane origin
*				endX	- max x value in plane
*				endY	- max y values in plane
*				dimension - dimensions of grid (dimension x dimension)
* */
void drawGridSPC(GLfloat originX, GLfloat originY, GLfloat endX, GLfloat endY, unsigned int dimension)
{
    // ************** DRAW SPC GRID (dimension x dimension) ****************
    glPushMatrix();
    glColor4f(0.0, 0.0, 0.0, 1.0);				// Draw in black
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Draw outlines

    glLineWidth(1.0);		// Set line width to 1
    glPushMatrix();			// Push new matrix

    GLfloat cellWidth = (endX - originX) / dimension;
    GLfloat cellHeight = (endY - originY) / dimension;

    for (unsigned int rowNum = 0; rowNum < dimension; ++rowNum)		// Rows
    {
        for (unsigned int colNum = 0; colNum < dimension; ++colNum)	// Columns
        {
            glRectf(		// Draw outlined rectangles for SPC grid
                originX + cellWidth * colNum,
                originY + cellHeight * rowNum,
                originX + cellWidth * (colNum + 1),
                originY + cellHeight * (rowNum + 1)
                );
        }
    }
    // Reset OpenGL state
    glPopMatrix();		// Pop a matrix from gl matrix stack
    glPopMatrix();		// Pop a matrix from gl matrix stack
    glLineWidth(2.0);		// Set line width to 2
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// Fill shapes
}

// IMPORT DATA
// EDITING CODE TO add class label to data vector at 10th index
int processData(QFile* dataFile, std::vector<std::vector<GLfloat>>* allData, std::vector<bool>* dataClass)
{
    // Read data file
    std::string line = "";

    //std::ifstream myFile("C:\\Users\\cutli\\source\\repos\\AdvancedPracOpenGL\\AdvancedPracOpenGL\\breast-cancer-wisconsin.txt");

    QTextStream in(dataFile);

    int count = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();

        //Unused variable
        //int vecCount = 10;

        // Convert QString to string
        std::string lineString = line.toLocal8Bit().constData();

        // Split string into a vector
        std::vector<int> dataInt;
        std::stringstream ss(lineString);

        //process_line(line);
        while (ss.good()) {		// Replace "?" in data
            std::string substr = "";
            getline(ss, substr, ',');

            if (substr == "?") substr = std::to_string(*dataInt.begin());
            dataInt.push_back(stoi(substr));
            continue;
        }

        std::vector<GLfloat> data(dataInt.begin(), dataInt.end());

        // Save class to dataClass vector
        if (*--(data.end()) == 4) dataClass->push_back(false);
        else					  dataClass->push_back(true);

        // Remove labels from data
        data.erase(data.begin());
        data.erase(--data.end());

        (*allData)[count] = data;
        ++count;
    }

    return 0;
}

int normalizeData(std::vector<std::vector<GLfloat>> *allData)
{
    // Normalize data
    int index = 0;
    for (auto& vec : *allData)
    {
        const unsigned int scaleFactor = 10;
        std::vector<float> normalData;		// Normalize data to [0, 1]
        for (std::vector<float>::iterator iter = vec.begin(); iter < vec.end(); iter++)
        {
            normalData.push_back(*iter / scaleFactor);
        }
        (*allData)[index] = normalData;
        ++index;
    }

    return 0;
}

// Draw tile grid, set to size determined by
// NUM_ROWS and NUM_COLUMNS
void drawGrid(float SCREEN_WIDTH, float SCREEN_HEIGHT)
{
    int GRID_MARGIN = 0.2;

    glClearColor(1.0, 1.0, 1.0, 0.0);	// Clear window to white
    glClearDepth(1.0f);					// Clear depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);	// Load GL_PROJECTION matrix
    glLoadIdentity();				// Load identity matrix
    glPushMatrix();					// Push new matrix

    // Set 1-to-1 mapping between viewport coordinates and final rendering
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, -1, 1);

    // ************** DRAW TILE FRAME (NUM_ROWS x NUM_COLUMNS) ****************
    glColor4f(0.0, 0.0, 0.0, 1.0);				// Draw in black
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Draw outlines

    // Draw the tile frame to contain the glyphs
    glLineWidth(2.0);		// Set line width to 2
    glPushMatrix();			// Push new matrix
    for (unsigned int rowNum = 0; rowNum < NUM_ROWS; ++rowNum)			// Rows
    {
        for (unsigned int colNum = 0; colNum < NUM_COLUMNS; ++colNum)	// Columns
        {
            glRectf(		// Draw rectangles for grid of glyphs
                0.0 + GRID_MARGIN,
                (SCREEN_HEIGHT - (SCREEN_HEIGHT / NUM_ROWS) * rowNum) - 1.0,
                ((SCREEN_WIDTH / NUM_COLUMNS) * (colNum + 1)) + GRID_MARGIN,
                (SCREEN_HEIGHT - (SCREEN_HEIGHT / NUM_ROWS) * (rowNum + 1)) - 1.0
                );
        }
    }
    glPopMatrix();		// Pop a matrix from gl matrix stack
    glPopMatrix();		// Pop a matrix from gl matrix stack
}

void drawArrow(Point2 start, Point2 end, const unsigned int winHeight)
{
    // Draw Line Segment
    glBegin(GL_LINE_STRIP);
    glVertex2i(start.getx(), start.gety());
    glVertex2i(end.getx(), end.gety());
    glEnd();

    // Draw trangle for arrow tip
    glBegin(GL_TRIANGLES);

    Vec2 p1 = { float(start.getx()), float(start.gety()) };
    Vec2 p2 = { float(end.getx()), float(end.gety()) };

    Vec2 direction = p2 - p1;
    direction.Normalize();

    Vec2 perpendicular = Vec2(direction);
    perpendicular.Perpendicu();

    Vec2 pt1 = p2 - direction * (winHeight / 80) * 3;
    Vec2 pt2 = pt1 - perpendicular * (winHeight / 80);
    Vec2 pt3 = pt1 + perpendicular * (winHeight / 80);

    glVertex2i(p2.getx(), p2.gety());
    glVertex2i(pt2.getx(), pt2.gety());
    glVertex2i(pt3.getx(), pt3.gety());

    glEnd();
}


void drawLocatedGLyphs(std::vector<GLfloat>* normalData, int width, int height) {

    // encode colors to bird glyph wings in located glyphs
    float colors[6];
    colors[0] = *(normalData->begin() + 4);
    colors[1] = *(normalData->begin() + 5);
    colors[2] = *(normalData->begin() + 6);
    colors[3] = *(normalData->begin() + 2);
    colors[4] = *(normalData->begin() + 7);
    colors[5] = *(normalData->begin() + 5);
    // If color values too high, round down so still visible
    for (unsigned int i = 0; i < 6; i++)
    {
        if (colors[i] > 0.8)
        {
            colors[i] = colors[i] - 0.1;
        }
    }
    GLfloat ucsize = *(normalData->begin() + 1);        // Encode custom attributes to SPC positions
    GLfloat ucshape = *(normalData->begin() + 2);       // Uniformity of cell shape
    GLfloat cl = *(normalData->begin());                // Clump thickness
    GLfloat bn = *(normalData->begin() + 5);            // Bland chromatin
    GLfloat bc = *(normalData->begin() + 6);            // Marginal adhesion
    std::vector<GLfloat> axesSPC{};
    axesSPC.push_back(*((normalData)->begin() + 3));	// X1
    axesSPC.push_back(*((normalData)->begin() + 2));	// Y1
    axesSPC.push_back(*((normalData)->begin() + 3));	// X2
    axesSPC.push_back(*((normalData)->begin() + 4));	// Y2
    axesSPC.push_back(*((normalData)->begin() + 8));	// X3
    axesSPC.push_back(*((normalData)->begin() + 7));	// Y3
    // Encode angles with most meaningful attributes
    std::vector<GLfloat> stickFig{};
    // Populate: CL (angle), UC (length), BN (angle), BC (length)
    stickFig.push_back(*((normalData)->begin()));			// Angle 1
    stickFig.push_back(*((normalData)->begin() + 1));	// Length 1
    stickFig.push_back(*((normalData)->begin() + 2));	// Angle 2
    stickFig.push_back(*((normalData)->begin() + 3));	// Length 2
    // Arrange greater SPC position attributes from optimal positioning described in Worland, Wagle, and Kovalerchuk
    std::vector<GLfloat> position{ cl, bn, ucsize, bc, bn, ucshape };
    //std::vector<GLfloat> position{ uc, bn, bc, cl, bn, mg };
    std::vector<GLfloat>::iterator positionIt = position.begin();

    // Construct glyph tool
    SpcSfGlyph glyph = SpcSfGlyph();
    // Construct turtle tool
    TurtleG* turt = new TurtleG();
    Point2* pos2 = new Point2();
    Point2* pos3 = new Point2();

    glPushMatrix();

        // DANGER DANGER!!!!!!!!
    // Locate the lower-left corner of viewport for glyph drawing
    GLfloat x1 = ((width / WIDTH_SCALE) * *positionIt) - ((GLint)width / (VIEWPORT_SCALE * 2));
    GLfloat y1 = ((height - (height / HEIGHT_SCALE)) * *++positionIt) - ((GLint)width / (VIEWPORT_SCALE * 2));

    // Locate the viewport for glyph drawing
    glViewport(
        x1,
        y1,
        width / VIEWPORT_SCALE,
        width / VIEWPORT_SCALE
        );


    //Glyph1
    glLineWidth(4.0);

    glyph.drawGlyphSF(pos2, pos3, stickFig.begin(), true, *turt, DYNAMIC_ANGLES, POS_ANGLE, GLYPH_SCALE_FACTOR, 5, 2.0, ANGLE_FOCUS, BIRD_FOCUS, colors);
    // RESET THE CP AND CD
    turt->setCP(0.0, 0.0);
    // *********************** DRAW SPC AXES ***********************
    if (DRAW_AXES)
    {
        glLineWidth(2.0);
        glyph.drawAxesSPC(*pos2, *pos3, axesSPC.begin(), AXIS_LENGTH, GLYPH_SCALE_FACTOR, DOTTED_AXES);
    }

    // Locate the viewport for glyph drawing
    GLfloat x2 = ((width / WIDTH_SCALE) * *++positionIt) + (width / 3) - ((GLint)width / (VIEWPORT_SCALE * 2));
    GLfloat y2 = ((height - (height / HEIGHT_SCALE)) * *++positionIt) - ((GLint)width / (VIEWPORT_SCALE * 2));

    // Locate the viewport for glyph drawing
    glViewport(
        x2,
        y2,
        width / VIEWPORT_SCALE,
        width / VIEWPORT_SCALE
        );

    //Glyph2
    glLineWidth(4.0);
    glyph.drawGlyphSF(pos2, pos3, stickFig.begin(), true, *turt, DYNAMIC_ANGLES, POS_ANGLE, 0.25, 5, 2.0, ANGLE_FOCUS, BIRD_FOCUS, colors);
    // RESET THE CP AND CD
    turt->setCP(0.0, 0.0);

    if (DRAW_AXES) {
        glLineWidth(2.0);
        glyph.drawAxesSPC(*pos2, *pos3, axesSPC.begin(), AXIS_LENGTH, 0.25, DOTTED_AXES);
    }

    // Locate the viewport for glyph drawing
    GLfloat x3 = ((width / WIDTH_SCALE) * *++positionIt) + (((2 * width) / 3)) - ((GLint)width / (VIEWPORT_SCALE * 2));
    GLfloat y3 = ((height - (height / HEIGHT_SCALE)) * *++positionIt) - ((GLint)width / (VIEWPORT_SCALE * 2));

    // Locate the viewport for glyph drawing
    glViewport(
        x3,
        y3,
        width / VIEWPORT_SCALE,
        width / VIEWPORT_SCALE
        );


    //Glyph3
    glLineWidth(4.0);
    glyph.drawGlyphSF(pos2, pos3, stickFig.begin(), true, *turt, DYNAMIC_ANGLES, POS_ANGLE, 0.25, 5, 2.0, ANGLE_FOCUS, BIRD_FOCUS, colors);
    // RESET THE CP AND CD
    turt->setCP(0.0, 0.0);
    // *********************** DRAW SPC AXES ***********************
    if (DRAW_AXES)
    {
        glLineWidth(2.0);
        glyph.drawAxesSPC(*pos2, *pos3, axesSPC.begin(), AXIS_LENGTH, 0.25, DOTTED_AXES);
    }
    glPopMatrix();
    // Set the viewport to the entire widget
    //glViewport(0, 0, width, height);
    glColor3f(0.7, 0.0, 0.7);
    glLineWidth(3.0);
    glPushMatrix();	// Push new modelview matrix for translation
    //glTranslatef(-((GLint)width / (VIEWPORT_SCALE * 2)), -((GLint)width / (VIEWPORT_SCALE * 2)), 0);

    // Draw first edge
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();

    // Draw a point at the node between connections
    glPointSize(6.0);
    glBegin(GL_POINTS);
    glVertex2f(x2 + (width / VIEWPORT_SCALE), y2 + (width / VIEWPORT_SCALE));
    glEnd();

    // Draw second edge
    drawArrow(Point2(x2 + (width / VIEWPORT_SCALE), y2 + (width / VIEWPORT_SCALE)),
              Point2(x3 + (width / VIEWPORT_SCALE), y3 + (width / VIEWPORT_SCALE)), (2 * (height / 3)));

    glPopMatrix(); // Pop modelview matrix used for translation
}
