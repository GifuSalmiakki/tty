// Datastructures.cc

#include "datastructures.hh"

#include <random>

#include <cmath>


std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)


Datastructures::Datastructures()
{
    // Replace this comment with your implementation

}

Datastructures::~Datastructures()
{

    // Replace this comment with your implementation
}

int Datastructures::place_count()
{
    return places_.size(); // performace O(1)
}

void Datastructures::clear_all()
{
    // All linear
    area_map_.clear();
    place_map_.clear();
    places_.clear();
    areas_.clear();

}

std::vector<PlaceID> Datastructures::all_places()
{
    return places_;
}

bool Datastructures::add_place(PlaceID id, const Name& name, PlaceType type, Coord xy)
{

    Place place_to_add = {id, name, type, xy};

    // checking if the place already exists i.e. if the insertion works
    if (place_map_.insert({id, place_to_add}).second) // on average constant
    {
        places_.push_back(place_to_add.id); // performance O(1)
        return true;
    }
    return false;
}

std::pair<Name, PlaceType> Datastructures::get_place_name_type(PlaceID id)
{

    std::unordered_map<PlaceID, Place>::iterator iter = place_map_.find(id);
    // O(n), on average constant

    if (iter != place_map_.end()) // performance O(1)
    {
        return {iter->second.name, iter->second.type};
    }
    return {NO_NAME, PlaceType::NO_TYPE};
}

Coord Datastructures::get_place_coord(PlaceID id)
{

    std::unordered_map<PlaceID, Place>::iterator iter = place_map_.find(id);
    // O(n), on average constant

    if (iter != place_map_.end()) // performance O(1)
    {
        return iter->second.xy;
    }
    return NO_COORD;
}

bool Datastructures::add_area(AreaID id, const Name &name, std::vector<Coord> coords)
{
    Area area_to_add = {id, name, coords};

    // checking if the area already exists i.e. if the insertion works
    if (area_map_.insert({id, area_to_add}).second) // on average constant
    {
        areas_.push_back(area_to_add.id); // performance O(1)
        return true;
    }
    return false;
}

Name Datastructures::get_area_name(AreaID id)
{

    std::unordered_map<AreaID, Area>::iterator iter  = area_map_.find(id);
    // O(n), on average constant

    if (iter != area_map_.end()) // O(1)
    {
        return iter->second.name;
    }

    return NO_NAME;
}

std::vector<Coord> Datastructures::get_area_coords(AreaID id)
{
    std::unordered_map<AreaID, Area>::iterator iter  = area_map_.find(id);
    // O(n), on average constant

    if (iter != area_map_.end()) // performance O(1)
    {
        return iter->second.coords;
    }
    return {NO_COORD};
}

void Datastructures::creation_finished()
{
    // Replace this comment with your implementation
    // NOTE!! It's quite ok to leave this empty, if you don't need operations
    // that are performed after all additions have been done.
}


std::vector<PlaceID> Datastructures::places_alphabetically()
{
    std::vector<Place*> places_to_sort = {};
    std::vector<PlaceID> sorted_ids = {};

    for (auto& place : place_map_) // linear
    {
        places_to_sort.push_back(&place.second); // O(1)
    }
    // std::sort should be O(n log n) but performs very slow (cannot figure out why)
    std::sort(places_to_sort.begin(), places_to_sort.end(),
              [](Place* place1, Place* place2) {return place1->name < place2->name;});

    for (auto& place : places_to_sort) // linear
    {
        sorted_ids.push_back(place->id); // O(1)
    }
    return sorted_ids;
}

std::vector<PlaceID> Datastructures::places_coord_order()
{
    std::vector<Place*> places_to_sort = {};
    std::vector<PlaceID> sorted_ids = {};

    for (auto& place : place_map_) // linear
    {
        places_to_sort.push_back(&place.second); // O(1)
    }
    // std::sort should be O(n log n) but perftests very slow (cannot figure out why)
    std::sort(places_to_sort.begin(), places_to_sort.end(),
              [this](Place* place1, Place* place2) {return compare_coords(place1, place2);});

    for (auto& place : places_to_sort) // linear
    {
        sorted_ids.push_back(place->id); // O(1)
    }
    return sorted_ids;
}

