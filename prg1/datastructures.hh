// Datastructures.hh
//
// Student name: Konsta Karjalainen
// Student email: konsta.karjalainen@tuni.fi
// Student number: 284983

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <unordered_map>


// Types for IDs
using TownID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
TownID const NO_TOWNID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: Always O(1)
    // Short rationale for estimate: Does only one command.
    unsigned int town_count();

    // Estimate of performance: Always O(n)
    // Short rationale for estimate: Clears every town linearly.
    void clear_all();

    // Estimate of performance: Worst case O(n), usually O(1)
    // Short rationale for estimate: Linear if town id is new.
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance: Worst case O(n), usually O(1)
    // Short rationale for estimate: Checks if the id is in towns_
    Name get_town_name(TownID id);

    // Estimate of performance: Worst case O(n), usually O(1)
    // Short rationale for estimate: Checks if the id is in towns
    Coord get_town_coordinates(TownID id);

    // Estimate of performance: Worst case O(n), usually O(1)
    // Short rationale for estimate: Checks if the id is in towns
    int get_town_tax(TownID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Gets info, from every stored town.
    std::vector<TownID> all_towns();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Goes through every town in towns_.
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance: Worst case O(n), usually O(1)
    // Short rationale for estimate: Checks if the id is in towns
    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Heap sort, stores towns in n*log(n) time and sorts in n*log(n) time.
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Heap sort, stores towns in n*log(n) time and sorts in n*log(n) time.
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Goes through every towns' distance
    TownID min_distance();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Goes through every towns' distance
    TownID max_distance();

    // Estimate of performance: Worst case O(n), usually O(1)
    // Short rationale for estimate: Checks if the id's are in towns_.
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance:O(n)
    // Short rationale for estimate: Goes through every vassal.
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: If there is only headmaster and others are vassals in queue performance is linear.
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: Checks if the id is in towns_.
    bool remove_town(TownID id);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Does exactly the same as towns_distance_increasing.
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: While loop is always O(n), getting vassals' vassals is O(log(n)).
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance: O(n*log(n))
    // Short rationale for estimate: Looping is always O(n), getting vassals' vassals is O(log(n)).
    int total_net_tax(TownID id);

    // O(n*log(n)
    // v is vector every town id in towns_.
    // i is place of the first element
    void heap_sort_names(std::vector<TownID> &v, int i);

    // O(n*log(n)
    // v is vector every town id in towns_.
    // i is place of the first element
    void heap_sort_dists(int x, int y, std::vector<TownID> &v, int i);

    // O(n)
    // id = vassal id
    // master = master id
    // Used in remove_town. Pops vassal id from master's vassals.
    void from_master_clear(TownID id, TownID master);

    // O(n*log(n)
    // Used in longest_vassal_path.
    std::vector<TownID> get_path(TownID id);

    // O(log(n)
    // id = vassal id
    // Used in total_net_tax. Calculates vassals net tax.
    // Returns 10% of vassals net tax
    int calc_vassal_taxes(TownID id);

private:
    // Add stuff needed for your class implementation here
    struct Town {
        TownID id_;
        Name name_;
        Coord coord_;
        int tax_;
        TownID master_ = "";
        std::vector<TownID> vassals_ ={};
    };

    unsigned int num_of_towns_ = 0;
    std::unordered_map<TownID,Town*> towns_;
};

#endif // DATASTRUCTURES_HH
