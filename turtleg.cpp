// Definitions for turtleg object.
// turtleg: Pen for drawing turtle graphics

#include "turtleg.h"

/*
    * TurtleG
    * Constructor. Construct new TurtleG object
    * with specified current position (CP) and
    * current direction (CD).
    * TurtleG object with current position (CP)
    * set to (0.0, 0.0)
    *
    * @param	x		x coordinate of CP
    *			y		y coordinate of CP
    *			angle	initial direction of CD
    */
TurtleG(GLfloat x, GLfloat y, GLfloat angle)
{
    this->CP = Point2(x, y);
    this->CD = angle;
}

/*
    * TurtleG
    * Constructor. Construct new TurtleG object
    * with specified current position (CP) and
    * current direction (CD).
    * TurtleG object with current position (CP)
    * set to (0.0, 0.0)
    *
    * @param	x		x coordinate of CP
    *			y		y coordinate of CP
    *			angle	initial direction of CD
    */
TurtleG(GLfloat x, GLfloat y, GLfloat angle)
{
    this->CP = Point2(x, y);
    this->CD = angle;
}

