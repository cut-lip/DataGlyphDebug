#ifndef OGLUTILITIES_H
#define OGLUTILITIES_H

#include <GL/glu.h>
#include <vector>
#include <string>
#include <iostream>		/*  */
#include <sstream>		/*  */
#include <fstream>		/*  */
#include <iomanip>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "point2.h"
#include "vec2.h"
#include "SPC_SF.h"

// Add namespace

// Import data from CVS file
int processData(QFile* dataFile, std::vector<std::vector<GLfloat>>* allData, std::vector<bool>* classify);

int normalizeData(std::vector<std::vector<GLfloat>>* data);

// Draw a  4x5 tile grid
void drawGrid(float width, float height);

void drawArrow(Point2 start, Point2 end, const unsigned int winHeight);

void drawGridSPC(GLfloat originX, GLfloat originY, GLfloat endX, GLfloat endY, unsigned int dimension);

void mergerHyperblock(std::vector<std::vector<GLfloat>>* all_Data, std::vector<bool> *classify);

std::vector<bool> computeAllDistances(std::vector<GLfloat>* curr, std::vector<std::vector<GLfloat>>* data);

bool isClose(std::vector<float>* vec1, std::vector<float>* vec2);

void drawLocatedGLyphs(std::vector<GLfloat>* normalData, int width, int height);

#endif // OGLUTILITIES_H
