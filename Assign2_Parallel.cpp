//CSCI415; Saeed Salem, 9/6/2017
//To compile: g++ -O3 -w assign2Graph.cpp -lpthread -o assign2Graph 
//To run: ./assign2Graph filename t
//./assign2Graph networkDatasets/toyGraph1.txt 4

#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>  /* atoi */
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <thread>         // std::thread
#include <mutex>          // std::mutex

using namespace std;

typedef std::vector< std::vector<int> > AdjacencyMatrix;
AdjacencyMatrix adjMatrix;
mutex mtx;
int n = 0;
int thread_count = 0;
double globalCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC;



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

//Parallel implementation
void *parallelClusteringCoefficient(void *thread_id)
{
	
	int size = n;
	int t = thread_count;
	long id = (long)thread_id;
	long start = id * (n/t);
	long end = 0;
	if((int) id == (int) t - 1) end = n - 1;
	else end = start + (n/t) - 1;
	
    int degree;
    std::vector< int > neighbors;
    float maxEdges, edgeCount, coefficient, result;

	cout << start << endl;
	cout << end << endl;
    for (int i=start; i<=end; i++)
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
	if(edgeCount > 1)
	{
	maxEdges = degree * (degree - 1);

	// Calculate node i's clustering coefficient
	coefficient = edgeCount/maxEdges;

	// Add node i's coefficient to sum
	mtx.lock();
	globalCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC += coefficient;
	mtx.unlock();
	}
    }
}
	
	int main(int argc, char** argv)
{
    if(argc<3){
      cout<<"To run: ./assign2Graph filename t"<<endl;
      cout<<"./assign2Graph networkDatasets/toyGraph1.txt 4"<<endl;
      return 0;
    }

    fstream myfile(argv[1],std::ios_base::in);
	thread_count = atoi(argv[2]);
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

    n = maxNode +1;  //Since nodes starts with 0
    cout<<"Graph has "<< n <<" nodes"<<endl;

    adjMatrix = AdjacencyMatrix(n,vector<int>(n));
    //populate the matrix
    for(int i =0; i<allEdges.size() ; i++){
       u = allEdges[i].first;
       v = allEdges[i].second;
       adjMatrix[u][v] = 1;
       adjMatrix[v][u] = 1;
    }
	

    if(n<=10)
      printAdjMatrix(adjMatrix);
  
  long       thread;
   pthread_t* thread_handles; 
   thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t));
   for (thread = 0; thread < thread_count; thread++)  
      pthread_create(&thread_handles[thread], NULL,
          parallelClusteringCoefficient, (void*) thread);
	
	for (thread = 0; thread < thread_count; thread++) 
		pthread_join(thread_handles[thread], NULL);
	
	float result = (float)globalCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC / adjMatrix.size();
	cout << "Result: " << result << endl;

	//You can also make a list of neighbors for each node if you want.

return 0;
}