#include "bfs.h"

using namespace std;

bool do_bfs(
        tree<Space*>::iterator &root_node,
        int &expansions,
        bool &_finished,
        bool &verbose,
        Space *space_robot,
        sf::RenderWindow &window,
        sf::RectangleShape &robot_shape,
        sf::RectangleShape &robot_parent)
{
    cout << "BFS" << endl;
    tree<Space*>::breadth_first_queued_iterator BFS(root_node);
    while (BFS != root_node.end())
    {
        expansions++;
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
    return _finished;
}
