// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <unordered_map>
#include <map>
#include <list>
#include <algorithm>

// Types for IDs
using PlaceID = long long int;
using AreaID = long long int;
using Name = std::string;
using WayID = std::string;

// Return values for cases where required thing was not found
PlaceID const NO_PLACE = -1;
AreaID const NO_AREA = -1;
WayID const NO_WAY = "!!No way!!";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Enumeration for different place types
// !!Note since this is a C++11 "scoped enumeration", you'll have to refer to
// individual values as PlaceType::SHELTER etc.
enum class PlaceType { OTHER=0, FIREPIT, SHELTER, PARKING, PEAK, BAY, AREA, NO_TYPE };

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

// Return value for cases where Duration is unknown
Distance const NO_DISTANCE = NO_VALUE;



// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: complexity of vector::size function
    int place_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: sum of linear clear() -operations
    void clear_all();

    // Estimate of performance: O(1)
    // only returns a vector
    std::vector<PlaceID> all_places();

    // Estimate of performance: on average O(1), worst case O(n)
    // Short rationale for estimate: insertion to unordered_map on average O(1),
    // adding to the end of vector O(1)
    bool add_place(PlaceID id, Name const& name, PlaceType type, Coord xy);

    // Estimate of performance: O(1), worst case O(n)
    // Short rationale for estimate: unordered_map.find() nearly always O(1) as said by lecturer
    // (so it is used quite heavily in this program)
    // comparison of iterators is a constant operation
    std::pair<Name, PlaceType> get_place_name_type(PlaceID id);

    // Estimate of performance: O(1), worst case O(n)
    // Short rationale for estimate: unordered_map.find() nearly always O(1)
    // comparison of iterators is a constant operation
    Coord get_place_coord(PlaceID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n) + O(nlogn) + O(n)*O(1) = O(nlogn)
    // Short rationale for estimate: std::sort should be of O(nlogn) complexity (cppreference)
    // however the algorithm performs quite badly
    std::vector<PlaceID> places_alphabetically();

    // Estimate of performance: O(n) + O(nlogn) + O(n)*O(1) = O(nlogn)
    // Short rationale for estimate: std::sort should be of O(nlogn) complexity (cppreference)
    // however the algorithm performs quite badly
    std::vector<PlaceID> places_coord_order();

    // Estimate of performance: O(n)
    // Short rationale for estimate: comparing every place to the given name
    std::vector<PlaceID> find_places_name(Name const& name);

    // Estimate of performance: O(n)
    // Short rationale for estimate: comparing every place to the given type
    std::vector<PlaceID> find_places_type(PlaceType type);

    // Estimate of performance: O(1), worst case O(n)
    // Short rationale for estimate: unordered_map.find() ~O(1) + iter comparison O(1)
    bool change_place_name(PlaceID id, Name const& newname);

    // Estimate of performance: O(1), worst case O(n)
    // Short rationale for estimate: unordered_map.find() ~O(1) + iter comparison O(1)
    bool change_place_coord(PlaceID id, Coord newcoord);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: on average O(1), worst case O(n)
    // Short rationale for estimate: insertion to unordered_map on average O(1),
    // adding to the end of vector O(1)
    bool add_area(AreaID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(1), worst case O(n)
    // Short rationale for estimate: unordered_map.find() ~O(1) + iter comparison O(1)
    Name get_area_name(AreaID id);

    // Estimate of performance: O(1), worst case O(n)
    // Short rationale for estimate: unordered_map.find() ~O(1) + iter comparison O(1)
    std::vector<Coord> get_area_coords(AreaID id);

    // Estimate of performance: O(1)
    // only return a vector
    std::vector<AreaID> all_areas();

    // Estimate of performance: on average O(1), worst case O(n) + O(n) = O(n)
    // Short rationale for estimate: finding two values from unordered_map (at worst O(n))
    // all other steps should be O(1)
    bool add_subarea_to_area(AreaID id, AreaID parentid);

    // Estimate of performance: O(1), worst case O(n)
    // Short rationale for estimate: unordered_map.find() on avg O(1)
    // comparing parentarea pointers is constant, as well as adding them to the
    // end of the vector, so the whole operation is at best
    // O(1) + O(1)*O(1) = O(1) and at worst O(n) + O(1)*O(1) = O(n)
    std::vector<AreaID> subarea_in_areas(AreaID id);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    void creation_finished();

    // Estimate of performance: O(n²)?
    // Short rationale for estimate: recursive function going through the subareas
    // and calling itself for each subarea found -> O(n)*O(n)?
    std::vector<AreaID> all_subareas_in_area(AreaID id);

    // Estimate of performance: should be O(n log n)
    // Short rationale for estimate: going through the places is linear,
    // sorting them by distance, std::sort should be O(n log n) according to cppreference
    // but the algorithm is very slow
    // then finding the closest three places with type 'type', which is at worst linear
    // if the last place type 'type' is farthest away from the point xy
    // so we get O(n) + O(n log n) + O(n) = O(n log n)
    std::vector<PlaceID> places_closest_to(Coord xy, PlaceType type);

    // Estimate of performance: O(n)
    // Short rationale for estimate: find the place from unordered_map is at worst O(n),
    // finding the placeid in the places_ -vector is at worst linear
    // and erasing the place from the u_map and vect is on average constant and at worst
    // linear, other steps are constant
    // so we get O(n) + O(n) + O(n) = O(n) at worst
    bool remove_place(PlaceID id);

    // Estimate of performance: O(n²)?
    // Short rationale for estimate: at worst comparing through
    // all of the parentareas of the two subarears
    // which is O(n)*O(m)
    AreaID common_area_of_subareas(AreaID id1, AreaID id2);



    // Phase 2 operations



    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<WayID> all_ways();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_way(WayID id, std::vector<Coord> coords);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<WayID, Coord>> ways_from(Coord xy);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<Coord> get_way_coords(WayID id);

    // Estimate of performance:
    // Short rationale for estimate:
    void clear_ways();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID, Distance>> route_any(Coord fromxy, Coord toxy);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_way(WayID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID, Distance>> route_least_crossroads(Coord fromxy, Coord toxy);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID>> route_with_cycle(Coord fromxy);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<Coord, WayID, Distance>> route_shortest_distance(Coord fromxy, Coord toxy);

    // Estimate of performance:
    // Short rationale for estimate:
    Distance trim_ways();

private:

    struct Place {PlaceID id; // for alphabetical and coord orders
                  Name name;
                  PlaceType type;
                  Coord xy;};

    struct Area {AreaID id; // ID needed for gathering subareas
                 Name name;
                 std::vector<Coord> coords = {};
                 Area* parentarea = nullptr;
                 std::vector<Area*> subareas = {};}; 

    struct Way {WayID id;
                std::vector<Coord> coords = {};};

    struct Crossing {bool visited;
                     Coord coords = {};
                     std::unordered_map<Crossing*, std::pair<Distance, WayID>> neighbours = {};};


    // for all_places, place_count and all_areas
    std::vector<PlaceID> places_;
    std::vector<AreaID> areas_;

    // for all_ways, way_coords etc.
    std::unordered_map<WayID, Way> way_map_;

    // main datastructures:

    std::unordered_map<PlaceID, Place> place_map_;
    std::unordered_map<AreaID, Area> area_map_;
    // unordered_map seems to have a hashing problem with either the Coord or the
    // pair in the Crossing, so using just map since the lookup isn't that much slower
    std::map<Coord, Crossing> cross_map_;


    // recursive function going through all of the subareas of a given area
    // and returns their IDs in a vector
    std::vector<AreaID> find_subareas(Area& subarea,
                                      std::vector<AreaID>& subareas_vect);

    // recursive function going through all of the parentareas of the subarea
    // and returning their IDs in a vector
    std::vector<AreaID> find_parentareas(Area& subarea,
                                         std::vector<AreaID>& parentareas_vect);

    // function for finding the first common parentarea of the subareas
    AreaID find_first_common_area(std::vector<AreaID>& first_vect,
                                  std::vector<AreaID>& second_vect);

    // Adds from-crossing to the neighbours of to-crossing and vice versa
    // with the weight of distance
    void connect_crossings(Coord& from, Coord& to, Distance& distance, WayID& id);

    std::vector<Crossing*> find_path(Crossing* from_cross, Crossing* to_cross, std::vector<Crossing*> route);
    // marking all crossings unvisited before calling find_path
    void mark_crossings_white();

    void BFS(Coord& fromxy);

    // returns place1 < place2 by distance from the origin
    bool compare_coords(Place*  place1, Place* place2);

    // returns a pair of the distance to the given point and the place
    std::pair<int, Place*> distance_between_points(Coord xy, Place& place);

    // returns the length of the given way
    int calculate_way_length(Way& way);

    // returns place's distance from the origin
    int distance(int x, int y);


};

#endif // DATASTRUCTURES_HH
