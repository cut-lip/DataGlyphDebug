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

/*
    * turnTo
    * Turn the turtle to given angle from horizontal.
    * In SPC-SF context, can be considered static angles.
    *
    * @param angle Value in degrees to  be turned to
    */
void turnTo(GLfloat angle)
{
    this->CD = angle;
}

/*
    * turn the turtle to given angle relative to current direction
    * In SPC-SF context, can be considered dynamic angles
    *
    * @param angle Value in degrees to be turned to
    */
void turn(GLfloat angle)
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
void forward(float distance, bool isVisible)
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
void setCP(GLfloat x, GLfloat y)
{
    this->CP = Point2(x, y);
}

/*
    * getCP
    * Get the Turtle's Current Position (CP).
    *
    * @return	Point2	turtle's current position
    */
Point2 getCP()
{
    return this->CP;
}

/*
    * setCD
    * Set the Turtle's Current Direction (CD).
    *
    * @param	angle	direction of the turtle
    */
void setCD(GLfloat angle)
{
    this->CD = angle;
}

/*
    * getCD
    * Get the Turtle's Current Direction (CD).
    *
    * @return	GLfloat	current direction of turtle
    */
GLfloat getCD()
{
    return this->CD;
}
