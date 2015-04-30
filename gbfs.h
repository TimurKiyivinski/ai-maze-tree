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

#ifndef ALGO_GBFS
#define ALGO_GBFS
#define BOX_SIZE 20
bool do_gbfs(
        tree<Space*>::iterator &root_node,
        int &expansions,
        bool &_finished,
        bool &verbose,
        Space *space_robot,
        sf::RenderWindow &window,
        sf::RectangleShape &robot_shape,
        sf::RectangleShape &robot_parent);
#endif
