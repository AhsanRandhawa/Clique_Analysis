#include <bitset>
#include <iostream>
#include <array>
#include <cstdint>

using namespace std;


const uint64_t sizeOfGraph = 10000;

array<bitset<sizeOfGraph>, sizeOfGraph> prevBitMasks;
array<bitset<sizeOfGraph>, sizeOfGraph> newBitMasks;

int getDeterministicAdjacentNode(int curNode, int round)
{
    long long nextOne = ((1LL << (round-1)) + curNode) % sizeOfGraph;
    return (int) nextOne;
}


int getRandomAjacentNode(int curNode)
{
    int index = rand() % (sizeOfGraph-1);
    if (curNode<= index)  index++;
    return index;
}

pair<int, int> experimentClique(bool randomize)
{
    if (sizeOfGraph == 1) return make_pair(0,0);

    for (auto& myBitset : prevBitMasks) {
        myBitset.reset();  // Set all bits to 0
    }
    for (auto& myBitset : newBitMasks) {
        myBitset.reset();  // Set all bits to 0
    }
    
    for (uint32_t i = 0; i < sizeOfGraph; i++)
    {
        prevBitMasks[i].set(i);
        newBitMasks[i].set(i);
    }

    int curRound = 0;
    int roundsFor1st = -1;


    bool found = true;
    while(found)
    {
        found = false;
        curRound++;
        for(uint32_t i = 0; i < sizeOfGraph; i++)
        {
            if (prevBitMasks[i].all()) continue;
            found = true;
            int chosenNeighbour;
            if(randomize) chosenNeighbour = getRandomAjacentNode(i);
            else chosenNeighbour = getDeterministicAdjacentNode(i, curRound);
            newBitMasks[i] |= prevBitMasks[chosenNeighbour];
        }

        for(uint32_t i = 0; i < sizeOfGraph; ++i)
            prevBitMasks[i] |= newBitMasks[i];
        

        if (roundsFor1st == -1 && prevBitMasks[0].all()) roundsFor1st = curRound;
    }
    curRound--;
    cout << "Experiment Completed\n";
    return make_pair(roundsFor1st, curRound);

}

void run_experiments(int iterations, bool randomize)
{
    long double mean_rounds_for_1st_node = 0;
    long double mean_rounds_for_all_nodes = 0;

    for(int i = 0; i < iterations; ++i)
    {
        pair<int, int> res = experimentClique(randomize);
        mean_rounds_for_1st_node += res.first;
        mean_rounds_for_all_nodes += res.second;
    }

    mean_rounds_for_all_nodes /= iterations;
    mean_rounds_for_1st_node /= iterations;

    cout << "Mean Rounds for 1st node to know the entire graph in clique of order " << sizeOfGraph << " was " << mean_rounds_for_1st_node << "\n";
    cout << "Mean Rounds for all node to know the entire graph in clique of order " << sizeOfGraph << " was " << mean_rounds_for_all_nodes << "\n";

}


int main()
{
    //srand(time(0));
    srand(5);  
    run_experiments(10, false);
}
