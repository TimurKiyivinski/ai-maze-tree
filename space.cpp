#include "space.h"
#include <string>

Space::Space():
    x(0), y(0), type('#')
{
}

Space::Space(int X, int Y):
    x(X), y(Y), type('#')
{
}

Space::Space(int X, int Y, char _type):
    x(X), y(Y), type(_type)
{
}

Space::~Space()
{
}

bool Space::is_start(){ return type == 's' || type == 'S';}

bool Space::is_finish(){ return type == 'f' || type == 'F';}

int Space::getX() { return x; }

int Space::getY() { return y; }

bool Space::operator==(Space* _compare_space)
{
    return x == _compare_space->getX() && y == _compare_space->getY();
}
