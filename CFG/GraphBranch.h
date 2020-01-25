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
#include "Analysis.h"
#endif
using namespace std;

class GraphBranch
{
private:
	list<Node *> *nodes;
	string functionName;
	string fileName;
	set<ADDRINT> insensitiveSet;
	// adjacency set for CFG
	// bad idea to use unordered_map here 
	vector<ADDRINT> vectorCFGAdj[];
	unordered_map<ADDRINT,set<ADDRINT> > CFGAdj; 
	unordered_map<ADDRINT,set<ADDRINT> > finalCFGAdj; 
	unordered_map<ADDRINT,set<ADDRINT> > reverseCFGAdj; 
	unordered_map<ADDRINT,set<ADDRINT> > finalReverseCFGAdj;
	unordered_map<ADDRINT,set<ADDRINT> >  insensitiveCFGAdj; 
	unordered_map<ADDRINT,set<ADDRINT> > insensitiveReverseCFGAdj; 

	//unordered_map<ADDRINT,set<ADDRINT> > dominatorTree; // need to specify of what dominator tree insensitive, cfg , reversecfg and so on.. 

	std::unordered_map<ADDRINT,std::set<ADDRINT> > predecessor;
	std::unordered_map<ADDRINT,std::set<ADDRINT> > successor;
	// this unordered_map dom(w)->set of nodes that dominate w
	std::unordered_map<ADDRINT,std::set<ADDRINT> > dominators;
	std::unordered_map<ADDRINT,ADDRINT> idominators; // Imediate dominators.  
	//function 
	void constructCFGAdj();
	void constructReverseCFGAdj();
	void constructInsensitiveCFGAdj();
	void constructInsensitiveReverseCFGAdj();
	void addCFGAdjEdge(ADDRINT v,ADDRINT w);
	void addReverseCFGAdjEdge(ADDRINT v,ADDRINT w);
	void addInsensitiveCFGAdjEdge(ADDRINT v,ADDRINT w);
	void addInsensitiveReverseCFGAdjEdge(ADDRINT v,ADDRINT w);
	void generateDot(unordered_map<ADDRINT,set<ADDRINT> > adj,string functionName);
	ADDRINT getBranchAddressNode(ADDRINT nodeAddress);
	bool isBranchInsensitive(ADDRINT brAddr);
	void getDenominatorTree(unordered_map<ADDRINT,set<ADDRINT> > CFG,std::unordered_map<ADDRINT,std::set<ADDRINT> > dom,unordered_map<ADDRINT,set<ADDRINT> > &dominatorTree);
	/*Analysis Object */
	Analysis *analysis;
public:
	GraphBranch(std::string functionName,list<Node*> *nodes);
	GraphBranch(std::string functionName,list<Node*> *nodes,set<ADDRINT> insensitiveSet);
	void printCFGToDot();
	void printInsensitiveCFGToDot();
	void printReverseCFGToDot();
	void printDominatorTreeToDot();
	void printPostDominatorTreeToDot();
	void printControlDependencyGraphToDot();
	void test();
	void printAdjacencyset(unordered_map<ADDRINT,set<ADDRINT> > adj);
	void printAdjacency();
	void testVector();
	void printToDot(std::vector<GraphNode*> &cfg);
	};
//CFG for all Branch
GraphBranch::GraphBranch(std::string functionName,list<Node*> *nodes){
	this->functionName=functionName;
	this->nodes=nodes;
	
	constructCFGAdj();
	constructReverseCFGAdj();
	constructInsensitiveCFGAdj();
	analysis=new Analysis();
}

//CFG for insensitive set 
GraphBranch::GraphBranch(std::string functionName,list<Node*> *nodes,set<ADDRINT> insensitiveSet){
	this->functionName=functionName;
	this->nodes=nodes;
	this->insensitiveSet=insensitiveSet;
	constructCFGAdj();// we need the CFG for easily construction of insensitive one
	constructReverseCFGAdj();
	constructInsensitiveCFGAdj();
}


ADDRINT GraphBranch::getBranchAddressNode(ADDRINT nodeAddress){

	for(list<Node*>::iterator it=nodes->begin();it!=nodes->end();++it){
			Node *node = (*it);
			if(nodeAddress==node->nodeAddress)
			{
			// return the exit branch address of the BasicBlock or a Node
			
			if(node->branchAddress==0){
				//recurse
				return GraphBranch::getBranchAddressNode(node->notTakenBB);
			}
			else 
				return node->branchAddress;
			}	    
	}

//not found zero
return 0;
	

}

