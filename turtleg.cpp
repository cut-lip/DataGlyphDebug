// Definitions for turtleg object.
// turtleg: Pen for drawing turtle graphics

#include "turtleg.h"

/*
    * TurtleG
    * Constructor. Construct new TurtleG object
    * with current position (CP)
    * set to (0.0, 0.0)
    *
    * @param	x		x coordinate of CP
    *			y		y coordinate of CP
    *			angle	initial direction of CD
    */
TurtleG::TurtleG()
{
    this->CP = Point2(0.0, 0.0);
    this->CD = 0.0;
}

/*
    * TurtleG
    * Constructor. Construct new TurtleG object
    * with specified current position (CP) and
    * current direction (CD).
    *
    * @param	x		x coordinate of CP
    *			y		y coordinate of CP
    *			angle	initial direction of CD
    */
TurtleG::TurtleG(GLfloat x, GLfloat y, GLfloat angle)
{
    this->CP = Point2(x, y);
    this->CD = angle;
}

/*
    * turnTo
    * Turn the turtle to given angle from horizontal.
    * In SPC-SF context, can be considered static angles.
    *
    * @param angle Value in degrees to  be turned to
    */
void TurtleG::turnTo(GLfloat angle)
{
    this->CD = angle;
}

/*
    * turn the turtle to given angle relative to current direction
    * In SPC-SF context, can be considered dynamic angles
    *
    * @param angle Value in degrees to be turned to
    */
void TurtleG::turn(GLfloat angle)
{
    this->CD += angle;
}

/*
    * forward
    * Move the turtle forward a given distance,
    * drawing a line from start to finish.
    *
    * @param	distance	the distance to be traveled
    *			isVisible	draw the line or not
    */
void TurtleG::forward(float distance, bool isVisible)
{
    // Determine endpoint based on radial distance
    float x = CP.getx() + distance * cos(RADIAN_PER_DEGREE * CD);
    float y = CP.gety() + distance * sin(RADIAN_PER_DEGREE * CD);

    if (isVisible) lineTo(x, y);	// Move CP
    else moveTo(x, y);
}

/*
    * setCP
    * Set the Turtle's Current Position (CP).
    *
    * @param	x	x coordinate of new CP
    *			y	y coordinate of new CP
    */
void TurtleG::setCP(GLfloat x, GLfloat y)
{
    this->CP = Point2(x, y);
}

/*
    * getCP
    * Get the Turtle's Current Position (CP).
    *
    * @return	Point2	turtle's current position
    */
Point2 TurtleG::getCP()
{
    return this->CP;
}

/*
    * setCD
    * Set the Turtle's Current Direction (CD).
    *
    * @param	angle	direction of the turtle
    */
void TurtleG::setCD(GLfloat angle)
{
    this->CD = angle;
}

/*
    * getCD
    * Get the Turtle's Current Direction (CD).
    *
    * @return	GLfloat	current direction of turtle
    */
GLfloat TurtleG::getCD()
{
    return this->CD;
}

// Draw a line from CP to the new vertex
void TurtleG::lineTo(float x, float y)
{
    glBegin(GL_LINES);
    glVertex2f((GLfloat)CP.getx(), (GLfloat)CP.gety());
    glVertex2f((GLfloat)x, (GLfloat)y);
    glEnd();

    CP.set(x, y);	// Update the CP
    glFlush();		// Flush the buffer
}

void TurtleG::lineTo(Point2 p)
{
    glBegin(GL_LINES);
    glVertex2f((GLfloat)CP.getx(), (GLfloat)CP.gety());
    glVertex2f((GLfloat)p.getx(), (GLfloat)p.gety());
    glEnd();

    CP.set(p.getx(), p.gety());	// Update the CP
    glFlush();		// Flush the buffer
}

// Update the CP
void TurtleG::moveTo(float x, float y) { CP.set(x, y); }

void TurtleG::moveTo(Point2 p) { CP.set(p.getx(), p.gety()); }
