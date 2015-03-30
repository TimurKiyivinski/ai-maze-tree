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
#include "tree.hh"
#include "space.h"

using namespace std;

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

int program_main(string file_name)
{
    // Loads map file
    ifstream map_file(file_name);
    int i(0);
    string map_line;
    vector<vector<Space*>> space_matrix;
    // Get map data, line by line
    while (getline(map_file, map_line))
    {
        // Loop every line on the map
        for (int ii(0); ii < map_line.length(); ii++)
        {
            if (map_line[ii] == '#')
                space_matrix[i].push_back(NULL);
            else
                space_matrix[i].push_back(new Space(i, ii));
        }
        i++;
    }
#ifdef BFS
    cout << "BFS" << endl;
#endif
#ifdef DFS
    cout << "DFS" << endl;
#endif
#ifdef GBFS
    cout << "GBFS" << endl;
#endif
#ifdef BS
    cout << "BS" << endl;
#endif
#ifdef ASS
    cout << "ASS" << endl;
#endif
#ifdef HS
    cout << "HS" << endl;
#endif
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
