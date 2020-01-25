/*
@Author: Bernard Nongpoh
@email: bernard.nongpoh@gmail.com
*/
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

using namespace std;
ofstream outFile;
map<std::string,std::set<ADDRINT> > functionInstAddress;



// Pin calls this function every time a new rtn is executed
VOID Routine(RTN rtn, VOID *v)
{

     RTN_Open(rtn);

    std::string function_name=RTN_Name(rtn);
  
// Iterate over BB and Track the dependency among them, ie, creating CFG using Basic Block
   
      
       
     
     for( INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins) )
      {
          // Filtering out all system functions.
         PIN_LockClient();
          IMG img = IMG_FindByAddress(INS_Address(ins));
          PIN_UnlockClient();
          if (IMG_Valid(img) && IMG_IsMainExecutable(img)){
             ADDRINT address=INS_Address(ins);
          
          // find the function in the map
          if(functionInstAddress.find(function_name)!=functionInstAddress.end()){
              map<std::string, set<ADDRINT> >::iterator iter=functionInstAddress.find(function_name);
              iter->second.insert(address);
          }
          else{
            // Insert a new one here
            std::set<ADDRINT> addressSet;
            addressSet.insert(address);
          functionInstAddress[function_name]=addressSet;
          }
          }
        // The address of the instruction here 
       
    }
    
    RTN_Close(rtn);
}




// This function is called when the application exits
// It prints the name and count for each procedure
VOID Fini(INT32 code, VOID *v)
{
  
     
map<std::string,std::set<ADDRINT> > :: iterator iterFunction;
for (iterFunction = functionInstAddress.begin(); iterFunction != functionInstAddress.end(); ++iterFunction)
{
   
    std::pair<std::string,std::set<ADDRINT> > pair=*iterFunction;
    outFile<<pair.first<<"\n";
    


}
outFile.close();
}
/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */

INT32 Usage()
{
    cerr << "Print all functions of a given binaries" << endl;
    cerr << "++++++++++" << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */

int main(int argc, char * argv[])
{
    // Initialize symbol table code, needed for rtn instrumentation
    PIN_InitSymbols();

    outFile.open("function.list");


  



    // Initialize pin
    if (PIN_Init(argc, argv)) return Usage();

    
  

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