std::vector<PlaceID> Datastructures::find_places_name(Name const& name)
{

    std::unordered_map<PlaceID, Place>::iterator iter = place_map_.begin(); // O(1)
    std::vector<PlaceID> places_named = {};

    // going through the place_map and checking at each place
    // if they correspond to the searched name
    while (iter != place_map_.end()) // linear
    {
        if (iter->second.name == name)
        {
            places_named.push_back(iter->first); // О(1)
        }
        ++iter;
    }
    return places_named;
}

std::vector<PlaceID> Datastructures::find_places_type(PlaceType type)
{
    std::unordered_map<PlaceID, Place>::iterator iter = place_map_.begin(); // O(1)
    std::vector<PlaceID> places_type = {};

    // going through the place_map and checking at each place
    // if they correspond to the searched type
    while (iter != place_map_.end()) // linear
    {
        if (iter->second.type == type)
        {
            places_type.push_back(iter->first); // О(1)
        }
        ++iter;
    }
    return places_type;
}

bool Datastructures::change_place_name(PlaceID id, const Name& newname)
{
    std::unordered_map<PlaceID, Place>::iterator iter = place_map_.find(id);
    // on average constant, worst case O(n)
    if (iter != place_map_.end()) // O(1)
    {
        iter->second.name = newname;
        return true;
    }
    return false;
}

bool Datastructures::change_place_coord(PlaceID id, Coord newcoord)
{
    std::unordered_map<PlaceID, Place>::iterator iter = place_map_.find(id);
    // on average constant, worst case O(n)
    if (iter != place_map_.end()) // O(1)
    {
        iter->second.xy = newcoord;
        return true;
    }
    return false;
}

std::vector<AreaID> Datastructures::all_areas()
{
    return areas_;
}

bool Datastructures::add_subarea_to_area(AreaID id, AreaID parentid)
{
    std::unordered_map<AreaID, Area>::iterator iter = area_map_.find(parentid);
    // constant on average, worst case O(n)
    std::unordered_map<AreaID, Area>::iterator subarea_iter = area_map_.find(id);
    // constant on average, O(n)

    if (iter != area_map_.end() && subarea_iter != area_map_.end()) // O(1)?
    {
        // checking that the area is not yet enclose in another area and
        // that we're not adding the area to be its own subarea
        if (subarea_iter->second.parentarea == nullptr && id != parentid) // O(1)
        {
            // saving the knowledge of sub- and parentarea as pointers
            Area* subarea_to_add = &subarea_iter->second;
            Area* parentarea = &iter->second;

            /* the parentarea has a vector containing pointers to all of its subareas
            * and the subarea has a pointer to the parentarea, since there
            * can only be one direct parent
            */
            parentarea->subareas.push_back(subarea_to_add); // O(1)
            subarea_to_add->parentarea = parentarea;
            return true;
        }
    }
    return false;
}

std::vector<AreaID> Datastructures::subarea_in_areas(AreaID id)
{
    std::unordered_map<AreaID, Area>::iterator iter  = area_map_.find(id); // on average constant, worst case O(n)
    std::vector<AreaID> parentareas = {};
    // going recursively through the parentareas and gathering them to a vector
    return find_parentareas(iter->second, parentareas);
}

std::vector<PlaceID> Datastructures::places_closest_to(Coord xy, PlaceType type)
{
    std::vector<PlaceID> closest_places = {};
    std::vector<std::pair<int, Place*>> distances = {};
    for (auto& place : place_map_) // linear
    {
        distances.push_back(distance_between_points(xy, place.second)); // const
    }
    // sorting the vector of the distances by size aka the closeness
    // should be O(n log n) but perftests very slow
    std::sort(distances.begin(), distances.end(),
              [](std::pair<int, Place*> pair1, std::pair<int, Place*> pair2)
    {return pair1.first < pair2.first;});

    // if no type is given we gather the first three elements
    if (type == PlaceType::NO_TYPE)
    {
        for (int i=0; i<3; ++i)
        {
            closest_places.push_back(distances[i].second->id); // O(1)
        }
    }
    // otherwise we gather the first three places with a corresponding type
    else
    {
        // at worst linear, if there are not three places of the type 'type'
        for (unsigned i=0; i < distances.size(); ++i)
        {
            if (distances[i].second->type == type && closest_places.size() < 3)
            {
                closest_places.push_back(distances[i].second->id); // O(1)
            }
        }
    }
    return closest_places;
}