void GraphBranch::constructCFGAdj(){
// Create Connection
	ofstream outFile;
	outFile.open("constructCFGAdj.log");
	list<Node *>::iterator iter;
	ADDRINT start=1;
	ADDRINT exit=2;
	

	iter = nodes->begin();
	Node *startNode =*iter;

	GraphBranch::addCFGAdjEdge(start,startNode->branchAddress);
	GraphBranch::addCFGAdjEdge(start,exit);

for (;iter != nodes->end(); ++iter)
{
	 Node *node =*iter;

	if(node->branchAddress==0 || node->nodeAddress==0) continue;
	 outFile<<"node -->"<<std::hex<<node->branchAddress<<"\n";
	  CFGAdj[node->branchAddress];
	if(node->takenBB!=0)
	{

		ADDRINT takenBrAddr=GraphBranch::getBranchAddressNode(node->takenBB);
		if(takenBrAddr!=0)
		GraphBranch::addCFGAdjEdge(node->branchAddress,takenBrAddr);
		else
			{
				CFGAdj[node->branchAddress];
				
			}

		
	}
	//outFile<<std::dec<<node->nodeAddress<<"->"<<node->takenBB<<";\n";
	 if(node->notTakenBB!=0){
		ADDRINT notTakenBrAddr=GraphBranch::getBranchAddressNode(node->notTakenBB);
		if(notTakenBrAddr!=0)
		GraphBranch::addCFGAdjEdge(node->branchAddress,notTakenBrAddr);
	  	else
	  	{
	  		CFGAdj[node->branchAddress];
	  	}
	  //outFile<<std::dec<<node->nodeAddress<<"->"<<node->notTakenBB<<";\n";
	}
	
}

// add end node 
// 
CFGAdj[exit];
	

for(unordered_map<ADDRINT,set<ADDRINT> >::iterator it=CFGAdj.begin();it!=CFGAdj.end();it++){
	if((*it).second.size()==0){
		//exit=2;
		if((*it).first!=exit){
		  //CFGAdj[(*it).first].insert(exit);
		}
	}
	}
//last element 



	for(unordered_map<ADDRINT,set<ADDRINT> >::iterator  it=CFGAdj.begin();it!=CFGAdj.end();++it){
				finalCFGAdj[(*it).first];
				outFile<<std::hex<<(*it).first<<"\n";
				for(set<ADDRINT>::reverse_iterator  i=(*it).second.rbegin();i!=(*it).second.rend();++i){
					finalCFGAdj[(*it).first].insert(*i);
				}

		}

// Add nodes to r and exit

}


void GraphBranch::addCFGAdjEdge(ADDRINT v,ADDRINT w){
	CFGAdj[v].insert(w); // add w to v's set
}

void GraphBranch::addReverseCFGAdjEdge(ADDRINT v,ADDRINT w){
	reverseCFGAdj[v].insert(w); // add w to v's set
}

void GraphBranch::constructReverseCFGAdj(){
	ofstream outFile;
	outFile.open("reversecfg.log");
	for(unordered_map<ADDRINT,set<ADDRINT> >::iterator i=CFGAdj.begin();i!=CFGAdj.end();++i){

			ADDRINT node = (*i).first;
			reverseCFGAdj[node];
		
			for(set<ADDRINT>::iterator it=(*i).second.begin();it!=(*i).second.end();++it){
				addReverseCFGAdjEdge(*it,node);
				
			}

			// This is for node that has no parent, we need to print to dot
			
				//reverseCFGAdj[node].clear();
		}

		// do the modification here 
		for(unordered_map<ADDRINT,set<ADDRINT> >::iterator  it=reverseCFGAdj.begin();it!=reverseCFGAdj.end();++it){
				finalReverseCFGAdj[(*it).first];
					outFile<<std::hex<<(*it).first<<"\n";
				for(set<ADDRINT>::reverse_iterator  i=(*it).second.rbegin();i!=(*it).second.rend();++i){
					finalReverseCFGAdj[(*it).first].insert(*i);
				}

		}



}

void GraphBranch::addInsensitiveCFGAdjEdge(ADDRINT v,ADDRINT w){
	insensitiveCFGAdj[v].insert(w); // add w to v's set
}

void GraphBranch::addInsensitiveReverseCFGAdjEdge(ADDRINT v,ADDRINT w){
	reverseCFGAdj[v].insert(w); // add w to v's set
}


