// Datastructures.cc
//
// Student name: Konsta Karjalainen
// Student email: konsta.karjalainen@tuni.fi
// Student number: 284983

#include "datastructures.hh"

#include <random>
#include <algorithm>
#include <cmath>
#include <climits>
#include <deque>


std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

const int origo_x = 0;
const int origo_y = 0;

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
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    for(auto it = towns_.begin();it != towns_.end();){
        delete it->second;
        it++;
    }
    clear_all();
}

unsigned int Datastructures::town_count()
{
    return num_of_towns_;
}

void Datastructures::clear_all()
{
    clear_roads();
    towns_.clear();
    num_of_towns_ = 0;
}

bool Datastructures::add_town(TownID id, Name const &name, Coord coord, int tax)
{
    if (towns_.find(id) == towns_.end()){
        Town* new_town = new Town{id, name, coord, tax};
        towns_[id] = new_town;
        num_of_towns_++;
        return true;
    } else {
        return false;
    }
}

Name Datastructures::get_town_name(TownID id)
{
    if (towns_.find(id) != towns_.end()){
        Name name = towns_[id]->name_;
        return name;
    }else{
        return NO_NAME;
    }

}

Coord Datastructures::get_town_coordinates(TownID id)
{
    if (towns_.find(id) != towns_.end()){
        Coord coord = towns_[id]->coord_;
        return coord;
    }else{
        return NO_COORD;
    }
}

int Datastructures::get_town_tax(TownID id)
{
    if (towns_.find(id) != towns_.end()){
        int tax = towns_[id]->tax_;
        return tax;
    }else{
        return NO_VALUE;
    }
}

