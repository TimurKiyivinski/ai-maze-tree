#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <random>
#include <regex>
#include <deque>
#include <vector>
#include "tree.hh"
#include "tree_util.hh"
#include "space.h"
#include "node_space.h"

#ifndef SHARED_H
#define SHARED_H

using namespace std;

// Standard function
string itos(int i);

/* *
 * Returns the manhattan distance between two sets
 * of coordinates
 *
 * x1           Start X coordinate
 * y1           Start Y coordinate
 * x2           Destination X coordinate
 * y2           Destination Y coordinate
 *
 * return       Manhattan distance
 * */
int get_manhattan(int x1, int y1, int x2, int y2);

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
 * Cross platform way to check if
 * a file exists.
 *
 * file_name    Name of map file
 *
 * return       Boolean
 * */
bool check_file(string file_name);

/* *
 * Returns a node parent
 *
 * tr           Tree pointer
 * node         Node
 * 
 * return       Iterator of node parent
 * */
tree<Space*>::iterator get_parent(tree<Space*> *tr, tree<Space*>::iterator *node);

/* *
 * Finds all parent nodes of a node
 *
 * s            Space to check
 * p            Parent vector
 *
 * return       Vector of all parent nodes
 * */
vector<tree<Space*>::iterator> get_parents(tree<Space*> *tr, tree<Space*>::iterator *node);

/* *
 * Checks if a Space parent exists
 * in a given iterator vector
 *
 * s            Space to check
 * p            Parent vector
 * 
 * return       Boolean p contains parent of s
 * */
bool is_parent(Space *s, vector<tree<Space*>::iterator> p);

/* *
 * Recursive Tree Construction
 *
 * map          Maze map
 * tr           Tree pointer
 * node         Node of last expansion
 * x            X coordinate of last expansion
 * y            Y coordinate of last expansion
 * */
void construct_tree(vector<vector<Space*>> map, tree<Space*> *tr, tree<Space*>::iterator *node, int x, int y);

/* *
 * Counts the number of parents a node has
 *
 * n            Node
 *
 * return       Parent count
 * */
int parent_count(tree_node_<Space*> *n);

/* *
 * Checks if a space exists in a deque
 *
 * s            Space
 * que          Deque to test
 *
 * return       Boolean if s is in que
 * */
bool in_queue(Space *s, deque<tree_node_<Space*>*> que);

/* *
 * Returns the children of a given space node
 *
 * tn           Node
 * c            Use A* style heuristics
 * */
priority_queue<tree_node_<Space*>*, vector<tree_node_<Space*>*>, NodeSpaceMin> get_children(tree_node_<Space*> *tn, bool c = false);

/* *
 * Returns all the nodes of the next depth
 *
 * tn           Node
 * */
priority_queue<tree_node_<Space*>*, vector<tree_node_<Space*>*>, NodeSpaceMin> get_breadth(tree_node_<Space*> *tn);
#endif
