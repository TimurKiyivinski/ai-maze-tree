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
        // Get coordinates
        int getX();
        int getY();
        // Easier to compare
        bool operator==(Space* _compare_space);
};
