#include <fstream>
#include <iomanip>
#include <iostream>
#include <string.h>
#include "pin.H"
#include <set>
#include <unordered_map>
#include <unordered_map>
#include "stdlib.h"
#include <sstream>
#include <set>
#include <cstring>
#include <bits/stdc++.h>
#ifndef SYMBOL
#define SYMBOL header
#include "DataType.h"
#include "AnalysisVec.h"
#endif
using namespace std;

class DataDependencyGraph
{
private:
	ADDRINT startID;
	ADDRINT exitID;
	list<Node *> *nodes;
	string functionName;
	string fileName;
	set<ADDRINT>insensitiveSet;
	int nodeNoCFG;
	// adjacency set for CFG
	// bad idea to use unordered_map here 
	// 1->2,3,4,
	vector<vector<int> > CFG;
	vector<vector<int> > reverseCFG; // accessing the node should be in reverse
	vector<vector<int> > insensitiveCFG;
	// mapIDtoAddr(ID)-->ADDR
	map<int,ADDRINT> mapIDtoAddr;
	map<ADDRINT,int> mapAddrtoID;
	
	void doMapping();
	void updateIDMapping(int ID,ADDRINT Addr);
	ADDRINT getAddrByID(int ID);
	int getIDByAddr(ADDRINT addr);
	void addCFGEdge(int u,int v);
	void addReverseCFGEdge(int u,int v);
	void constructCFG();
	void constructReverseCFG();
	ADDRINT getBranchAddressNode(ADDRINT nodeAddress);
	void generateDot(vector<vector<int> > adj,string fileName);
	int getBranchNodeSize();
	void printAdjacencyset(vector<vector<int> > adj,bool isReverse);
	void printValueSet(vector<vector<int> > vec,string fileName);
	void printValueSet(map<int,set<int> > vec,string fileName);
	void constructInsensitiveCFG();
	void findInsensitive(int u,int sensitiveId);
	void addInsensitiveCFGEdge(int u,int v);
	

	AnalysisVec *analysis;


public:
	DataDependencyGraph(std::string functionName,list<Node*> *nodes);
	DataDependencyGraph(std::string functionName,list<Node*> *nodes,set<ADDRINT> insensitiveSet);
	void printCFGAdjList();
	void printCFGToDot();
	void printReverseCFGToDot();
	void printDominatorTree();
	void printPostDominatorTree();
	void printCDG();
	void printDominator();
	void printAdjacency();
	void printInsensitiveCFGToDot();
	void printInsensitiveDominatorTree();


		};
//CFG for all Branch
DataDependencyGraph::DataDependencyGraph(std::string functionName,list<Node*> *nodes){
	this->functionName=functionName;
	this->nodes=nodes;
	nodeNoCFG=DataDependencyGraph::getBranchNodeSize();
	CFG.resize(nodeNoCFG+2); // one for start one for exit node 
	reverseCFG.resize(nodeNoCFG+2);
	startID=0; // also start ID =0
	exitID=nodeNoCFG+1; // just to know we are appending this one 

	doMapping();
	constructCFG();
	constructReverseCFG();
	
	//constructReverseCFGAdj();
	//constructInsensitiveCFGAdj();
	analysis=new AnalysisVec();
}

DataDependencyGraph::DataDependencyGraph(std::string functionName,list<Node*> *nodes,set<ADDRINT> insensitiveSet){
	this->functionName=functionName;
	this->nodes=nodes;
	this->insensitiveSet=insensitiveSet;
	nodeNoCFG=DataDependencyGraph::getBranchNodeSize();
	CFG.resize(nodeNoCFG+2); // one for start one for exit node 
	reverseCFG.resize(nodeNoCFG+2);
	insensitiveCFG.resize(insensitiveSet.size()+2);
	startID=0; // also start ID =0
	exitID=nodeNoCFG+1; // just to know we are appending this one 

	doMapping();
	constructCFG();
	constructReverseCFG();
	this->insensitiveSet.insert(startID);
	this->insensitiveSet.insert(exitID);
		//constructInsensitiveCFG();

	

	//constructReverseCFGAdj();
	//constructInsensitiveCFGAdj();
	analysis=new AnalysisVec();
}


int DataDependencyGraph::getBranchNodeSize(){
	int count=0;
	for(list<Node *>::iterator iter=nodes->begin();iter != nodes->end(); ++iter){
		Node *node =*iter;
	    if(node->branchAddress==0 || node->nodeAddress==0) continue;
	    count++;
	}

	return count;
}

ADDRINT DataDependencyGraph::getAddrByID(int ID){
	return mapIDtoAddr[ID];
}
int DataDependencyGraph::getIDByAddr(ADDRINT addr){
	return mapAddrtoID[addr];
}

