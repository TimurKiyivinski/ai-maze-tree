/*  Introduction to Artificial Intelligence Assignment 1
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
#include "shared.h"
#include "node_space.h"
#include "bfs.h"
#include "dfs.h"
#include "gbfs.h"
#include "ass.h"
#include "bs.h"
#include "hs.h"

// Constants
#define BOX_SIZE 20

using namespace std;

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
    // Total expansions made
    int expansions(0);
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
            _finished = do_bfs(root_node, expansions, _finished, verbose, space_robot, window, robot_shape, robot_parent);
            cout << "Expansions made: " << expansions << endl;
        }
        else if (algorithm == 1)
        {
            _finished = do_dfs(root_node, expansions, _finished, verbose, space_robot, window, robot_shape, robot_parent);
            cout << "Expansions made: " << expansions << endl;
        }
        else if (algorithm == 2)
        {
            _finished = do_gbfs(root_node, expansions, _finished, verbose, space_robot, window, robot_shape, robot_parent);
            cout << "Expansions made: " << expansions << endl;
        }
        else if (algorithm == 3)
        {
            _finished = do_ass(root_node, expansions, _finished, verbose, space_robot, window, robot_shape, robot_parent);
            cout << "Expansions made: " << expansions << endl;
        }
        else if (algorithm == 4)
        {
            _finished = do_bs(root_node, expansions, _finished, verbose, space_robot, window, robot_shape, robot_parent);
            cout << "Expansions made: " << expansions << endl;
        }
        else if (algorithm == 5)
        {
            _finished = do_hs(root_node, space_tree.size() - 1, expansions, _finished, verbose, space_robot, window, robot_shape, robot_parent);
            cout << "Expansions made: " << expansions << endl;
        }
        if (!_finished) window.clear();
        //if (_finished) window.close();
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
