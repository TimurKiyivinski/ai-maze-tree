/*  Introduction to Artificial Intelligence Assignment 1
 *  Sorts mazes with <name> algorithm.
 *  Copyright (C) 2015 Timothy Kiyui & Brian Sim

 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

// Constants
#define BOX_SIZE 20
#define DIR_UP 0
#define DIR_LEFT 1
#define DIR_DOWN 2
#define DIR_RIGHT 3

using namespace std;

// Standard function
string itos(int i)
{
    stringstream ss;
    ss << i;
    return ss.str();
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

/* *
 * Returns the relative direction of
 * a child Space to a parent Space
 * using predefined constants
 *
 * p            Parent Space
 * s            Child Space
 * */
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

// Comparator class for priority queue
class NodeSpaceMin
{
    public:
        bool operator() (tree_node_<Space*> *lhs, tree_node_<Space*> *rhs)
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
};

/* *
 * Cross platform way to check if
 * a file exists.
 *
 * file_name    Name of map file
 *
 * return       Boolean
 * */
bool check_file(string file_name)
{
    ifstream map_file(file_name);
    return map_file.good();
}

/* *
 * Returns a node parent
 *
 * tr           Tree pointer
 * node         Node
 * 
 * return       Iterator of node parent
 * */
tree<Space*>::iterator get_parent(tree<Space*> *tr, tree<Space*>::iterator *node)
{
    return tr->parent(*node);
}

/* *
 * Finds all parent nodes of a node
 *
 * s            Space to check
 * p            Parent vector
 *
 * return       Vector of all parent nodes
 * */
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

/* *
 * Checks if a Space parent exists
 * in a given iterator vector
 *
 * s            Space to check
 * p            Parent vector
 * 
 * return       Boolean p contains parent of s
 * */
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

/* *
 * Recursive Tree Construction
 *
 * map          Maze map
 * tr           Tree pointer
 * node         Node of last expansion
 * x            X coordinate of last expansion
 * y            Y coordinate of last expansion
 * */
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

/* *
 * Counts the number of parents a node has
 *
 * n            Node
 *
 * return       Parent count
 * */
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

priority_queue<tree_node_<Space*>*, vector<tree_node_<Space*>*>, NodeSpaceMin> get_children(tree_node_<Space*> *tn, bool c = false)
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

/* *
 * Checks if a space exists in a deque
 *
 * s            Space
 * que          Deque to test
 *
 * return       Boolean if s is in que
 * */
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

/* *
 * Main program function
 *
 * file_name    Name of map file to load
 * algorithm    Algorithm to use
 * window_fps   Frames per second
 * verbose      Verbose option
 *
 * return       Program EXIT code
 * */