std::vector<TownID> Datastructures::all_towns()
{
    std::vector<TownID> towns;
    for(auto town:towns_){
        towns.push_back(town.first);
    }
    return towns;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{
    std::vector<TownID> towns;
    for(auto it = towns_.begin();it != towns_.end();it++){
        if (name == it->second->name_){
            towns.push_back(it->first);
        }
    }
    return towns;
}

bool Datastructures::change_town_name(TownID id, const Name &newname)
{
    if (towns_.find(id) != towns_.end()){
        towns_[id]->name_ = newname;
        return true;
    }else{
        return false;
    }
}

void Datastructures::heap_sort_names(std::vector<TownID> &v, int i)
{
    TownID largest;
    int index = 2*i+1;
    int size = v.size();

    while (index<size){
        TownID &parent = v[i];
        TownID &left = v[index];
        TownID &right = v[index+1];

        auto l = towns_[left]->name_;
        auto r = towns_[right]->name_;
        auto p = towns_[parent]->name_;

        if (index+1 == size){
            if (l<p){
                largest = left;
                std::swap(parent,left);
            }else
                break;
        }else {
            if (l < r){
                largest = left;
                std::swap(parent,left);
            }else {
                largest = right;
                std::swap(parent,right);
                index++;
            }
        }
        i = index;
        index = 2*i+1;
    }
}


std::vector<TownID> Datastructures::towns_alphabetically()
{
    std::vector<TownID> towns_ordered;
    std::vector<TownID> towns;

    for(auto i = towns_.begin();i != towns_.end();i++){
        TownID id = i->first;
        towns.push_back(id);
        int c_index = towns.size() - 1;
        int p_index;
        if (c_index>0){
            if (c_index%2==0){
                p_index = c_index/2-1;
            }
            else{
                p_index = c_index/2;
            }
            auto c_name = get_town_name(towns.at(c_index));
            auto p_name = get_town_name(towns.at(p_index));
            while (p_name > c_name){
                swap(towns.at(c_index),towns.at(p_index));
                c_index = p_index;
                if (c_index%2==0)
                    p_index = c_index/2-1;
                else
                    p_index = c_index/2;
                if (p_index<0)
                    break;
                c_name = get_town_name(towns.at(c_index));
                p_name = get_town_name(towns.at(p_index));
            }
        }
    }
    while (towns.size()>0){
        towns_ordered.push_back(towns.front());
        swap(towns.front(),towns.back());
        towns.pop_back();
        heap_sort_names(towns,0);
    }
    return towns_ordered;

}


int calculate_distance(int x1,int y1,int x2,int y2){
    int in_root = ((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1));
    double d = sqrt(in_root);
    int d_int = int(trunc(d));
    return d_int;
}


void Datastructures::heap_sort_dists(int x, int y, std::vector<TownID> &v, int i)
{
    TownID largest;
    int index = 2*i+1;
    int size = v.size();

    while (index<size){
        auto &parent = v[i];
        auto &left = v[index];
        auto &right = v[index+1];
        Coord l = towns_[left]->coord_;
        Coord r = towns_[right]->coord_;
        Coord p = towns_[parent]->coord_;
        int dist_l = calculate_distance(x,y,l.x,l.y);
        int dist_r = calculate_distance(x,y,r.x,r.y);
        int dist_p = calculate_distance(x,y,p.x,p.y);

        if (index+1 == size){
            if (dist_l < dist_p){
            largest = left;
            std::swap(parent,left);
            }else
                break;
        }else {
            if (dist_l < dist_r){
                largest = left;
                std::swap(parent,left);
            }else {
                largest = right;
                std::swap(parent,right);
                index++;
            }
        }
        i = index;
        index = 2*i+1;
    }
}


std::vector<TownID> Datastructures::towns_distance_increasing()
{
    std::vector<TownID> dists_ordered;
    std::vector<TownID> towns;

    for (auto i = towns_.begin();i != towns_.end();i++){
        TownID id = i->first;
        towns.push_back(id);
        int c_index = towns.size() - 1;
        int p_index;
        if (c_index>0){
            if (c_index%2==0){
                p_index = c_index/2-1;
            }
            else{
                p_index = c_index/2;
            }
            auto c_coords = get_town_coordinates(towns.at(c_index));
            auto p_coords = get_town_coordinates(towns.at(p_index));
            auto c_dist = calculate_distance(origo_x,origo_y,c_coords.x,c_coords.y);
            auto p_dist = calculate_distance(origo_x,origo_y,p_coords.x,p_coords.y);

            while (c_dist < p_dist){
                swap(towns.at(c_index),towns.at(p_index));
                c_index = p_index;
                if (c_index%2==0)
                    p_index = c_index/2-1;
                else
                    p_index = c_index/2;
                if (p_index<0)
                    break;
                c_coords = get_town_coordinates(towns.at(c_index));
                p_coords = get_town_coordinates(towns.at(p_index));
                c_dist = calculate_distance(origo_x,origo_y,c_coords.x,c_coords.y);
                p_dist = calculate_distance(origo_x,origo_y,p_coords.x,p_coords.y);
            }
        }
    }
    while (towns.size()>0){
        dists_ordered.push_back(towns.front());
        swap(towns.front(),towns.back());
        towns.pop_back();
        heap_sort_dists(origo_x,origo_y,towns,0);
    }
    return dists_ordered;
}

TownID Datastructures::min_distance()
{
    if (town_count() > 0){
        TownID min_dist_town = "";
        int min = INT_MAX;

        for (auto pair:towns_){
            Town* town = pair.second;
            Coord coord = town->coord_;
            int dist = calculate_distance(origo_x,origo_y,coord.x,coord.y);
            if (dist < min){
                min_dist_town = pair.first;
                min = dist;
            }
        }
        return min_dist_town;
    }else {
        return NO_TOWNID;
    }


}

TownID Datastructures::max_distance()
{
    if (town_count() > 0){
        TownID max_dist_town = "";
        int max = 0;

        for (auto pair:towns_){
            Town* town = pair.second;
            Coord coord = town->coord_;
            int dist = calculate_distance(origo_x,origo_y,coord.x,coord.y);
            if (dist > max){
                max_dist_town = pair.first;
                max = dist;
            }
        }
        return max_dist_town;
    }else {
        return NO_TOWNID;
    }

}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{

    if (towns_.find(vassalid)!=towns_.end() && towns_.find(masterid)!=towns_.end() && towns_[vassalid]->master_ == ""){
        auto vassal = towns_[vassalid];
        auto master = towns_[masterid];
        vassal->master_ = masterid;
        master->vassals_.push_back(vassalid);
        return true;
    }else {
        return false;
    }
}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    std::vector<TownID> towns;

    if (towns_.find(id) != towns_.end()){
        auto town = towns_[id];
        auto vassals = town->vassals_;
        for (auto vassal:vassals){
            towns.push_back(vassal);
        }
    }else {
        towns.push_back(NO_TOWNID);
    }
    return towns;
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    std::vector<TownID> towns;

    if (towns_.find(id) != towns_.end()){
        TownID town_id = id;
        towns.push_back(town_id);
        while (towns_[town_id]->master_ != ""){
            town_id = towns_[town_id]->master_;
            towns.push_back(town_id);
        }
    }else {
        towns.push_back(NO_TOWNID);
    }return towns;
}


void Datastructures::from_master_clear(TownID id, TownID master){
    std::vector<TownID> &v = towns_[master]->vassals_;

    if (v.back()==id){
        v.pop_back();
    }else{
        for (auto it = v.begin();it!=v.end();it++){
            if (*it == id){
                swap(*it,v.back());
                v.pop_back();
                break;
            }
        }
    }

}