void DataDependencyGraph::updateIDMapping(int ID,ADDRINT Addr){
	mapIDtoAddr[ID]=Addr;
	mapAddrtoID[Addr]=ID;
}

void DataDependencyGraph::addCFGEdge(int u,int v){
	CFG[u].push_back(v);
}
void DataDependencyGraph::addReverseCFGEdge(int u,int v){
	reverseCFG[u].push_back(v);
}

void DataDependencyGraph::addInsensitiveCFGEdge(int u,int v){
	insensitiveCFG[u].push_back(v);
}

void DataDependencyGraph::doMapping(){
	ofstream outFile;
	outFile.open("vec.log");
	
	int ID=1;
	updateIDMapping(startID,startID);
	for(list<Node *>::iterator it=nodes->begin();it!=nodes->end();it++){
		Node *node =*it;
		if(node->branchAddress==0 || node->nodeAddress==0) continue;
		updateIDMapping(ID,node->branchAddress);
		ID++; //Update the ID
	}
	outFile<<startID<<" "<<exitID;
	updateIDMapping(exitID,exitID);

}

void DataDependencyGraph::printCFGAdjList(){
	ofstream outFile;
	outFile.open("adjcfg.log");
	int i;
	for(i=0;i<(int)CFG.size();i++){
		outFile<<"node ID: "<<i<<"{";
		int adjI=0;
		for(adjI=0;adjI!=(int)CFG[i].size();adjI++){
			outFile<<CFG[i][adjI]<<",";	
		}
		outFile<<"\n";
		
	}
}


ADDRINT DataDependencyGraph::getBranchAddressNode(ADDRINT nodeAddress){

	for(list<Node*>::iterator it=nodes->begin();it!=nodes->end();++it){
			Node *node = (*it);
			if(nodeAddress==node->nodeAddress)
			{
			// return the exit branch address of the BasicBlock or a Node
			
			if(node->branchAddress==0){
				//recurse
				return DataDependencyGraph::getBranchAddressNode(node->notTakenBB);
			}
			else 
				return node->branchAddress;
			}	    
	}

//not found zero
return 0;
	

}

void DataDependencyGraph::constructCFG(){
// Create Connection
	ofstream outFile;
	outFile.open("constructCFGAdj.log");
	list<Node *>::iterator iter;
	iter = nodes->begin();
	Node *firstNode =*iter;
	//start node 
	int firstNodeID=DataDependencyGraph::getIDByAddr(firstNode->branchAddress);
	DataDependencyGraph::addCFGEdge(startID,firstNodeID);
	DataDependencyGraph::addCFGEdge(startID,exitID);

for (;iter != nodes->end(); ++iter)
{
	 Node *node =*iter;

	if(node->branchAddress==0 || node->nodeAddress==0) continue;
	 outFile<<"node -->"<<std::hex<<node->branchAddress<<"\n";
	  //CFGAdj[node->branchAddress];
	if(node->takenBB!=0)
	{

		ADDRINT takenBrAddr=DataDependencyGraph::getBranchAddressNode(node->takenBB);
		if(takenBrAddr!=0)
		{
			int u=DataDependencyGraph::getIDByAddr(node->branchAddress);
			int v=DataDependencyGraph::getIDByAddr(takenBrAddr);
			DataDependencyGraph::addCFGEdge(u,v);
		}
		
		
	}
	//outFile<<std::dec<<node->nodeAddress<<"->"<<node->takenBB<<";\n";
	 if(node->notTakenBB!=0){
		ADDRINT notTakenBrAddr=DataDependencyGraph::getBranchAddressNode(node->notTakenBB);
		if(notTakenBrAddr!=0)
		{
			int u=DataDependencyGraph::getIDByAddr(node->branchAddress);
			int v=DataDependencyGraph::getIDByAddr(notTakenBrAddr);
			DataDependencyGraph::addCFGEdge(u,v);
		}
	  	
	  
	}
	}

// Create connection to the end of the node 

for(int nodeId=0;nodeId!=(int)CFG.size();nodeId++)
{
	// this node has no children, make it point to last node 
	if(nodeId==(int)exitID) continue;
	if(((int)CFG[nodeId].size())==0){
		DataDependencyGraph::addCFGEdge(nodeId,exitID);
	}

}
}


