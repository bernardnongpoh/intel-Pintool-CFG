#include <fstream>
#include <iomanip>
#include <iostream>
#include <string.h>
#include "pin.H"
#include <set>
#include <unordered_map>
#include "stdlib.h"
#include <sstream>
#include <set>
#include <cstring>
#include <bits/stdc++.h>

class Analysis
{
public:
	Analysis();
	~Analysis();
	ofstream AnalysisLog;
	
	// input an adj matrix aout a predessor
	 unordered_map<ADDRINT,std::set<ADDRINT> > getPredecessor(unordered_map<ADDRINT,set<ADDRINT> > adj);
	 unordered_map<ADDRINT,std::set<ADDRINT> > getSuccessor(unordered_map<ADDRINT,set<ADDRINT> > adj);
	  unordered_map<ADDRINT,std::set<ADDRINT> > getDominator(unordered_map<ADDRINT,set<ADDRINT> > adj);
	   unordered_map<ADDRINT,ADDRINT> getImmediateDominator(unordered_map<ADDRINT,set<ADDRINT> > adj);
	   unordered_map<ADDRINT,set<ADDRINT> > getDominanceFrontier(unordered_map<ADDRINT,set<ADDRINT> > adj);
	   unordered_map<ADDRINT,set<ADDRINT> > getDependencyGraph(unordered_map<ADDRINT,set<ADDRINT> > adj,unordered_map<ADDRINT,set<ADDRINT> > CFG); // return a graph in adjacency set
	  set<ADDRINT>  intersection(set<ADDRINT> first,set<ADDRINT> segetd);
	  list<ADDRINT> getPostOrder(unordered_map<ADDRINT,set<ADDRINT> > adj);
	  void dfs_walk(unordered_map<ADDRINT,set<ADDRINT> > adj,set<ADDRINT> &visited,list<ADDRINT> &order,ADDRINT node);
	ADDRINT intersect(ADDRINT node1,ADDRINT node2,list<ADDRINT> set,unordered_map<ADDRINT,ADDRINT> Idom);
	int findIndex(ADDRINT node,list<ADDRINT> set);
	set<ADDRINT> getDF(ADDRINT x,unordered_map<ADDRINT,set<ADDRINT> > &adj,unordered_map<ADDRINT,ADDRINT> &idom,unordered_map<ADDRINT,set<ADDRINT> > &dom,int n);
};

Analysis::Analysis(){
	AnalysisLog.open("Analysis.log");
}

