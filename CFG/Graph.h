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
#include <bits/stdc++.h>


#ifndef SYMBOL
#define SYMBOL header
#include "DataType.h"
#endif


using namespace std;



class Graph{
	int v; // No fo vertices
	std::map<ADDRINT,list<ADDRINT> > adj;
	std::map<ADDRINT,list<ADDRINT> > reverseCFG;
	std::map<ADDRINT,pair<ADDRINT,string> > GEN;
	std::map<ADDRINT,set<pair<ADDRINT,string> > > KILL;
	/**
	adjBranch : where adj(m)-> {n,o,op..} list
	it store only branch instruction in the CFG, if a Basic Block is not having an exit instruction
	a branch follow the connection and eliminate the BB 
	**/
	
	vector<ADDRINT> *adjVec;
	list<Node *> *nodes;
	std::map<ADDRINT,Node* > NodeMap;
	std::string functionName;

	std::map<ADDRINT,std::set<ADDRINT> > predecessor; // this immediate predessor
		std::map<ADDRINT,std::set<ADDRINT> > Allpredecessor; // All predessor
	std::map<ADDRINT,std::set<ADDRINT> > successor;
	// this map dom(w)->set of nodes that dominate w
	std::map<ADDRINT,std::set<ADDRINT> > dominators;
	
	std::map<ADDRINT,ADDRINT> idominators; // Imediate dominators.  
	
	set<ADDRINT> intersection(set<ADDRINT> first,set<ADDRINT> second);
set<ADDRINT> getKillSet(ADDRINT nodeAddress,std::string descOperand);
	void constructCFG();
	void computeDominator();
	void buildpredecessor();
	void computeImmediateDenminator();
	void constructReverseCFG();
	void constructAdjBranch();
	void buildAllpredecessor();
	void printCFGToDot(map<ADDRINT,list<ADDRINT> > adjacencyList,string fileName);
std::set<ADDRINT> getPredessesor(ADDRINT nodeAddress);

	void constructDataDependencyGraph();

public:
	Graph(std::string functionName,list<Node*> *nodes);
	void addEdge(ADDRINT v,ADDRINT w);
    void addNodes(Node *node);
    void printToDot();
    void printToDot(set<ADDRINT> insensitiveList);
    void printToDotOnlyInsensitive(set<ADDRINT> insensitiveList);
    void printAdj();
    // This function will print to dot insensitive branch node  + intermediate node without all instructions
    void printToDotCFG();
    std::map<ADDRINT,list<ADDRINT> > getAdj();
    void printPredecessorToDot();
   
};



Graph::Graph(std::string functionName,list<Node*> *nodes){
	this->functionName=functionName;
	this->nodes=nodes;
	//adj=new map<ADDRINT,list<ADDRINT> >();
	nodes=new list<Node*>();
	//NodeMap=new map<ADDRINT,Node*>();
	adjVec=new std::vector<ADDRINT>(nodes->size());

	constructCFG();
	buildpredecessor();
	computeDominator();
	computeImmediateDenminator();
	constructReverseCFG();
	buildAllpredecessor();
	constructDataDependencyGraph();
	

}

