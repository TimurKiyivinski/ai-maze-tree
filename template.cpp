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

void render_thread(sf::RenderWindow* window, vector<vector<Space*>>* space_matrix)
{
    // the rendering loop
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }
        for (int i(0); i < (*space_matrix).size(); i++)
        for (int ii(0); ii < (*space_matrix)[i].size(); ii++)
        {
            cout << i << " " << ii << endl;
            sf::RectangleShape shape(sf::Vector2f(10, 10));
            shape.setPosition(sf::Vector2f(ii * 10, i * 10));
            Space *_c = (*space_matrix)[i][ii];
            if (_c == NULL)
                shape.setFillColor(sf::Color(100, 250, 50));
            else
                shape.setFillColor(sf::Color(200, 250, 250));
            window->draw(shape);
        }
        window->display();
        window->clear();
    }
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
        bool b_add(true);
        for (tree<Space*>::iterator parent: _parents)
        {
            if (*parent == _c)
            {
                b_add = false;
            }
        }
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
        bool b_add(true);
        for (tree<Space*>::iterator parent: _parents)
        {
            if (*parent == _c)
                b_add = false;
        }
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
        bool b_add(true);
        for (tree<Space*>::iterator parent: _parents)
        {
            if (*parent == _c)
            {
                b_add = false;
            }
        }
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
        bool b_add(true);
        for (tree<Space*>::iterator parent: _parents)
        {
            if (*parent == _c)
                b_add = false;
        }
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

    /* *
     * Section: Graphics
     *
     * Use SFML to draw graphics in a separate thread
     * */
    sf::RenderWindow window(sf::VideoMode(_width, _height), "AI");
    window.setActive(false);
    sf::Thread thread(std::bind(&render_thread, &window, &space_matrix));
    thread.launch();
    while (window.isOpen())
    {
        cout << "x" << endl;
    }
    cout << "xx" << endl;
    
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
        cout << ((*BFS))->getX() << " " << (*BFS)->getY() << endl;
        if (*BFS->is_finish()) break;
        BFS++;
    }
#endif
#ifdef ALGO_DFS
    cout << "DFS" << endl;
    tree<Space*>::pre_order_iterator DFS(root_node);
    while (DFS != root_node.end())
    {
        cout << (*DFS)->getX() << " " << (*DFS)->getY() << endl;
        if ((*DFS)->is_finish()) break;
        DFS++;
    }
#endif
#ifdef ALGO_GBFS
    cout << "GBFS" << endl;
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
