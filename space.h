#include <ostream>
#include <iostream>

#ifndef SPACE_H
#define SPACE_H
class Space;
class Space
{
    private:
        // X position
        int x;
        // Y position
        int y;
        // Space heuristic
        int h;
        // Start/Fin char
        char type;
    public:
        // Empty constructor
        Space();
        // X, Y constructor
        Space(int x, int y);
        // X, Y, Start/Fin Constructor
        Space(int x, int y, char _type);
        // Destructor
        ~Space();
        // Sets the space heuristic
        void set_heuristic(int H);
        // Returns type if type is start
        bool is_start() const;
        // Returns type if type is finish
        bool is_finish() const;
        // Get coordinates
        int getX() const;
        int getY() const;
        // Gets the space heuristic
        int get_heuristic() const;
        // Easier to compare
        bool operator==(Space* _compare_space);
        friend std::ostream& operator<<(std::ostream &os, const Space &s);
        friend std::ostream& operator<<(std::ostream &os, const Space *s);
};
#endif