void Graph::constructDataDependencyGraph(){

	ofstream outFile;
	outFile.open("test.txt");
 	for(std::map<ADDRINT,list<ADDRINT> >::iterator it = adj.begin(); it != adj.end(); ++it) {
    /* std::cout << *it; ... */
    // String operation. Here 
	 /* istringstream f(*it);
    string s;    
    while (getline(f, s, ';')) {
        cout << s << endl;
        
    }*/

	}

list<Node *>::iterator iter;
   
 	for (iter = nodes->begin(); iter != nodes->end(); ++iter)
{
	 Node *node =*iter;
	 list<Instruction*>::iterator instIter;	
		
		for(instIter=node->instructions->begin();instIter!=node->instructions->end();++instIter){
			Instruction *ins = *instIter;

			if(ins->opcode== XED_ICLASS_CMOVB|| ins->opcode== XED_ICLASS_CMOVBE ||
ins->opcode==XED_ICLASS_CMOVL||
  ins->opcode==XED_ICLASS_CMOVLE||
  ins->opcode==XED_ICLASS_CMOVNB ||
  ins->opcode==XED_ICLASS_CMOVNBE ||
  ins->opcode==XED_ICLASS_CMOVNL ||
  ins->opcode==XED_ICLASS_CMOVNLE ||
  ins->opcode==XED_ICLASS_CMOVNO ||
  ins->opcode==XED_ICLASS_CMOVNP ||
  ins->opcode==XED_ICLASS_CMOVNS ||
  ins->opcode==XED_ICLASS_CMOVNZ ||
  ins->opcode==XED_ICLASS_CMOVO ||
  ins->opcode==XED_ICLASS_CMOVP ||
  ins->opcode==XED_ICLASS_CMOVS||
  ins->opcode==XED_ICLASS_CMOVZ ||
  ins->opcode==XED_ICLASS_MOV||
  ins->opcode==XED_ICLASS_MOVAPD||
 ins->opcode== XED_ICLASS_MOVAPS||
 ins->opcode== XED_ICLASS_MOVBE||
 ins->opcode== XED_ICLASS_MOVD||
 ins->opcode== XED_ICLASS_MOVDDUP||
 ins->opcode== XED_ICLASS_MOVDQ2Q||
  ins->opcode==XED_ICLASS_MOVDQA||
 ins->opcode== XED_ICLASS_MOVDQU||
 ins->opcode== XED_ICLASS_MOVHLPS||
  ins->opcode==XED_ICLASS_MOVHPD||
ins->opcode==  XED_ICLASS_MOVHPS||
ins->opcode==  XED_ICLASS_MOVLHPS||
 ins->opcode== XED_ICLASS_MOVLPD||
 ins->opcode== XED_ICLASS_MOVLPS||
ins->opcode==  XED_ICLASS_MOVMSKPD||
 ins->opcode== XED_ICLASS_MOVMSKPS||
 ins->opcode== XED_ICLASS_MOVNTDQ||
 ins->opcode== XED_ICLASS_MOVNTDQA||
ins->opcode==  XED_ICLASS_MOVNTI||
ins->opcode==  XED_ICLASS_MOVNTPD||
ins->opcode==  XED_ICLASS_MOVNTPS||
 ins->opcode== XED_ICLASS_MOVNTQ||
 ins->opcode== XED_ICLASS_MOVNTSD||
 ins->opcode== XED_ICLASS_MOVNTSS||
 ins->opcode== XED_ICLASS_MOVQ||
 ins->opcode== XED_ICLASS_MOVQ2DQ||
 ins->opcode== XED_ICLASS_MOVSB||
 ins->opcode== XED_ICLASS_MOVSD||
 ins->opcode== XED_ICLASS_MOVSD_XMM||
 ins->opcode== XED_ICLASS_MOVSHDUP||
  ins->opcode==XED_ICLASS_MOVSLDUP||
 ins->opcode== XED_ICLASS_MOVSQ||
 ins->opcode== XED_ICLASS_MOVSS||
 ins->opcode== XED_ICLASS_MOVSW||
 ins->opcode== XED_ICLASS_MOVSX||
ins->opcode==  XED_ICLASS_MOVSXD||
ins->opcode==  XED_ICLASS_MOVUPD||
  ins->opcode==XED_ICLASS_MOVUPS||
  ins->opcode==XED_ICLASS_MOVZX||
 ins->opcode== XED_ICLASS_MOV_CR||
 ins->opcode== XED_ICLASS_MOV_DR ||
 ins->opcode==XED_ICLASS_ADD ||
  ins->opcode==XED_ICLASS_ADDPD ||
  ins->opcode==XED_ICLASS_ADDPS ||
  ins->opcode==XED_ICLASS_ADDSD ||
  ins->opcode==XED_ICLASS_ADDSS ||
  ins->opcode==XED_ICLASS_ADDSUBPD ||
  ins->opcode==XED_ICLASS_ADDSUBPS ||

 ins->opcode== XED_ICLASS_AND||
  ins->opcode==XED_ICLASS_ANDN||
  ins->opcode==XED_ICLASS_ANDNPD||
  ins->opcode==XED_ICLASS_ANDNPS ||
  ins->opcode==XED_ICLASS_ANDPD ||
  ins->opcode==XED_ICLASS_ANDPS||

 ins->opcode== XED_ICLASS_DIV ||
  ins->opcode==XED_ICLASS_DIVPD||
  ins->opcode==XED_ICLASS_DIVPS||
  ins->opcode==XED_ICLASS_DIVSD||
  ins->opcode==XED_ICLASS_DIVSS||

     ins->opcode==XED_ICLASS_FCMOVB ||
   ins->opcode==XED_ICLASS_FCMOVBE ||
   ins->opcode==XED_ICLASS_FCMOVE ||
  ins->opcode== XED_ICLASS_FCMOVNB ||
  ins->opcode== XED_ICLASS_FCMOVNBE ||
   ins->opcode==XED_ICLASS_FCMOVNE ||
   ins->opcode==XED_ICLASS_FCMOVNU ||
   ins->opcode==XED_ICLASS_FCMOVU ||

	ins->opcode==XED_ICLASS_FDIV ||
	ins->opcode==XED_ICLASS_FDIVP ||
	ins->opcode== XED_ICLASS_FDIVR ||
 	ins->opcode== XED_ICLASS_FDIVRP ||

  	ins->opcode== XED_ICLASS_SUB ||
   	ins->opcode== XED_ICLASS_SUBPD ||
	ins->opcode==   XED_ICLASS_SUBPS ||
   ins->opcode== XED_ICLASS_SUBSD ||
   ins->opcode== XED_ICLASS_SUBSS 
  
  
  ){
	  
	  //  outFile<<ins->operands[1]<<"\n";
	  //std::map<ADDRINT,pair<ADDRINT,string> > GEN;
	  //outFile<<ins->assemblyCode<<"\n";
	  if(ins->operands.size()>0){
	  //outFile<<ins->operands[0]<<"\n";
	  std::pair <ADDRINT,string> GenPair=make_pair(ins->instructionAddr,ins->operands[0]);
	  GEN[ins->instructionAddr]=GenPair;




	// KILL SET HERE 

	// getKillset , we need to provide the operand as argument 


	  }
	  // What is the GEN Set here 



  }


		}

		std::set<ADDRINT> pred=getKillSet(node->nodeAddress,"");
		outFile<<std::hex<<node->nodeAddress<<"\n";
		outFile<<pred.size()<<"\n";
		
	
	}


 std::map<ADDRINT,pair<ADDRINT,string> >::iterator it;

for(it=GEN.begin();it!=GEN.end();++it){
	
	std::pair <ADDRINT,string> Pair = (*it).second;
	outFile<<std::hex<<Pair.first<<std::dec<<Pair.second<<"\n";
}




outFile.close();

}

