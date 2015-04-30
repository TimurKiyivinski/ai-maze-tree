#include "shared.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <random>
#include <regex>
#include <deque>
#include <vector>
#include <SFML/Graphics.hpp>
#include "tree.hh"
#include "tree_util.hh"
#include "space.h"
#include "node_space.h"

using namespace std;

// Standard function
string itos(int i)
{
    stringstream ss;
    ss << i;
    return ss.str();
}

int get_manhattan(int x1, int y1, int x2, int y2)
{
    int X(0), Y(0);
    x1 >= x2 ?
        X = x1 - x2:
        X = x2 - x1;
    y1 >= y2 ?
        Y = y1 - y2:
        Y = y2 - y1;
    return X + Y;
}

bool check_file(string file_name)
{
    ifstream map_file(file_name);
    return map_file.good();
}

tree<Space*>::iterator get_parent(tree<Space*> *tr, tree<Space*>::iterator *node)
{
    return tr->parent(*node);
}

vector<tree<Space*>::iterator> get_parents(tree<Space*> *tr, tree<Space*>::iterator *node)
{
    // Load parents
    vector<tree<Space*>::iterator> _parents;
    if (! (**node)->is_start())
    {
        tree<Space*>::iterator parent;
        parent = tr->parent(*node);
        _parents.push_back(parent);
        while (parent != tr->begin())
        {
            parent = get_parent(tr, &parent);
            _parents.push_back(parent);
        }
    }
    return _parents;
}

bool is_parent(Space *s, vector<tree<Space*>::iterator> p)
{
    for (tree<Space*>::iterator parent: p)
    {
        if (*parent == s)
        {
            return true;
        }
    }
    return false;
}

void construct_tree(
        vector<vector<Space*>> map,
        tree<Space*> *tr,
        tree<Space*>::iterator *node,
        int x,
        int y)
{
    // Add children
    if (x > 0)                  if (map[x-1][y] != NULL) // Up
    {
        // Load parents
        vector<tree<Space*>::iterator> _parents = get_parents(tr, node);
        tree<Space*>::iterator child;
        Space *_c = map[x-1][y];
        // Only add if node is not a parent
        bool b_add = ! is_parent(_c, _parents);
        if (b_add)
        {
            child = tr->append_child(*node, _c);
            construct_tree(map, tr, &child, x-1, y);
        }
    }
    if (y > 0)                  if (map[x][y-1] != NULL) // Left
    {
        // Load parents
        vector<tree<Space*>::iterator> _parents = get_parents(tr, node);
        tree<Space*>::iterator child;
        Space *_c = map[x][y-1];
        // Only add if node is not a parent
        bool b_add = ! is_parent(_c, _parents);
        if (b_add)
        {
            child = tr->append_child(*node, _c);
            construct_tree(map, tr, &child, x, y-1);
        }
    }
    if (x < map.size() - 1)     if (map[x+1][y] != NULL) // Down
    {
        // Load parents
        vector<tree<Space*>::iterator> _parents = get_parents(tr, node);
        tree<Space*>::iterator child;
        Space *_c = map[x+1][y];
        // Only add if node is not a parent
        bool b_add = ! is_parent(_c, _parents);
        if (b_add)
        {
            child = tr->append_child(*node, _c);
            construct_tree(map, tr, &child, x+1, y);
        }
    }
    if (y < map[0].size() -1) if (map[x][y+1] != NULL) // Right
    {
        // Load parents
        vector<tree<Space*>::iterator> _parents = get_parents(tr, node);
        tree<Space*>::iterator child;
        Space *_c = map[x][y+1];
        // Only add if node is not a parent
        bool b_add = ! is_parent(_c, _parents);
        if (b_add)
        {
            child = tr->append_child(*node, _c);
            construct_tree(map, tr, &child, x, y+1);
        }
    }
}

int parent_count(tree_node_<Space*> *n)
{
    // Get finished node
    tree_node_<Space*> solution_node = *n;
    // Iterate parents
    tree_node_<Space*> *solution_node_parent= solution_node.parent;
    // Count steps required
    int steps(1);
    while(!solution_node_parent->data->is_start())
    {
        solution_node_parent= solution_node_parent->parent;
        steps++;
    }
    return steps;
}

bool in_queue(Space *s, deque<tree_node_<Space*>*> que)
{
    while (! que.empty())
    {
        tree_node_<Space*> *que_node = que.front();
        Space *qs = que_node->data;
        if (s == qs)
            return true;
        que.pop_front();
    }
    return false;
}

priority_queue<tree_node_<Space*>*, vector<tree_node_<Space*>*>, NodeSpaceMin> get_children(tree_node_<Space*> *tn, bool c)
{
    priority_queue<tree_node_<Space*>*, vector<tree_node_<Space*>*>, NodeSpaceMin> children;
    tree<Space*>::sibling_iterator SIB(tn);

    // Find all child siblings
    for (auto IT = SIB.begin(); IT != SIB.end(); IT++)
    {
        tree_node_<Space*> *child_node = IT.node;
        if (c)
        {
            Space *s = child_node->data;
            s->set_heuristic(s->get_init_heuristic() + parent_count(child_node) + 1);
        }
        children.push(child_node);
    }
    return children;
}

priority_queue<tree_node_<Space*>*, vector<tree_node_<Space*>*>, NodeSpaceMin> get_breadth(tree_node_<Space*> *tn)
{
    priority_queue<tree_node_<Space*>*, vector<tree_node_<Space*>*>, NodeSpaceMin> children;
    tree<Space*>::fixed_depth_iterator FDI(tn);

    // Find all child siblings
    for (auto IT = FDI.begin(); IT != FDI.end(); IT++)
    {
        tree_node_<Space*> *child_node = IT.node;
        children.push(child_node);
    }
    return children;
}
