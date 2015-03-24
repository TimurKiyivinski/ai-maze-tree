class Space;
class Space
{
    public:
        // X position
        int x;
        // Y position
        int y;
        // Empty constructor
        Space();
        // X, Y constructor
        Space(int x, int y);
        // Destructor
        ~Space();
    private:
        // Returns true if a Space is a parent node
        bool is_parent(Space* s);
        // Adds Space as child node in tree
        void add_child(Space* s);
};
