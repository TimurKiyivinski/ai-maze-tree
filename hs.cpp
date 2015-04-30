#include "hs.h"

using namespace std;

bool do_hs(
        tree<Space*>::iterator &root_node,
        int tr_max,
        int &expansions,
        bool &_finished,
        bool &verbose,
        Space *space_robot,
        sf::RenderWindow &window,
        sf::RectangleShape &robot_shape,
        sf::RectangleShape &robot_parent)
{
    cout << "HS" << endl;
    tree_node_<Space*> *current_node = root_node.node;
    deque<tree_node_<Space*>*> visited;
    // Random number generator, Mersenne Twister
    random_device random_generator; 
    uint32_t seed_val;
    uniform_int_distribution<uint32_t> uint_dist(1, tr_max);
    while (! _finished)
    {
        expansions++;
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
    return _finished;
}
