#include "bs.h"

using namespace std;

bool do_bs(
        tree<Space*>::iterator &root_node,
        int &expansions,
        bool &_finished,
        bool &verbose,
        Space *space_robot,
        sf::RenderWindow &window,
        sf::RectangleShape &robot_shape,
        sf::RectangleShape &robot_parent)
{
    cout << "BS" << endl;
    deque<tree_node_<Space*>*> path;
    tree_node_<Space*> *root_node_ = root_node.node;
    path.push_front(root_node_);
    int beam_size(1);
    while (! _finished)
    {
        expansions++;
        // Get current state
        tree_node_<Space*> *current_node = path.front();
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

        priority_queue<tree_node_<Space*>*, vector<tree_node_<Space*>*>, NodeSpaceMin> b_children;
        NodeSpaceMin sorter;
        for (int i(0); i < path.size(); i++)
        {
            tree_node_<Space*> *path_node = path.front();
            path.pop_front();
            // Find successors
            priority_queue<tree_node_<Space*>*, vector<tree_node_<Space*>*>, NodeSpaceMin> children = get_breadth(path_node);

            if (children.size() == 0) continue;

            // Evaluate children
            while (! children.empty())
            {
                tree_node_<Space*> *child_node = children.top();
                children.pop();
                Space *child_space = child_node->data;
                b_children.push(child_node);

                space_robot = child_space;
                robot_shape.setPosition(sf::Vector2f(space_robot->getY() * BOX_SIZE, space_robot->getX() * BOX_SIZE));
                window.draw(robot_shape);
                window.display();
            }
        }
        while (! b_children.empty())
        {
            path.push_back(b_children.top());
            b_children.pop();
        }
        sort(path.begin(), path.end(), sorter);
    }
    return _finished;
}