int program_main(string file_name, int algorithm, int window_fps = 30, bool verbose = false)
{
    // Function scope variables:
    // Loads map file
    ifstream map_file(file_name);
    // Loop counters
    int i(0);
    int ii(0);
    // Space matrix
    vector<vector<Space*>> space_matrix;
    // Space tree
    tree<Space*> space_tree;
    // Map Start & Finish
    Space* space_start = NULL;
    Space* space_finish = NULL;
    // Map size
    int _width, _height;
    
    if (verbose)
        cout << "Verbose mode enabled" << endl;

    /* *
     * Section: Load map
     *
     * Populates the map matrix with discovered
     * Spaces as well as find the start & finish
     * point of the maze.
     * */
    cout << "Loading map..." << endl;
    // Temporarily stores map lines
    string map_line;
    // Get map data, line by line
    while (getline(map_file, map_line))
    {
        // Loop every line on the map
        vector<Space*> line_vector;
        for (ii = 0; ii < map_line.length(); ii++)
        {
            if (map_line[ii] == '#')
                line_vector.push_back(NULL);
            else if (map_line[ii] == 'S')
            {
                space_start = new Space(i, ii, 'S');
                line_vector.push_back(space_start);
            }
            else if (map_line[ii] == 'F')
            {
                space_finish  = new Space(i, ii, 'F');
                line_vector.push_back(space_finish);
            }
            else if (map_line[ii] == ' ')
                line_vector.push_back(new Space(i, ii));
        }
        space_matrix.push_back(line_vector);
        i++;
    }
    _width = ii * BOX_SIZE;
    _height = i * BOX_SIZE;

    /* *
     * Section: Create Heuristics
     *
     * Now that the entire map has been discovered,
     * the heuristic function can be defined. In this
     * program, the manhattan distance between points
     * in the map are used. The heuristic function is
     * only used in algorithms that bvenefit from it.
     * */
    if (verbose)
        cout << "Heuristic Table:" << endl;
    for (int i(0); i < space_matrix.size(); i++)
    {
        if (i == 0)
        {
            if (verbose)
                cout << "x#y\t";
            for (int ii(0); ii < space_matrix[i].size(); ii++)
                if (verbose)
                    cout << ii << "\t";
            if (verbose)
                cout << endl;
        }
        if (verbose)
            cout << i << "\t";

        for (int ii(0); ii < space_matrix[i].size(); ii++)
        {
            Space *_c = space_matrix[i][ii];
            if (_c != NULL)
            {
                _c->set_heuristic(get_manhattan(
                            space_finish->getX(),
                            space_finish->getY(),
                            _c->getX(),
                            _c->getY()));
                if (verbose)
                    cout << _c->get_heuristic() << "\t";
            }
            else if (verbose)
                cout << "###\t";
        }
        if (verbose)
            cout << endl;
    }

    /* *
     * Section: Create Tree
     *
     * Use the Space matrix to populate the tree
     * by checking surrounding points in the matrix
     * */
    cout << "Loading tree..." << endl;
    tree<Space*>::iterator space_root, root_node;
    space_root = space_tree.begin();
    root_node = space_tree.insert(space_root, space_start);
    construct_tree(space_matrix, &space_tree, &root_node, space_start->getX(), space_start->getY());
    if (verbose)
    {
        cout << "Generated tree:" << endl;
        kptree::print_tree_bracketed(space_tree, cout);
        cout << endl;
    }
    
    /* *
     * Section: Graphics
     *
     * Use SFML to draw graphics in a separate thread
     * */
    cout << "Loading graphics..." << endl;
    // Create inital Window
    sf::RenderWindow window(sf::VideoMode(_width, _height), "AI");
    // Optimal speed to view results
    window.setFramerateLimit(window_fps);
    // Robot begins at starting space
    Space *space_robot = space_start;
    // Initial frame counters, explained below
    int init_frame_count(0);
    // Maze solved boolean
    bool _finished(false);
    // Window draw loop 
    while (window.isOpen())
    {
        // Handle window closing events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        // If the maze is solved, don't redraw it
        if (_finished) continue;

        // Draw the maze
        for (int i(0); i < space_matrix.size(); i++)
        for (int ii(0); ii < space_matrix[i].size(); ii++)
        {
            sf::RectangleShape shape(sf::Vector2f(BOX_SIZE, BOX_SIZE));
            shape.setPosition(sf::Vector2f(ii * BOX_SIZE, i * BOX_SIZE));
            sf::Font font;
            font.loadFromFile("LSR.ttf");
            sf::Text text;
            text.setCharacterSize(BOX_SIZE);
            text.setFont(font); 
            Space *_c = space_matrix[i][ii];
            if (_c == NULL)
                shape.setFillColor(sf::Color(255, 255, 255));
            else if (_c->is_start())
                shape.setFillColor(sf::Color(0, 250, 0));
            else if (_c->is_finish())
                shape.setFillColor(sf::Color(0, 0, 250));
            else
            {
                shape.setFillColor(sf::Color(0, 0, 0));
                text.setString(itos(_c->get_heuristic()));
                text.setColor(sf::Color::Green);
                text.setPosition(sf::Vector2f(_c->getY() * BOX_SIZE, _c->getX() * BOX_SIZE));
            }

            window.draw(shape);
            if (verbose)
                window.draw(text);
        }
        
        // Draw out robot
        sf::RectangleShape robot_shape(sf::Vector2f(BOX_SIZE, BOX_SIZE));
        sf::RectangleShape robot_parent(sf::Vector2f(BOX_SIZE, BOX_SIZE));
        robot_shape.setFillColor(sf::Color(250, 0, 0));
        robot_parent.setFillColor(sf::Color(250, 0, 250));
        robot_shape.setPosition(sf::Vector2f(space_robot->getY() * BOX_SIZE, space_robot->getX() * BOX_SIZE));
        window.draw(robot_shape);
        window.display();
        
        // Skip first 60 frames to avoid initial
        // artifacts that form on startup
        if (init_frame_count < 6)
        {
            window.clear();
            init_frame_count++;
            continue;
        }

        /* *
         * Section: Algorithm
         *
         * Different algorithm sections
         * */
        if (algorithm == 0)
        {
            cout << "BFS" << endl;
            tree<Space*>::breadth_first_queued_iterator BFS(root_node);
            while (BFS != root_node.end())
            {
                space_robot = *BFS;
                robot_shape.setPosition(sf::Vector2f(space_robot->getY() * BOX_SIZE, space_robot->getX() * BOX_SIZE));
                window.draw(robot_shape);
                window.display();
                if (verbose)
                    cout << space_robot << endl;
                if ((*BFS)->is_finish())
                {
                    cout << "Solved" << endl;
                    _finished = true;
                    // Get finished node
                    tree_node_<Space*> solution_node = BFS.get_node();
                    // Iterate parents
                    tree_node_<Space*> *solution_node_parent= solution_node.parent;
                    // Count steps required
                    int steps(1);
                    while(!solution_node_parent->data->is_start())
                    {
                        robot_parent.setPosition(sf::Vector2f(
                                    solution_node_parent->data->getY() * BOX_SIZE,
                                    solution_node_parent->data->getX() * BOX_SIZE));
                        window.draw(robot_parent);
                        window.display();
                        if (verbose)
                            cout << solution_node_parent->data << endl;
                        solution_node_parent= solution_node_parent->parent;
                        steps++;
                    }
                    cout << "Required steps: " << steps << endl;
                    break;
                }
                BFS++;
            }
        }
        else if (algorithm == 1)
        {
            cout << "DFS" << endl;
            tree<Space*>::pre_order_iterator DFS(root_node);
            while (DFS != root_node.end())
            {
                space_robot = *DFS;
                robot_shape.setPosition(sf::Vector2f(space_robot->getY() * BOX_SIZE, space_robot->getX() * BOX_SIZE));
                window.draw(robot_shape);
                window.display();
                if (verbose)
                    cout << space_robot << endl;
                if ((*DFS)->is_finish())
                {
                    cout << "Solved" << endl;
                    _finished = true;
                    // Get finished node
                    tree_node_<Space*> solution_node = DFS.get_node();
                    // Iterate parents
                    tree_node_<Space*> *solution_node_parent= solution_node.parent;
                    // Count steps required
                    int steps(1);
                    while(!solution_node_parent->data->is_start())
                    {
                        robot_parent.setPosition(sf::Vector2f(
                                    solution_node_parent->data->getY() * BOX_SIZE,
                                    solution_node_parent->data->getX() * BOX_SIZE));
                        window.draw(robot_parent);
                        window.display();
                        if (verbose)
                            cout << solution_node_parent->data << endl;
                        solution_node_parent= solution_node_parent->parent;
                        steps++;
                    }
                    cout << "Required steps: " << steps << endl;
                    break;
                }
                DFS++;
            }
        }
        else if (algorithm == 2)
        {
            cout << "GBFS" << endl;
            deque<tree_node_<Space*>*> path;
            deque<tree_node_<Space*>*> dead_path;
            tree_node_<Space*> *root_node_ = root_node.node;
            path.push_front(root_node_);
            while (! path.empty())
            {
                // Get current state
                tree_node_<Space*> *current_node = path.front();
                path.pop_front();
                dead_path.push_front(current_node);
                Space *current_space = current_node->data;
                if (verbose)
                    cout << current_space << endl;

                space_robot = current_space;
                robot_shape.setPosition(sf::Vector2f(space_robot->getY() * BOX_SIZE, space_robot->getX() * BOX_SIZE));
                window.draw(robot_shape);
                window.display();

                // If we have found the solution
                if (current_space->is_finish())
                {
                    cout << "Solved" << endl;
                    _finished = true;
                    // Get finished node
                    tree_node_<Space*> solution_node = *current_node;
                    // Iterate parents
                    tree_node_<Space*> *solution_node_parent= solution_node.parent;
                    // Count steps required
                    int steps(1);
                    while(!solution_node_parent->data->is_start())
                    {
                        robot_parent.setPosition(sf::Vector2f(
                                    solution_node_parent->data->getY() * BOX_SIZE,
                                    solution_node_parent->data->getX() * BOX_SIZE));
                        window.draw(robot_parent);
                        window.display();
                        if (verbose)
                            cout << solution_node_parent->data << endl;
                        solution_node_parent= solution_node_parent->parent;
                        steps++;
                    }
                    cout << "Required steps: " << steps << endl;
                    break;
                }

                // Find successors
                priority_queue<tree_node_<Space*>*, vector<tree_node_<Space*>*>, NodeSpaceMin> children = get_children(current_node);

                if (children.size() == 0) continue;

                // Evaluate children
                while (! children.empty())
                {
                    tree_node_<Space*> *child_node = children.top();
                    children.pop();
                    Space *child_space = child_node->data;
                    if (! in_queue(child_space, path) && ! in_queue(child_space, dead_path))
                    {
                        path.push_front(child_node);
                    }
                    else if (child_space < current_space)
                    {
                        if (! in_queue(child_space, path))
                        {
                            path.push_front(child_node);
                        }
                        else
                        {
                            for (auto IT = path.begin(); IT != path.end(); IT++)
                            {
                                tree_node_<Space*> *IT_node = *IT;
                                Space *IT_space = IT_node->data;
                                if (IT_space == child_space)
                                {
                                    path.erase(IT);
                                }
                                path.push_front(child_node);
                                break;
                            }
                        }
                    }
                }
                NodeSpaceMin sorter;
                sort(path.begin(), path.end(), sorter);
            }
        }
        else if (algorithm == 3)
        {
            cout << "ASS" << endl;
            deque<tree_node_<Space*>*> path;
            deque<tree_node_<Space*>*> dead_path;
            tree_node_<Space*> *root_node_ = root_node.node;
            path.push_front(root_node_);
            int g_score(0);
            int f_score(g_score + space_start->get_heuristic());
            while (! path.empty())
            {
                // Get current state
                tree_node_<Space*> *current_node = path.front();
                path.pop_front();
                dead_path.push_front(current_node);
                Space *current_space = current_node->data;
                if (verbose)
                    cout << current_space << endl;

                space_robot = current_space;
                robot_shape.setPosition(sf::Vector2f(space_robot->getY() * BOX_SIZE, space_robot->getX() * BOX_SIZE));
                window.draw(robot_shape);
                window.display();

                // If we have found the solution
                if (current_space->is_finish())
                {
                    cout << "Solved" << endl;
                    _finished = true;
                    // Get finished node
                    tree_node_<Space*> solution_node = *current_node;
                    // Iterate parents
                    tree_node_<Space*> *solution_node_parent= solution_node.parent;
                    // Count steps required
                    int steps(1);
                    while(!solution_node_parent->data->is_start())
                    {
                        robot_parent.setPosition(sf::Vector2f(
                                    solution_node_parent->data->getY() * BOX_SIZE,
                                    solution_node_parent->data->getX() * BOX_SIZE));
                        window.draw(robot_parent);
                        window.display();
                        if (verbose)
                            cout << solution_node_parent->data << endl;
                        solution_node_parent= solution_node_parent->parent;
                        steps++;
                    }
                    cout << "Required steps: " << steps << endl;
                    break;
                }

                // Find successors
                priority_queue<tree_node_<Space*>*, vector<tree_node_<Space*>*>, NodeSpaceMin> children = get_children(current_node, true);

                if (children.size() == 0) continue;

                // Evaluate children
                while (! children.empty())
                {
                    tree_node_<Space*> *child_node = children.top();
                    children.pop();
                    Space *child_space = child_node->data;
                    if (in_queue(child_space, dead_path)) continue;
                    int g_child_score(parent_count(child_node) + 1);
                    if (! in_queue(child_space, path))
                    {
                        path.push_front(child_node);
                    }
                }
                NodeSpaceMin sorter;
                sort(path.begin(), path.end(), sorter);
            }
        }
        else if (algorithm == 4)
        {
            cout << "BS" << endl;
            deque<tree_node_<Space*>*> path;
            tree_node_<Space*> *root_node_ = root_node.node;
            path.push_front(root_node_);
            while (! path.empty())
            {
                // Get current state
                tree_node_<Space*> *current_node = path.front();
                path.pop_front();
                Space *current_space = current_node->data;
                if (verbose)
                    cout << current_space << endl;

                space_robot = current_space;
                robot_shape.setPosition(sf::Vector2f(space_robot->getY() * BOX_SIZE, space_robot->getX() * BOX_SIZE));
                window.draw(robot_shape);
                window.display();

                // If we have found the solution
                if (current_space->is_finish())
                {
                    _finished = true;
                    // Get finished node
                    tree_node_<Space*> solution_node = *current_node;
                    // Iterate parents
                    tree_node_<Space*> *solution_node_parent= solution_node.parent;
                    // Count steps required
                    int steps(1);
                    while(!solution_node_parent->data->is_start())
                    {
                        robot_parent.setPosition(sf::Vector2f(
                                    solution_node_parent->data->getY() * BOX_SIZE,
                                    solution_node_parent->data->getX() * BOX_SIZE));
                        window.draw(robot_parent);
                        window.display();
                        if (verbose)
                            cout << solution_node_parent->data << endl;
                        solution_node_parent= solution_node_parent->parent;
                        steps++;
                    }
                    cout << "Required steps: " << steps << endl;
                    break;
                }

                // Find successors
                priority_queue<tree_node_<Space*>*, vector<tree_node_<Space*>*>, NodeSpaceMin> children = get_breadth(current_node);

                if (children.size() == 0) continue;

                // Evaluate children
                while (! children.empty())
                {
                    tree_node_<Space*> *child_node = children.top();
                    children.pop();
                    Space *child_space = child_node->data;
                    path.push_front(child_node);
                }
            }
        }
        else if (algorithm == 5)
        {
            cout << "HS" << endl;
            tree_node_<Space*> *current_node = root_node.node;
            deque<tree_node_<Space*>*> visited;
            // Random number generator, Mersenne Twister
            random_device random_generator; 
            uint32_t seed_val;
            uniform_int_distribution<uint32_t> uint_dist(1, space_tree.size() - 1);
            while (! _finished)
            {
                // Get current state
                Space *current_space = current_node->data;
                visited.push_front(current_node);
                if (verbose)
                    cout << current_space << endl;

                space_robot = current_space;
                robot_shape.setPosition(sf::Vector2f(space_robot->getY() * BOX_SIZE, space_robot->getX() * BOX_SIZE));
                window.draw(robot_shape);
                window.display();

                // If we have found the solution
                if (current_space->is_finish())
                {
                    _finished = true;
                    // Get finished node
                    tree_node_<Space*> solution_node = *current_node;
                    // Iterate parents
                    tree_node_<Space*> *solution_node_parent= solution_node.parent;
                    // Count steps required
                    int steps(1);
                    while(!solution_node_parent->data->is_start())
                    {
                        robot_parent.setPosition(sf::Vector2f(
                                    solution_node_parent->data->getY() * BOX_SIZE,
                                    solution_node_parent->data->getX() * BOX_SIZE));
                        window.draw(robot_parent);
                        window.display();
                        if (verbose)
                            cout << solution_node_parent->data << endl;
                        solution_node_parent= solution_node_parent->parent;
                        steps++;
                    }
                    cout << "Required steps: " << steps << endl;
                }

                // Find successors
                priority_queue<tree_node_<Space*>*, vector<tree_node_<Space*>*>, NodeSpaceMin> children = get_children(current_node);

                if (children.size() == 0)
                {
                    tree<Space*>::iterator random_node = root_node.begin();
                    bool random_found(false);
                    while (! random_found)
                    {
                        for (int i(0); i < uint_dist(random_generator); i++)
                        {
                            random_node++;
                        }
                        if (! in_queue(random_node.node->data, visited))
                            random_found = true;
                    }
                    children.push(random_node.node);
                }
                current_node = children.top();
            }
        }
        if (!_finished) window.clear();
    }

    /* *
     * Section: Clearing up
     *
     * Clear up memory
     * */
    for (i = 0; i < space_matrix.size(); i++)
        for (ii = 0; ii < space_matrix[i].size(); ii++)
            if (space_matrix[i][ii] != NULL)
                delete(space_matrix[i][ii]);
    return EXIT_SUCCESS;
}