set<ADDRINT> Graph::getKillSet(ADDRINT nodeAddress,std::string descOperand){
ofstream outFile;
	outFile.open("kill.txt");
	
		std::set<ADDRINT> pred=Allpredecessor[nodeAddress];//  This is working 
		
		for(ADDRINT nodeAddr:pred){
			
			Node *node = NodeMap[nodeAddr];
			std::cout<<node->instructions->size()<<"\n";
			std::list<Instruction*>::iterator it;

			for(it=node->instructions->begin();it!=node->instructions->end();++it){
					Instruction *ins=*it;
					
					outFile<<ins->assemblyCode	<<"\n";
			}


		}

		
		return pred;
		
	
 	
	 	// get Predessesor Node 

	// Look at the node that is predessesor of this node 
	
	outFile.close();
}





std::map<ADDRINT,list<ADDRINT> > Graph::getAdj(){
	return adj;
}

void Graph::printAdj(){
	ofstream outFile;
	outFile.open("test.txt");
	//for (std::list<ADDRINT>::iterator it=adj->begin(); it != adj->end(); ++it)
    //outFile<< *it;
    for(std::map<ADDRINT,list<ADDRINT> >::iterator it = adj.begin(); it != adj.end(); ++it) {
    /* std::cout << *it; ... */
    	outFile<<(*it).first<<"\n";
    	outFile<<(*it).second.size()<<"\n";
}
	outFile.close();
}



