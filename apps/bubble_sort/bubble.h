//---------------------------------------------------------------------------
//
//
// File Name    : bubble.h
// Description  : Bubble Sort
// Release Date : 16/07/13
// Author       : PolyU DARC Lab
//                Benjamin Carrion Schafer, Anushree Mahapatra
//
// Revision History
//---------------------------------------------------------------------------
// Date       Version      Author       Description
//---------------------------------------------------------------------------
//16/07/13    1.0           PolyU       Initial version
//---------------------------------------------------------------------------

#ifndef BUBBLE_H
#define BUBBLE_H

#include "define.h"



SC_MODULE(sort)
{
    sc_in<bool> clk;
    sc_in<bool> rst;

   sc_in<sc_uint<8> > indata;
   sc_out<sc_uint<8> > odata;

   /* R */	
   void run() ;
	
   SC_CTOR(sort)
     {
       SC_CTHREAD(run, clk.pos()) ;
       reset_signal_is(rst, false) ;
     }

   ~sort(){}

};

#endif // BUBBLE_H

