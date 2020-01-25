//========================================================================================
// 
//
// File Name    : tb_bubble.h
// Description  : Testbench for Bubble sort
// Release Date : 16/07/2013
// Author       : PolyU DARC Lab
//                Benjamin Carrion Schafer, Anushree Mahapatra
//
// Revision History
//---------------------------------------------------------------------------------------
// Date        Version  Author          Description
//---------------------------------------------------------------------------------------
//16/07/2013    1.0   PolyU DARC Lab   Bubble sort module declaration
//=======================================================================================

#include "define.h"

SC_MODULE (test_bubble){

  // Inputs
  sc_in<bool>          clk;
  sc_in<bool>          rst;

  sc_in<sc_uint<8> > odata; 


  //  Outputs
  sc_out<sc_uint<8> >  indata;


  //For data feeding
  FILE * in_file, *out_golden_file, *out_file_read;
  FILE  *out_file, *diff_file;


  /* C */
  void compare_results();

  /* R */
  void recv();

  /* S */
  void send();



  SC_CTOR ( test_bubble ) {
 
    SC_CTHREAD(send,clk.pos());
    reset_signal_is(rst,false);

    SC_CTHREAD(recv,clk.pos());
    reset_signal_is(rst,false);
  }

  ~test_bubble(){}

};
