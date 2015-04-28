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
#include <deque>
#include <vector>
#include <SFML/Graphics.hpp>
#include "tree.hh"
#include "tree_util.hh"
#include "space.h"

using namespace std;

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

// Comparator class for priority queue
class SpaceMin
{
    public:
        bool operator() (Space *lhs, Space *rhs)
        {
            return lhs->get_heuristic() <= rhs->get_heuristic();
        }
};

// Comparator class for priority queue
class NodeSpaceMin
{
    public:
        bool operator() (tree_node_<Space*> *lhs, tree_node_<Space*> *rhs)
        {
            Space *sl = lhs->data;
            Space *sr = rhs->data;
            return sl->get_heuristic() <= sr->get_heuristic();
        }
};

string itos(int i)
{
    stringstream ss;
    ss << i;
    return ss.str();
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

bool in_history(Space *s, vector<Space*> history)
{
    for (Space *h: history)
        if (s == h)
            return true;
    return false;
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

void print_queue(deque<tree_node_<Space*>*> que)
{
    cout << "Que of size " << que.size() << endl;
    while (! que.empty())
    {
        tree_node_<Space*> *que_node = que.front();
        Space *qs = que_node->data;
        cout << qs << endl;
        que.pop_front();
    }
    cout << "Done" << endl;
}

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

priority_queue<tree_node_<Space*>*, vector<tree_node_<Space*>*>, NodeSpaceMin> get_children(tree_node_<Space*> *tn, int c = 0)
{
    priority_queue<tree_node_<Space*>*, vector<tree_node_<Space*>*>, NodeSpaceMin> children;
    tree<Space*>::sibling_iterator SIB(tn);

    // Find all child siblings
    for (auto IT = SIB.begin(); IT != SIB.end(); IT++)
    {
        tree_node_<Space*> *child_node = IT.node;
        if (c != 0)
        {
            Space *s = child_node->data;
            s->set_heuristic(s->get_init_heuristic() + parent_count(child_node) + c);
        }
        children.push(child_node);
    }
    return children;
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

bool construct_tree(
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

int program_main(string file_name)
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

    /* *
     * Section: Load map
     *
     * Populates the map matrix with discovered
     * Spaces as well as find the start & finish
     * point of the maze.
     * */
    cout << "Loading map" << endl;
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
    _width = ii * 10;
    _height = i * 10;

    /* *
     * Section: Create Heuristics
     *
     * Now that the entire map has been discovered,
     * the heuristic function can be defined. In this
     * program, the manhattan distance between points
     * in the map are used. The heuristic function is
     * only used in algorithms that bvenefit from it.
     * */
    cout << "Heuristic Table:" << endl;
    for (int i(0); i < space_matrix.size(); i++)
    {
        if (i == 0)
        {
            cout << "###\t";
            for (int ii(0); ii < space_matrix[i].size(); ii++)
                cout << ii << "\t";
            cout << endl;
        }
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
                cout << _c->get_heuristic() << "\t";
            }
            else cout << "###\t";
        }
        cout << endl;
    }

    /* *
     * Section: Create Tree
     *
     * Use the Space matrix to populate the tree
     * by checking surrounding points in the matrix
     * */
    cout << "Loading tree" << endl;
    tree<Space*>::iterator space_root, root_node;
    space_root = space_tree.begin();
    root_node = space_tree.insert(space_root, space_start);
    construct_tree(space_matrix, &space_tree, &root_node, space_start->getX(), space_start->getY());
    
#ifdef DEBUG
    cout << "Debug" << endl;
    kptree::print_tree_bracketed(space_tree, cout);
    cout << endl;
#endif
    
    /* *
     * Section: Graphics
     *
     * Use SFML to draw graphics in a separate thread
     * */
    cout << "Loading graphics" << endl;
    // Create inital Window
    sf::RenderWindow window(sf::VideoMode(_width, _height), "AI");
    // Optimal speed to view results
    int window_fps(6);
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
            sf::RectangleShape shape(sf::Vector2f(10, 10));
            shape.setPosition(sf::Vector2f(ii * 10, i * 10));
            sf::Font font;
            font.loadFromFile("LSR.ttf");
            sf::Text text;
            text.setCharacterSize(10);
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
                text.setPosition(sf::Vector2f(_c->getY() * 10, _c->getX() * 10));
            }

            window.draw(shape);
            window.draw(text);
        }
        
        // Draw out robot
        sf::RectangleShape robot_shape(sf::Vector2f(10, 10));
        sf::RectangleShape robot_parent(sf::Vector2f(10, 10));
        robot_shape.setFillColor(sf::Color(250, 0, 0));
        robot_parent.setFillColor(sf::Color(250, 0, 250));
        robot_shape.setPosition(sf::Vector2f(space_robot->getY() * 10, space_robot->getX() * 10));
        window.draw(robot_shape);
        window.display();
        
        // Skip first 60 frames to avoid initial
        // artifacts that form on startup
        if (init_frame_count < window_fps)
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
#ifdef ALGO_BFS
        cout << "BFS" << endl;
        tree<Space*>::breadth_first_queued_iterator BFS(root_node);
        while (BFS != root_node.end())
        {
            space_robot = *BFS;
            robot_shape.setPosition(sf::Vector2f(space_robot->getY() * 10, space_robot->getX() * 10));
            window.draw(robot_shape);
            window.display();
            cout << space_robot << endl;
            if ((*BFS)->is_finish())
            {
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
                                solution_node_parent->data->getY() * 10,
                                solution_node_parent->data->getX() * 10));
                    window.draw(robot_parent);
                    window.display();
                    cout << solution_node_parent->data << endl;
                    solution_node_parent= solution_node_parent->parent;
                    steps++;
                }
                cout << "Required steps: " << steps << endl;
                break;
            }
            BFS++;
        }
