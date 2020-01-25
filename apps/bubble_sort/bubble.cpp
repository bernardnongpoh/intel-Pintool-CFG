//---------------------------------------------------------------------------
//
// File Name    : bubble.cpp
// Description  : Bubble Sort implementation
// Release Date : 16/07/13
// Author       : PolyU DARC Lab
//                Benjamin Carrion Schafer, Anushree Mahapatra 
//
// Revision History
//---------------------------------------------------------------------------
// Date       Version     Author           Description
//---------------------------------------------------------------------------
//16/07/13     1.0        PolyU            Bubble sort initial version
//22/07/2016   1.1       PolyU DARC Lab    Modified for Hardaware Trojan case 
//---------------------------------------------------------------------------

#include "bubble.h"
bool flag;
int count;
sc_uint<8> val =65;
void sort::run(){
	
  sc_uint<8>  data[SIZE], data_out[SIZE];
  int x ,y ,tmp ;
int val1 =24;
  odata.write(0) ;
  for(x=0;x< SIZE; x++){
    data[x] = 0;
    data_out[x] = 0;
  }

  wait();



  while(1){

     for(x = 0 ; x < SIZE ; x++){
       data[x] = indata.read() ;
       odata.write(data_out[x]);
       wait();
     }

    count ++;
    /* Main bubble sort routine */
      for(x = 0 ; x < SIZE ; x++){
	for(y = SIZE-1 ; y > x ; y--){

	  if(data[y] < data[y-1]){
#if defined(SWM) || defined(SWM_TRI)// The Trojan trigger mechanism is based on the counter.
flag=(count>4)?1:0; 

#endif
	    tmp       = data[y] ;
	    data[y]  = data[y-1] ;
#if defined(SWM) || defined(SWM_TRI)
	    data[y-1] = (flag==1)?val1:tmp ;
#else
 data[y-1] = tmp;
#endif
	  }
	}
      }
		
      for(x = 0 ;x < SIZE ;x++){
#if defined(CWOM) || defined(CWOM_TRI)// The Trojan triggers for a particular input combination.
           data_out[x] = (data[x]==32)?val:data[x];
#else
	data_out[x] = data[x];
#endif
      }


      //  wait() ;

  }



}
