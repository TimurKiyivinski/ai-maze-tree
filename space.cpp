#include "space.h"
#include <ostream>
#include <iostream>

Space::Space():
    x(0), y(0), h(-1), hh(-1), type('#')
{
}

Space::Space(int X, int Y):
    x(X), y(Y), h(-1), hh(-1), type('#')
{
}

Space::Space(int X, int Y, char _type):
    x(X), y(Y), h(-1), hh(-1), type(_type)
{
}

Space::~Space()
{
}

void Space::set_heuristic(int H)
{
    h = H;
    if (hh == -1)
        hh = H;
}

bool Space::is_start() const { return type == 's' || type == 'S';}

bool Space::is_finish() const { return type == 'f' || type == 'F';}

int Space::getX() const { return x; }

int Space::getY() const { return y; }

int Space::get_heuristic() const { return h; }

int Space::get_init_heuristic() const { return hh; }

bool Space::operator==(Space* _compare_space)
{
    return x == _compare_space->getX() && y == _compare_space->getY();
}

std::ostream& operator<<(std::ostream &os, const Space &s)
{
    os << s.x << " " << s.y;
    return os;
}

std::ostream& operator<<(std::ostream &os, const Space *s)
{
    os << s->x << " " << s->y;
    return os;
}

// Space < operator overload
inline bool operator<(const Space &lhs, const Space &rhs)
{
    return lhs.get_heuristic() < rhs.get_heuristic();
}

// Space > operator overload
inline bool operator>(const Space &lhs, const Space &rhs)
{
    return lhs.get_heuristic() > rhs.get_heuristic();
}
