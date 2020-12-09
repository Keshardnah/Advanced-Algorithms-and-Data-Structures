#include <iostream>
#include <vector>
#include <climits> // UINT_MAX

typedef unsigned int uint;

// Function that finds the path, from the start element to the element in
// the last row on the right side, so as to destroy the least number of flowers.
// int *mem: auxiliary matrix of size N * M in which we remember the cost of the path
// (number of trampled flowers) from the current place in the matrix to the destination
// returns the number of trampled flowers
uint optimal_path(std::pair<uint, uint> start_coords, const uint N, const uint M, uint16_t *const problem,
              int *mem) {
    // if we are on the last (goal) element
    if(start_coords.first == N-1 && start_coords.second == M-1)
        // return the number of flowers in it
        return *(problem+M*start_coords.first+start_coords.second);

    // if the number in the 'mem' matrix (cost of the path from the current element to the goal)
    // is different from '-1' (initialization value)
    if(*(mem+M*start_coords.first+start_coords.second) != -1)
        return *(mem+M*start_coords.first+start_coords.second);

    // return value is set to the highest possible
    uint ret = UINT_MAX;

    // if the x coordinate is less than the x coordinate of the last row
    // then we can move down
    if(start_coords.first < N-1)
        // we are looking for a minimum between the current ‘ret’ and
        // the path cost of the lower element (found recursively) summed with
        // the number of flowers in the current element
        ret = std::min(ret, optimal_path({start_coords.first + 1, start_coords.second}, N, M, problem, mem)
                            + *(problem + M * start_coords.first + start_coords.second));

    // if the y coordinate is less than the y coordinate of the last column
    // then we can move right
    if(start_coords.second < M-1)
        // we are looking for a minimum between the current ‘ret’ and
        // the path cost of the right element (found recursively) summed with
        // the number of flowers in the current element
        ret = std::min(ret, optimal_path({start_coords.first, start_coords.second + 1}, N, M, problem, mem)
                            + *(problem + M * start_coords.first + start_coords.second));

    // put the minimum path cost (from the current element to the goal)
    // in the 'mem' matrix in place of the current element
    *(mem+M*start_coords.first+start_coords.second) = ret;
    return ret;
}

// Function that solves the problem of finding the path from the initial element
// to the last element by destroying the smallest number of flowers.
// returns the number of trampled flowers
// puts the optimal path into the 'solution'
uint solve(const uint N, const uint M, uint16_t *const problem, std::vector<std::pair<uint, uint>> &solution) {
    // auxiliary matrix whose values are initialized to '-1'
    // because these are illegal values for the path cost (number of trampled flowers)
    int *mem = (int*)malloc(N*M * sizeof(int));
    std::fill(mem, mem + N*M, -1);

    // finding out the optimal number of trampled flowers from the element (0,0) to (N-1, M-1)
    uint ret = optimal_path(std::make_pair(0,0),N,M,problem,mem);

    // we want to get the coordinates of the optimal path
    std::pair<uint, uint> path;
    // start coordinates
    path = std::make_pair(0,0);
    // until we reach the coordinates (N-1, M-1)
    while (path != std::make_pair(N-1,M-1)) {
        // put coordinates in 'solution'
        solution.push_back(path);
        // if the x coordinate is equal to the x coordinate of the last row
        if (path.first == (N - 1))
            // then the only possibility is that we went right
            path = {path.first, path.second + 1};
        // if the y coordinate is equal to the y coordinate of the last column
        else if (path.second == (M - 1))
            // then the only possibility is that we went down
            path = {path.first + 1, path.second};
        // if the element below the current is larger than the element to the right of the current (in the 'mem' matrix)
        else if (*(mem + M * (path.first + 1) + path.second) > *(mem + M * path.first + (path.second + 1)))
            // go to the item on the right
            path = {path.first, path.second + 1};
        else
            // go to the item below
            path = {path.first + 1, path.second};
    }
    // put (N-1, M-1) to 'solution'
    solution.push_back(path);
    free(mem);
    return ret;
}

int main() {
    uint16_t example[3][4] = {
                                   {100, 200,  1000, 0},
                                   {200, 100,  600,  0},
                                   {300, 1600, 100,  0}
                        };
    std::vector<std::pair<uint, uint>> solution;
    std::cout<<"Optimal value: ";
    std::cout << solve(3, 4, reinterpret_cast<uint16_t *const>(&example),solution) << std::endl;
    std::cout<<"Path:"<<std::endl;
    for (std::vector<std::pair<uint, uint>>::iterator it = solution.begin() ; it != solution.end(); ++it) {
        std::cout <<'('<<it->first<<','<<it->second<<')'<<std::endl;
    }
}