Analysis::~Analysis(){
	AnalysisLog.close();
}

 unordered_map<ADDRINT,std::set<ADDRINT> > Analysis::getPredecessor(unordered_map<ADDRINT,set<ADDRINT> > adj){
	unordered_map<ADDRINT,set<ADDRINT> > predecessor;
	for(std::unordered_map<ADDRINT,set<ADDRINT> >::iterator it = adj.begin(); it != adj.end(); ++it) {
        for(std::set<ADDRINT> ::iterator adjIter=(*it).second.begin();adjIter!=(*it).second.end();++adjIter){
    		predecessor[*adjIter].insert((*it).first);	
    		} 
    }
return predecessor;
}


 	unordered_map<ADDRINT,std::set<ADDRINT> > Analysis::getDominator(unordered_map<ADDRINT,set<ADDRINT> > adj){
 		ofstream outFile;
outFile.open("dominators.log");
 	unordered_map<ADDRINT,set<ADDRINT> > dominators;
 	unordered_map<ADDRINT,set<ADDRINT> > predecessor=Analysis::getPredecessor(adj);
	std::unordered_map<ADDRINT,set<ADDRINT> >::iterator it = adj.begin();
	//dom(s)<--{s}
	dominators[(*it).first].insert((*it).first);

	++it;
	// foreach w belong to V-{s} do dom(w)<-- V where V is set of all nodes
	for(;it != adj.end(); ++it) {
		//std::unordered_map<ADDRINT,set<ADDRINT> >::iterator initIt = adj.begin();
		for(std::unordered_map<ADDRINT,set<ADDRINT> >::iterator initIt = adj.begin();initIt != adj.end(); ++initIt) {
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
         	intersectionDom = Analysis::intersection(intersectionDom,dominators[*predIt]);     	

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
outFile<<"OK";
outFile.close();
return dominators;
 }


 set<ADDRINT>  Analysis::intersection(set<ADDRINT> first,set<ADDRINT> second){
	// Insersection of two set 
	set<ADDRINT> result;
	for(set<ADDRINT>::iterator i=first.begin();i!=first.end();++i){
		if(second.find(*i)!=second.end()){
			result.insert(*i);
		}
	}

	return result;


}
// Need to REDO THIS ONE 
// Immediate Dominator bugs
// Taken from https://books.google.co.in/books?id=lXYkAwAAQBAJ&pg=PA70&lpg=PA70&dq=Immediate+dominator+Intersect&source=bl&ots=VzFMLLjZus&sig=bLlzwuAdzLCIkiFjTy7Z9PQfSsk&hl=en&sa=X&ved=0ahUKEwjL3o_43vzYAhWBsY8KHbdxAlEQ6AEIZzAI#v=onepage&q=Immediate%20dominator%20Intersect&f=false
unordered_map<ADDRINT,ADDRINT> Analysis::getImmediateDominator(unordered_map<ADDRINT,set<ADDRINT> > adj){
			ofstream outFile;
			outFile.open("im.log");
	        outFile<<"ok";
			unordered_map<ADDRINT,set<ADDRINT> > predecessor=Analysis::getPredecessor(adj);		
			unordered_map<ADDRINT,ADDRINT> Idom;
			std::unordered_map<ADDRINT,set<ADDRINT> >::iterator b=adj.begin();
			
			//if(start==0)
				//it++;
//
			ADDRINT s=(*b).first;

			Idom[s]=s;

			b++;
			for(;b!=adj.end();++b){
						Idom[(*b).first]=0; // undefined 
			}

			bool change=true;
			while(change){
				change=false;
				// Get reverse post 
				list<ADDRINT> reversepostOrderset=Analysis::getPostOrder(adj);
				reversepostOrderset.reverse();
				for(list<ADDRINT>::iterator it=reversepostOrderset.begin();it!=reversepostOrderset.end();it++){
	// need to know exactly what is processed 


					ADDRINT w=*it;
						
						if(w==s) continue;

					ADDRINT u=0;
					for(set<ADDRINT>::iterator i=predecessor[w].begin();i!=predecessor[w].end();i++){
						u=*i;
						if(Idom[u]!=0){
							break;
						}

					}
					
					for(set<ADDRINT>::iterator i=predecessor[w].begin();i!=predecessor[w].end();i++){
							ADDRINT v=*i;
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

	return Idom;
}

ADDRINT Analysis::intersect(ADDRINT node1,ADDRINT node2,list<ADDRINT> set,unordered_map<ADDRINT,ADDRINT> Idom){
	// get i and j 

	
	ADDRINT finger1=node1;
	ADDRINT finger2=node2;

	while(finger1!=finger2){
		int indexFinger1=Analysis::findIndex(finger1,set);
		int indexFinger2=Analysis::findIndex(finger2,set);
		AnalysisLog<<"indexFinger1-->"<<indexFinger1<<"\n"<<"indexFinger2-->"<<indexFinger2<<"\n";
		while(indexFinger1 > indexFinger2){
			finger1=Idom[finger1];
			indexFinger1=Analysis::findIndex(finger1,set);
			AnalysisLog<<"indexFinger1-->"<<indexFinger1<<"\n"<<"indexFinger2-->"<<indexFinger2<<"\n";
			
			}

		while(indexFinger2>indexFinger1){
			finger2=Idom[finger2];
			 indexFinger2=Analysis::findIndex(finger2,set);
			 AnalysisLog<<"indexFinger1-->"<<indexFinger1<<"\n"<<"indexFinger2-->"<<indexFinger2<<"\n";
		}

	}
	
	return finger1;

}

int Analysis::findIndex(ADDRINT node,list<ADDRINT> set){
	int count = 0;
	std::list<ADDRINT>::iterator it=set.begin();

	for(;it!=set.end();it++){
		if(*it==node)
			return count;

		count++;
	}
	return -1; // not found 
}
/*Algorithm taken from --> https://plg.uwaterloo.ca/~olhotak/cs744/slides/5dom.pdf SLIDE: 10 */
unordered_map<ADDRINT,set<ADDRINT> > Analysis::getDominanceFrontier(unordered_map<ADDRINT,set<ADDRINT> > adj){
	//for all nodes w do 
		unordered_map<ADDRINT,set<ADDRINT> > DF;
		
		unordered_map<ADDRINT,set<ADDRINT> > predecessor=Analysis::getPredecessor(adj);
	 	
	 	unordered_map<ADDRINT,ADDRINT> idom =Analysis::getImmediateDominator(adj); // BUG HERE
	 	
	 	unordered_map<ADDRINT,set<ADDRINT> > dom=Analysis::getDominator(adj); 


	 for(unordered_map<ADDRINT,set<ADDRINT> >::iterator it=adj.begin();it!=adj.end();++it){
	 	
	 		set<ADDRINT> DFx=getDF((*it).first,adj,idom,dom,100);
	 		for(set<ADDRINT>::iterator x=DFx.begin();x!=DFx.end();++x){
	 				DF[(*it).first].insert(*x);
	 		}
	}
	return DF;
}

set<ADDRINT> Analysis::getDF(ADDRINT x,unordered_map<ADDRINT,set<ADDRINT> > &adj,unordered_map<ADDRINT,ADDRINT> &Idom,unordered_map<ADDRINT,set<ADDRINT> > &dom,int n){
		
	AnalysisLog<<"\n"<<n<<"\n";
	AnalysisLog<<x;
	set<ADDRINT> S;
	
	// for all nodes w  belong to succ(x) do
	for(set<ADDRINT>::iterator w=adj[x].begin();w!=adj[x].end();w++){
		if(Idom[*w]!=x){
			S.insert(*w);
			
		}
	}
	

// for all node y  for which  idom(y)=x
	for(unordered_map<ADDRINT,set<ADDRINT> >::iterator y=adj.begin();y!=adj.end();++y){

		AnalysisLog<<std::hex<<"Idom("<<(*y).first<<")"<<Idom[(*y).first]<<" \n";
		//idom(y)=x
		if(Idom[(*y).first]==x && (*y).first!=x){
			AnalysisLog<<std::hex<<(*y).first<<" == ->"<<std::hex<<x<<"\n";
			set<ADDRINT> DFy=Analysis::getDF((*y).first,adj,Idom,dom,n-1);
			AnalysisLog<<"Here";
			for(set<ADDRINT>::iterator w=DFy.begin();w!=DFy.end();++w){
				//if x does not dominate w  od x==w
				// dom(w)--{x}
					if(dom[*w].find(x)==dom[*w].end() || x==*w){
						S.insert(*w);
					}
			}

		}
	}

	
	return S;

}
// the adj we expect is a reverseCFG already
//https://cseweb.ucsd.edu/classes/fa03/cse231/lec6seq.pdf
unordered_map<ADDRINT,set<ADDRINT> > Analysis::getDependencyGraph(unordered_map<ADDRINT,set<ADDRINT> > adj,unordered_map<ADDRINT,set<ADDRINT> > CFG){
	unordered_map<ADDRINT,set<ADDRINT> > DG;
	unordered_map<ADDRINT,set<ADDRINT> > DF=Analysis::getDominanceFrontier(adj);
	ofstream outFile;
	outFile.open("DG.log");

	for(unordered_map<ADDRINT,set<ADDRINT> >::iterator it=DF.begin();it!=DF.end();it++){
		outFile<<"DF["<<std::hex<<(*it).first<<"]-->{";
		for(set<ADDRINT>::iterator i=(*it).second.begin();i!=(*it).second.end();i++){
			outFile<<std::hex<<*i<<",";
		}	
		outFile<<"}\n";
	}

	
	for(unordered_map<ADDRINT,set<ADDRINT> >::iterator xit=CFG.begin();xit!=CFG.end();++xit){
			ADDRINT x=(*xit).first;
			DG[x];
		for(unordered_map<ADDRINT,set<ADDRINT> >::iterator yit=CFG.begin();yit!=CFG.end();++yit){
			ADDRINT y=(*yit).first;
			// 
			
			//Mistake :- The CDG has edge x → y whenever x∉ DFG′[y].
			//Y is in CD(X) in CFG G 
			//X in DF(Y) in Reverse CFG
			if(DF[y].find(x)!=DF[y].end()){

				DG[x].insert(y);
								
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
list<ADDRINT> Analysis::getPostOrder(unordered_map<ADDRINT,set<ADDRINT> > adj){
	//
	unordered_map<ADDRINT,set<ADDRINT> >::iterator it=adj.begin();
	set<ADDRINT> visited;
	list<ADDRINT> order;

	Analysis::dfs_walk(adj,visited,order,(*it).first);
	return order;

}




void Analysis::dfs_walk(unordered_map<ADDRINT,set<ADDRINT> > adj,set<ADDRINT> &visited,list<ADDRINT> &order,ADDRINT node){	
	visited.insert(node);
	for(set<ADDRINT>::iterator it=adj[node].begin();it!=adj[node].end();it++){
		if(visited.find(*it)==visited.end()){
			dfs_walk(adj,visited,order,*it);
		}
	}
	order.push_back(node);

}