bool Datastructures::remove_place(PlaceID id)
{
    std::unordered_map<PlaceID, Place>::iterator to_be_removed = place_map_.find(id);
    // on average constant, worst case O(n)
    std::vector<PlaceID>::iterator iter = places_.begin();

    // removing the place from unordered_map
    if (to_be_removed != place_map_.end()) // O(1)
    {
        place_map_.erase(to_be_removed); // on average constant, worst case O(n)

        // removing the placeid from places_ -vector
        for (unsigned i = 0; i < places_.size(); ++i) // at worst O(n)
        {
            if (places_[i] == id)
            {
                places_.erase(iter); // O(1)
            }++iter;
        }
        return true;
    }
    return false;
}

std::vector<AreaID> Datastructures::all_subareas_in_area(AreaID id)
{
    std::vector<AreaID> all_subareas = {};
    std::unordered_map<AreaID, Area>::iterator iter  = area_map_.find(id); // constant on average, worst case O(n)

    // going recursively through the subareas and gathering them all in a vector
    return find_subareas(iter->second, all_subareas); // O(n²)?
}

AreaID Datastructures::common_area_of_subareas(AreaID id1, AreaID id2)
{
    std::unordered_map<AreaID, Area>::iterator first_area = area_map_.find(id1);
    std::unordered_map<AreaID, Area>::iterator second_area = area_map_.find(id2);
    // at worst O(n), on average O(1)

    std::vector<AreaID> first_area_parents = {};
    std::vector<AreaID> second_area_parents = {};

    first_area_parents = find_parentareas(first_area->second, first_area_parents);
    second_area_parents = find_parentareas(second_area->second, second_area_parents);

    return find_first_common_area(first_area_parents, second_area_parents);
}

std::vector<AreaID> Datastructures::find_subareas(Datastructures::Area &area,
                                                  std::vector<AreaID> &subareas_vect)
{
    // linear in recursive = O(n²)? seems to perform okay in the perftests

    // checking if we've found the lowest area in this branch
    if (area.subareas.size() != 0) // O(1)
    {
        for (auto subarea : area.subareas) // linear
        {
            subareas_vect.push_back(subarea->id); // O(1)
            find_subareas(*subarea, subareas_vect);
        }
    }return subareas_vect;
}

std::vector<AreaID> Datastructures::find_parentareas(Datastructures::Area &subarea,
                                                     std::vector<AreaID> &parentareas_vect)
{
    // checking if we've found the area with no parent (by default nullptr)
    if (subarea.parentarea != nullptr) // O(1)
    {
        AreaID parent_id = subarea.parentarea->id;
        parentareas_vect.push_back(parent_id); // O(1)
        find_parentareas(*subarea.parentarea, parentareas_vect); // O(1)?
    }
    return parentareas_vect;
}

AreaID Datastructures::find_first_common_area(std::vector<AreaID> &first_vect,
                                              std::vector<AreaID> &second_vect)
{
    // O(n²) oof :/

    for (unsigned i = 0; i < first_vect.size(); ++i)
    {
        for (unsigned j = 0; j < second_vect.size(); ++j)
        {
            if (first_vect[i] == second_vect[j])
            {
                return first_vect[i];
            }
        }
    }return NO_AREA;
}



// -------------------------------- prog2 starts here --------------------------------




std::vector<WayID> Datastructures::all_ways()
{
    std::vector<WayID> allways;

    for (auto&& way : way_map_) // O(n)
    {
        allways.push_back(way.first); // O(1)
    }
    return allways;
}