void DataDependencyGraph::constructReverseCFG(){
		int nodeId;
	for(nodeId =0;nodeId<(int)CFG.size();nodeId++){
			int v=nodeId;		
			int edgeID;
			for(edgeID=0;edgeID<(int)CFG[nodeId].size();edgeID++){
				int u=CFG[nodeId][edgeID];
				DataDependencyGraph::addReverseCFGEdge(u,v);
		
			}

			// This is for node that has no parent, we need to print to dot
			
				//reverseCFGAdj[node].clear();
		}
}
void DataDependencyGraph::generateDot(vector<vector<int> > adj,string fileName){
	ofstream outFile;
	fileName=fileName+".dot";
	outFile.open(fileName.c_str());

	outFile<<"digraph G {\nnode[shape=record]";
		
		// Vertices
	int ID=0;
	for(ID=0;ID<(int)adj.size();ID++) {
		if(adj[ID].size()==1 && adj[ID][0]==-1) continue;
		//if((*it).first==0) continue;
		ADDRINT addr=DataDependencyGraph::getAddrByID(ID);
		if(insensitiveSet.find(addr)!=insensitiveSet.end())
		outFile<<std::dec<<ID<<"[label=\""<<std::hex<<addr<<"\", style=filled, fillcolor=green];\n";
		else
			outFile<<std::dec<<ID<<"[label=\""<<std::hex<<addr<<"\", style=filled, fillcolor=white];\n";
	}
	// Edges : Connection here 
	for(ID=0;ID<(int)adj.size();ID++) {
		if(adj[ID].size()==1 && adj[ID][0]==-1) continue;
		int edgeID;	

		for(edgeID=0;edgeID<(int)adj[ID].size();edgeID++){
			//if(*lit==0 || (*it).first==0)continue;
			outFile<<std::dec<<ID<<"->"<<adj[ID][edgeID]<<";\n";
		}
		
	}
	outFile<<"}\n";

	
	outFile.close();
}




void DataDependencyGraph::printCFGToDot(){
		string fileName="CFG_"+functionName;
		generateDot(CFG,fileName);
}

void DataDependencyGraph::printReverseCFGToDot(){
	
		string fileName="RCFG_"+functionName;
		generateDot(reverseCFG,fileName);
}
void DataDependencyGraph::printInsensitiveCFGToDot(){
		string fileName="CFG_"+functionName;
		generateDot(insensitiveCFG,fileName);
}


void DataDependencyGraph::printDominatorTree(){
		string fileName="data/streamcluster/DT/DT_"+functionName;
		map<int,set<int> > dominators=analysis->getDominator(CFG);
		
		vector<vector<int> > DTree(CFG.size());
		analysis->getDominatorTree(CFG,dominators,DTree);
		generateDot(DTree,fileName);
}

// This will print to Database or to a file 
void DataDependencyGraph::printInsensitiveDominatorTree(){
		
		map<int,set<int> > dominators=analysis->getDominator(CFG);
		
		vector<vector<int> > DTree(CFG.size());
		analysis->getDominatorTree(CFG,dominators,DTree);
		vector<vector<int> > IDTree(CFG.size());
		
		/** */
		
		// start from root node 
		int rootNodeId= 0;

		stack<int> WorklistStack;
		stack<int> InsensitiveStack;
		InsensitiveStack.push(0);
		WorklistStack.push(rootNodeId); // start with root
		
		
		while(!WorklistStack.empty()){
			int ID=WorklistStack.top();
			WorklistStack.pop();
			

			ADDRINT addr=DataDependencyGraph::getAddrByID(ID);
			

			//bool isNodeInsensitive=false;

			if(insensitiveSet.find(addr)!=insensitiveSet.end()){
				
				//InsensitiveStack.push(ID);	
				//isNodeInsensitive=true;
			
			}
			else{
				IDTree[ID].push_back(-1);
			}
		
		int parent=InsensitiveStack.top();
		InsensitiveStack.pop();
		
			for(int edgeId=(int)DTree[ID].size()-1;edgeId>=0;edgeId--)
			{
				ADDRINT addr=DataDependencyGraph::getAddrByID(DTree[ID][edgeId]);
				if(insensitiveSet.find(addr)!=insensitiveSet.end()){
					
							
							//if node is insensive connect directly else connect parent

							IDTree[parent].push_back(DTree[ID][edgeId]);
							InsensitiveStack.push(DTree[ID][edgeId]);
							//ITemp.push(DTree[ID][edgeId]);
							//ITemp.pop();
				}
				else{
					InsensitiveStack.push(parent);
				}



				WorklistStack.push(DTree[ID][edgeId]);
			}

		

		}	

		/*** */			


		generateDot(IDTree,functionName);

			ofstream outFile;
			string outFileName="data/streamcluster/DATA_"+functionName+".data";
			outFile.open(outFileName);
			
			
		// generate to file 
		for(int nodeId=0;nodeId<(int)IDTree.size();nodeId++){
			ADDRINT parentAddr=getAddrByID(nodeId);
			if(insensitiveSet.find(parentAddr)==insensitiveSet.end()) continue;
			outFile<<std::hex<<parentAddr;
			for(int edgeId=0;edgeId<(int)IDTree[nodeId].size();edgeId++){
				ADDRINT nodeAddr= getAddrByID(IDTree[nodeId][edgeId]);
	
					outFile<<","<<std::hex<<nodeAddr;
			}
			outFile<<"\n";

		}

		//outFile.close();


}

