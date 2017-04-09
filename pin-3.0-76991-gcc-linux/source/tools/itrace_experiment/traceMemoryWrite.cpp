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

FILE * trace;
using namespace std;

// Print a memory write record
static VOID RecordMemWrite(ADDRINT ip, UINT32 val, ADDRINT memOp){
	cout << val << endl;
	/*cout << "syntax = " <<INS_Disassemble(ins) << " pc = "<< INS_Address(ins)<<" immediateValue" <<INS_OperandImmediate(ins,memOp)<<endl;*/

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

			if( INS_MemoryOperandIsWritten(ins, memOp)){

				//check if the operand is immediate
				if(INS_OperandIsImmediate(ins, 1)){

					UINT32 val =  INS_OperandImmediate(ins,1);
				
					if( INS_IsStackWrite(ins) && INS_MemoryWriteSize(ins) == 4){
					INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR) RecordMemWrite,
						IARG_INST_PTR, 
					 	IARG_UINT32, val,
						IARG_MEMORYOP_EA, memOp,
					   	IARG_END);
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