#endif
#ifdef ALGO_DFS
        cout << "DFS" << endl;
        tree<Space*>::pre_order_iterator DFS(root_node);
        while (DFS != root_node.end())
        {
            space_robot = *DFS;
            robot_shape.setPosition(sf::Vector2f(space_robot->getY() * 10, space_robot->getX() * 10));
            window.draw(robot_shape);
            window.display();
            cout << space_robot << endl;
            if ((*DFS)->is_finish())
            {
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
                                solution_node_parent->data->getY() * 10,
                                solution_node_parent->data->getX() * 10));
                    window.draw(robot_parent);
                    window.display();
                    cout << solution_node_parent->data << endl;
                    solution_node_parent= solution_node_parent->parent;
                    steps++;
                }
                cout << "Required steps: " << steps << endl;
                break;
            }
            DFS++;
        }
#endif
#ifdef ALGO_GBFS
        cout << "GBFS" << endl;
        deque<tree_node_<Space*>*> path;
        deque<tree_node_<Space*>*> dead_path;
        tree_node_<Space*> *root_node_ = root_node.node;
        path.push_front(root_node_);
        while (! path.empty())
        {
            cout << "S" << endl;
            print_queue(path);
            // Get current state
            tree_node_<Space*> *current_node = path.front();
            path.pop_front();
            dead_path.push_front(current_node);
            cout << "Parent" << endl;
            Space *current_space = current_node->data;
            cout << current_space << endl;
            
            space_robot = current_space;
            robot_shape.setPosition(sf::Vector2f(space_robot->getY() * 10, space_robot->getX() * 10));
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
                                solution_node_parent->data->getY() * 10,
                                solution_node_parent->data->getX() * 10));
                    window.draw(robot_parent);
                    window.display();
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
                cout << "Children" << endl;
                tree_node_<Space*> *child_node = children.top();
                children.pop();
                Space *child_space = child_node->data;
                cout << child_space << endl;
                cout << child_space->get_heuristic() << endl;
                if (! in_queue(child_space, path) && ! in_queue(child_space, dead_path))
                {
                    path.push_front(child_node);
                    cout << "X" << endl;
                    print_queue(path);
                }
            }
            cout << "E" << endl;
            print_queue(path);
        }
#endif
#ifdef ALGO_ASS
        cout << "ASS" << endl;
        // priority_queue<tree_node_<Space*>*> path;
        priority_queue<tree_node_<Space*>*, vector<tree_node_<Space*>*>, NodeSpaceMin> path;
        priority_queue<tree_node_<Space*>*> dead_path;
        tree_node_<Space*> *root_node_ = root_node.node;
        path.push(root_node_);
        int cost(0);
        while (! path.empty())
        {
            // Get current state
            tree_node_<Space*> *current_node = path.top();
            path.pop();
            dead_path.push(current_node);
            Space *current_space = current_node->data;
            cout << current_space << endl;
            cost++;
            
            space_robot = current_space;
            robot_shape.setPosition(sf::Vector2f(space_robot->getY() * 10, space_robot->getX() * 10));
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
                                solution_node_parent->data->getY() * 10,
                                solution_node_parent->data->getX() * 10));
                    window.draw(robot_parent);
                    window.display();
                    cout << solution_node_parent->data << endl;
                    solution_node_parent= solution_node_parent->parent;
                    steps++;
                }
                cout << "Required steps: " << steps << endl;
                break;
            }
            
            // Find successors
            priority_queue<tree_node_<Space*>*, vector<tree_node_<Space*>*>, NodeSpaceMin> children = get_children(current_node, cost);
            
            if (children.size() == 0)
            {
                cost--;
                continue;
            }
            
            // Evaluate children
            while (! children.empty())
            {
                tree_node_<Space*> *child_node = children.top();
                children.pop();
                Space *child_space = child_node->data;
                if (! in_queue(child_space, dead_path))
                {
                    path.push(child_node);
                }
            }
        }
#endif
#ifdef ALGO_BS
        cout << "BS" << endl;
#endif
#ifdef ALGO_HS
        cout << "HS" << endl;
        
#endif
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
 * Main program
 *
 * argc         ISO argument count standard
 * argv         ISO argument variable standard
 *              Name of map file to load
 *
 * return       Program EXIT code
 * */
int main(int argc, char **argv)
{
    // There must be 2 arguments:
    // $program.bin $map.txt
    if (argc == 2)
    {
        // The second argument is the map file name
        string file_name = argv[1];
        if (check_file(file_name))
        {
            return program_main(file_name);
        }
        else
            return EXIT_FAILURE;
    }
    else
        return EXIT_FAILURE;
} 
