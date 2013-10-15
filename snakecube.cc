//#include <stdlib.h>
#include <numeric>
#include <iostream>
#include <array>
#include <vector>
#include <list>

typedef std::list<int> List;

// Predefined sequences of numbers specifying the directions orthogonal to
// the previous direction.
typedef std::array<int, 4> OrthogonalDirections;
const OrthogonalDirections U1 = {2,-2,3,-3};
const OrthogonalDirections U2 = {1,-1,3,-3};
const OrthogonalDirections U3 = {1,-1,2,-2};

typedef std::array<int, 6> Directions;
const Directions U = {1,-1,2,-2,3,-3};

typedef std::array<int, 3> Point;

// 4x4x4 array used for testing if the current configuration leads to
// intersections or separated regions.
bool cube[4][4][4];


void print_directions(const List& D){
    std::cout << "(";
    List::const_iterator it;
    for(it=D.begin(); it!=D.end(); ++it){
        std::cout << *it << ",";
    }
    std::cout << ")" << std::endl;
}


template<int SIZE>
int count_connected(const Point& P){
    // Make sure the point is inside the cube and not already occupied.
    for (unsigned i=0; i<SIZE; ++i){
        if (P[i]<0 || P[i]>SIZE)
            return 0;
    }
    if (cube[P[0]][P[1]][P[2]])
        return 0;

    // Mark the current point as occupied and test all adjacent sites.
    cube[P[0]][P[1]][P[2]] = true;
    int count=1;
    Point P_new;
    Directions::const_iterator it;
    for(it=U.begin(); it!=U.end(); ++it){
        P_new = P;
        unsigned index = unsigned(abs(*it))-1;
        if (*it>0)
            P_new[index] += 1;
        else
            P_new[index] -= 1;
        count += count_connected<SIZE>(P_new);
    }
    return count;
}


template<int SIZE>
bool is_valid(const List& D, const List& C){
    // Check if the path leaves the bounderies of the cube.
    Point minima = {0,0,0};
    Point maxima = {0,0,0};
    Point p = {0,0,0};

    List::const_iterator dim = D.begin();
    List::const_iterator direction;
    for(direction=C.begin(); direction!=C.end(); ++direction){
        unsigned index = unsigned(abs(*direction))-1;
        if (*direction>0){
            p[index] += ((*dim)-1);
            if (maxima[index]<p[index]) maxima[index] = p[index];
        }
        else{
            p[index] -= ((*dim)-1);
            if (minima[index]>p[index]) minima[index] = p[index];
        }
        ++dim;
    }
    bool max_extended = true;
    for(unsigned i=0; i<3; i++){
        int x = maxima[i]-minima[i];
        if (x>SIZE)
            return false;
        if (x<SIZE)
            max_extended = false;
        p[i] = -minima[i];
    }
    // Check if places in the cube are occupied more than once.
    for(int i=0;i<=SIZE;i++){
        for(int j=0;j<=SIZE;j++){
            for(int k=0;k<=SIZE;k++){
                cube[i][j][k] = false;
            }
        }
    }
    cube[p[0]][p[1]][p[2]] = true;
    int length = 1;
    dim = D.begin();
    for(direction=C.begin(); direction!=C.end(); ++direction){
        int s;
        unsigned index = unsigned(abs(*direction))-1;
        if ((*direction)>0)
            s = 1;
        else
            s = -1;
        for(int i=1; i<(*dim); i++){
            p[index] += s;
            ++length;
            if (cube[p[0]][p[1]][p[2]])
                return false;
            cube[p[0]][p[1]][p[2]] = true;
        }
        ++dim;
    }
    // Check if all remaining sites still can be reached.
    if (max_extended){
        // Start from the current end point (It has to be marked as not
        // visited because of the current implementation of count_connected).
        cube[p[0]][p[1]][p[2]] = false;
        --length;
        int connected_sites = count_connected<SIZE>(p);
        if (connected_sites<(SIZE+1)*(SIZE+1)*(SIZE+1)-length)
            return false;
    }
    return true;
}


template<int SIZE>
void step(const List& D, List& C, const bool orientation_fixed){
    // Test if current configuration is valid.
    if (!is_valid<SIZE>(D, C)){
        C.pop_back();
        return;
    }
    // If we found a complete solution print it.
    if (D.size()==C.size()){
        print_directions(C);
        C.pop_back();
        return;
    }
    // Find next possible directions.
    const OrthogonalDirections* u;
    switch(abs(C.back())){
        case 1: u = &U1;break;
        case 2: u = &U2;break;
        case 3: u = &U3;break;
        default: throw "?";
    }
    OrthogonalDirections::const_iterator it;
    for(it=u->begin(); it!=u->end(); ++it){
        if (!orientation_fixed && (*it==3)){
            C.push_back(*it);
            step<SIZE>(D, C, true);
        }
        else if (!orientation_fixed && (*it==-3)){
            continue;
        }
        else {
            C.push_back(*it);
            step<SIZE>(D, C, orientation_fixed);
        }
    }
    // Ran out of options for the current configuration. Go back.
    C.pop_back();
}


int main(int argc, const char* argv[]){
    if (argc!=2){
        std::cout << "Incorrect number of arguments!" << std::endl;
        return 1;
    }
    // Input specifying the length of successive parts of the cube.
    List D;
    // Sequence of numbers specifying in which direction the corresponding
    // section of the cube points.
    List C = {1,2};
    const char *c;
    for(c=argv[1]; *c; ++c){
        if ((*c)==',')
            continue;
        D.push_back((*c)-'0');
    }
    //List D2 = {3,2,3,2,2,4,2,3,2,3,2,3,2,2,2,2,2,2,2,2,3,3,2,2,2,2,2,3,4,2,2,2,4,2,3,2,2,2,2,2,2,2,2,2,4,2};
    int length = std::accumulate(D.begin(), D.end(), 0) - int(D.size()) + 1;
    switch (length){
        case 27: // 3x3x3 cube
            step<2>(D, C, false); break;
        case 64: // 4x4x4 cube
            step<3>(D, C, false); break;
        default:
            std::cout << "Incorrect length!" << std::endl;
            return 1;
    }
}
