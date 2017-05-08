/*
 * This file contains an ISA-portable PIN tool for tracing memory write.
 */

/*
 * Filename: traceMemoryWrite.c
 * Author: Zhiran Chen, Yuxuan Zhang, Vivian Lam
 * Description: This file contains an ISA-portable PIN tool for tracing memory write.
 * Source of Help: Pin Tool user guide, Mentor Atieh
 */
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include "pin.H"
#include <fstream>
#include <cassert>
#include "../Utils/regvalue_utils.h"
#include <bits/stdc++.h>
FILE * trace;
using namespace std;

// Print a memory write record
static VOID RecordMemWrite(ADDRINT ip, UINT32 val, ADDRINT memOp){
  ofstream myfile;
  myfile.open( "ImmediateOutput.txt", ios::app);
  myfile << "IP:"<<ip<<"\t\t Immediate Value: "<< val << endl;
  /*cout << "syntax = " <<INS_Disassemble(ins) << " pc = "<< INS_Address(ins)<<" immediateValue" <<INS_OperandImmediate(ins,memOp)<<endl;*/
}

// Print register value
static void printRegVal(ADDRINT ip, REG* reg_r,ADDRINT addr){
  //val = PIN_GetContextRegval(ctxt,(REG)reg,reinterpret_cast<UINT8*>(&val));
  /*
  const UINT32 max = INS_MaxNumRRegs(ins);
  for( UINT i = 0; i < max; i++){
    regVal = INS_RegR(ins,i);
    cout<<"here"<<( * regVal)<<endl;
  }
  */
 // cout<<"RegVal size: "<<PIN_GetRegvalSize(*reg_r)<<endl; this line results in a compiler error, REGVAL not defined
  
//  cout<<"RegVal size: "<<REG_Size(*reg_r)<<endl; //This line leads to a coredump
  //cout<<"Regval Size: "<<sizeof(*reg_r)<<endl;
  ofstream myfile;
  myfile.open("RegisterOutput.txt",ios::app);
  myfile << "IP:"<<ip<<"\t\t Register Value: " << *reg_r<<endl;
}

// Is called for every instruction and instruments writes
VOID Instruction(INS ins, VOID *v)
{
  // Instrucments memory accesses using a predicated call, i.e.
  // the instrumentation is called iff the instruction will actually
  // be executed.


  // Check if the instruction is a data-transfer type instruction
  if( INS_Category(ins) == XED_CATEGORY_DATAXFER ){
    UINT32 memOperands = INS_MemoryOperandCount(ins);
    // Iterate over each memory operand of the instuction.
    for(UINT32 memOp = 0; memOp <memOperands; memOp++)
    {
      // First check if the memory operand is written
      if(INS_MemoryOperandIsWritten(ins, memOp)){
        // Check if the store instruction is a register to memory store
        if(INS_OperandIsReg(ins,1)){
          const UINT32 max = INS_MaxNumRRegs(ins);
          for(UINT i = 0; i <max ; i++){
            if(REG_is_gr(INS_RegR(ins,i))){
              //cout<<"RegVal size: "<<PIN_GetRegvalSize(*INS_RegR(ins,i))<<endl;
              INS_InsertPredicatedCall(ins,IPOINT_BEFORE,(AFUNPTR)printRegVal,
                                       IARG_INST_PTR,IARG_REG_REFERENCE,INS_RegR(ins,i),
                                       IARG_MEMORYWRITE_EA,IARG_END);
	    }
          }
        }
	//check if the store instruction is an immediate value to memory store
	else if(INS_OperandIsImmediate(ins, 1)){
          UINT32 val =  INS_OperandImmediate(ins,1);
          if(INS_IsStackWrite(ins) ){
	    INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR) RecordMemWrite,
                                     IARG_INST_PTR, IARG_UINT32, val,
                                     IARG_MEMORYOP_EA, memOp,IARG_END);
	  }
	}
      }		
    }
  }	
}


VOID Fini(INT32 code, VOID *v)
{
	fprintf(trace, "#eof\n");
	fclose(trace);
}

INT32 Usage()
{
	PIN_ERROR("This Pintool prints a trace of memory address\n"
			+ KNOB_BASE::StringKnobSummary()+"\n");
	return -1;
}


/*================================================================
 * Main
 *================================================================
 */
int main(int argc, char *argv[])
{
	if (PIN_Init(argc, argv)) return Usage();

	trace = fopen("traceMemoryWrite.out", "w");

	INS_AddInstrumentFunction(Instruction, 0);
	PIN_AddFiniFunction(Fini, 0);

	// Never returns
	PIN_StartProgram();

	return 0;

}