bool Datastructures::remove_town(TownID id)
{

    if (towns_.find(id) != towns_.end()){
        auto town = towns_[id];
        TownID master = town->master_;
        auto vassals = town->vassals_;

        if(vassals.size()>0){
            if (master!=""){
                from_master_clear(id,master);
            }
            for (auto vassal:vassals){
                towns_[vassal]->master_= master;
                if (master!=""){
                    towns_[master]->vassals_.push_back(vassal);
                }
            }
        }else{
            if (master!=""){
                from_master_clear(id,master);
            }
        }
        delete towns_[id];
        towns_.erase(id);
        num_of_towns_--;
        return true;
    }else
        return false;
}

std::vector<TownID> Datastructures::towns_nearest(Coord coord)
{
    std::vector<TownID> dists_ordered;
    std::vector<TownID> towns;

    for(auto i = towns_.begin();i != towns_.end();i++){
        TownID id = i->first;
        towns.push_back(id);
        int c_index = towns.size() - 1;
        int p_index;
        if (c_index>0){
            if (c_index%2==0){
                p_index = c_index/2-1;
            }
            else{
                p_index = c_index/2;
            }
            auto c_coords = get_town_coordinates(towns.at(c_index));
            auto p_coords = get_town_coordinates(towns.at(p_index));

            auto c_dist = calculate_distance(coord.x,coord.y,c_coords.x,c_coords.y);
            auto p_dist = calculate_distance(coord.x,coord.y,p_coords.x,p_coords.y);

            while (c_dist < p_dist){
                swap(towns.at(c_index),towns.at(p_index));
                c_index = p_index;
                if (c_index%2==0)
                    p_index = c_index/2-1;
                else
                    p_index = c_index/2;
                if (p_index<0)
                    break;
                c_coords = get_town_coordinates(towns.at(c_index));
                p_coords = get_town_coordinates(towns.at(p_index));
                c_dist = calculate_distance(coord.x,coord.y,c_coords.x,c_coords.y);
                p_dist = calculate_distance(coord.x,coord.y,p_coords.x,p_coords.y);
            }
        }
    }
    while (towns.size()>0){
        dists_ordered.push_back(towns.front());
        swap(towns.front(),towns.back());
        towns.pop_back();
        heap_sort_dists(coord.x,coord.y,towns,0);
    }
    return dists_ordered;
}


std::vector<TownID> Datastructures::get_path(TownID id){
    std::vector<TownID> longest_path;
    std::vector<TownID> path_v;
    auto vassals = towns_[id]->vassals_;
    int i = 0;
    int size = vassals.size();
    longest_path.push_back(id);
    int path_size = path_v.size();

    while (size>i){
        auto &id_v = vassals[i];
        auto new_path = get_path(id_v);
        int new_size = new_path.size();
        if (new_size > path_size){
            path_v = new_path;
            path_size = path_v.size();
        }
        i++;
    }
    longest_path.insert(longest_path.end(),path_v.begin(),path_v.end());

    return longest_path;
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID id)
{
    std::vector<TownID> longest_path;
    if (towns_.find(id) != towns_.end()){
        longest_path = get_path(id);
    }else {
        longest_path.push_back(NO_TOWNID);
    }return longest_path;
}

int Datastructures::calc_vassal_taxes(TownID id){
    int taxes = get_town_tax(id);
    auto town = towns_[id];
    if (town->vassals_.size()>0){
        for (auto vassal:town->vassals_){
            taxes = taxes + calc_vassal_taxes(vassal);
        }
    }

    return int(trunc(0.1*taxes));
}


int Datastructures::total_net_tax(TownID id)
{
    if (towns_.find(id) != towns_.end()){
        int net_tax = get_town_tax(id);
        auto town = towns_[id];
        if (town->vassals_.size()>0){
            for (auto vassal:town->vassals_){
                net_tax = net_tax + calc_vassal_taxes(vassal);
            }
        }
        if (town->master_=="")
            return net_tax;
        else
            return (net_tax - int(trunc(0.1*net_tax)));
    }else {
        return NO_VALUE;
    }
}


//
// Phase 2 operations
//


void Datastructures::clear_roads()
{
    roads_.clear();
    for (auto pair:towns_){
        pair.second->neighbors_.clear();
        pair.second->neighbors_.shrink_to_fit();
    }
}