void Graph::constructCFG(){
// Create Connection
	list<Node *>::iterator iter;
for (iter = nodes->begin(); iter != nodes->end(); ++iter)
{
	
	 Node *node =*iter;
		NodeMap[node->nodeAddress]=node;

	
	if(node->takenBB!=0)
		Graph::addEdge(node->nodeAddress,node->takenBB);
	//outFile<<std::dec<<node->nodeAddress<<"->"<<node->takenBB<<";\n";
	if(node->notTakenBB!=0)
		Graph::addEdge(node->nodeAddress,node->notTakenBB);
	  //outFile<<std::dec<<node->nodeAddress<<"->"<<node->notTakenBB<<";\n";

}

}
// TODO 
void Graph::buildpredecessor(){
	
 for(std::map<ADDRINT,list<ADDRINT> >::iterator it = adj.begin(); it != adj.end(); ++it) {

        for(std::list<ADDRINT> ::iterator adjIter=(*it).second.begin();adjIter!=(*it).second.end();++adjIter){
    		predecessor[*adjIter].insert((*it).first);	
    		} 
    
}

	
}

void Graph::buildAllpredecessor(){
	
	std::map<ADDRINT,std::set<ADDRINT> >::iterator predIt;

	for(predIt=predecessor.begin();predIt!=predecessor.end();++predIt){
			ADDRINT nodeAddress=(*predIt).first;
			set<ADDRINT> predNodes= (*predIt).second;
			std::stack<ADDRINT> stack;

			for(ADDRINT p: predNodes){
				Allpredecessor[nodeAddress].insert(p);
				stack.push(p);
			}

			while(!stack.empty()){
				ADDRINT addr=stack.top();
				stack.pop();	

				set<ADDRINT> pr=getPredessesor(addr);
					for(ADDRINT pp: pr){
						Allpredecessor[nodeAddress].insert(pp);
						stack.push(pp);
					}


				
			}



	}



	
}

std::set<ADDRINT> Graph::getPredessesor(ADDRINT nodeAddress){
			return predecessor[nodeAddress];

}

void Graph::printPredecessorToDot(){
// print immediate predescessor
	ofstream outFile;
	string file="predescessor_"+functionName;
	outFile.open(file.c_str());

	for(std::map<ADDRINT,set<ADDRINT> >::iterator it = predecessor.begin(); it != predecessor.end(); ++it) {
				outFile<<"Node :" <<std::hex<< (*it).first<<"\n";	
			    for(std::set<ADDRINT> ::iterator pred=(*it).second.begin();pred!=(*it).second.end();++pred){
    				outFile<<std::dec<<"PRED :" <<std::hex<< *pred<<"\n";	
    			} 
    
	}	

	outFile.close();

}

void Graph::addEdge(ADDRINT v,ADDRINT w){
	adj[v].push_back(w); // add w to v's list
}

void Graph::addNodes(Node *node){
	nodes->push_back(node);
}


void Graph::printToDotCFG(){
	ofstream outFile;
	//char *str=strcat(functionName.c_str(),".dot");
	outFile.open(functionName.c_str());
	outFile<<"digraph G {\nnode[shape=record]";
		
		// Vertices
	for(std::map<ADDRINT,list<ADDRINT> >::iterator it = adj.begin(); it != adj.end(); ++it) {
		outFile<<std::dec<<(*it).first<<std::dec<<"[label=\""<<std::hex<<(*it).first<<"\"];\n";
	}
	// Edges : Connection here 
	for(std::map<ADDRINT,list<ADDRINT> >::iterator it = adj.begin(); it != adj.end(); ++it) {
		
		for(std::list<ADDRINT>::iterator lit=(*it).second.begin();lit!=(*it).second.end();++lit){
			outFile<<std::dec<<(*it).first<<"->"<<*lit<<";\n";
		}
		
	}
	outFile<<"}\n";
	outFile.close();
}

