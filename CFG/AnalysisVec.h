#include <fstream>
#include <iomanip>
#include <iostream>
#include <string.h>
#include "pin.H"
#include <set>
#include <map>
#include "stdlib.h"
#include <sstream>
#include <set>
#include <cstring>
#include <bits/stdc++.h>

class AnalysisVec
{
public:
	AnalysisVec();
	~AnalysisVec();
	ofstream AnalysisVecLog;
	
	// input an adj matrix aout a predessor
	vector<set<int> >  getPredecessor(vector<vector<int> > adj,bool isReverseCFG);
	 map<int,std::set<int> > getDominator(vector<vector<int> > CFG);
	  map<int,int> getImmediateDominator(vector<vector<int> > G,bool isReverseCFG);
	   vector<vector<int> > getDominanceFrontier(vector<vector<int> > adj,bool isReverseCFG);
	   vector<vector<int> > getDependencyGraph(vector<vector<int> > reverseCFG,vector<vector<int> > CFG); // return a graph in adjacency set
	  vector<int>  intersection(vector<int> first,vector<int> segetd);
	  list<int> getPostOrder(vector<vector<int> > G);
	  void dfs_walk(vector<vector<int> > G,set<int> &visited,list<int> &order,int node);
	int intersect(int node1,int node2,list<int> set,map<int,int> Idom);
	int findIndex(int node,list<int> set);
	set<int> getDF(int x,vector<vector<int> > &G,map<int,int> &idom,map<int,set<int> > &dom);
	 bool difference(vector<int> first,vector<int> second);
	 bool isItemInVector(vector<int> vec,int item);
	void getDominatorTree(vector<vector<int> > CFG,std::map<int,set<int> > dom,vector<vector<int> > &dominatorTree);
	void removeElement(vector<int> &vec,int element);
	set<int>  intersectionSet(set<int> first,set<int> second);
	map<int,std::set<int> > getPostDominator(vector<vector<int> > reverseCFG);
	void getPostDominatorTree(vector<vector<int> > reverseCFG,std::map<int,set<int> > dom,vector<vector<int> > &dominatorTree);
};

AnalysisVec::AnalysisVec(){
	AnalysisVecLog.open("AnalysisVec.log");
}

AnalysisVec::~AnalysisVec(){
	AnalysisVecLog.close();
}

 vector<set<int> > AnalysisVec::getPredecessor(vector<vector<int> > G,bool isReverseCFG){
	vector<set<int> >  predecessor(G.size());
	int nodeId;
	if(!isReverseCFG){
	for(nodeId=0;nodeId<(int)G.size();nodeId++) {
        int u=nodeId;
        int edgeId;
        for(edgeId=0;edgeId<(int)G[nodeId].size();edgeId++){
    		int v=G[u][edgeId];		
    		predecessor[v].insert(u);	
    	} 
    }
}
else{
    for(nodeId=(int)G.size()-1;nodeId>=0;nodeId--) {
        int u=nodeId;
        int edgeId;
        for(edgeId=0;edgeId<(int)G[nodeId].size();edgeId++){
    		int v=G[u][edgeId];		
    		predecessor[v].insert(u);	
    	} 
    }
}
LOG("predessor");
return predecessor;
}

void AnalysisVec::removeElement(vector<int> &vec,int element){
	int index;
	for(index=0;index<(int)vec.size();index++){
		if(vec[index]==element)
			break;
	}	

	vec.erase(vec.begin()+index);
}

