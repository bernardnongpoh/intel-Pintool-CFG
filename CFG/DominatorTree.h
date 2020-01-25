#include <fstream>
#include <iomanip>
#include <iostream>
#include <string.h>
#include "pin.H"
#include <set>
#include <map>
#include "stdlib.h"
#include <sstream>
#include <list>
#include <cstring>
using namespace std;
// Using simple bit vector alorithm to calc the dominator tree given the CFG, 
class DominatorTree{
	private:
		// this map dom(w)->set of nodes that dominate w
		std::map<ADDRINT,std::set<ADDRINT> > dominators;
	public:
		// Input a CFG of type Graph found in Graph.h
		DominatorTree();

};


DominatorTree::DominatorTree(){

// Best is to have a wrapper class for adjacency List with interface expose only
//	std::map<ADDRINT,list<ADDRINT> > adj=graph->getAdj();
		ofstream outFile;
	outFile.open("tree.txt");
//	outFile<<adj.size();
	outFile.close();
	// perform calc using the graph 
	// get adjacency list and compute the dominators

}
