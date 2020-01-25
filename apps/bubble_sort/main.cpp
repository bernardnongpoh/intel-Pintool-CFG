//========================================================================================
// 
//
// File Name    : tb_bubble.cpp
// Description  : Testbench for bubble sort
// Release Date : 17/07/2013
// Author       : PolyU DARC Lab
//                Benjamin Carrion Schafer, Anushree Mahapatra 
//
// Revision History
//---------------------------------------------------------------------------------------
// Date        Version        Author              Description
//----------------------------------------------------------------------------------------
//14/02/2013    1.0           PolyU                Bubble sort original testbench
//22/07/2016    1.1           PolyU DARC Lab       Modified for Hardaware Trojan case 
// g++ -o bubble.exe main.cpp tb_bubble.cpp bubble.cpp -I$SYSTEMC_HOME/include -L$SYSTEMC_HOME/lib -lsystemc -lm
//=======================================================================================


#include "bubble.h"
#include "tb_bubble.h"

int p;
char* infile=0;
char* goldenfile =0;

void print_help(){

  printf("\nHELP Menu for ADPCM");
  printf("\nbubble.exe -infile <name> -goldfile <name> ");
  printf("\nOptions : ");
  printf("\n\t-infile : <name>   name of input stimuli (MUST) ");
  printf("\n\t-golden : <name>   name of golden output (OPTIONAL)  \n");
  exit(1);
}


void read_arguments(int argc, char ** argv){

 char* temp;
 int p=0;

for(p=1;p<argc;++p)
{
temp = argv[p];

 if(!temp){
   printf("Error: Input arguments missing\n");
   print_help();
   return;
 }

if(strcmp(temp,"-infile")== 0)
{ 
   if((p+1)< argc){
      infile = argv[p+1];}
    else{
       printf("Missing %s argument needed\n",temp);
       return;
}}

 else if(strcmp(temp,"-golden")== 0)
{ 
   if((p+1)< argc){
      goldenfile = argv[p+1];}
    else{
       printf("Missing %s argument needed\n",temp);
       return;
}

 }

 else if(strcmp(temp,"-h")==0){
   print_help();
 

 }

 }

// Check inputs needed have been successfully read
 if(infile == 0){
       printf("Missing input file\n");
       print_help();

 }


}

int sc_main(int argc, char** argv)
{
  sc_clock                clk("clk", 25, SC_NS, 0.5, 12.5, SC_NS, true);
  sc_signal<bool>         rst;
 
   sc_signal<sc_uint<8> > indata;
   sc_signal<sc_uint<8> >odata;

read_arguments(argc, argv);

   sort u_BUBBLE("BUBBLE");
   test_bubble test("test_BUBBLE");

   //connect to bubble sort
   u_BUBBLE.clk( clk );
   u_BUBBLE.rst( rst );

   u_BUBBLE.indata( indata);
   u_BUBBLE.odata(  odata );
 
  // connect to test bench
  test.clk( clk );
  test.rst( rst );
  
  test.indata( indata );
  test.odata( odata );


#ifdef WAVE_DUMP
  // Trace files
  sc_trace_file* trace_file = sc_create_vcd_trace_file("trace_behav");

  // Top level signals
  sc_trace(trace_file, clk              , "clk");
  sc_trace(trace_file, rst              , "rst");
 
   sc_trace(trace_file, indata          , "indata"); 
   sc_trace(trace_file,  odata            ,"odata");
 
#endif  // End WAVE_DUMP

  sc_start( 10, SC_NS );
  rst.write(1);

  sc_start( 25, SC_NS );
  rst.write(0);

  sc_start( 25, SC_NS );
  rst.write(1);

  sc_start();

#ifdef WAVE_DUMP
   sc_close_vcd_trace_file(trace_file);
#endif

    return 0;

};