// Verify to be correct 	
map<int,std::set<int> > AnalysisVec::getDominator(vector<vector<int> > CFG){
 		ofstream outFile;
		outFile.open("dominators.log");
 	map<int,set<int> > dominators;

 	vector<set<int> > predecessor=AnalysisVec::getPredecessor(CFG,false);
		
	//dom(s)<--{s}
	dominators[0].insert(0);
	// foreach w belong to V-{s} do dom(w)<-- V where V is set of all nodes
	for(int nodeId=1;nodeId<(int)CFG.size();nodeId++) {
		//std::unordered_map<int,set<int> >::iterator initIt = adj.begin();
		for(int nodeId1=0;nodeId1<(int)CFG.size();nodeId1++) {
			dominators[nodeId].insert(nodeId1);		
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

	for(int nodeId=1;nodeId<(int)CFG.size();nodeId++) {
			int w=nodeId;
			set<int> old = dominators[w];
			// find the intersection of predessor and union them
			// get all Dom(p) 
			set<int> predW = predecessor[w];
			set<int>::iterator predIt=predW.begin();

			set<int> intersectionDom=dominators[*predIt];
            	predIt++;
	        	
	
	for(;predIt!=predW.end();++predIt){
         	intersectionDom = AnalysisVec::intersectionSet(intersectionDom,dominators[*predIt]);     	

		}
		         
            set<int>::iterator i;
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
            	std::set<int> diff;

			std::set_difference(old.begin(), old.end(), dominators[w].begin(),dominators[w].end(), 
                        std::inserter(diff, diff.begin()));	
			
			
			if(diff.size()!=0) {
				change=true;
				
			}
		}
}
outFile<<"OK";
outFile.close();
return dominators;
 }


// Verify to be correct 	
map<int,std::set<int> > AnalysisVec::getPostDominator(vector<vector<int> > reverseCFG){
 		ofstream outFile;
		outFile.open("dominators.log");
 	map<int,set<int> > dominators;

 	vector<set<int> > predecessor=AnalysisVec::getPredecessor(reverseCFG,false);
		
	//dom(s)<--{s}
	dominators[reverseCFG.size()-1].insert(reverseCFG.size()-1);
	// foreach w belong to V-{s} do dom(w)<-- V where V is set of all nodes
	for(int nodeId=reverseCFG.size()-2;nodeId>=0;nodeId--) {
		//std::unordered_map<int,set<int> >::iterator initIt = adj.begin();
		for(int nodeId1=0;nodeId1<(int)reverseCFG.size();nodeId1++) {
			dominators[nodeId].insert(nodeId1);		
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

	for(int nodeId=reverseCFG.size()-2;nodeId>=0;nodeId--) {
			int w=nodeId;
			set<int> old = dominators[w];
			// find the intersection of predessor and union them
			// get all Dom(p) 
			set<int> predW = predecessor[w];
			set<int>::iterator predIt=predW.begin();

			set<int> intersectionDom=dominators[*predIt];
            	predIt++;
	        	
	
	for(;predIt!=predW.end();++predIt){
         	intersectionDom = AnalysisVec::intersectionSet(intersectionDom,dominators[*predIt]);     	

		}
		         
            set<int>::iterator i;
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
            	std::set<int> diff;

			std::set_difference(old.begin(), old.end(), dominators[w].begin(),dominators[w].end(), 
                        std::inserter(diff, diff.begin()));	
			
			
			if(diff.size()!=0) {
				change=true;
				
			}
		}
}
outFile<<"OK";
outFile.close();
return dominators;
 }



 vector<int>  AnalysisVec::intersection(vector<int> first,vector<int> second){
	// Intersection of two set 
	vector<int> result(first.size());
	for(int id=0;id<(int)first.size();id++){
		//if(second.find(*i)!=second.end()){
		if(isItemInVector(second,first[id])){
			result.push_back(first[id]);
		}
	}

	return result;


}

 set<int>  AnalysisVec::intersectionSet(set<int> first,set<int> second){
	// Insersection of two set 
	set<int> result;
	for(set<int>::iterator i=first.begin();i!=first.end();++i){
		if(second.find(*i)!=second.end()){
			result.insert(*i);
		}
	}

	return result;


}

bool AnalysisVec::isItemInVector(vector<int> vec,int item){
	for(int i=0;i<(int)vec.size();i++){
		if(vec[i]==item)
			return true;
	}

	return false;
}

 bool  AnalysisVec::difference(vector<int> first,vector<int> second){
	// Insersection of two set 
 	bool diff=false;
 	if(first.size()!=second.size()) return true;
	for(int id=0;id<(int)first.size();id++){
		if(!isItemInVector(second,first[id])){
			diff=true;
			break;	
		}
	}

	return diff;


}
// Need to REDO THIS ONE 
// Immediate Dominator bugs
// Taken from https://books.google.co.in/books?id=lXYkAwAAQBAJ&pg=PA70&lpg=PA70&dq=Immediate+dominator+Intersect&source=bl&ots=VzFMLLjZus&sig=bLlzwuAdzLCIkiFjTy7Z9PQfSsk&hl=en&sa=X&ved=0ahUKEwjL3o_43vzYAhWBsY8KHbdxAlEQ6AEIZzAI#v=onepage&q=Immediate%20dominator%20Intersect&f=false
map<int,int> AnalysisVec::getImmediateDominator(vector<vector<int> > G,bool isReverseCFG){
			ofstream outFile;
			outFile.open("im.log");
	        outFile<<"ok";
			vector<set<int> > predecessor=AnalysisVec::getPredecessor(G,isReverseCFG);		
			map<int,int> Idom;
			
			int nodeId=0;	
			int s=nodeId;

			Idom[s]=s;
			
			for(nodeId=1;nodeId<(int)G.size();nodeId++){
						
						Idom[nodeId]=0; // undefined 
			}

			bool change=true;
			while(change){
				change=false;
				// Get reverse post 
				LOG("post order\n");
				list<int> reversepostOrderset=AnalysisVec::getPostOrder(G);
				reversepostOrderset.reverse();
			
				for(list<int>::iterator it=reversepostOrderset.begin();it!=reversepostOrderset.end();it++){
	// need to know exactly what is processed 


					int w=*it;
						
						if(w==s) continue;

					int u=0;
					
					//predId=0;predId<(int)predecessor[w].size();predId++
					for(set<int>::iterator predId=predecessor[w].begin();predId!=predecessor[w].end();++predId){
						u=*predId;
						if(Idom[u]!=0){
							break;
						}

					}
					
					for(set<int>::iterator predId=predecessor[w].begin();predId!=predecessor[w].end();++predId){
							int v=*predId;
							if(v!=u)
							{
								
								if(Idom[v]!=0){
									u=intersect(u,v,reversepostOrderset,Idom);
									
								}		

							}
					}
					if(u!=Idom[w]){
						Idom[w]=u;
						outFile<<std::hex<<w<<"-->"<<std::hex<<u<<"\n";
						change=true;
					}

					
				}
				
			}

			outFile.close();
			LOG("Immediate end \n");

	return Idom;
}

int AnalysisVec::intersect(int node1,int node2,list<int> set,map<int,int> Idom){
	// get i and j 

	
	int finger1=node1;
	int finger2=node2;

	while(finger1!=finger2){
		int indexFinger1=AnalysisVec::findIndex(finger1,set);
		int indexFinger2=AnalysisVec::findIndex(finger2,set);
		AnalysisVecLog<<"indexFinger1-->"<<indexFinger1<<"\n"<<"indexFinger2-->"<<indexFinger2<<"\n";
		while(indexFinger1 > indexFinger2){
			finger1=Idom[finger1];
			indexFinger1=AnalysisVec::findIndex(finger1,set);
			AnalysisVecLog<<"indexFinger1-->"<<indexFinger1<<"\n"<<"indexFinger2-->"<<indexFinger2<<"\n";
			
			}

		while(indexFinger2>indexFinger1){
			finger2=Idom[finger2];
			 indexFinger2=AnalysisVec::findIndex(finger2,set);
			 AnalysisVecLog<<"indexFinger1-->"<<indexFinger1<<"\n"<<"indexFinger2-->"<<indexFinger2<<"\n";
		}

	}
	
	return finger1;

}

int AnalysisVec::findIndex(int node,list<int> set){
	int count = 0;
	std::list<int>::iterator it=set.begin();

	for(;it!=set.end();it++){
		if(*it==node)
			return count;

		count++;
	}
	return -1; // not found 
}
/*Algorithm taken from --> https://plg.uwaterloo.ca/~olhotak/cs744/slides/5dom.pdf SLIDE: 10 */
vector<vector<int> > AnalysisVec::getDominanceFrontier(vector<vector<int> > G, bool isReverseCFG){
	//for all nodes w do 
		//ordering of node required
		ofstream outFile;
		outFile.open("DominanceFrontier.log");
		vector<vector<int> > DF(G.size());
		

		vector<set<int> > predecessor=AnalysisVec::getPredecessor(G,true);
	 	
		LOG("after predecessor ");
	 	map<int,int> idom=AnalysisVec::getImmediateDominator(G,true); // BUG HERE
	 		LOG("after getImmediateDominator");
	 	map<int,set<int> > dom=AnalysisVec::getPostDominator(G); 

	LOG("line 327 ");
	 	int nodeId;
	 for(nodeId=(int)G.size()-1;nodeId>=0;nodeId--){
	 	
	 		set<int> DFx=getDF(nodeId,G,idom,dom);
	 		for(set<int>::iterator x=DFx.begin();x!=DFx.end();++x){
	 				DF[nodeId].push_back(*x);
	 		}
	}
	outFile<<"ok ";
	LOG("DF end ");
	return DF;
}

set<int> AnalysisVec::getDF(int x,vector<vector<int> > &G,map<int,int> &Idom,map<int,set<int> > &dom){
		
	
	
	set<int> S;
	
	// for all nodes w  belong to succ(x) do
	int edgeId;	
	for(edgeId=0;edgeId<(int)G[x].size();edgeId++){
		int w=G[x][edgeId];
		if(Idom[w]!=x){
			S.insert(w);
		}
	}
	

// for all node y  for which  idom(y)=x
	int nodeId;
	for(nodeId=0;nodeId<(int)G.size();nodeId++){

		
		//idom(y)=x
		if(Idom[nodeId]==x && nodeId!=x){
			
			set<int> DFy=AnalysisVec::getDF(nodeId,G,Idom,dom);
			
			for(set<int>::iterator w=DFy.begin();w!=DFy.end();++w){
				//if x does not dominate w  od x==w
				// dom(w)--{x}
					//dom[*w].find(x)==dom[*w].end()
					
					if(dom[*w].find(x)==dom[*w].end() || x==*w){
						S.insert(*w);
					}
			}

		}
	}

	
	return S;

}



// this function take a CFG,dominators and dominatorTree the resultance is dominatorTree
void AnalysisVec::getDominatorTree(vector<vector<int> > CFG,std::map<int,set<int> > dom,vector<vector<int> > &dominatorTree){
	//Let no be the root of DT
	// put n_o on Queue Q;
	
	
	int nodeId=0;
	queue<int> Q;
	Q.push(nodeId);
	map<int,set<int> > tDom; // D(n) TODO  BUG 
	/************************D(n)=D(n)-n**********************/
	for(nodeId=0;nodeId<(int)CFG.size();nodeId++){
		
		set<int> setDom = dom[nodeId];
		//iterate over setDom and erase the n node 
		for(set<int>::iterator it=setDom.begin();it!=setDom.end();it++){
			if(nodeId==*it){
				setDom.erase(*it);
				//removeElement(setDom,setDom[setDomId]);
			}
		} 
		
		for(set<int>::iterator it=setDom.begin();it!=setDom.end();it++){
			tDom[nodeId].insert(*it);
		}
	}
	/******************************************** **/	
	while(!Q.empty())
	{
		int m = Q.front();
		Q.pop();
		// for each node n in N
		for(int nodeId=0;nodeId<(int)CFG.size();nodeId++){
			
			if(tDom[nodeId].empty()) continue;
			
			
				// if D(n) contains m
			for(set<int>::iterator it=tDom[nodeId].begin();it!=tDom[nodeId].end();it++){
				
				if(*it==m){
					// D(n)= D[](n)-m
					tDom[nodeId].erase(*it);
					//removeElement(tDom[nodeId],tDom[nodeId][tdomId]);
				}
			}

			if(tDom[nodeId].empty()){
				//addn to DT as a child to m;
				// add n to Q
				dominatorTree[m].push_back(nodeId); // this is add edges , what about leaf node
				Q.push(nodeId);

			}

		}
	}





}




// this function take a CFG,dominators and dominatorTree the resultance is dominatorTree, here the reverse one 

void AnalysisVec::getPostDominatorTree(vector<vector<int> > CFG,std::map<int,set<int> > dom,vector<vector<int> > &dominatorTree){
	//Let no be the root of DT
	// put n_o on Queue Q;
	
	
	int nodeId=(int)CFG.size()-1;
	queue<int> Q;
	Q.push(nodeId);
	map<int,set<int> > tDom; // D(n) TODO  BUG 
	/************************D(n)=D(n)-n**********************/
	for(nodeId=(int)CFG.size()-1;nodeId>=0;nodeId--){
		
		set<int> setDom = dom[nodeId];
		//iterate over setDom and erase the n node 
		for(set<int>::iterator it=setDom.begin();it!=setDom.end();it++){
			if(nodeId==*it){
				setDom.erase(*it);
				//removeElement(setDom,setDom[setDomId]);
			}
		} 
		
		for(set<int>::iterator it=setDom.begin();it!=setDom.end();it++){
			tDom[nodeId].insert(*it);
		}
	}
	/******************************************** **/	
	while(!Q.empty())
	{
		int m = Q.front();
		Q.pop();
		// for each node n in N
		for(int nodeId=(int)CFG.size()-1;nodeId>=0;nodeId--){
			
			if(tDom[nodeId].empty()) continue;
			
			
				// if D(n) contains m
			for(set<int>::iterator it=tDom[nodeId].begin();it!=tDom[nodeId].end();it++){
				
				if(*it==m){
					// D(n)= D[](n)-m
					tDom[nodeId].erase(*it);
					//removeElement(tDom[nodeId],tDom[nodeId][tdomId]);
				}
			}

			if(tDom[nodeId].empty()){
				//addn to DT as a child to m;
				// add n to Q
				dominatorTree[m].push_back(nodeId); // this is add edges , what about leaf node
				Q.push(nodeId);

			}

		}
	}





}






// the adj we expect is a reverseCFG already
//https://cseweb.ucsd.edu/classes/fa03/cse231/lec6seq.pdf
vector<vector<int> > AnalysisVec::getDependencyGraph(vector<vector<int> > reverseCFG,vector<vector<int> > CFG){
	vector<vector<int> > DG(CFG.size());
	
	vector<vector<int> > DF=AnalysisVec::getDominanceFrontier(reverseCFG,true);
	
	ofstream outFile;
	outFile.open("DFLog.log");
	outFile<<"LOG";
   	for(int nodeId=0;nodeId<(int)DF.size();nodeId++){
   		outFile<<"DF("<<nodeId<<")-->{";
   		for(int dfId=0;dfId<(int)DF[nodeId].size();dfId++){
   			outFile<<DF[nodeId][dfId]<<",";
   		}
   		outFile<<"}\n";
   	}

			 LOG("Here\n");

	for(int x=0;x<(int)CFG.size();x++){
		for(int y=0;y<(int)CFG.size();y++){
			// 
			
			//Mistake :- The CDG has edge x → y whenever x∉ DFG′[y].
			//Y is in CD(X) in CFG G 
			//X in DF(Y) in Reverse CFG
			if(isItemInVector(DF[y],x)){
				DG[x].push_back(y);
								
			}
		}	
	}

	outFile.close();
	return DG;
}

/*
def postorder(graph, root):
    """Return a post-order ordering of nodes in the graph."""
    visited = set()
    order = []
    def dfs_walk(node):
        visited.add(node)
        for succ in graph.successors(node):
            if not succ in visited:
                dfs_walk(succ)
        order.append(node)
    dfs_walk(root)
    return order
*/
list<int> AnalysisVec::getPostOrder(vector<vector<int> > G){
	//
	
	set<int> visited;
	list<int> order;
	// here 0 is node 0
	AnalysisVec::dfs_walk(G,visited,order,0);
	return order;

}



void AnalysisVec::dfs_walk(vector<vector<int> > G,set<int> &visited,list<int> &order,int node){	
	visited.insert(node);
	// visit adjacency of node 
	int adjId;
	for(adjId=0;adjId<(int)G[node].size();adjId++){
		if(visited.find(G[node][adjId])==visited.end()){
			dfs_walk(G,visited,order,G[node][adjId]);
		}
	}
	order.push_back(node);

}






