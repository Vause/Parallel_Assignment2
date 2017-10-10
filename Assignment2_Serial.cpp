//CSCI415; Saeed Salem, 9/6/2017
//To compile: g++ -O3 -w assign2Graph.cpp -lpthread -o assign2Graph
//To run: ./assign2Graph filename
//./assign2Graph networkDatasets/toyGraph1.txt

#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>  /* atoi */
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>

using namespace std;

typedef std::vector< std::vector<int> > AdjacencyMatrix;
AdjacencyMatrix adjMatrix;


void printAdjMatrix(AdjacencyMatrix adjMatrix)
{
    for (int i=0; i<adjMatrix.size(); i++)
    {
        for (int j=0; j<adjMatrix[i].size(); j++) 
        {
            std::cout << adjMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

//Serial implementation
void serialClusteringCoefficient(AdjacencyMatrix adjMatrix)
{
    int degree;
    std::vector< int > neighbors;
    float coefficientsSum = 0.f;
    float maxEdges, edgeCount, coefficient, result;

    for (int i=0; i<adjMatrix.size(); i++)
    {
	// Reset variables for each node
	degree = 0;
        edgeCount = 0.f;
	maxEdges = 0.f;
	coefficient = 0.f;
        neighbors.clear();

	// Get degree of node
        for(int j=0; j<adjMatrix[i].size(); j++)
 	{
	    if(adjMatrix[i][j] == 1)
	    {
		degree++;
		neighbors.push_back(j);
	    }
	}
	
	// Find edges between node i's neighbors
	for(int k=0; k<neighbors.size(); k++)
	{
	    // Iterate through node k's neighbors
	    for(int n=0; n<adjMatrix[i].size(); n++)
	    {
		// If node k's neighbor is also a neighbor of node i, increment edgeCount
		if(adjMatrix[neighbors[k]][n] == 1 && adjMatrix[i][n] == 1)
		{
		    edgeCount++;
		}
	    }
	}
	
	// Calculate max number of edges
	maxEdges = degree * (degree - 1);

	// Calculate node i's clustering coefficient
	coefficient = edgeCount/maxEdges;

	// Add node i's coefficient to sum
	coefficientsSum += coefficient;
    }
    
    // Calculate graph's clustering coefficient and print result
    result = coefficientsSum/adjMatrix.size();
    cout << "Parallel Clustering Coefficient Result: " << result << endl;
}
	
	int main(int argc, char** argv)
{
    if(argc<2){
      cout<<"To run: ./assign2Graph filename"<<endl;
      cout<<"./assign2Graph networkDatasets/toyGraph1.txt"<<endl;
      return 0;
    }

    fstream myfile(argv[1],std::ios_base::in);
    int u,v;
    int maxNode = 0;
    vector<pair<int,int> > allEdges;
    while(myfile >> u >> v)
    {
        allEdges.push_back(make_pair(u,v));
        if(u > maxNode)
          maxNode = u;

        if(v > maxNode)
          maxNode = v;
    }

    int n = maxNode +1;  //Since nodes starts with 0
    cout<<"Graph has "<< n <<" nodes"<<endl;

    adjMatrix = AdjacencyMatrix(n,vector<int>(n));
    //populate the matrix
    for(int i =0; i<allEdges.size() ; i++){
       u = allEdges[i].first;
       v = allEdges[i].second;
       adjMatrix[u][v] = 1;
       adjMatrix[v][u] = 1;
    }

    serialClusteringCoefficient(adjMatrix);

    if(n<=10)
      printAdjMatrix(adjMatrix);

	//You can also make a list of neighbors for each node if you want.

return 0;
}