void GraphBranch::constructInsensitiveCFGAdj(){

	set<ADDRINT> visited;
	
	for(unordered_map<ADDRINT,set<ADDRINT> >::iterator it=finalCFGAdj.begin();it!=finalCFGAdj.end();++it){
		ADDRINT v=(*it).first;
		if(isBranchInsensitive(v)){
			
			for(set<ADDRINT>::iterator cit=(*it).second.begin();cit!=(*it).second.end();++cit){
				ADDRINT w=*cit;
				addInsensitiveCFGAdjEdge(v,w);
			}
		}
	}

}

bool GraphBranch::isBranchInsensitive(ADDRINT brAddr)
{
	return insensitiveSet.find(brAddr)!=insensitiveSet.end();
}


// this function take a CFG,dominators and dominatorTree the resultance is dominatorTree
void GraphBranch::getDenominatorTree(unordered_map<ADDRINT,set<ADDRINT> > CFG,std::unordered_map<ADDRINT,std::set<ADDRINT> > dom,unordered_map<ADDRINT,set<ADDRINT> > &dominatorTree){
	//Let no be the root of DT
	// put n_o on Queue Q;
	unordered_map<ADDRINT,set<ADDRINT> >::iterator it=CFG.begin();// CFG
	ADDRINT n0=(*it).first; // n0

	queue<ADDRINT> Q;
	Q.push(n0);
	unordered_map<ADDRINT,set<ADDRINT> > tDom; // D(n) TODO  BUG 
	/************************D(n)=D(n)-n**********************/
	for(it=CFG.begin();it!=CFG.end();++it){
		
		set<ADDRINT> setDom = dom[(*it).first];
		//iterate over setDom and erase the n node 
		for(set<ADDRINT>::iterator eraseIT=setDom.begin();eraseIT!=setDom.end();++eraseIT){
			if(*eraseIT==(*it).first){
				setDom.erase(eraseIT);
			}
		} 
		
		for(set<ADDRINT>::iterator eraseIT=setDom.begin();eraseIT!=setDom.end();++eraseIT){
			tDom[(*it).first].insert(*eraseIT);
		}
	}
	/******************************************** **/	
	while(!Q.empty())
	{
		ADDRINT m = Q.front();
		Q.pop();
		// for each node n in N
		for(it=CFG.begin();it!=CFG.end();++it){
			ADDRINT n = (*it).first;
			if(tDom[n].empty()) continue;
			
			
				// if D(n) contains m
			for(set<ADDRINT>::iterator i=(tDom[n]).begin();i!=tDom[n].end();++i){
				
				if(*i==m){
					// D(n)= D(n)-m
					tDom[n].erase(i);
				}
			}

			if(tDom[n].empty()){
				//addn to DT as a child to m;
				// add n to Q
				dominatorTree[m].insert(n); // this is add edges , what about leaf node
				Q.push(n);

			}

		}
	}

	for(it=CFG.begin();it!=CFG.end();++it){
		ADDRINT n = (*it).first;
		//leaf node only
		if(dominatorTree.find(n)==dominatorTree.end()){
			dominatorTree[n].insert(0);
			dominatorTree[n].clear();
		}
	}



}



/**
DOT GraphViz Helper functions here 
***/
void GraphBranch::generateDot(unordered_map<ADDRINT,set<ADDRINT> > adj,string fileName){
	ofstream outFile;
	fileName=fileName+".dot";
	outFile.open(fileName.c_str());
	outFile<<"digraph G {\nnode[shape=record]";
		
		// Vertices
	for(std::unordered_map<ADDRINT,set<ADDRINT> >::iterator it = adj.begin(); it != adj.end(); ++it) {
		//if((*it).first==0) continue;
		outFile<<std::dec<<(*it).first<<std::dec<<"[label=\""<<std::hex<<(*it).first<<"\"];\n";
	}
	// Edges : Connection here 
	for(std::unordered_map<ADDRINT,set<ADDRINT> >::iterator it = adj.begin(); it != adj.end(); ++it) {
		
		for(std::set<ADDRINT>::iterator lit=(*it).second.begin();lit!=(*it).second.end();++lit){
			//if(*lit==0 || (*it).first==0)continue;
			outFile<<std::dec<<(*it).first<<"->"<<*lit<<";\n";
		}
		
	}
	outFile<<"}\n";

	
	outFile.close();
}


