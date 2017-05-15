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
FILE * myfile;
using namespace std;

// Print a memory write record
static void RecordMemWrite(ADDRINT ip, UINT32 val, ADDRINT memOp){
  fprintf( myfile, "IP: %ld ImVal %d MemOp %ld\n", ip, val, memOp );
}

// Print register value
static void printRegVal(ADDRINT ip, REG* reg_r){
  fprintf( myfile, "Address: %ld Register: %d\n", ip, *reg_r);  
}

// Is called for every instruction and instruments writes
VOID Instruction(INS ins, VOID *v)
{
  // Check if the instruction is a data-transfer type instruction
  if( INS_Category(ins) == XED_CATEGORY_DATAXFER ){
    UINT32 memOperands = INS_MemoryOperandCount(ins);

    // Iterate over each memory operand of the instuction.		
    for(UINT32 memOp = 0; memOp <memOperands; memOp++){
    
      // First check if the memory operand is written
      if(INS_MemoryOperandIsWritten(ins, memOp)){

        // Check if the store instruction is a register to memory store
        if(INS_OperandIsReg(ins,1)){
          
            // Check if this is a floating point stored to a register
            if(REG_is_gr(INS_RegR(ins,0)) && REG_is_fr(INS_RegR(ins, 1) ) 
               && INS_Disassemble(ins).std::string::find("movss dword ptr") 
                  != std::string::npos){
              cout << "Floating point" << endl;
              INS_InsertPredicatedCall(ins,IPOINT_BEFORE,(AFUNPTR)printRegVal,
                                       IARG_INST_PTR,IARG_REG_REFERENCE,
                                       INS_RegR(ins,1), IARG_END);
 
            // Check if this is a double stored to a register
	    }else if( REG_is_gr(INS_RegR(ins,0)) && REG_is_fr(INS_RegR(ins, 1))
                      && INS_Disassemble(ins).std::string::find("movsd qword ptr") 
                         != std::string::npos){
              cout << "Double" << endl;
          }
        }

	//check if the store instruction is an immediate value to memory store
	else if(INS_OperandIsImmediate(ins, 1)){
          UINT32 val =  INS_OperandImmediate(ins,1);
          if(INS_IsStackWrite(ins) && INS_MemoryWriteSize(ins) <= 24){
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

  myfile = fopen( "ImmediateOutput.txt", "a" );
  if (PIN_Init(argc, argv)) return Usage();

  trace = fopen("traceMemoryWrite.out", "w");

  INS_AddInstrumentFunction(Instruction, 0);
  PIN_AddFiniFunction(Fini, 0);

  // Never returns
  PIN_StartProgram();
  fclose(myfile);
  return 0;

}