void Graph::printToDot(){

	// Connstruct the graph Here 
list<Node *>::iterator iter;

	ofstream outFile;
	//char *str=strcat(functionName.c_str(),".dot");
	outFile.open(functionName.c_str());
	outFile<<"digraph G {\nnode[shape=record]";

	
	for (iter = nodes->begin(); iter != nodes->end(); ++iter)
{
	//std::string instructions="";


   Node *node =*iter;
   	std::list<Instruction*>::iterator iterInst;
   	outFile<<std::dec<<node->nodeAddress<<std::dec<<"[label=\"";
 	
 	for (iterInst = node->instructions->begin(); iterInst != node->instructions->end(); ++iterInst){
 		
 		Instruction *ins=*iterInst;
   		outFile<<std::hex<<ins->instructionAddr<<":"<<std::dec<<ins->assemblyCode<<"\\n";
 		
 	}  	
 	outFile<<"\"];\n";
   
   //outFile<<"Entry "<<std::hex<<node->nodeAddress<<"\n";
   // Create the graph here

 	
   
}
// Create Connection
for (iter = nodes->begin(); iter != nodes->end(); ++iter)
{
	 Node *node =*iter;
	if(node->takenBB!=0)
	outFile<<std::dec<<node->nodeAddress<<"->"<<node->takenBB<<";\n";
	if(node->notTakenBB!=0)
	  outFile<<std::dec<<node->nodeAddress<<"->"<<node->notTakenBB<<";\n";

}

outFile<<"}";

outFile.close();
}

void Graph::printToDot(set<ADDRINT> insensitiveList){
ofstream outFile;
outFile.open(functionName.c_str());
	// Connstruct the graph Here 
list<Node *>::iterator iter;

	//char *str=strcat(functionName.c_str(),".dot");
	
	outFile<<"digraph G {\nnode[shape=record]";

	
	for (iter = nodes->begin(); iter != nodes->end(); ++iter)
{
	//std::string instructions="";


   Node *node =*iter;
   	std::list<Instruction*>::iterator iterInst;
   	outFile<<std::dec<<node->nodeAddress<<std::dec<<"[label=\"";
 	
 	for (iterInst = node->instructions->begin(); iterInst != node->instructions->end(); ++iterInst){
 		
 		Instruction *ins=*iterInst;
   		//outFile<<"<FONT COLOR=\"RED\">OK</FONT>\n";
   		outFile<<std::hex<<ins->instructionAddr<<":"<<std::dec<<ins->assemblyCode<<"\\n";
 		
 	}  	
 	outFile<<"\"];\n";
   
   //outFile<<"Entry "<<std::hex<<node->nodeAddress<<"\n";
   // Create the graph here

 	
   
}
// Create Connection
for (iter = nodes->begin(); iter != nodes->end(); ++iter)
{
	 Node *node =*iter;
	 
	if(node->takenBB!=0)
	{

		if(insensitiveList.find(node->branchAddress)!=insensitiveList.end())
	 	outFile<<"edge [color=red];\n";
		outFile<<std::dec<<node->nodeAddress<<"->"<<node->takenBB<<";\n";
		
	}
	
	if(node->notTakenBB!=0)
	  {
	  	
	 	
	  	outFile<<std::dec<<node->nodeAddress<<"->"<<node->notTakenBB<<";\n";
	  	outFile<<"edge [color=black];\n";
	  }

}

outFile<<"}";

outFile.close();
}


