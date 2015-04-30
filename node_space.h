#include "space.h"
#include "tree.hh"

#ifndef NODESPACE_H
#define NODESPACE_H
#define DIR_UP 0
#define DIR_LEFT 1
#define DIR_DOWN 2
#define DIR_RIGHT 3

/* *
 * Returns the relative direction of
 * a child Space to a parent Space
 * using predefined constants
 *
 * p            Parent Space
 * s            Child Space
 * */
int get_dir(Space *p, Space *s);

/* *
 * This class is used for comparising tree nodes containing
 * Space pointers, used in sorting queues
 * */
class NodeSpaceMin
{
    public:
        bool operator() (tree_node_<Space*> *lhs, tree_node_<Space*> *rhs);
};
#endif
