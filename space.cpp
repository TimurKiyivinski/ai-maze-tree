#include "space.h"

Space::Space():
    x(0), y(0)
{
}

Space::Space(int X, int Y):
    x(X), y(Y)
{
}

Space::~Space()
{
}

bool Space::is_parent(Space *s)
{
    return false;
}

void Space::add_child(Space *s)
{
}