/* *
 * Main help menu
 *
 *
 * return       Failure EXIT
 * */
int program_help()
{
    cout << "Usage" << endl;
    cout << "MAP ALGORITHM [FPS: 1 | 2 | etc] [-v]" << endl;
    cout << "Algorithms:" << endl
         << "-bfs\tBreadth First Search" << endl
         << "-dfs\tDepth First Search" << endl
         << "-gdfs\tGreedy Best First Search" << endl
         << "-ass\tA* Search" << endl
         << "-bs\tBeam Search" << endl
         << "-hs\tHill Climbing" << endl;
    return EXIT_FAILURE;
}

/* *
 * Main program
 *
 * argc         ISO argument count standard
 * argv         ISO argument variable standard
 *              Name of map file to load
 *              Algorithm to use
 *              Frames per second
 *              Verbose option
 *
 * return       Program EXIT code
 * */
int main(int argc, char **argv)
{
    if (argc > 1)
    {
        // The second argument is the map file name
        string file_name = argv[1];
        int algorithm(-1);
        int window_fps(30);
        bool verbose(false);
        // Look for next arguments
        if (argc > 2)
        {
            for (int i(2); i < argc; i++)
            {
                string arg = argv[i];
                // Checks for appropriate algorithm
                string::size_type algo_bfs;
                algo_bfs = arg.find("-bfs");
                if (algo_bfs != string::npos)
                    algorithm = 0;
                string::size_type algo_dfs;
                algo_dfs = arg.find("-dfs");
                if (algo_dfs != string::npos)
                    algorithm = 1;
                string::size_type algo_gbfs;
                algo_gbfs = arg.find("-gbfs");
                if (algo_gbfs != string::npos)
                    algorithm = 2;
                string::size_type algo_ass;
                algo_ass = arg.find("-ass");
                if (algo_ass != string::npos)
                    algorithm = 3;
                string::size_type algo_bs;
                algo_bs = arg.find("-bs");
                if (algo_bs != string::npos)
                    algorithm = 4;
                string::size_type algo_hs;
                algo_hs = arg.find("-hs");
                if (algo_hs != string::npos)
                    algorithm = 5;
                // Check for FPS setting
                if (regex_match(arg, regex("[0-9]+")))
                {
                    cout << "Yes" << endl;
                    window_fps = stoi(arg);
                }
                // Check for verbose mode
                string::size_type arg_verbose;
                arg_verbose = arg.find("-v");
                if (arg_verbose != string::npos)
                    verbose = true;
            }
        }
        if (check_file(file_name) && algorithm != -1)
        {
            return program_main(file_name, algorithm, window_fps, verbose);
        }
        else
            program_help();
    }
    else
        program_help();
} 