void Graph::printToDotOnlyInsensitive(set<ADDRINT> insensitiveList){
ofstream outFile;
outFile.open(functionName.c_str());
	// Connstruct the graph Here 
list<Node *>::iterator iter;

for (iter = nodes->begin(); iter != nodes->end(); ++iter)
{

}

	
	//char *str=strcat(functionName.c_str(),".dot");
	
	outFile<<"digraph G {\nnode[shape=record]";

	
	for (iter = nodes->begin(); iter != nodes->end(); ++iter)
{
	//std::string instructions="";
	

   Node *node =*iter;
   
   if(insensitiveList.find(node->branchAddress)==insensitiveList.end()) continue;

   	std::list<Instruction*>::iterator iterInst;
   	outFile<<std::dec<<node->nodeAddress<<std::dec<<"[label=\"";
 	
 	for (iterInst = node->instructions->begin(); iterInst != node->instructions->end(); ++iterInst){
 		
 		Instruction *ins=*iterInst;
   		//outFile<<"<FONT COLOR=\"RED\">OK</FONT>\n";
   		outFile<<std::hex<<ins->instructionAddr<<":"<<std::dec<<ins->assemblyCode<<"\\n";
 		
 	}  	
 	outFile<<"\"];\n";
   
   //outFile<<"Entry "<<std::hex<<node->nodeAddress<<"\n";
   // Create the graph here

 	
   
}
// Create Connection
for (iter = nodes->begin(); iter != nodes->end(); ++iter)
{
	 Node *node =*iter;
	 if(insensitiveList.find(node->branchAddress)==insensitiveList.end()) continue;
	 
	if(node->takenBB!=0)
	{
		if(insensitiveList.find(node->branchAddress)!=insensitiveList.end())
	 	outFile<<"edge [color=red];\n";
		outFile<<std::dec<<node->nodeAddress<<"->"<<node->takenBB<<";\n";
		
	}
	
	if(node->notTakenBB!=0)
	  {
	  		 	
	  	outFile<<std::dec<<node->nodeAddress<<"->"<<node->notTakenBB<<";\n";
	  	outFile<<"edge [color=black];\n";
	  }

}

outFile<<"}";

outFile.close();
}// simple implementation of set intersection 
set<ADDRINT>  Graph::intersection(set<ADDRINT> first,set<ADDRINT> second){
	// Insersection of two set 
	set<ADDRINT> result;
	for(set<ADDRINT>::iterator i=first.begin();i!=first.end();++i){
		if(second.find(*i)!=second.end()){
			result.insert(*i);
		}
	}

	return result;


}

void Graph::computeDominator(){
	ofstream outFile;
outFile.open("dominators.log");
	
	std::map<ADDRINT,list<ADDRINT> >::iterator it = adj.begin();
	//dom(s)<--{s}
	dominators[(*it).first].insert((*it).first);

	++it;
	// foreach w belong to V-{s} do dom(w)<-- V where V is set of all nodes
	for(;it != adj.end(); ++it) {
		//std::map<ADDRINT,list<ADDRINT> >::iterator initIt = adj.begin();
		for(std::map<ADDRINT,list<ADDRINT> >::iterator initIt = adj.begin();initIt != adj.end(); ++initIt) {
			dominators[(*it).first].insert((*initIt).first);		
		}
		
	}
/**for each w belong to V-{s}
	do
	 old = dom(w)
	 dom(w)<-- {w} U (Intersection of dom(p) where p is the intermediate predecessor of w
	 if(old=dom(w))
	   change is true 
**/
bool change =true;

while(change){
	change=false;
it = adj.begin();
	++it;
	for(;it != adj.end(); ++it) {
			ADDRINT w=(*it).first;
			set<ADDRINT> old = dominators[w];
			// find the intersection of predessor and union them
			// get all Dom(p) 
			set<ADDRINT> predW = predecessor[w];
			set<ADDRINT>::iterator predIt=predW.begin();

			set<ADDRINT> intersectionDom=dominators[*predIt];
            	predIt++;
	        	
	
	for(;predIt!=predW.end();++predIt){
         	intersectionDom = Graph::intersection(intersectionDom,dominators[*predIt]);     	

		}
		         
            set<ADDRINT>::iterator i;
			//continue;
            // union with the intersection
           
            // segmentation here
           // union canbe cust inserted right
           // set_union(dominators[w].begin(),dominators[w].end(), intersectionDom.begin(), intersectionDom.end(), diff.begin());
			 // remove everything from dominators
            dominators[w].clear();
            dominators[w].insert(w);
            // Union
		for(i=intersectionDom.begin();i!=intersectionDom.end();++i){
           	 	dominators[w].insert(*i);
           		}           	
            	std::set<ADDRINT> diff;

			std::set_difference(old.begin(), old.end(), dominators[w].begin(),dominators[w].end(), 
                        std::inserter(diff, diff.begin()));	
			
			
			if(diff.size()!=0) {
				change=true;
				
			}
		}
}
std::map<ADDRINT,set<ADDRINT> >::iterator dom = dominators.begin();
	for(;dom!=dominators.end();++dom){
		outFile<<"NODE -->"<<std::hex<<(*dom).first<<"\n";
    	 for(set<ADDRINT>::iterator i=(*dom).second.begin();i!=(*dom).second.end();++i){
           	    outFile<<std::hex<<"dominates by -->"<<*i<<"\n";
           } 
          
}
outFile.close();

}

