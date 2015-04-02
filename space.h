class Space;
class Space
{
    private:
        // X position
        int x;
        // Y position
        int y;
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
        // Returns type if type is start
        bool is_start();
        // Returns type if type is finish
        bool is_finish();
        // Returns true if a Space is a parent node
        bool is_parent(Space* s);
        // Adds Space as child node in tree
        void add_child(Space* s);
};