bool Datastructures::add_way(WayID id, std::vector<Coord> coords)
{

    Way way_to_add = {id, coords};

    std::map<Coord, Crossing>::iterator iter_from = cross_map_.find(coords[0]); // O(log n)
    std::map<Coord, Crossing>::iterator iter_to = cross_map_.find(coords[1]); // O(log n)

    // checking if the way already exists i.e. if the insertion works
    if (way_map_.insert({id, way_to_add}).second) // on average constant
    {
        // if the way is not connected from either end to a crossing
        // we add a new crossing to those coordinates
        if (iter_from == cross_map_.end())
        {
            // adding a new crossing to the beginning of the path
            Crossing new_from = {false, coords[0]};
            cross_map_.insert({coords[0], new_from});
        }
        if (iter_to == cross_map_.end())
        {
            // adding a new crossing to the end of the path
            Crossing new_to = {false, coords[1]};
            cross_map_.insert({coords[1], new_to});
        }

        // connecting the two crossings at the beginning and end of the way
        // and giving the way (edge) between them the weight of the distance between them

        Distance distance = calculate_way_length(way_to_add);
        connect_crossings(coords[0], coords[1], distance, id); // O(log n)

        return true;
    }
    return false;
}

std::vector<std::pair<WayID, Coord>> Datastructures::ways_from(Coord xy)
{

    std::vector<std::pair<WayID, Coord>> ways_from_coord = {};
    std::pair<WayID, Coord> found_way;
    std::unordered_map<WayID, Way>::iterator iter = way_map_.begin();


    // checking for each way if their beginning or end coordinates
    // correspond to the ones given
    while (iter != way_map_.end()) // O(n)
    {

        if (iter->second.coords[0] == xy) // maybe make this nicer later?
        {
            found_way = std::make_pair(iter->second.id, iter->second.coords[1]);
            ways_from_coord.push_back(found_way);
        }
        else if (iter->second.coords[1] == xy)
        {
            found_way = std::make_pair(iter->second.id, iter->second.coords[0]);
            ways_from_coord.push_back(found_way);
        }
        ++iter;
    }

    return {ways_from_coord};
}

std::vector<Coord> Datastructures::get_way_coords(WayID id)
{
    std::unordered_map<WayID, Way>::iterator iter = way_map_.find(id);
    // on average O(1), worst case O(n)

    if (iter != way_map_.end()) // O(1)
    {
        return iter->second.coords;
    }
    return {NO_COORD};
}

