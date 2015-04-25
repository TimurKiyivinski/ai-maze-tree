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
#include <string>
#include <algorithm>
#include <vector>
#include <SFML/Graphics.hpp>
#include "tree.hh"
#include "space.h"

using namespace std;

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

bool in_queue(Space *s, priority_queue<Space*> que)
{
    while (! que.empty())
    {
        if (s == que.top())
            return true;
        que.pop();
    }
    return false;
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
                space_finish = new Space(i, ii, 'F');
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
     * Section: Create Tree
     *
     * Use the Space matrix to populate the tree
     * by checking surrounding points in the matrix
     * */
    tree<Space*>::iterator space_root, root_node;
    space_root = space_tree.begin();
    root_node = space_tree.insert(space_root, space_start);
    construct_tree(space_matrix, &space_tree, &root_node, space_start->getX(), space_start->getY());

    for (int i(0); i < space_matrix.size(); i++)
    for (int ii(0); ii < space_matrix[i].size(); ii++)
    {
        Space *_c = space_matrix[i][ii];
        if (_c != NULL)
            _c->set_heuristic(get_manhattan(
                        space_finish->getX(),
                        space_finish->getY(),
                        _c->getX(),
                        _c->getY()));
    }
    
    /* *
     * Section: Graphics
     *
     * Use SFML to draw graphics in a separate thread
     * */
    // Create inital Window
    sf::RenderWindow window(sf::VideoMode(_width, _height), "AI");
    // Optimal speed to view results
    window.setFramerateLimit(60);
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
            Space *_c = space_matrix[i][ii];
            if (_c == NULL)
                shape.setFillColor(sf::Color(255, 255, 255));
            else if (_c->is_start())
                shape.setFillColor(sf::Color(0, 250, 0));
            else if (_c->is_finish())
                shape.setFillColor(sf::Color(0, 0, 250));
            else
                shape.setFillColor(sf::Color(0, 0, 0));
            window.draw(shape);
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
        if (init_frame_count < 60)
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
            cout << space_robot->getX() << " " << space_robot->getY() << endl;
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
                    cout << solution_node_parent->data->getX()
                         << " "
                         << solution_node_parent->data->getY()
                         << endl;
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
            cout << space_robot->getX() << " " << space_robot->getY() << endl;
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
                    cout << solution_node_parent->data->getX()
                         << " "
                         << solution_node_parent->data->getY()
                         << endl;
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
        priority_queue<Space*, vector<Space*>, less<Space*>> path;
        priority_queue<Space*> dead_path;
        tree<Space*>::pre_order_iterator DFS(root_node);
        tree_node_<Space*> *current_node = DFS.node;
        path.push(space_robot);
        while (! path.empty())
        {
            Space *current = path.top();
            path.pop();
            dead_path.push(current);

            // If Found solution
            if (current->is_finish())
            {
                cout << "Done" << endl;
                _finished = true;
                break;
            }

            // Draw robot
            space_robot = current;
            robot_shape.setPosition(sf::Vector2f(space_robot->getY() * 10, space_robot->getX() * 10));
            window.draw(robot_shape);
            window.display();
            cout << space_robot->getX() << " " << space_robot->getY() << endl;
            
            // Get children
            if (space_tree.number_of_children(current_node) > 0)
                current_node = current_node->first_child;
            else
                continue;

            if (current_node == NULL) continue;

            tree<Space*>::sibling_iterator child_siblings(current_node);
            priority_queue<Space*, vector<Space*>, less<Space*>> children;
            while (child_siblings != child_siblings.end())
            {
                children.push((*child_siblings));
                child_siblings++;
            }
            while (! children.empty())
            {
                Space* current_child = children.top();
                children.pop();
                if (! (in_queue(current_child, path) && in_queue(current_child, dead_path)))
                {
                    path.push(current_child);
                }
                else if (current_child < current)
                {
                    if (! in_queue(current_child, path))
                    {
                        path.push(current_child);
                    }
                    else
                    {
                        priority_queue<Space*> path_head;
                        while (path.top() != current_child)
                        {
                            path_head.push(path.top());
                            path.pop();
                        }
                        path.pop();
                        while (! path_head.empty())
                        {
                            path.push(path_head.top());
                            path_head.pop();
                        }
                        path.push(current_child);
                    }
                }
            }
        }
#endif
#ifdef ALGO_BS
        cout << "BS" << endl;
#endif
#ifdef ALGO_ASS
        cout << "ASS" << endl;
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
