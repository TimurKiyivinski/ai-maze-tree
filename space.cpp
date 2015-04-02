#include "space.h"

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

Space::is_start(){ return type == 's' || type == 'S';}

Space::is_finish(){ return type == 'f' || type == 'F';}

bool Space::is_parent(Space *s)
{
    return false;
}

void Space::add_child(Space *s)
{
}