void GraphBranch::printCFGToDot(){
	 fileName="CFG_"+functionName;
	generateDot(CFGAdj,fileName);
	

}

// Validated from paper
void GraphBranch::printDominatorTreeToDot(){
	fileName="DT_"+functionName;

	unordered_map<ADDRINT,std::set<ADDRINT> > dominators=analysis->getDominator(finalReverseCFGAdj);
	unordered_map<ADDRINT,set<ADDRINT> > DTree;
	GraphBranch::getDenominatorTree(finalReverseCFGAdj,dominators,DTree);
	generateDot(DTree,fileName);
}

void GraphBranch::printPostDominatorTreeToDot(){
	fileName="RDT_"+functionName;
	unordered_map<ADDRINT,std::set<ADDRINT> > dominators=analysis->getDominator(reverseCFGAdj);
	unordered_map<ADDRINT,set<ADDRINT> > DTree;
	GraphBranch::getDenominatorTree(reverseCFGAdj,dominators,DTree);
	generateDot(DTree,fileName);
}

void GraphBranch::printInsensitiveCFGToDot(){
	 fileName="ICFG_"+functionName;
	generateDot(insensitiveCFGAdj,fileName);
}

void GraphBranch::printReverseCFGToDot(){
	 fileName="reverseCFG_"+functionName;
	generateDot(finalReverseCFGAdj,fileName);
}

void GraphBranch::printControlDependencyGraphToDot(){
	unordered_map<ADDRINT,set<ADDRINT> > cdg=analysis->getDependencyGraph(finalReverseCFGAdj,CFGAdj);
	fileName="CDG_"+functionName;
	generateDot(cdg,fileName);
}

void GraphBranch::test(){
	ofstream outFile;
	outFile.open("test.log");

	list<ADDRINT> order=analysis->getPostOrder(reverseCFGAdj);

	for(list<ADDRINT>::iterator it=order.begin();it!=order.end();it++){
		outFile<<std::hex<<*it<<"\n";
	}
	order.reverse();

		outFile<<"\n";
	for(list<ADDRINT>::iterator it=order.begin();it!=order.end();it++){
		outFile<<std::hex<<*it<<"\n";
	}

	unordered_map<ADDRINT,ADDRINT> IDom= analysis->getImmediateDominator(reverseCFGAdj);
		
	for(unordered_map<ADDRINT,ADDRINT>::iterator it=IDom.begin();it!=IDom.end();it++){
		outFile<<std::hex<<(*it).first<<"IDOM -->"<<std::hex<<(*it).second<<"\n";
	}	

outFile.close();
}

void GraphBranch::printAdjacency(){

	printAdjacencyset(finalCFGAdj);
}


void GraphBranch::printAdjacencyset(unordered_map<ADDRINT,set<ADDRINT> > adj){
	ofstream outFile;
	outFile.open("adjacency.log");

	for(unordered_map<ADDRINT,set<ADDRINT> >::iterator it=adj.begin();it!=adj.end();++it){
		outFile<<""<<std::hex<<(*it).first<<"-->{ ";
		for(set<ADDRINT>::iterator i=(*it).second.begin();i!=(*it).second.end();i++){
			outFile<<std::hex<<*i<<",";
		}
		outFile<<"}\n\n";
	}
}

// need to shift to some proper ds for representing the node 
void GraphBranch::testVector(){

	std::vector<GraphNode*> cfg[2];


	// 1--> 2,3,4
	// 4 --> 4,5
	// Operate by node number, that all man
	GraphNode *gNode = new GraphNode(1,5757657);
	cfg[1]->push_back(gNode);
	GraphNode *gNode2 = new GraphNode(2,6778);
	cfg[1]->push_back(gNode2);
	GraphNode *gNode3 = new GraphNode(3,667778);
	cfg[1]->push_back(gNode3);

	// print to dot
	GraphBranch::printToDot(cfg);


}


void GraphBranch::printToDot(std::vector<GraphNode*> &cfg){
	ofstream outFile;
	outFile.open("vector.dot");
	outFile<<"ok"<<cfg.size();
	// print all node 
	for(std::vector<GraphNode *>::iterator it=cfg.begin();it!=cfg.end();it++){
		GraphNode *node = *it;
		outFile<<"ID"<<node->nodeId;
		outFile<<"\n"<<node->data<<"\n";

	}

}



// TODO : BUG IN IMMEDIATE DOMINATOR.. 
