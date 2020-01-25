// You  may have subgraph depend on how many function you have here 
class Instruction{
public:
  ADDRINT instructionAddr;
  std::string assemblyCode;
  int opcode; // this is taken from Pintool 
  vector<string> operands;

  Instruction(ADDRINT instructionAddr,std::string assemblyCode,int opcodeNumber);

};

Instruction::Instruction(ADDRINT instructionAddr,std::string assemblyCode,int opcode){
	this->instructionAddr=instructionAddr;
	this->assemblyCode=assemblyCode;
	
	this->opcode=opcode;
	
}

class Node{
public:
	ADDRINT nodeAddress; // Address of the first instruction in the basic block
	std::list<Instruction*> *instructions;
	ADDRINT takenBB;
	ADDRINT notTakenBB;
	ADDRINT branchAddress; // The node that has exit node with branch
	void addInstruction(Instruction *ins);
	Node();

};

Node::Node(){
	this->instructions=new std::list<Instruction*>();
	this->notTakenBB=0;
	this->branchAddress=0;
	this->takenBB=0;
}

void Node::addInstruction(Instruction *ins){
	instructions->push_back(ins);
}

// so we need to store in a a vector<Graph>[]
class GraphNode
{

public:
	GraphNode(int nodeId,ADDRINT data);
	int nodeId;
	ADDRINT data; // This 
 
};

GraphNode::GraphNode(int nodeId,ADDRINT data){
	this->nodeId=nodeId;
	this->data=data;
}



