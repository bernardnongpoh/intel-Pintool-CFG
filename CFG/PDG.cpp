//
// This tool counts the number of times a routine is executed and 
// the number of instructions executed in a routine
//

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string.h>
#include "pin.H"
#include <set>
#include <map>
#include "stdlib.h"
#include "DataDependencyGraph.h"
#include "Graph.h"
#include <sstream>
#include <list>
#include <initializer_list>

using namespace std;
ofstream outFile;
BOOL instrumentCodes = false;
set<ADDRINT> insensitiveSet;
map<ADDRINT,std::string> insensitivityFunctionName;
// This map function name to a set of insensitive instruction belong to the function
map<std::string,std::set<ADDRINT> > functionInstAddress;
std::string currentFunctionName;
map<ADDRINT,std::string> entryInstmap;

std::list<Node*> *NodeList;
std::string targetFunctionName;

std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}





// Pin calls this function every time a new rtn is executed
VOID Routine(RTN rtn, VOID *v)
{
       
     RTN_Open(rtn);

    std::string function_name=RTN_Name(rtn);
    // Only for the target function name
  if(function_name.compare(targetFunctionName)!=0){
    RTN_Close(rtn);
    return;
  } 
// Iterate over BB and Track the dependency among them, ie, creating CFG using Basic Block
         
    set<ADDRINT> entryInstAddr; // BB start address... 

    // Collect Entry Instruction to a Basic Block
        
        // Check if it is a branch instruction.
        // always entry
        INS entryInst = RTN_InsHead(rtn);
        ADDRINT addrEntry=INS_Address(entryInst);
        entryInstAddr.insert(addrEntry);

        for( INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins) )
        {
          
           if(INS_IsBranch(ins)){

            // Resolve the branch here 
            if(INS_HasFallThrough(ins)){
               
              if(INS_IsDirectBranchOrCall(ins))
              {
                ADDRINT addr=INS_DirectBranchOrCallTargetAddress(ins);
               entryInstAddr.insert(addr);
              }
              INS nextIns = INS_Next(ins);
              if(INS_Valid(nextIns)){
                ADDRINT nextAddress=INS_Address(nextIns);
               entryInstAddr.insert(nextAddress);
              }
            }
            else{
               if(INS_IsDirectBranchOrCall(ins))
              {
                ADDRINT addr=INS_DirectBranchOrCallTargetAddress(ins);
               entryInstAddr.insert(addr);
              }
            }
        }
    }
      Node *node=new Node();
      bool startNode=true;
      bool lastInstructionIsBranch=false;
      for( INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins) )
        {
          ADDRINT entryAddr=INS_Address(ins);

          if(entryInstAddr.find(entryAddr)!=entryInstAddr.end()){
              if(!lastInstructionIsBranch){
                // Get the address of the next instruction as the not taken branch
                 
              if(INS_Valid(ins)){
                //Bug here 
                ADDRINT prevAddress=INS_Address(ins);
                 node->notTakenBB=prevAddress;
              
              }


              }

              if(!startNode)
                NodeList->push_back(node);
             
              node=new Node();

              node->nodeAddress=entryAddr;
              
            }

             if(INS_IsRet(ins)){
                    node->takenBB=0;
                    node->notTakenBB=0;
                }
               
              std::string assembly= INS_Disassemble(ins);

              OPCODE opcode = INS_Opcode(ins);
              // Need to check which instruction is operating on this ins
             Instruction *instruction=new Instruction(INS_Address(ins),assembly,opcode);
             std::vector<std::string> str=split(assembly,',');
             if(str.size()>0){
               std::vector<std::string> descoperand=split(str[0],' ');
               if(descoperand.size()>0){
                 instruction->operands.push_back(descoperand[descoperand.size()-1]);
               }
             }





              
             
              node->addInstruction(instruction);

              // Check if it is a branch for connection between node
        if(INS_IsBranch(ins)){
          ADDRINT branchAddress=INS_Address(ins);
          node->branchAddress=branchAddress;
          lastInstructionIsBranch=true;
            // Resolve the branch here 
            if(INS_HasFallThrough(ins)){ 
               
              if(INS_IsDirectBranchOrCall(ins))
              {
                ADDRINT addr=INS_DirectBranchOrCallTargetAddress(ins);
                node->takenBB=addr;
              }
              INS nextIns = INS_Next(ins);
              if(INS_Valid(nextIns)){
                ADDRINT nextAddress=INS_Address(nextIns);
               
                node->notTakenBB=nextAddress;
              }
            }
            else{
               if(INS_IsDirectBranchOrCall(ins))
              {
                ADDRINT addr=INS_DirectBranchOrCallTargetAddress(ins);
                node->notTakenBB=addr;
              }
            }
        }
        else{
          lastInstructionIsBranch=false;
        }



      startNode=false;
        }
        // Push the last node that is return statement i quess
         NodeList->push_back(node);
        RTN_Close(rtn);
}

// This function is called when the application exits
// It prints the name and count for each procedure
VOID Fini(INT32 code, VOID *v)
{
 

Graph g(targetFunctionName,NodeList);
g.printToDot();

//g.printDominator();
//g.printCDG();
//g.printDominator();
//g.printDominatorTree();
//g.printInsensitiveDominatorTree();
//g.printInsensitiveCFGToDot();
//g.testVector();

//g.printAdjacency();
//g.printInsensitiveCFGToDot();
//g.printDominatorTreeToDot();

//Graph g1(targetFunctionName,NodeList);
// color the branch instruction that is insensitive 
//g1.printToDot(insensitiveSet);
//g.printToDotCFG();
//g.printAdj();

outFile.close();


}

/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */

INT32 Usage()
{
    cerr << "This Pintool counts the number of times a routine is executed" << endl;
    cerr << "and the number of instructions executed in a routine" << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */
KNOB<string> KnobFunctionName(
    KNOB_MODE_WRITEONCE, "pintool",
    "function", "main",
    "The target function for CFG Generation");
KNOB<string> KnobInsensitiveFile(
    KNOB_MODE_WRITEONCE, "pintool",
    "insensitiveFile", "candidate.insensitivity",
    "File contain insensitive instructions in HEX");

int main(int argc, char * argv[])
{
    // Initialize symbol table code, needed for rtn instrumentation
    PIN_InitSymbols();
    std::cout<<"Here again Just to Try it Out";
    outFile.open("PGGlog.out");
    outFile<<"OK";

    


    // Initialize pin
    if (PIN_Init(argc, argv)) return Usage();
 std::string fileName= KnobInsensitiveFile.Value();
ifstream insensitiveFile;
insensitiveFile.open(fileName.c_str());
  
  string addrline;
while(std::getline(insensitiveFile, addrline))
{
  std::stringstream convert;
  convert<<std::hex<<addrline;
  ADDRINT IP;
  convert>>IP;
  insensitiveSet.insert(IP);
   
}

    targetFunctionName = KnobFunctionName.Value();


    NodeList = new list<Node*>();
    // Register Routine to be called to instrument rtn
    RTN_AddInstrumentFunction(Routine, 0);
    // INS_AddInstrumentFunction(Instructions, 0);
     // TRACE_AddInstrumentFunction(Trace, 0);
    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);
    
    // Start the program, never returns
    PIN_StartProgram();
    
    return 0;
}

/**
Node M is the immediate dominator of 
node N ==> Node M must be the last dominator of N on any path from the start node to N.
**/


