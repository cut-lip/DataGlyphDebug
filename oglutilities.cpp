#include "oglutilities.h"

//int DATA_SIZE = 683;
int NUM_ROWS = 4;
int NUM_COLUMNS = 4;

bool HYPERBLOCKS_COLLECTED;
std::vector< std::vector<GLfloat>> hyperblocks{};
std::vector<std::string> hyperblockLabels{};

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

void mergerHyperblock(std::vector<std::vector<GLfloat>>* all_Data, std::vector<bool>* classify)
{
    // Initialize new vector copy of data
    std::vector<std::vector<GLfloat>> allData(*all_Data);
    std::vector<bool> classifyCopy(*classify);
    std::vector<bool> thresholds{};	// track hyperblock membership

    bool addThis = false;   // Initialize flag & class counters
    int classACount = 0;
    int classBCount = 0;

    int count = 0;
    while (allData.size() != 0)
    {
        // Compute points within threshold
        thresholds = computeAllDistances(&allData[0], &allData);

        std::vector< std::vector<GLfloat>> tempData{};		// temp vec for average glyph
        std::vector< std::vector<GLfloat>> tempDataClassA{};
        std::vector< std::vector<GLfloat>> tempDataClassB{};

        // Initialize vector iterators
        std::vector<std::vector<GLfloat>>::iterator dataIt = allData.begin();
        std::vector<bool>::iterator classIt = classifyCopy.begin();
        std::vector<bool>::iterator threshIt = thresholds.begin();

        // Save first point in cluster for comparison
        std::vector<float> currPoint = (*dataIt);

        addThis = false;	// reset add curr point flag
        // Use threshold values to remove points
        while (dataIt != allData.end())
        {
            // If current point is in current neighborhood
            if (*threshIt)
            {
                // Add class to class count
                if (*classIt)
                {
                    ++ClassACount;
                    tempDataClassA.push_back(*dataIt);
                }
                else if (!*classIt)
                {
                    ++classBCount;
                    tempDataClassB.pushBack(*dataIt);
                }

                // Add point to temp vector
                tempData.push_back(*dataIt);

                // Remove current point from vectors
                dataIt = allData.erase(dataIt);
                classIt = classifyCopy.erase(classIt);
                threshIt = thresholds.erase(threshIt);
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

        ++count;
        hyperblockLabels.push_back(std::to_string(classACount) + " Class A, " +
                                std::to_string(classBCount) + " Class B");

        // Initialize new rep vector
        std::vector<GLfloat> tempVec{};

        /* Compute average glyph of neighborhood by taking tempData,
        *  the set of vectors saved during clustering, and computing
        *  the average value for each attribute from the data points
        *  in the set.
        */
        if (classACount > classBCount)
        {
            // CHECK INDEX VARIABLE RELEVANCY!!
            for (unsigned int index = 0; index < 10; ++index)
            {	// For the current index of each vector
                GLfloat tempAttr = 0.0;
                for (auto& vec : tempDataClassA)
                {	// For each vector
                    tempAttr += vec[index];
                }
                // Compute average value from sum
                tempAttr /= tempDataClassA.size();
                // Add to representative point
                tempVec.push_back(tempAttr);
            }
        }
        else // class B count is larger than class A count
        {
            for (unsigned int index = 0; index < 10; ++index)
            {	// For the current index of each vector
                GLfloat tempAttr = 0.0;
                for (auto& vec : tempDataClassB)
                {	// For each vector
                    tempAttr += vec[index];
                }
                // Compute average value from sum
                tempAttr /= tempDataClassB.size();
                // Add to representative point
                tempVec.push_back(tempAttr);
            }
        }

        // Add representative vector to set
        hyperblocks.push_back(tempVec);

        // Reset class counters
        classACount = 0;
        classBCount = 0;
    }

    HYPERBLOCKS_COLLECTED = true;	// set collected flag
}

/* drawGridSPC - draw a grid for each SPC axis
*  @params -	originX - x coord of plane origin
*				originY - y coord of plane origin
*				endX	- max x value in plane
*				endY	- max y values in plane
*				dimension - dimensions of grid (dimension x dimension)
* */
void drawGridSPC(GLfloat originX, GLfloat originY, GLfloat endX, GLfloat endY, int dimension)
{
    // ************** DRAW SPC GRID (dimension x dimension) ****************
    glPushMatrix();
    glColor4f(0.0, 0.0, 0.0, 1.0);				// Draw in black
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Draw outlines

    glLineWidth(1.0);		// Set line width to 1
    glPushMatrix();			// Push new matrix
    for (unsigned int rowNum = 0; rowNum < dimension; ++rowNum)		// Rows
    {
        for (unsigned int colNum = 0; colNum < dimension; ++colNum)	// Columns
        {
            glRectf(		// Draw outlined rectangles for SPC grid
                originX + ((endX / dimension) * colNum),
                originY + ((endY / dimension) * rowNum),
                ((endX) / dimension) * (colNum + 1),
                ((endY) / dimension) * (rowNum + 1)
                );
        }
    }
    // Reset OpenGL state
    glLineWidth(2.0);		// Set line width to 2
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// Fill shapes
    glPopMatrix();		// Pop a matrix from gl matrix stack
    glPopMatrix();		// Pop a matrix from gl matrix stack
}

// IMPORT DATA
// EDITING CODE TO add class label to data vector at 10th index
int processData(QFile* dataFile, std::vector<std::vector<GLfloat>>* allData, std::vector<bool>* classify)
{
    // Read data file
    std::string line = "";

    //std::ifstream myFile("C:\\Users\\cutli\\source\\repos\\AdvancedPracOpenGL\\AdvancedPracOpenGL\\breast-cancer-wisconsin.txt");

    QTextStream in(dataFile);

    int count = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        int vecCount = 10;

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

        // Remove labels from data
        data.erase(data.begin());

        // Save Class Label Into Classify Vector
        // 2 for positive, 4 for negative
        // This must be standardized in a way which we didn't do before
        data.erase(--data.end());

        (*allData)[count] = data;
        ++count;
    }
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

/*
void drawGlyphs(std::vector<std::vector<GLfloat>> allData, int SCREEN_HEIGHT, int SCREEN_WIDTH)
{
    // Organization of Data for Glyph (10-D, CL replicated) =
    //	{ SPC:[ (UC, CL), (BC, CL), (MG, BN) ] SF:[ UCShape, SE, NN, M ] }
    // Construct glyph tool
    SpcSfGlyph glyph = SpcSfGlyph();
    // Construct turtle tool
    TurtleG* turt = new TurtleG();
    Point2* pos2 = new Point2();
    Point2* pos3 = new Point2();

    // Initialize data iterator
    std::vector<std::vector<GLfloat>>::iterator it = allData.begin();

    glPushMatrix();		// Scale and translate glyph

    unsigned int rowNum = 0;	// Current Row index
    unsigned int colNum = 0;	// Current Column index

    while (rowNum < NUM_ROWS)		// Grid Rows loop
    {
        while (colNum < NUM_COLUMNS)	// Grid Columns loop
        {
            // If within threshold of current point,
            // display glyph at current grid index
            if (it != allData.end())
            {
                // Draw polygon outlines
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

                // Pointer to current data point
                std::vector<GLfloat> processedData = *it;

                // Encode angles with most meaningful attributes
                std::vector<GLfloat> stickFig{};
                // Populate: CL (angle), UC (length), BN (angle), BC (length)
                stickFig.push_back(*processedData.begin());			// Angle 1
                stickFig.push_back(*(processedData.begin() + 1));	// Length 1
                stickFig.push_back(*(processedData.begin() + 5));	// Angle 2
                stickFig.push_back(*(processedData.begin() + 6));	// Length 2

                std::vector<GLfloat> axesSPC{};

                // Populate: UCsh, MA, SIN, MA, NN, MIT
                // (MA, UCsh) (MA, SIN) (MIT, NN)
                axesSPC.push_back(*(processedData.begin() + 3));	// X1
                axesSPC.push_back(*(processedData.begin() + 2));	// Y1
                axesSPC.push_back(*(processedData.begin() + 3));	// X2
                axesSPC.push_back(*(processedData.begin() + 4));	// Y2
                axesSPC.push_back(*(processedData.begin() + 8));	// X3
                axesSPC.push_back(*(processedData.begin() + 7));	// Y3

                GLfloat maxAtr = 0.0;	// Initialize max attribute variable
                // Check for max shift
                for (std::vector<GLfloat>::iterator maxValIt = (*it).begin();
                     maxValIt < (*it).begin() + 6; ++maxValIt)
                {
                    maxAtr = std::max(maxAtr, *maxValIt);
                }

                glViewport(		// (rowNum x colNum)
                    // Encode shift based off of first SPC axis horizontal/vertical shift.
                    // Shift calculation:
                    // the LARGER the value, the SMALLER the shift
                    // maybe just subtract attribute value from (SCREEN_WIDTH / NUM_COLUMNS) / 2)
                    ((SCREEN_WIDTH / NUM_COLUMNS) * (colNum)),// - (maxAtr * ((SCREEN_WIDTH / NUM_COLUMNS) / 2)),
                    ((SCREEN_HEIGHT / NUM_ROWS) * (rowNum)),// - (maxAtr * ((SCREEN_HEIGHT / NUM_ROWS) / 2)),
                    (SCREEN_WIDTH / NUM_COLUMNS),// * (1.0 + (maxAtr / 2)),
                    (SCREEN_HEIGHT / NUM_ROWS)// * (1.0 + (maxAtr / 2))
                    );

                glPushMatrix();		// Push new matrix
                glMatrixMode(GL_PROJECTION);
                glLineWidth(4.0);	// Line width = 4.0
                // Translate glyph based on value of first SPC x-coordinate
                glTranslatef(-(1.0 - *(processedData.begin() + 3)), 0.0, 0.0);

                float colors[6];
                // *********************** DRAW STICK FIGURE ***********************
                glyph.drawGlyphSF(pos2, pos3, stickFig.begin(), true, *turt, true, true,
                                  GLYPH_SCALE_FACTOR, SF_SEGMENT_CONSTANT, SF_ANGLE_SCALE, true, false, colors);

                // RESET THE CP AND CD
                turt->setCP(0.0, 0.0);

                // *********************** DRAW SPC AXES ***********************
                if (false)
                {
                    glLineWidth(2.0);	// Line width = 2.0
                    glyph.drawAxesSPC(*pos2, *pos3, axesSPC.begin(),
                                      1, 0.25, false);
                }

                glPopMatrix();	// Pop from gl matrix stack
                ++colNum;		// Increment column index
            }

            if (it == allData.end())
            {	// If at end of data, set loop exit conditions
                colNum = NUM_COLUMNS;
                rowNum = NUM_ROWS;
            }
            else
            {	// Otherwise increment iterators
                ++it;
            }
        }

        // If exit conditions have not been met
        if (rowNum != NUM_ROWS)
        {
            colNum = 0;		// Reset column index
            ++rowNum;		// Increment row index
        }
    }

    // Draw filled polygons
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Return to original modelview matrix
    glPopMatrix();
}
*/

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