void Graph::computeImmediateDenminator(){
	ofstream outFile;
outFile.open("immediatedominators.log");

	for(std::map<ADDRINT,set<ADDRINT> >::iterator it=dominators.begin();it!=dominators.end();++it){
		
		int numberOfDominators=(*it).second.size();
		int index = numberOfDominators-2;

		set<ADDRINT>::iterator immediateItem=(*it).second.begin();
		outFile<<index;
		advance(immediateItem, index);


		ADDRINT immDom = *immediateItem;

		idominators[(*it).first]=immDom;

	}

	// print to log file immediate dominator

	for(map<ADDRINT,ADDRINT>::iterator i=idominators.begin();i!=idominators.end();++i){
		outFile<<"Idom("<<std::hex<<(*i).first<<") = "<<(*i).second<<"\n";
				
			}



outFile.close();
}

void Graph::constructReverseCFG(){
		//
		for(map<ADDRINT,list<ADDRINT> >::iterator i=adj.begin();i!=adj.end();++i){

			ADDRINT node = (*i).first;
			for(list<ADDRINT>::iterator it=(*i).second.begin();it!=(*i).second.end();++it){
				reverseCFG[*it].push_back(node);
			}
			// This is for node that has no parent, we need to print to dot
			if(reverseCFG.find(node)==reverseCFG.end())
				reverseCFG[node].push_back(0); // this will just create the node...
		} 

		string fileName = "reverseCFG_"+functionName;
		printCFGToDot(reverseCFG,fileName);
}

// This function takes an input an adjacency list of the type map<ADDRINT,list<ADDRINT> > and a file name and print to dot
void Graph::printCFGToDot(map<ADDRINT,list<ADDRINT> > adjacencyList,string fileName){
	ofstream outFile;
	outFile.open(fileName.c_str());
	outFile<<"digraph G {\nnode[shape=record]";
		
		// Vertices
	for(std::map<ADDRINT,list<ADDRINT> >::iterator it = adjacencyList.begin(); it != adjacencyList.end(); ++it) {
		outFile<<std::dec<<(*it).first<<std::dec<<"[label=\""<<std::hex<<(*it).first<<"\"];\n";
	}
	// Edges : Connection here 
	for(std::map<ADDRINT,list<ADDRINT> >::iterator it = adjacencyList.begin(); it != adjacencyList.end(); ++it) {
		
		for(std::list<ADDRINT>::iterator lit=(*it).second.begin();lit!=(*it).second.end();++lit){
			outFile<<std::dec<<(*it).first<<"->"<<*lit<<";\n";
		}
		
	}
	outFile<<"}\n";


	outFile.close();
}

void Graph::constructAdjBranch(){
	//Need to look at the node here 
	// Create Connection
	list<Node *>::iterator iter;
	for (iter = nodes->begin(); iter != nodes->end(); ++iter)
	{
	 Node *node =*iter;
	if(node->takenBB!=0)
		Graph::addEdge(node->nodeAddress,node->takenBB);
	//outFile<<std::dec<<node->nodeAddress<<"->"<<node->takenBB<<";\n";
	if(node->notTakenBB!=0)
		Graph::addEdge(node->nodeAddress,node->notTakenBB);
	  //outFile<<std::dec<<node->nodeAddress<<"->"<<node->notTakenBB<<";\n";

	}
}