std::vector<std::pair<TownID, TownID>> Datastructures::all_roads()
{
    std::vector<std::pair<TownID,TownID>> roads;
    for(auto ptr:roads_){
        std::pair<TownID,TownID> road;
        road.first = ptr->first_;
        road.second = ptr->second_;
        roads.push_back(road);
    }
    return roads;
}

bool Datastructures::add_road(TownID town1, TownID town2)
{
    if (towns_.find(town1)!=towns_.end() && towns_.find(town2)!=towns_.end()){
        Town* t1 = towns_[town1];
        Town* t2 = towns_[town2];
        auto it = find(t1->neighbors_.begin(),t1->neighbors_.end(),town2);
        if (it == t1->neighbors_.end()){
            t1->neighbors_.push_back(town2);
            t2->neighbors_.push_back(town1);
            if (town1<town2){
                Road* new_road = new Road{town1,town2};
                roads_.insert(new_road);
            }else{
                Road* new_road = new Road{town2,town1};
                roads_.insert(new_road);
            }return true;
        }else
            return false;
    }else
        return false;
}

std::vector<TownID> Datastructures::get_roads_from(TownID id)
{
    std::vector<TownID> towns;

    if (towns_.find(id)!=towns_.end()){
        towns = towns_[id]->neighbors_;
    }else{
        towns.push_back(NO_TOWNID);
    }
    return towns;
}

std::vector<TownID> Datastructures::get_route(TownID fromid, TownID toid)
{
    std::vector<TownID> route;
    int i = 0;
    towns_[fromid]->color_=GRAY;
    route.push_back(fromid);
    auto neighbors = towns_[fromid]->neighbors_;
    int size = neighbors.size();

    while (size>i){
        TownID id = neighbors.at(i);
        if (id == toid){
            route.push_back(id);
            return route;
        }if (towns_[id]->color_ == WHITE){
            std::vector<TownID> new_route;
            new_route = get_route(id,toid);

            // Route has been found
            if (new_route.size()>1){
                route.insert(route.end(),new_route.begin(),new_route.end());
                return route;
            }
        }
        i++;
    }route.clear();
    return route;
}

void Datastructures::reset_helpers(){
    for (auto pair:towns_){
        pair.second->color_ = WHITE;
        pair.second->from_="";
    }
}

std::vector<TownID> Datastructures::any_route(TownID fromid, TownID toid)
{

    std::vector<TownID> towns;

    if (towns_.find(fromid)!=towns_.end() && towns_.find(toid)!=towns_.end()){
        std::vector<TownID> route;
        route = get_route(fromid,toid);
        if (route.size()>1)
            towns = route;
    }else{
        towns.push_back(NO_TOWNID);
    }
    reset_helpers();
    return towns;
}

bool Datastructures::remove_road(TownID town1, TownID town2)
{
    if (towns_.find(town1)!=towns_.end() && towns_.find(town2)!=towns_.end()){
        Town* t1 = towns_[town1];
        Town* t2 = towns_[town2];
        auto it1 = std::find(t1->neighbors_.begin(),t1->neighbors_.end(),town2);
        if (it1 != t1->neighbors_.end()){
            t1->neighbors_.erase(it1);
            auto it2 = std::find(t2->neighbors_.begin(),t2->neighbors_.end(),town1);
            t2->neighbors_.erase(it2);

            // Deletes road from roads_
            if (town1<town2){
                for (auto ptr:roads_){
                    if (ptr->first_ == town1 && ptr->second_ == town2){
                        roads_.erase(ptr);
                        delete ptr;
                    }
                }
            }else{
                for (auto ptr:roads_){
                    if (ptr->first_ == town2 && ptr->second_ == town1){
                        roads_.erase(ptr);
                        delete ptr;
                    }
                }
            }
            return true;
        }else
            return false;
    }else{
        return false;
    }
}

std::vector<TownID> Datastructures::get_route_least(TownID fromid, TownID toid)
{

    std::vector<TownID> route;
    std::deque<TownID> que;
    towns_[fromid]->color_=GRAY;
    que.push_back(fromid);

    while (que.size()>0){
        auto town = que.front();
        que.pop_front();
        auto neighbors = towns_[town]->neighbors_;

        for (TownID nb: neighbors){
            auto nb_ptr = towns_[nb];
            if (nb_ptr->color_ == WHITE){
                que.push_back(nb);
                nb_ptr->from_= town;
                nb_ptr->color_=GRAY;

                if (nb == toid){
                    route.push_back(nb);
                    TownID from_id = nb_ptr->from_;
                    while (from_id != fromid){
                        route.push_back(from_id);
                        from_id = towns_[from_id]->from_;
                    }
                    route.push_back(fromid);
                    std::reverse(route.begin(),route.end());
                    return route;
                }
            }
        }
    }
    return route;
}

