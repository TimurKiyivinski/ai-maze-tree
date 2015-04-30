#include "node_space.h"

int get_dir(Space *p, Space *s)
{
    if (p->getX() == s->getX())
        if (p->getY() > s->getY())
            return DIR_LEFT;
        else return DIR_RIGHT;
    if (p->getY() == s->getY())
        if (p->getX() > s->getX())
            return DIR_UP;
        else return DIR_DOWN;
}

bool NodeSpaceMin::operator() (tree_node_<Space*> *lhs, tree_node_<Space*> *rhs)
{
    Space *sl = lhs->data;
    Space *sr = rhs->data;
    if (sl->get_heuristic() == sr->get_heuristic())
    {
        tree_node_<Space*> *pl = lhs->parent;
        tree_node_<Space*> *pr = rhs->parent;
        Space *ls = pl->data;
        Space *rs = pr->data;
        int ldir = get_dir(ls, sl);
        int rdir = get_dir(rs, sr);
        return ldir < rdir;
    }
    return sl->get_heuristic() < sr->get_heuristic();
}