void DataDependencyGraph::constructInsensitiveCFG(){
	// Base on  adjacency list 

	int nodeId=0;

	for(nodeId=0;nodeId<(int)CFG.size();nodeId++){
		// find if node in insensitive set
		if(insensitiveSet.find(getAddrByID(nodeId))!=insensitiveSet.end()){
			// Insensitive node
				// find it edges 
			for(int edgeId=0;edgeId<(int)CFG[nodeId].size();edgeId++){

				if(insensitiveSet.find(getAddrByID(CFG[nodeId][edgeId]))!=insensitiveSet.end()){

					bool exist=false;
					for(int i=0;i<(int)insensitiveCFG[nodeId].size();i++){
						if(insensitiveCFG[nodeId][i]==CFG[nodeId][edgeId]){
							exist=true;
							break;
						}
					}
					if(!exist)
					addInsensitiveCFGEdge(nodeId,CFG[nodeId][edgeId]);
				}
				else{
					findInsensitive(nodeId,CFG[nodeId][edgeId]);
				}
			}
		}
			



	}
}
	// This is a recursive function, here childId is sensitivenode
void DataDependencyGraph::findInsensitive(int u,int sensitiveId){
			for(int edgeID=0;edgeID<(int)CFG[sensitiveId].size();edgeID++){
				if(insensitiveSet.find(getAddrByID(CFG[sensitiveId][edgeID]))!=insensitiveSet.end()){
			// Insensitive node
					// find if the edge already in a set 
					bool exist=false;
					for(int i=0;i<(int)insensitiveCFG[u].size();i++){
						if(insensitiveCFG[u][i]==CFG[sensitiveId][edgeID]){
							exist=true;
							break;
						}
					}
					if(!exist)
					addInsensitiveCFGEdge(u,CFG[sensitiveId][edgeID]);	
		}	
		else{
			findInsensitive(u,CFG[sensitiveId][edgeID]);
		}	
	
			}
}





void DataDependencyGraph::printDominator(){
	map<int,set<int> > dominators=analysis->getPostDominator(reverseCFG);
	string fileName="Dominator.log";
	LOG(to_string(dominators.size()));
	printValueSet(dominators,fileName);

}


void DataDependencyGraph::printPostDominatorTree(){
		string fileName="RDT_"+functionName;
		generateDot(CFG,fileName);
}

void DataDependencyGraph::printCDG(){
		vector<vector<int> > cfg= analysis->getDependencyGraph(reverseCFG,CFG);
		string fileName="CDG_"+functionName;
		generateDot(cfg,fileName);
}


void DataDependencyGraph::printAdjacency(){
	printAdjacencyset(insensitiveCFG,true);
}

void DataDependencyGraph::printValueSet(vector<vector<int> > vec,string fileName){
		ofstream outFile;
		outFile.open(fileName);
		for(int nodeId=0;nodeId<(int)vec.size();nodeId++){
		outFile<<""<<std::hex<<nodeId<<"-->{ ";
		for(int adjId=0;adjId<(int)vec[nodeId].size();adjId++){
			outFile<<std::hex<<vec[nodeId][adjId]<<",";
		}
		outFile<<"}\n\n";
	}
	outFile.close();
}

void DataDependencyGraph::printValueSet(map<int,set<int> > G,string fileName){
		ofstream outFile;
		outFile.open(fileName);
		for(map<int,set<int> >::iterator it=G.begin();it!=G.end();++it){
		outFile<<""<<std::hex<<getAddrByID((*it).first)<<"-->{ ";
		for(set<int>::iterator sit=(*it).second.begin();sit!=(*it).second.end();++sit){
			outFile<<std::hex<<getAddrByID(*sit)<<",";
		}
		outFile<<"}\n\n";
	}
	outFile.close();
}

void DataDependencyGraph::printAdjacencyset(vector<vector<int> > adj,bool isReverse){
	ofstream outFile;
	outFile.open("adjacency.log");
	if(!isReverse){
	for(int nodeId=0;nodeId<(int)adj.size();nodeId++){
		outFile<<""<<std::hex<<nodeId<<"-->{ ";
		for(int adjId=0;adjId<(int)adj[nodeId].size();adjId++){
			outFile<<std::hex<<adj[nodeId][adjId]<<",";
		}
		outFile<<"}\n\n";
	}
}
else{
// this has some problem

	for(int nodeId=(int)adj.size()-1;nodeId>=0;nodeId--){
		LOG("asjaaja" + to_string(nodeId));
		outFile<<""<<std::hex<<nodeId<<"-->{ ";
		for(int adjId=0;adjId<(int)adj[nodeId].size();adjId++){
			outFile<<std::hex<<adj[nodeId][adjId]<<",";
		}
		outFile<<"}\n\n";
	}

}

}