void Datastructures::clear_ways()
{
    way_map_.clear(); // O(n)
    cross_map_.clear(); // O(n)
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_any(Coord fromxy, Coord toxy)
{
    std::vector<Crossing*> crossings = {};
    std::vector<std::tuple<Coord, WayID, Distance>> route = {};
    mark_crossings_white(); // O(n)

    Crossing* from_cross = &cross_map_.find(fromxy)->second; // O(log n)
    Crossing* to_cross = &cross_map_.find(toxy)->second; // O(log n)

    crossings = find_path(from_cross, to_cross, crossings);

    if (crossings.size() != 0) // if a path was not found
    {
        for (unsigned int i = 0; i < crossings.size()-1; ++i)
        {
            Coord coords = crossings[i]->coords;
            WayID id = crossings[i]->neighbours.find(crossings[i+1])->second.second;
            Distance dist = crossings[i]->neighbours.find(crossings[i+1])->second.first;

            std::tuple<Coord, WayID, Distance> path_between = std::make_tuple(coords, id, dist);
            route.push_back(path_between);
        }
        return route;
    }
    //return {{NO_COORD, NO_WAY, NO_DISTANCE}};

}


bool Datastructures::remove_way(WayID id)
{
    // Replace this comment with your implementation
    return false;
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_least_crossroads(Coord fromxy, Coord toxy)
{
    // Replace this comment with your implementation
    return {{NO_COORD, NO_WAY, NO_DISTANCE}};
}

std::vector<std::tuple<Coord, WayID> > Datastructures::route_with_cycle(Coord fromxy)
{
    // Replace this comment with your implementation
    return {{NO_COORD, NO_WAY}};
}

std::vector<std::tuple<Coord, WayID, Distance> > Datastructures::route_shortest_distance(Coord fromxy, Coord toxy)
{
    // Replace this comment with your implementation
    return {{NO_COORD, NO_WAY, NO_DISTANCE}};
}

Distance Datastructures::trim_ways()
{
    // Replace this comment with your implementation
    return NO_DISTANCE;
}


// Additional helper-functions

bool Datastructures::compare_coords(Datastructures::Place* place1, Datastructures::Place* place2)
{

    int distance1 = distance(place1->xy.x, place1->xy.y);
    int distance2 = distance(place2->xy.x, place2->xy.y);

    // if the distance is the same, we compare y coordinates
    if (distance1 == distance2)
    {
        return (place1->xy.y < place2->xy.y);
    }
    else
    {
        return (distance1 < distance2);
    }
}

std::pair<int, Datastructures::Place*> Datastructures::distance_between_points
(Coord xy, Datastructures::Place &place)
{
    int delta_x = xy.x-place.xy.x;
    int delta_y = xy.y-place.xy.y;

    int distance_between = distance(delta_x, delta_y);

    return std::make_pair(distance_between, &place);
}

int Datastructures::calculate_way_length(Datastructures::Way &way)
{
    int delta_x = way.coords[1].x - way.coords[0].x;
    int delta_y = way.coords[1].y - way.coords[0].y;
    std::floor(delta_x); // rounding down
    std::floor(delta_y);

    return distance(delta_x, delta_y);
}

int Datastructures::distance(int x, int y)
{
    int pow_x = pow(x, 2); // x²
    int pow_y = pow(y, 2); // y²
    return sqrt(pow_x+pow_y);
}

void Datastructures::connect_crossings(Coord &from, Coord &to, Distance& dist, WayID& id)
{
    std::map<Coord, Crossing>::iterator from_iter = cross_map_.find(from); // O(log n)
    Crossing* from_crossing = &from_iter->second;

    std::map<Coord, Crossing>::iterator to_iter = cross_map_.find(to); // O(log n)
    Crossing* to_crossing = &to_iter->second;

    std::pair<Distance, WayID> neighbour_from = std::make_pair(dist, id);
    std::pair<Distance, WayID> neighbour_to = std::make_pair(dist, id);

    from_crossing->neighbours.insert({to_crossing, neighbour_to}); // O(1)
    to_crossing->neighbours.insert({from_crossing, neighbour_from});
}

std::vector<Datastructures::Crossing*> Datastructures::find_path(Datastructures::Crossing *from_cross,
                                                                 Datastructures::Crossing *to_cross,
                                                                 std::vector<Crossing*> route)
{
    route.push_back(from_cross);
    from_cross->visited = true;

    if (from_cross != to_cross && from_cross != nullptr) // until we've found the destination
    {
        if (! from_cross->neighbours.empty()) // if the current crossing is not a dead-end
        {
            std::unordered_map<Crossing*, std::pair<Distance, WayID>>::iterator iter;
            for (iter = from_cross->neighbours.begin(); iter != from_cross->neighbours.end(); ++iter)
            {
                if (! iter->first->visited) // finding a new path
                {
                    find_path(iter->first, to_cross, route);
                }
            }
        }route.pop_back(); // if we've come to a dead-end which is not the target crossing
    }
    return route;
}


void Datastructures::mark_crossings_white()
{
    for (auto crossing : cross_map_) // O(n)
    {
        crossing.second.visited = false;
    }
}



/*
void Datastructures::BFS(Coord& fromxy)
{
    std::list<Crossing> queue; // crossings to go through
    std::vector<std::tuple<Crossing*, Distance, WayID>> path; // storing found path

    std::map<Coord, Crossing>::iterator start_iter = cross_map_.find(fromxy); // O(log n)
    std::vector<std::tuple<Crossing*, Distance, WayID>>::iterator nb_iter; // going thru neighbouring crossings


    queue.push_back(start_iter->second);

    // while there are unvisited crossings
    while (!queue.empty())
    {
        // moving to the next crossing
        Crossing current_point = queue.front();
        queue.pop_front();

        // if the current crossing isn't a dead-end
        if (current_point.neighbours.size() != 0)
        {
            // going through the neighbouring crossings of the current crossing
            for (nb_iter = current_point.neighbours.begin(); nb_iter != current_point.neighbours.end();
                 ++nb_iter)
            {
                // if the neighbouring crossing has not been visited yet
                // we mark it visited and add it to the waiting list so that its
                // neighbours will be checked as well
                Crossing* neighbour = std::get<0>(*nb_iter);

                if (!neighbour->visited)
                {
                    neighbour->visited = true;
                    queue.push_back(*neighbour);
                }

            }
        }

}*/