std::vector<TownID> Datastructures::least_towns_route(TownID fromid, TownID toid)
{
    std::vector<TownID> towns;
    if (towns_.find(fromid)!=towns_.end() && towns_.find(toid)!=towns_.end()){
        std::vector<TownID> route;
        route = get_route_least(fromid,toid);
        if (route.size()>1)
            towns = route;
    }else{
        towns.push_back(NO_TOWNID);
    }
    reset_helpers();
    return towns;
}

std::vector<TownID> Datastructures::get_cycle(TownID startid)
{

    std::vector<TownID> route;
    std::vector<TownID> que;
    que.push_back(startid);
    while (que.size()>0){
        auto town = que.back();
        if (towns_[town]->color_==WHITE){
            auto neighbors = towns_[town]->neighbors_;
            towns_[town]->color_=GRAY;

            for (TownID nb: neighbors){
                auto nb_ptr = towns_[nb];

                if (nb_ptr->color_==WHITE){
                    nb_ptr->from_=town;
                    que.push_back(nb);

                }else if (nb_ptr->color_==GRAY){
                    TownID from_id = towns_[town]->from_;

                    //Cycle found
                    if (nb!=from_id){
                        route.push_back(nb);
                        route.push_back(town);
                        while (from_id != startid){
                            route.push_back(from_id);
                            from_id = towns_[from_id]->from_;
                        }
                        route.push_back(startid);
                        std::reverse(route.begin(),route.end());
                        return route;
                    }
                }
            }
       }else{
            towns_[town]->color_=BLACK;
            que.pop_back();
       }
    }
    return route;
}

std::vector<TownID> Datastructures::road_cycle_route(TownID startid)
{
    std::vector<TownID> towns;
    if (towns_.find(startid)!=towns_.end()){
        std::vector<TownID> route;
        route = get_cycle(startid);

        // At least 3 can form a "cycle" and then route must have at least 4 towns.
        if (route.size()>3)
            towns = route;
    }else{
        towns.push_back(NO_TOWNID);
    }
    reset_helpers();
    return towns;
}

int Datastructures::calculate_route_distance (std::vector<TownID> route){
    int distance = 0;
    int i = 1;
    int size = route.size();
    TownID prev = route.at(0);
    while (size > i){
        TownID next = route.at(i);
        auto from = towns_[prev];
        auto to = towns_[next];

        Coord from_coord = from->coord_;
        Coord to_coord = to->coord_;

        distance = distance + calculate_distance(from_coord.x,from_coord.y,to_coord.x,to_coord.y);
        i++;
    }
    return distance;
}

std::vector<TownID> Datastructures::get_route_shortest(TownID fromid, TownID toid)
{
    std::vector<TownID> shortest_route;
    std::vector<TownID> route;
    int shortest_dist;
    std::deque<TownID> que;
    que.push_back(fromid);

    while (que.size()>0){
        auto town = que.front();
        que.pop_front();
        towns_[fromid]->color_=GRAY;
        auto neighbors = towns_[town]->neighbors_;

        for (TownID nb: neighbors){
            auto nb_ptr = towns_[nb];
            if (nb_ptr->color_ != BLACK){
                que.push_back(nb);
                nb_ptr->from_= town;
                nb_ptr->color_=GRAY;

                if (nb == toid){
                    route.push_back(nb);
                    TownID from_id = nb_ptr->from_;
                    while (from_id != fromid){
                        route.push_back(from_id);
                        from_id = towns_[from_id]->from_;
                    }
                    route.push_back(fromid);
                    std::reverse(route.begin(),route.end());
                    int new_dist = calculate_route_distance(route);

                    if (shortest_route.size()>0){
                        if (new_dist<shortest_dist){
                            shortest_route = route;
                            shortest_dist = new_dist;
                        }
                    }else{
                        shortest_dist = new_dist;
                        shortest_route = route;
                    }
                    route.clear();
                }
            }
        }
        towns_[town]->color_=BLACK;
    }
    return shortest_route;
}

std::vector<TownID> Datastructures::shortest_route(TownID fromid, TownID toid)
{
    std::vector<TownID> towns;
    if (towns_.find(fromid)!=towns_.end() && towns_.find(toid)!=towns_.end()){
        std::vector<TownID> route;
        route = get_route_shortest(fromid,toid);
        if (route.size()>1)
            towns = route;
    }else{
        towns.push_back(NO_TOWNID);
    }
    reset_helpers();
    return towns;
}

Distance Datastructures::trim_road_network()
{
    // Replace the line below with your implementation
    throw NotImplemented("trim_road_network()");
}
