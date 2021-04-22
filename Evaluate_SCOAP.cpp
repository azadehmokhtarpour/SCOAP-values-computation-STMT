//#define NOMINMAX      //for using QueryPerformanceCounter
#include <algorithm>    // std::min
using namespace std;
//#include <windows.h>  //for using QueryPerformanceCounter
#include <iomanip>      // std::setw
#include <limits>
#include <math.h>
//#pragma warning (disable:4996) //for using QueryPerformanceCounter

//typedef unsigned long long LARGE_INTEGER; //for using QueryPerformanceCounter

#include "Evaluate_SCOAP.h"

//evaluate and print scoap parameters


#define sqr(x) ((x)*(x))

int Evaluate_SCOAP_CC(node *n)
{
	SCOAP s;
	//printf("type = %d", n->type);
	//for readability of code:
	SCOAP * Ins[MAX];
	SCOAP * Outs[MAX];
	for (int i = 0; i < n->nIn; i++)
	{
		Ins[i] = &n->In[i]->st.s;
	}
	int i = 0;
	for ( i = 0; i < n->nOut; i++)
	{
		Outs[i] = &n->Out[i]->st.s;
	}
	Outs[i] = NULL;
	if (n->type == DUMMY) {
		Outs[0]->cc[0] = Ins[0]->cc[0] - 1;
		Outs[0]->cc[1] = Ins[0]->cc[1] - 1;
		Outs[0]->sc[0] = Ins[0]->sc[0];
		Outs[0]->sc[1] = Ins[0]->sc[1];
	}
	else if (n->type >= ADDFXL && n->type <= FADDX2) {
		//output S
		//AND gates are based on NANDS
		s = cc_XOR3(*Ins[0], *Ins[1], *Ins[2]);
		memcpy(Outs[1]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[1]->sc, s.sc, sizeof(s.sc));
		//output CO
		//AND gates are based on NANDS
		s = cc_OR2(cc_AND2(cc_XOR2(*Ins[0], *Ins[1]), *Ins[2]), cc_AND2(*Ins[0], *Ins[1]));
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= ADDHXL && n->type <= HADDX2) {
		//output S
		s = cc_XOR2(*Ins[0], *Ins[1]);
		memcpy(Outs[1]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[1]->sc, s.sc, sizeof(s.sc));
		//output CO
		//AND gates are based on NANDS
		s = cc_AND2(*Ins[0], *Ins[1]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AND2XL && n->type <= ISOLANDX8) {
		s = cc_AND2(*Ins[0], *Ins[1]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AND3XL && n->type <= AND3X4) {
		s = cc_AND3(*Ins[0], *Ins[1], *Ins[2]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AND4XL && n->type <= AND4X4) {
		s = cc_AND3(*Ins[0], *Ins[1], *Ins[2]);
		//s.cc[0] -= 2;
		//s.cc[1] -= 2;
		s = cc_AND2(s, *Ins[3]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AO21X1 && n->type <= AO21X2) {
		s = cc_OR2(cc_AND2(*Ins[0], *Ins[1]), *Ins[2]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AOI21XL && n->type <= AOI21X4) {
		s = cc_NOR2(cc_AND2(*Ins[0], *Ins[1]), *Ins[2]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AOI211XL && n->type <= AOI211X4) {
		s = cc_NOR3(cc_AND2(*Ins[0], *Ins[1]), *Ins[2], *Ins[3]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AO22X1 && n->type <= AO22X2) {
		s = cc_OR2(cc_AND2(*Ins[0], *Ins[1]), cc_AND2(*Ins[2], *Ins[3]));
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AOI22XL && n->type <= AOI22X4) {
		s = cc_NOR2(cc_AND2(*Ins[0], *Ins[1]), cc_AND2(*Ins[2], *Ins[3]));
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AO221X1 && n->type <= AO221X2) {
		s = cc_OR3(cc_AND2(*Ins[0], *Ins[1]), cc_AND2(*Ins[2], *Ins[3]), *Ins[4]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AOI221XL && n->type <= AOI221X4) {
		s = cc_NOR3(cc_AND2(*Ins[0], *Ins[1]), cc_AND2(*Ins[2], *Ins[3]), *Ins[4]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AO222X1 && n->type <= AO222X2) {
		s = cc_OR3(cc_AND2(*Ins[0], *Ins[1]), cc_AND2(*Ins[2], *Ins[3]), cc_AND2(*Ins[4], *Ins[5]));
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AOI222XL && n->type <= AOI222X4) {
		s = cc_NOR3(cc_AND2(*Ins[0], *Ins[1]), cc_AND2(*Ins[2], *Ins[3]), cc_AND2(*Ins[4], *Ins[5]));
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AOI2BB1XL && n->type <= AOI2BB1X4) {
		s = cc_NOR2(cc_AND2(cc_NOT(*Ins[0]), cc_NOT(*Ins[1])), *Ins[2]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AOI2BB2XL && n->type <= AOI2BB2X4) {
		s = cc_NOR2(cc_AND2(cc_NOT(*Ins[0]), cc_NOT(*Ins[1])), cc_AND2(*Ins[2], *Ins[3]));
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AOI31XL && n->type <= AOI31X4) {
		s = cc_NOR2(cc_AND3(*Ins[0], *Ins[1], *Ins[2]), *Ins[3]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AOI32XL && n->type <= AOI32X4) {
		s = cc_NOR2(cc_AND3(*Ins[0], *Ins[1], *Ins[2]), cc_AND2(*Ins[3], *Ins[4]));
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AOI33XL && n->type <= AOI33X4) {
		s = cc_NOR2(cc_AND3(*Ins[0], *Ins[1], *Ins[2]), cc_AND3(*Ins[3], *Ins[4], *Ins[5]));
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OA21X1 && n->type <= OA21X2) {
		s = cc_AND2(cc_OR2(*Ins[0], *Ins[1]), *Ins[2]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OAI21XL && n->type <= OAI21X4) {
		s = cc_NAND2(cc_OR2(*Ins[0], *Ins[1]), *Ins[2]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OAI211XL && n->type <= OAI211X4) {
		s = cc_NAND3(cc_OR2(*Ins[0], *Ins[1]), *Ins[2], *Ins[3]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OA22X1 && n->type <= OA22X2) {
		s = cc_AND2(cc_OR2(*Ins[0], *Ins[1]), cc_OR2(*Ins[2], *Ins[3]));
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OAI22XL && n->type <= OAI22X4) {
		s = cc_NAND2(cc_OR2(*Ins[0], *Ins[1]), cc_OR2(*Ins[2], *Ins[3]));
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OA221X1 && n->type <= OA221X2) {
		s = cc_AND3(cc_OR2(*Ins[0], *Ins[1]), cc_OR2(*Ins[2], *Ins[3]), *Ins[4]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OAI221XL && n->type <= OAI221X4) {
		s = cc_NAND3(cc_OR2(*Ins[0], *Ins[1]), cc_OR2(*Ins[2], *Ins[3]), *Ins[4]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OA222X1 && n->type <= OA222X2) {
		s = cc_AND3(cc_OR2(*Ins[0], *Ins[1]), cc_OR2(*Ins[2], *Ins[3]), cc_OR2(*Ins[4], *Ins[5]));
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OAI222XL && n->type <= OAI222X4) {
		s = cc_NAND3(cc_OR2(*Ins[0], *Ins[1]), cc_OR2(*Ins[2], *Ins[3]), cc_OR2(*Ins[4], *Ins[5]));
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OAI2BB1XL && n->type <= OAI2BB1X4) {
		s = cc_NAND2(cc_OR2(cc_NOT(*Ins[0]), cc_NOT(*Ins[1])), *Ins[2]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OAI2BB2XL && n->type <= OAI2BB2X4) {
		s = cc_NAND2(cc_OR2(cc_NOT(*Ins[0]), cc_NOT(*Ins[1])), cc_OR2(*Ins[2], *Ins[3]));
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OAI31XL && n->type <= OAI31X4) {
		s = cc_NAND2(cc_OR3(*Ins[0], *Ins[1], *Ins[2]), *Ins[3]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OAI32XL && n->type <= OAI32X4) {
		s = cc_NAND2(cc_OR3(*Ins[0], *Ins[1], *Ins[2]), cc_OR2(*Ins[3], *Ins[4]));
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OAI33XL && n->type <= OAI33X4) {
		s = cc_NAND2(cc_OR3(*Ins[0], *Ins[1], *Ins[2]), cc_OR3(*Ins[3], *Ins[4], *Ins[5]));
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= MUX21X1 && n->type <= MX2X4) {
		s = cc_OR2(cc_AND2(*Ins[0], cc_NOT(*Ins[2])), cc_AND2(*Ins[1], cc_NOT(cc_NOT(*Ins[2]))));
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= MUX41X1 && n->type <= MX4X4) {
		s = cc_OR2(cc_AND3(*Ins[0], cc_NOT(*Ins[4]), cc_NOT(*Ins[5])), cc_AND3(*Ins[1], cc_NOT(cc_NOT(*Ins[4])), cc_NOT(*Ins[5])));
		SCOAP s2 = cc_OR2(cc_AND3(*Ins[2], cc_NOT(*Ins[4]), cc_NOT(cc_NOT(*Ins[5]))), cc_AND3(*Ins[3], cc_NOT(cc_NOT(*Ins[4])), cc_NOT(cc_NOT(*Ins[5]))));
		s = cc_OR2(s, s2);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= MXI2XL && n->type <= MXI2X4) {
		s = cc_NOR2(cc_AND2(*Ins[0], cc_NOT(*Ins[2])), cc_AND2(*Ins[1], cc_NOT(cc_NOT(*Ins[2]))));
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= MXI4XL && n->type <= MXI4X4) {
		s = cc_OR2(cc_AND3(*Ins[0], cc_NOT(*Ins[4]), cc_NOT(*Ins[5])), cc_AND3(*Ins[1], cc_NOT(cc_NOT(*Ins[4])), cc_NOT(*Ins[5])));
		SCOAP s2 = cc_OR2(cc_AND3(*Ins[2], cc_NOT(*Ins[4]), cc_NOT(cc_NOT(*Ins[5]))), cc_AND3(*Ins[3], cc_NOT(cc_NOT(*Ins[4])), cc_NOT(cc_NOT(*Ins[5]))));
		s = cc_NOR2(s, s2);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NAND2X0 && n->type <= NAND2X4) {
		s = cc_NAND2(*Ins[0], *Ins[1]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NAND2BXL && n->type <= NAND2BX4) {
		s = cc_NAND2(cc_NOT(*Ins[0]), *Ins[1]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NAND3X0 && n->type <= NAND3X4) {
		s = cc_NAND3(*Ins[0], *Ins[1], *Ins[2]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NAND3BXL && n->type <= NAND3BX4) {
		s = cc_NAND3(cc_NOT(*Ins[0]), *Ins[1], *Ins[2]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NAND4X0 && n->type <= NAND4X4) {
		s = cc_AND3(*Ins[0], *Ins[1], *Ins[2]);
		s = cc_NAND2(s, *Ins[3]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NAND4BXL && n->type <= NAND4BX4) {
		s = cc_AND3(*Ins[3], *Ins[1], *Ins[2]);
		SCOAP s2 = cc_NAND2(s, cc_NOT(*Ins[0]));
		memcpy(Outs[0]->cc, s2.cc, sizeof(s2.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NAND4BBXL && n->type <= NAND4BBX4) {
		s = cc_AND3(cc_NOT(*Ins[0]), cc_NOT(*Ins[1]), *Ins[2]);
		s = cc_NAND2(s, *Ins[3]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= CLKINVXL && n->type <= IBUFFX32) {
		s = cc_NOT(*Ins[0]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NOR2X0 && n->type <= NOR2X4) {
		s = cc_NOR2(*Ins[0], *Ins[1]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NOR2BXL && n->type <= NOR2BX4) {
		s = cc_NOR2(cc_NOT(*Ins[0]), *Ins[1]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NOR3X0 && n->type <= NOR3X4) {
		s = cc_NOR3(*Ins[0], *Ins[1], *Ins[2]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NOR3BXL && n->type <= NOR3BX4) {
		s = cc_NOR3(cc_NOT(*Ins[0]), *Ins[1], *Ins[2]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NOR4X0 && n->type <= NOR4X4) {
		s = cc_OR3(*Ins[0], *Ins[1], *Ins[2]);
		s = cc_NOR2(s, *Ins[3]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NOR4BXL && n->type <= NOR4BX4) {
		s = cc_OR3(cc_NOT(*Ins[0]), *Ins[1], *Ins[2]);
		s = cc_NOR2(s, *Ins[3]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NOR4BBXL && n->type <= NOR4BBX4) {
		s = cc_OR3(cc_NOT(*Ins[0]), cc_NOT(*Ins[1]), *Ins[2]);
		s = cc_NOR2(s, *Ins[3]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OR2XL && n->type <= ISOLORX8) {
		s = cc_OR2(*Ins[0], *Ins[1]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OR3XL && n->type <= OR3X4) {
		s = cc_OR3(*Ins[0], *Ins[1], *Ins[2]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OR4XL && n->type <= OR4X4) {
		s = cc_OR3(*Ins[0], *Ins[1], *Ins[2]);
		s = cc_OR2(s, *Ins[3]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= XOR2XL && n->type <= XOR2X4) {
		s = cc_XOR2(*Ins[0], *Ins[1]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= XNOR2XL && n->type <= XNOR2X4) {
		s = cc_NOT(cc_XOR2(*Ins[0], *Ins[1]));
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	//XOR3 & XNOR3 are not correct in cc0 & cc1
	else if (n->type >= XOR3X1 && n->type <= XOR3X2) {
		s = cc_XOR3(*Ins[0], *Ins[1], *Ins[2]);
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= XNOR3X1 && n->type <= XNOR3X2) {
		s = cc_NOT(cc_XOR3(*Ins[0], *Ins[1], *Ins[2]));
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= BUFXL && n->type <= CLKBUFX20) {
		s = cc_NOT(cc_NOT(*Ins[0]));
		memcpy(Outs[0]->cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0]->sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= SDFFXL && n->type <= SDFFX4) {
		Outs[0]->cc[0] = std::min(Ins[0]->cc[0] + Ins[2]->cc[0], Ins[3]->cc[0] + Ins[2]->cc[1]) + Ins[1]->cc[0] + Ins[1]->cc[1] ;
		Outs[0]->cc[1] = std::min(Ins[0]->cc[1] + Ins[2]->cc[0], Ins[3]->cc[1] + Ins[2]->cc[1]) + Ins[1]->cc[0] + Ins[1]->cc[1] ;
		Outs[0]->sc[0] = std::min(Ins[0]->sc[0] + Ins[2]->sc[0], Ins[3]->sc[0] + Ins[2]->sc[1]) + Ins[1]->sc[0] + Ins[1]->sc[1] ;
		Outs[0]->sc[1] = std::min(Ins[0]->sc[1] + Ins[2]->sc[0], Ins[3]->sc[1] + Ins[2]->sc[1]) + Ins[1]->sc[0] + Ins[1]->sc[1] ;
		if (Outs[1] != NULL) {
			Outs[1]->cc[0] = std::min(Ins[0]->cc[1] + Ins[2]->cc[0], Ins[3]->cc[1] + Ins[2]->cc[1]) + Ins[1]->cc[0] + Ins[1]->cc[1] ;
			Outs[1]->cc[1] = std::min(Ins[0]->cc[0] + Ins[2]->cc[0], Ins[3]->cc[0] + Ins[2]->cc[1]) + Ins[1]->cc[0] + Ins[1]->cc[1] ;
			Outs[1]->sc[0] = std::min(Ins[0]->sc[1] + Ins[2]->sc[0], Ins[3]->sc[1] + Ins[2]->sc[1]) + Ins[1]->sc[0] + Ins[1]->sc[1] ;
			Outs[1]->sc[1] = std::min(Ins[0]->sc[0] + Ins[2]->sc[0], Ins[3]->sc[0] + Ins[2]->sc[1]) + Ins[1]->sc[0] + Ins[1]->sc[1] ;
		}
	}
	//sequential elements //TODO change to real formulations
	else if (n->type >= SDFFSRXL && n->type <= SDFFASRX2) {
		printf("invoke SDFFSRXL");
		Outs[0]->cc[0] = std::min({ Ins[4]->cc[0] + Ins[5]->cc[1],(Ins[4]->cc[1] + Ins[1]->cc[5] + Ins[1]->cc[0] + Ins[1]->cc[1]) + std::min(Ins[0]->cc[0] + Ins[2]->cc[0] , Ins[2]->cc[1] + Ins[3]->cc[0]) });
		Outs[0]->cc[1] = std::min({ Ins[5]->cc[0],(Ins[4]->cc[1] + Ins[5]->cc[1] + Ins[1]->cc[0] + Ins[1]->cc[1]) + std::min(Ins[2]->cc[0] + Ins[0]->cc[1] , Ins[2]->cc[1] + Ins[3]->cc[1]) });
		Outs[0]->sc[0] = std::min({ Ins[4]->sc[0] + Ins[5]->sc[1],(Ins[4]->sc[1] + Ins[1]->sc[5] + Ins[1]->sc[0] + Ins[1]->sc[1]) + std::min(Ins[0]->sc[0] + Ins[2]->sc[0] , Ins[2]->sc[1] + Ins[3]->sc[0]) });
		Outs[0]->sc[1] = std::min({ Ins[5]->sc[0],(Ins[4]->sc[1] + Ins[5]->sc[1] + Ins[1]->sc[0] + Ins[1]->sc[1]) + std::min(Ins[2]->sc[0] + Ins[0]->sc[1] , Ins[2]->sc[1] + Ins[3]->sc[1]) });
		if (Outs[1] != NULL) {
			Outs[1]->cc[0] = Outs[0]->cc[1];
			Outs[1]->cc[1] = Outs[0]->cc[0];
			Outs[1]->sc[0] = Outs[0]->sc[1];
			Outs[1]->sc[1] = Outs[0]->sc[0];
		}

	}
	else if (n->type >= SDFFARX1 && n->type <= SDFFARX2) {
		Outs[0]->cc[0] = std::min({ Ins[4]->cc[0] ,(Ins[4]->cc[1] + Ins[1]->cc[0] + Ins[1]->cc[1]) + std::min(Ins[0]->cc[0] + Ins[2]->cc[0] , Ins[2]->cc[1] + Ins[3]->cc[0]) });
		Outs[0]->cc[1] = (Ins[4]->cc[1] + Ins[1]->cc[0] + Ins[1]->cc[1]) + std::min(Ins[0]->cc[1] + Ins[2]->cc[0], Ins[2]->cc[1] + Ins[3]->cc[1]);
		Outs[0]->sc[0] = std::min({ Ins[4]->sc[0] ,(Ins[4]->sc[1] + Ins[1]->sc[0] + Ins[1]->sc[1]) + std::min(Ins[0]->sc[0] + Ins[2]->sc[0] , Ins[2]->sc[1] + Ins[3]->sc[0]) });
		Outs[0]->sc[1] = (Ins[4]->sc[1] + Ins[1]->sc[0] + Ins[1]->sc[1]) + std::min(Ins[0]->sc[1] + Ins[2]->sc[0], Ins[2]->sc[1] + Ins[3]->sc[1]);
		if (Outs[1] != NULL) {
			Outs[1]->cc[0] = Outs[0]->cc[1];
			Outs[1]->cc[1] = Outs[0]->cc[0];
			Outs[1]->sc[0] = Outs[0]->sc[1];
			Outs[1]->sc[1] = Outs[0]->sc[0];
		}

	}
	else if (n->type >= DFFARX1 && n->type <= DFFARX2) {
		printf("invoke DFFARX1");
		Outs[0]->cc[0] = std::min(Ins[1]->cc[0], Ins[0]->cc[0] + Ins[1]->cc[1] + Ins[2]->cc[0] + Ins[2]->cc[1]) ;
		Outs[0]->cc[1] = Ins[0]->cc[1] + Ins[1]->cc[1] + Ins[2]->cc[0] + Ins[2]->cc[1] ;
		Outs[0]->sc[0] = std::min(Ins[1]->sc[0], Ins[0]->sc[0] + Ins[1]->sc[1] + Ins[2]->sc[0] + Ins[2]->sc[1]) ;
		Outs[0]->sc[1] = Ins[0]->sc[1] + Ins[1]->sc[1] + Ins[2]->sc[0] + Ins[2]->sc[1] ;
		if (Outs[1] != NULL) {
			Outs[1]->cc[0] = Ins[0]->cc[1] + Ins[1]->cc[1] + Ins[2]->cc[0] + Ins[2]->cc[1] ;
			Outs[1]->cc[1] = std::min(Ins[1]->cc[0], Ins[0]->cc[0] + Ins[1]->cc[1] + Ins[2]->cc[0] + Ins[2]->cc[1]) ;
			Outs[1]->sc[0] = Ins[0]->sc[1] + Ins[1]->sc[1] + Ins[2]->sc[0] + Ins[2]->sc[1] ;
			Outs[1]->sc[1] = std::min(Ins[1]->sc[0], Ins[0]->sc[0] + Ins[1]->sc[1] + Ins[2]->sc[0] + Ins[2]->sc[1]) ;
		}
	}
	else if (n->type >= DFFXL && n->type <= DFFX4) {
		Outs[0]->cc[0] = Ins[0]->cc[0] + Ins[1]->cc[1] + Ins[1]->cc[0];
		Outs[0]->cc[1] = Ins[0]->cc[1] + Ins[1]->cc[1] + Ins[1]->cc[0];
		Outs[0]->sc[0] = Ins[0]->sc[0] + Ins[1]->sc[1] + Ins[1]->sc[0];
		Outs[0]->sc[1] = Ins[0]->sc[1] + Ins[1]->sc[1] + Ins[1]->sc[0];
		if (Outs[1] != NULL) {
			Outs[1]->cc[0] = Ins[0]->cc[1] + Ins[1]->cc[1] + Ins[1]->cc[0];
			Outs[1]->cc[1] = Ins[0]->cc[0] + Ins[1]->cc[1] + Ins[1]->cc[0];
			Outs[1]->sc[0] = Ins[0]->sc[1] + Ins[1]->sc[1] + Ins[1]->sc[0];
			Outs[1]->sc[1] = Ins[0]->sc[0] + Ins[1]->sc[1] + Ins[1]->sc[0];
		}
	}
	else if (n->type >= DFFNXL && n->type <= DFFNX4) {
		Outs[0]->cc[0] = Ins[0]->cc[1] + Ins[1]->cc[1] + Ins[1]->cc[0];
		Outs[0]->cc[1] = Ins[0]->cc[0] + Ins[1]->cc[1] + Ins[1]->cc[0];
		Outs[0]->sc[0] = Ins[0]->sc[1] + Ins[1]->sc[1] + Ins[1]->sc[0];
		Outs[0]->sc[1] = Ins[0]->sc[0] + Ins[1]->sc[1] + Ins[1]->sc[0];
		if (Outs[1] != NULL) {
			Outs[1]->cc[0] = Ins[0]->cc[0] + Ins[1]->cc[1] + Ins[1]->cc[0];
			Outs[1]->cc[1] = Ins[0]->cc[1] + Ins[1]->cc[1] + Ins[1]->cc[0];
			Outs[1]->sc[0] = Ins[0]->sc[0] + Ins[1]->sc[1] + Ins[1]->sc[0];
			Outs[1]->sc[1] = Ins[0]->sc[1] + Ins[1]->sc[1] + Ins[1]->sc[0];
		}
	}
	else if (n->type >= DFFXL) {
		int min_cc0 = INF;
		int min_cc1 = INF;
		for (int i = 0; i < n->nOut; i++)
		{
			Outs[i]->cc[0] = Ins[0]->cc[0];
			Outs[i]->cc[1] = Ins[0]->cc[1];
			Outs[i]->sc[0] = Ins[0]->sc[0];
			Outs[i]->sc[1] = Ins[0]->sc[1];
		}
	}
	for (int i = 0; i < n->nOut; i++)
	{
		//if gate is sequential
		if (n->type >= DFFXL) {
			Outs[i]->sc[0] ++;
			Outs[i]->sc[1] ++;
		}
		//if gate is combinational
		if (n->type < DFFXL) {
			Outs[i]->cc[0] ++;
			Outs[i]->cc[1] ++;
		}
		if (Outs[i]->cc[0] > INF)
			Outs[i]->cc[0] = INF;
		if (Outs[i]->cc[1] > INF)
			Outs[i]->cc[1] = INF;
		if (Outs[i]->sc[0] > INF)
			Outs[i]->sc[0] = INF;
		if (Outs[i]->sc[1] > INF)
			Outs[i]->sc[1] = INF;
	}
	return 0;
}

int Evaluate_SCOAP_CC_v2(node *n)
{
	SCOAP s;
	//printf("type = %d\n", n->type);
	//for readability of code:
	SCOAP * Ins[MAX];
	SCOAP  Outs[MAX];
	for (int i = 0; i < n->nIn; i++)
	{
		Ins[i] = &n->In[i]->st.s;
	}
	//using st_updated field is the difference between two versions of function Evaluate_CC for nodes
	for (int i = 0; i < n->nOut; i++)
		n->Out[i]->st_updated = false;
	if (n->type == DUMMY) {
		Outs[0].cc[0] = Ins[0]->cc[0] - 1;
		Outs[0].cc[1] = Ins[0]->cc[1] - 1;
		Outs[0].sc[0] = Ins[0]->sc[0];
		Outs[0].sc[1] = Ins[0]->sc[1];
	}
	else if (n->type >= ADDFXL && n->type <= FADDX2) {
		//output S
		//AND gates are based on NANDS
		s = cc_XOR3(*Ins[0], *Ins[1], *Ins[2]);
		memcpy(Outs[1].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[1].sc, s.sc, sizeof(s.sc));
		//output CO
		//AND gates are based on NANDS
		s = cc_OR2(cc_AND2(cc_XOR2(*Ins[0], *Ins[1]), *Ins[2]), cc_AND2(*Ins[0], *Ins[1]));
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= ADDHXL && n->type <= HADDX2) {
		//output S
		s = cc_XOR2(*Ins[0], *Ins[1]);
		memcpy(Outs[1].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[1].sc, s.sc, sizeof(s.sc));
		//output CO
		//AND gates are based on NANDS
		s = cc_AND2(*Ins[0], *Ins[1]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AND2XL && n->type <= ISOLANDX8) {
		s = cc_AND2(*Ins[0], *Ins[1]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AND3XL && n->type <= AND3X4) {
		s = cc_AND3(*Ins[0], *Ins[1], *Ins[2]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AND4XL && n->type <= AND4X4) {
		s = cc_AND3(*Ins[0], *Ins[1], *Ins[2]);
		s = cc_AND2(s, *Ins[3]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AO21X1 && n->type <= AO21X2) {
		s = cc_OR2(cc_AND2(*Ins[0], *Ins[1]), *Ins[2]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AOI21XL && n->type <= AOI21X4) {
		s = cc_NOR2(cc_AND2(*Ins[0], *Ins[1]), *Ins[2]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AOI211XL && n->type <= AOI211X4) {
		s = cc_NOR3(cc_AND2(*Ins[0], *Ins[1]), *Ins[2], *Ins[3]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AO22X1 && n->type <= AO22X2) {
		s = cc_OR2(cc_AND2(*Ins[0], *Ins[1]), cc_AND2(*Ins[2], *Ins[3]));
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AOI22XL && n->type <= AOI22X4) {
		s = cc_NOR2(cc_AND2(*Ins[0], *Ins[1]), cc_AND2(*Ins[2], *Ins[3]));
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AO221X1 && n->type <= AO221X2) {
		s = cc_OR3(cc_AND2(*Ins[0], *Ins[1]), cc_AND2(*Ins[2], *Ins[3]), *Ins[4]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AOI221XL && n->type <= AOI221X4) {
		s = cc_NOR3(cc_AND2(*Ins[0], *Ins[1]), cc_AND2(*Ins[2], *Ins[3]), *Ins[4]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AO222X1 && n->type <= AO222X2) {
		s = cc_OR3(cc_AND2(*Ins[0], *Ins[1]), cc_AND2(*Ins[2], *Ins[3]), cc_AND2(*Ins[4], *Ins[5]));
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AOI222XL && n->type <= AOI222X4) {
		s = cc_NOR3(cc_AND2(*Ins[0], *Ins[1]), cc_AND2(*Ins[2], *Ins[3]), cc_AND2(*Ins[4], *Ins[5]));
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AOI2BB1XL && n->type <= AOI2BB1X4) {
		s = cc_NOR2(cc_AND2(cc_NOT(*Ins[0]), cc_NOT(*Ins[1])), *Ins[2]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AOI2BB2XL && n->type <= AOI2BB2X4) {
		s = cc_NOR2(cc_AND2(cc_NOT(*Ins[0]), cc_NOT(*Ins[1])), cc_AND2(*Ins[2], *Ins[3]));
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AOI31XL && n->type <= AOI31X4) {
		s = cc_NOR2(cc_AND3(*Ins[0], *Ins[1], *Ins[2]), *Ins[3]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AOI32XL && n->type <= AOI32X4) {
		s = cc_NOR2(cc_AND3(*Ins[0], *Ins[1], *Ins[2]), cc_AND2(*Ins[3], *Ins[4]));
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= AOI33XL && n->type <= AOI33X4) {
		s = cc_NOR2(cc_AND3(*Ins[0], *Ins[1], *Ins[2]), cc_AND3(*Ins[3], *Ins[4], *Ins[5]));
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OA21X1 && n->type <= OA21X2) {
		s = cc_AND2(cc_OR2(*Ins[0], *Ins[1]), *Ins[2]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OAI21XL && n->type <= OAI21X4) {
		s = cc_NAND2(cc_OR2(*Ins[0], *Ins[1]), *Ins[2]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OAI211XL && n->type <= OAI211X4) {
		s = cc_NAND3(cc_OR2(*Ins[0], *Ins[1]), *Ins[2], *Ins[3]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OA22X1 && n->type <= OA22X2) {
		s = cc_AND2(cc_OR2(*Ins[0], *Ins[1]), cc_OR2(*Ins[2], *Ins[3]));
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OAI22XL && n->type <= OAI22X4) {
		s = cc_NAND2(cc_OR2(*Ins[0], *Ins[1]), cc_OR2(*Ins[2], *Ins[3]));
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OA221X1 && n->type <= OA221X2) {
		s = cc_AND3(cc_OR2(*Ins[0], *Ins[1]), cc_OR2(*Ins[2], *Ins[3]), *Ins[4]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OAI221XL && n->type <= OAI221X4) {
		s = cc_NAND3(cc_OR2(*Ins[0], *Ins[1]), cc_OR2(*Ins[2], *Ins[3]), *Ins[4]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OA222X1 && n->type <= OA222X2) {
		s = cc_AND3(cc_OR2(*Ins[0], *Ins[1]), cc_OR2(*Ins[2], *Ins[3]), cc_OR2(*Ins[4], *Ins[5]));
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OAI222XL && n->type <= OAI222X4) {
		s = cc_NAND3(cc_OR2(*Ins[0], *Ins[1]), cc_OR2(*Ins[2], *Ins[3]), cc_OR2(*Ins[4], *Ins[5]));
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OAI2BB1XL && n->type <= OAI2BB1X4) {
		s = cc_NAND2(cc_OR2(cc_NOT(*Ins[0]), cc_NOT(*Ins[1])), *Ins[2]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OAI2BB2XL && n->type <= OAI2BB2X4) {
		s = cc_NAND2(cc_OR2(cc_NOT(*Ins[0]), cc_NOT(*Ins[1])), cc_OR2(*Ins[2], *Ins[3]));
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OAI31XL && n->type <= OAI31X4) {
		s = cc_NAND2(cc_OR3(*Ins[0], *Ins[1], *Ins[2]), *Ins[3]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OAI32XL && n->type <= OAI32X4) {
		s = cc_NAND2(cc_OR3(*Ins[0], *Ins[1], *Ins[2]), cc_OR2(*Ins[3], *Ins[4]));
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OAI33XL && n->type <= OAI33X4) {
		s = cc_NAND2(cc_OR3(*Ins[0], *Ins[1], *Ins[2]), cc_OR3(*Ins[3], *Ins[4], *Ins[5]));
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= MUX21X1 && n->type <= MX2X4) {
		s = cc_OR2(cc_AND2(*Ins[0], cc_NOT(*Ins[2])), cc_AND2(*Ins[1], cc_NOT(cc_NOT(*Ins[2]))));
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= MUX41X1 && n->type <= MX4X4) {
		s = cc_OR2(cc_AND3(*Ins[0], cc_NOT(*Ins[4]), cc_NOT(*Ins[5])), cc_AND3(*Ins[1], cc_NOT(cc_NOT(*Ins[4])), cc_NOT(*Ins[5])));
		SCOAP s2 = cc_OR2(cc_AND3(*Ins[2], cc_NOT(*Ins[4]), cc_NOT(cc_NOT(*Ins[5]))), cc_AND3(*Ins[3], cc_NOT(cc_NOT(*Ins[4])), cc_NOT(cc_NOT(*Ins[5]))));
		s = cc_OR2(s, s2);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= MXI2XL && n->type <= MXI2X4) {
		s = cc_NOR2(cc_AND2(*Ins[0], cc_NOT(*Ins[2])), cc_AND2(*Ins[1], cc_NOT(cc_NOT(*Ins[2]))));
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= MXI4XL && n->type <= MXI4X4) {
		s = cc_OR2(cc_AND3(*Ins[0], cc_NOT(*Ins[4]), cc_NOT(*Ins[5])), cc_AND3(*Ins[1], cc_NOT(cc_NOT(*Ins[4])), cc_NOT(*Ins[5])));
		SCOAP s2 = cc_OR2(cc_AND3(*Ins[2], cc_NOT(*Ins[4]), cc_NOT(cc_NOT(*Ins[5]))), cc_AND3(*Ins[3], cc_NOT(cc_NOT(*Ins[4])), cc_NOT(cc_NOT(*Ins[5]))));
		s = cc_NOR2(s, s2);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NAND2X0 && n->type <= NAND2X4) {
		s = cc_NAND2(*Ins[0], *Ins[1]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NAND2BXL && n->type <= NAND2BX4) {
		s = cc_NAND2(cc_NOT(*Ins[0]), *Ins[1]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NAND3X0 && n->type <= NAND3X4) {
		s = cc_NAND3(*Ins[0], *Ins[1], *Ins[2]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NAND3BXL && n->type <= NAND3BX4) {
		s = cc_NAND3(cc_NOT(*Ins[0]), *Ins[1], *Ins[2]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NAND4X0 && n->type <= NAND4X4) {
		s = cc_AND3(*Ins[0], *Ins[1], *Ins[2]);
		s = cc_NAND2(s, *Ins[3]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NAND4BXL && n->type <= NAND4BX4) {
		s = cc_AND3(*Ins[3], *Ins[1], *Ins[2]);
		SCOAP s2 = cc_NAND2(s, cc_NOT(*Ins[0]));
		memcpy(Outs[0].cc, s2.cc, sizeof(s2.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NAND4BBXL && n->type <= NAND4BBX4) {
		s = cc_AND3(cc_NOT(*Ins[0]), cc_NOT(*Ins[1]), *Ins[2]);
		s = cc_NAND2(s, *Ins[3]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= CLKINVXL && n->type <= IBUFFX32) {
		s = cc_NOT(*Ins[0]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NOR2X0 && n->type <= NOR2X4) {
		s = cc_NOR2(*Ins[0], *Ins[1]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NOR2BXL && n->type <= NOR2BX4) {
		s = cc_NOR2(cc_NOT(*Ins[0]), *Ins[1]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NOR3X0 && n->type <= NOR3X4) {
		s = cc_NOR3(*Ins[0], *Ins[1], *Ins[2]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NOR3BXL && n->type <= NOR3BX4) {
		s = cc_NOR3(cc_NOT(*Ins[0]), *Ins[1], *Ins[2]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NOR4X0 && n->type <= NOR4X4) {
		s = cc_OR3(*Ins[0], *Ins[1], *Ins[2]);
		s = cc_NOR2(s, *Ins[3]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NOR4BXL && n->type <= NOR4BX4) {
		s = cc_OR3(cc_NOT(*Ins[0]), *Ins[1], *Ins[2]);
		s = cc_NOR2(s, *Ins[3]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= NOR4BBXL && n->type <= NOR4BBX4) {
		s = cc_OR3(cc_NOT(*Ins[0]), cc_NOT(*Ins[1]), *Ins[2]);
		s = cc_NOR2(s, *Ins[3]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OR2XL && n->type <= ISOLORX8) {
		s = cc_OR2(*Ins[0], *Ins[1]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OR3XL && n->type <= OR3X4) {
		s = cc_OR3(*Ins[0], *Ins[1], *Ins[2]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= OR4XL && n->type <= OR4X4) {
		s = cc_OR3(*Ins[0], *Ins[1], *Ins[2]);
		s = cc_OR2(s, *Ins[3]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= XOR2XL && n->type <= XOR2X4) {
		s = cc_XOR2(*Ins[0], *Ins[1]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= XNOR2XL && n->type <= XNOR2X4) {
		s = cc_NOT(cc_XOR2(*Ins[0], *Ins[1]));
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	//XOR3 & XNOR3 are not correct in cc0 & cc1
	else if (n->type >= XOR3X1 && n->type <= XOR3X2) {
		s = cc_XOR3(*Ins[0], *Ins[1], *Ins[2]);
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= XNOR3X1 && n->type <= XNOR3X2) {
		s = cc_NOT(cc_XOR3(*Ins[0], *Ins[1], *Ins[2]));
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	else if (n->type >= BUFXL && n->type <= CLKBUFX20) {
		s = cc_NOT(cc_NOT(*Ins[0]));
		memcpy(Outs[0].cc, s.cc, sizeof(s.cc));
		memcpy(Outs[0].sc, s.sc, sizeof(s.sc));
	}
	//sequential elements //TODO change to real formulations
	else if (n->type >= SDFFSRXL && n->type <= SDFFASRX2) {
		printf("invoke SDFFSRXL");
		Outs[0].cc[0] = std::min({ Ins[4]->cc[0] + Ins[5]->cc[1],(Ins[4]->cc[1] + Ins[1]->cc[5] + Ins[1]->cc[0] + Ins[1]->cc[1]) + std::min(Ins[0]->cc[0] + Ins[2]->cc[0] , Ins[2]->cc[1] + Ins[3]->cc[0]) });
		Outs[0].cc[1] = std::min({ Ins[5]->cc[0],(Ins[4]->cc[1] + Ins[5]->cc[1] + Ins[1]->cc[0] + Ins[1]->cc[1]) + std::min(Ins[2]->cc[0] + Ins[0]->cc[1] , Ins[2]->cc[1] + Ins[3]->cc[1]) });
		Outs[0].sc[0] = std::min({ Ins[4]->sc[0] + Ins[5]->sc[1],(Ins[4]->sc[1] + Ins[1]->sc[5] + Ins[1]->sc[0] + Ins[1]->sc[1]) + std::min(Ins[0]->sc[0] + Ins[2]->sc[0] , Ins[2]->sc[1] + Ins[3]->sc[0]) });
		Outs[0].sc[1] = std::min({ Ins[5]->sc[0],(Ins[4]->sc[1] + Ins[5]->sc[1] + Ins[1]->sc[0] + Ins[1]->sc[1]) + std::min(Ins[2]->sc[0] + Ins[0]->sc[1] , Ins[2]->sc[1] + Ins[3]->sc[1]) });
		if (n->Out.size() > 1 && n->Out[1] != NULL) {
			Outs[1].cc[0] = Outs[0].cc[1];
			Outs[1].cc[1] = Outs[0].cc[0];
			Outs[1].sc[0] = Outs[0].sc[1];
			Outs[1].sc[1] = Outs[0].sc[0];
		}

	}
	else if (n->type >= DFFRHQXL && n->type <= DFFRHQX4)
	{
		SCOAP *clk = Ins[1];
		SCOAP *d   = Ins[0];
		SCOAP *rst = Ins[2];
		Outs[0].cc[0] = std::min(d->cc[0] + rst->cc[0], rst->cc[1]) + clk->cc[0] + clk->cc[1];
		Outs[0].cc[1] = d->cc[1] + rst->cc[0] + clk->cc[0] + clk->cc[1];

		Outs[0].sc[0] = std::min(d->sc[0] + rst->sc[0], rst->sc[1]) + clk->sc[0] + clk->sc[1];
		Outs[0].sc[1] = d->sc[1] + rst->sc[0] + clk->sc[0] + clk->sc[1];

		if (n->Out.size() > 1 && n->Out[1] != NULL) {
			Outs[0].cc[0] = Outs[0].cc[1];
			Outs[0].cc[1] = Outs[0].cc[0];

			Outs[0].sc[0] = Outs[0].sc[1];
			Outs[0].sc[1] = Outs[0].sc[0];
		}
	}
	else if (n->type >= SDFFXL && n->type <= SDFFX4) {
		Outs[0].cc[0] = std::min(Ins[0]->cc[0] + Ins[2]->cc[0], Ins[3]->cc[0] + Ins[2]->cc[1]) + Ins[1]->cc[0] + Ins[1]->cc[1];
		Outs[0].cc[1] = std::min(Ins[0]->cc[1] + Ins[2]->cc[0], Ins[3]->cc[1] + Ins[2]->cc[1]) + Ins[1]->cc[0] + Ins[1]->cc[1];
		Outs[0].sc[0] = std::min(Ins[0]->sc[0] + Ins[2]->sc[0], Ins[3]->sc[0] + Ins[2]->sc[1]) + Ins[1]->sc[0] + Ins[1]->sc[1];
		Outs[0].sc[1] = std::min(Ins[0]->sc[1] + Ins[2]->sc[0], Ins[3]->sc[1] + Ins[2]->sc[1]) + Ins[1]->sc[0] + Ins[1]->sc[1];
		if (n->Out.size() > 1 && n->Out[1] != NULL) {
			Outs[1].cc[0] = std::min(Ins[0]->cc[1] + Ins[2]->cc[0], Ins[3]->cc[1] + Ins[2]->cc[1]) + Ins[1]->cc[0] + Ins[1]->cc[1];
			Outs[1].cc[1] = std::min(Ins[0]->cc[0] + Ins[2]->cc[0], Ins[3]->cc[0] + Ins[2]->cc[1]) + Ins[1]->cc[0] + Ins[1]->cc[1];
			Outs[1].sc[0] = std::min(Ins[0]->sc[1] + Ins[2]->sc[0], Ins[3]->sc[1] + Ins[2]->sc[1]) + Ins[1]->sc[0] + Ins[1]->sc[1];
			Outs[1].sc[1] = std::min(Ins[0]->sc[0] + Ins[2]->sc[0], Ins[3]->sc[0] + Ins[2]->sc[1]) + Ins[1]->sc[0] + Ins[1]->sc[1];
		}
	}
	else if (n->type >= DFFARX1 && n->type <= DFFARX2) {
		Outs[0].cc[0] = std::min(Ins[1]->cc[0], Ins[0]->cc[0] + Ins[1]->cc[1] + Ins[2]->cc[0] + Ins[2]->cc[1]);
		Outs[0].cc[1] = Ins[0]->cc[1] + Ins[1]->cc[1] + Ins[2]->cc[0] + Ins[2]->cc[1];
		Outs[0].sc[0] = std::min(Ins[1]->sc[0], Ins[0]->sc[0] + Ins[1]->sc[1] + Ins[2]->sc[0] + Ins[2]->sc[1]);
		Outs[0].sc[1] = Ins[0]->sc[1] + Ins[1]->sc[1] + Ins[2]->sc[0] + Ins[2]->sc[1];
		if (n->Out.size() > 1 && n->Out[1] != NULL) {
			Outs[1].cc[0] = Ins[0]->cc[1] + Ins[1]->cc[1] + Ins[2]->cc[0] + Ins[2]->cc[1];
			Outs[1].cc[1] = std::min(Ins[1]->cc[0], Ins[0]->cc[0] + Ins[1]->cc[1] + Ins[2]->cc[0] + Ins[2]->cc[1]);
			Outs[1].sc[0] = Ins[0]->sc[1] + Ins[1]->sc[1] + Ins[2]->sc[0] + Ins[2]->sc[1];
			Outs[1].sc[1] = std::min(Ins[1]->sc[0], Ins[0]->sc[0] + Ins[1]->sc[1] + Ins[2]->sc[0] + Ins[2]->sc[1]);
		}
	}
	else if (n->type >= DFFXL && n->type <= DFFX4) {
		Outs[0].cc[0] = Ins[0]->cc[0] + Ins[1]->cc[1] + Ins[1]->cc[0];
		Outs[0].cc[1] = Ins[0]->cc[1] + Ins[1]->cc[1] + Ins[1]->cc[0];
		Outs[0].sc[0] = Ins[0]->sc[0] + Ins[1]->sc[1] + Ins[1]->sc[0];
		Outs[0].sc[1] = Ins[0]->sc[1] + Ins[1]->sc[1] + Ins[1]->sc[0];
		if (n->Out.size() > 1 && n->Out[1] != NULL) {
			Outs[1].cc[0] = Ins[0]->cc[1] + Ins[1]->cc[1] + Ins[1]->cc[0];
			Outs[1].cc[1] = Ins[0]->cc[0] + Ins[1]->cc[1] + Ins[1]->cc[0];
			Outs[1].sc[0] = Ins[0]->sc[1] + Ins[1]->sc[1] + Ins[1]->sc[0];
			Outs[1].sc[1] = Ins[0]->sc[0] + Ins[1]->sc[1] + Ins[1]->sc[0];
		}
	}
	else if (n->type >= LSDNENX1 && n->type <= LSDNENX8) {
		Outs[0].cc[0] = Ins[0]->cc[0] + Ins[1]->cc[1] ;
		Outs[0].cc[1] = std::min(Ins[1]->cc[0], Ins[1]->cc[1]+Ins[0]->cc[1]);
		Outs[0].sc[0] = Ins[0]->sc[0] + Ins[1]->sc[1];
		Outs[0].sc[1] = std::min(Ins[1]->sc[0], Ins[1]->sc[1] + Ins[0]->sc[1]);
	}
	else if (n->type >= DFFNXL && n->type <= DFFNX4) {
		Outs[0].cc[0] = Ins[0]->cc[1] + Ins[1]->cc[1] + Ins[1]->cc[0];
		Outs[0].cc[1] = Ins[0]->cc[0] + Ins[1]->cc[1] + Ins[1]->cc[0];
		Outs[0].sc[0] = Ins[0]->sc[1] + Ins[1]->sc[1] + Ins[1]->sc[0];
		Outs[0].sc[1] = Ins[0]->sc[0] + Ins[1]->sc[1] + Ins[1]->sc[0];
		if (n->Out.size() > 1 && n->Out[1] != NULL) {
			Outs[1].cc[0] = Ins[0]->cc[0] + Ins[1]->cc[1] + Ins[1]->cc[0];
			Outs[1].cc[1] = Ins[0]->cc[1] + Ins[1]->cc[1] + Ins[1]->cc[0];
			Outs[1].sc[0] = Ins[0]->sc[0] + Ins[1]->sc[1] + Ins[1]->sc[0];
			Outs[1].sc[1] = Ins[0]->sc[1] + Ins[1]->sc[1] + Ins[1]->sc[0];
		}
	}
	else if (n->type >= RDFFNX1 && n->type <= RDFFNX2) {
		Outs[0].cc[0] = Ins[0]->cc[0] + Ins[1]->cc[1] + Ins[1]->cc[0] + Ins[2]->cc[1];
		Outs[0].cc[1] = Ins[0]->cc[1] + Ins[1]->cc[1] + Ins[1]->cc[0] + Ins[2]->cc[1];
		Outs[0].sc[0] = Ins[0]->sc[0] + Ins[1]->sc[1] + Ins[1]->sc[0] + Ins[2]->sc[1];
		Outs[0].sc[1] = Ins[0]->sc[1] + Ins[1]->sc[1] + Ins[1]->sc[0] + Ins[2]->sc[1];
		if (n->Out.size() > 1 && n->Out[1] != NULL) {
			Outs[1].cc[0] = Ins[0]->cc[0] + Ins[1]->cc[1] + Ins[1]->cc[0];
			Outs[1].cc[1] = Ins[0]->cc[1] + Ins[1]->cc[1] + Ins[1]->cc[0];
			Outs[1].sc[0] = Ins[0]->sc[0] + Ins[1]->sc[1] + Ins[1]->sc[0];
			Outs[1].sc[1] = Ins[0]->sc[1] + Ins[1]->sc[1] + Ins[1]->sc[0];
		}
	}
	else if (n->type >= SDFFARX1 && n->type <= SDFFARX2) {
		Outs[0].cc[0] = std::min({ Ins[4]->cc[0] ,(Ins[4]->cc[1] + Ins[1]->cc[0] + Ins[1]->cc[1]) + std::min(Ins[0]->cc[0] + Ins[2]->cc[0] , Ins[2]->cc[1] + Ins[3]->cc[0]) });
		Outs[0].cc[1] = (Ins[4]->cc[1] + Ins[1]->cc[0] + Ins[1]->cc[1]) + std::min(Ins[0]->cc[1] + Ins[2]->cc[0], Ins[2]->cc[1] + Ins[3]->cc[1]);
		Outs[0].sc[0] = std::min({ Ins[4]->sc[0] ,(Ins[4]->sc[1] + Ins[1]->sc[0] + Ins[1]->sc[1]) + std::min(Ins[0]->sc[0] + Ins[2]->sc[0] , Ins[2]->sc[1] + Ins[3]->sc[0]) });
		Outs[0].sc[1] = (Ins[4]->sc[1] + Ins[1]->sc[0] + Ins[1]->sc[1]) + std::min(Ins[0]->sc[1] + Ins[2]->sc[0], Ins[2]->sc[1] + Ins[3]->sc[1]);
		if (n->Out.size() > 1 && n->Out[1] != NULL) {
			Outs[1].cc[0] = Outs[0].cc[1];
			Outs[1].cc[1] = Outs[0].cc[0];
			Outs[1].sc[0] = Outs[0].sc[1];
			Outs[1].sc[1] = Outs[0].sc[0];
		}
	}
	else if (n->type >= DFFXL) {
		int min_cc0 = INF;
		int min_cc1 = INF;
		for (int i = 0; i < n->nOut; i++)
		{
			//		s = cc_NOT(cc_NOT(*Ins[0]));
			Outs[i].cc[0] = Ins[0]->cc[0];
			Outs[i].cc[1] = Ins[0]->cc[1];
			Outs[i].sc[0] = Ins[0]->sc[0];
			Outs[i].sc[1] = Ins[0]->sc[1];
		}
	}
	for (int i = 0; i < n->nOut; i++)
	{
		//if gate is sequential
		if (n->type >= DFFXL) {
			Outs[i].sc[0] ++;
			Outs[i].sc[1] ++;
		}
		//if gate is combinational
		if (n->type < DFFXL) {
			Outs[i].cc[0] ++;
			Outs[i].cc[1] ++;
		}
		if (Outs[i].cc[0] > INF)
			Outs[i].cc[0] = INF;
		if (Outs[i].cc[1] > INF)
			Outs[i].cc[1] = INF;
		if (Outs[i].sc[0] > INF)
			Outs[i].sc[0] = INF;
		if (Outs[i].sc[1] > INF)
			Outs[i].sc[1] = INF;
	}
	for (int i = 0; i < n->nOut; i++)
	{
		if (n->Out[i] != NULL)
			if (n->Out[i]->st.s.cc[0] != Outs[i].cc[0] || n->Out[i]->st.s.cc[1] != Outs[i].cc[1] || n->Out[i]->st.s.sc[0] != Outs[i].sc[0] || n->Out[i]->st.s.sc[1] != Outs[i].sc[1]) {
				n->Out[i]->st.s.cc[0] = Outs[i].cc[0];
				n->Out[i]->st.s.cc[1] = Outs[i].cc[1];
				n->Out[i]->st.s.sc[0] = Outs[i].sc[0];
				n->Out[i]->st.s.sc[1] = Outs[i].sc[1];
				n->Out[i]->st_updated = true;
			}
	}
	return 0;
}

void Evaluate_SCOAP_CO(node *n)
{
	EType nodeType = n->type;
	StdCellInfo* nodeEI = FindStdCellInfo(nodeType, n->stdType);
	bool is_changed = false;
	//for readability of code:
	SCOAP s;
	SCOAP  Ins[MAX];
	SCOAP * Outs[MAX];
	for (int i = 0; i < n->nIn; i++)
	{
		Ins[i] = n->In[i]->st.s;
	}
	for (int i = 0; i < n->nOut; i++)
	{
		Outs[i] = &n->Out[i]->st.s;
	}
	for (int i = 0; i < n->nIn; i++)
		n->In[i]->st_updated = false;

	if (n->type == DUMMY) {
		Ins[0].co = Outs[0]->co - 1;
		Ins[0].so = Outs[0]->so;
	}
	else if (n->type >= ADDFXL && n->type <= FADDX2) {
		//input A, B
		Ins[0].co = std::min({
			co_XNOR3_v2(*Outs[1], Ins[1], Ins[2]).co,
			co_XNOR2(co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[0], Ins[1])) , Ins[2]) , Ins[1]).co,
			co_AND2(co_OR2(*Outs[0], cc_AND2(cc_XOR2(Ins[0], Ins[1]), Ins[2])) , Ins[1]).co
		});
		Ins[0].so = std::min({
			co_XNOR3_v2(*Outs[1], Ins[1], Ins[2]).so,
			co_XNOR2(co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[0], Ins[1])) , Ins[2]) , Ins[1]).so,
			co_AND2(co_OR2(*Outs[0], cc_AND2(cc_XOR2(Ins[0], Ins[1]), Ins[2])) , Ins[1]).so
		});
		Ins[1].co = std::min({
			co_XNOR3_v2(*Outs[1], Ins[0], Ins[2]).co,
			co_XNOR2(co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[0], Ins[1])) , Ins[2]) , Ins[0]).co,
			co_AND2(co_OR2(*Outs[0], cc_AND2(cc_XOR2(Ins[0], Ins[1]), Ins[2])) , Ins[0]).co
		});
		Ins[1].so = std::min({
			co_XNOR3_v2(*Outs[1], Ins[0], Ins[2]).so,
			co_XNOR2(co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[0], Ins[1])) , Ins[2]) , Ins[0]).so,
			co_AND2(co_OR2(*Outs[0], cc_AND2(cc_XOR2(Ins[0], Ins[1]), Ins[2])) , Ins[0]).so
		});
		//input C
		Ins[2].co = std::min({
			co_XNOR3_v2(*Outs[1], Ins[0], Ins[1]).co,
			co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[0], Ins[1])) , cc_XOR2(Ins[0], Ins[1])).co
		});
		Ins[2].so = std::min({
			co_XNOR3_v2(*Outs[1], Ins[0], Ins[1]).so,
			co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[0], Ins[1])) , cc_XOR2(Ins[0], Ins[1])).so
		});
	}
	else if (n->type >= ADDHXL && n->type <= HADDX2) {
		Ins[0].co = std::min({ co_XNOR2(*Outs[1], Ins[1]).co, co_AND2(*Outs[0], Ins[1]).co });
		Ins[1].co = std::min({ co_XNOR2(*Outs[1], Ins[0]).co, co_AND2(*Outs[0], Ins[0]).co });
		Ins[0].so = std::min({ co_XNOR2(*Outs[1], Ins[1]).so, co_AND2(*Outs[0], Ins[1]).so });
		Ins[1].so = std::min({ co_XNOR2(*Outs[1], Ins[0]).so, co_AND2(*Outs[0], Ins[0]).so });
	}
	else if (n->type >= AND2XL && n->type <= ISOLANDX8) {
		Ins[0].co = co_AND2(*Outs[0], Ins[1]).co;
		Ins[1].co = co_AND2(*Outs[0], Ins[0]).co;
		Ins[0].so = co_AND2(*Outs[0], Ins[1]).so;
		Ins[1].so = co_AND2(*Outs[0], Ins[0]).so;
	}
	else if (n->type >= AND3XL && n->type <= AND3X4) {
		Ins[0].co = co_AND3(*Outs[0], Ins[1], Ins[2]).co;
		Ins[1].co = co_AND3(*Outs[0], Ins[0], Ins[2]).co;
		Ins[2].co = co_AND3(*Outs[0], Ins[1], Ins[0]).co;
		Ins[0].so = co_AND3(*Outs[0], Ins[1], Ins[2]).so;
		Ins[1].so = co_AND3(*Outs[0], Ins[0], Ins[2]).so;
		Ins[2].so = co_AND3(*Outs[0], Ins[1], Ins[0]).so;
	}
	else if (n->type >= AND4XL && n->type <= AND4X4) {
		memcpy(s.cc, cc_AND3(Ins[0], Ins[1], Ins[2]).cc, sizeof(s.cc));
		s.co = co_AND2(*Outs[0], Ins[3]).co - 0;
		Ins[0].co = co_AND3(s, Ins[1], Ins[2]).co;
		Ins[1].co = co_AND3(s, Ins[0], Ins[2]).co;
		Ins[2].co = co_AND3(s, Ins[1], Ins[0]).co;
		Ins[3].co = co_AND2(*Outs[0], s).co - 0;
		memcpy(s.sc, cc_AND3(Ins[0], Ins[1], Ins[2]).sc, sizeof(s.sc));
		s.so = co_AND2(*Outs[0], Ins[3]).so - 0;
		Ins[0].so = co_AND3(s, Ins[1], Ins[2]).so;
		Ins[1].so = co_AND3(s, Ins[0], Ins[2]).so;
		Ins[2].so = co_AND3(s, Ins[1], Ins[0]).so;
		Ins[3].so = co_AND2(*Outs[0], s).so - 0;
	}
	else if (n->type >= AO21X1 && n->type <= AO21X2) {
		Ins[0].co = co_AND2(co_OR2(*Outs[0], Ins[2]), Ins[1]).co;
		Ins[1].co = co_AND2(co_OR2(*Outs[0], Ins[2]), Ins[0]).co;
		Ins[2].co = co_OR2(*Outs[0], cc_AND2(Ins[0], Ins[1])).co;
		Ins[0].so = co_AND2(co_OR2(*Outs[0], Ins[2]), Ins[1]).so;
		Ins[1].so = co_AND2(co_OR2(*Outs[0], Ins[2]), Ins[0]).so;
		Ins[2].so = co_OR2(*Outs[0], cc_AND2(Ins[0], Ins[1])).so;
	}
	else if (n->type >= AOI21XL && n->type <= AOI21X4) {
		Ins[0].co = co_AND2(co_NOR2(*Outs[0], Ins[2]), Ins[1]).co;
		Ins[1].co = co_AND2(co_NOR2(*Outs[0], Ins[2]), Ins[0]).co;
		Ins[2].co = co_NOR2(*Outs[0], cc_AND2(Ins[0], Ins[1])).co;
		Ins[0].so = co_AND2(co_NOR2(*Outs[0], Ins[2]), Ins[1]).so;
		Ins[1].so = co_AND2(co_NOR2(*Outs[0], Ins[2]), Ins[0]).so;
		Ins[2].so = co_NOR2(*Outs[0], cc_AND2(Ins[0], Ins[1])).so;
	}
	else if (n->type >= AOI211XL && n->type <= AOI211X4) {
		Ins[0].co = co_AND2(co_NOR3(*Outs[0], Ins[2], Ins[3]), Ins[1]).co;
		Ins[1].co = co_AND2(co_NOR3(*Outs[0], Ins[2], Ins[3]), Ins[0]).co;
		Ins[2].co = co_NOR2(*Outs[0], cc_AND2(Ins[0], Ins[1])).co;
		Ins[3].co = co_NOR2(*Outs[0], cc_AND2(Ins[0], Ins[1])).co;
		Ins[0].so = co_AND2(co_NOR3(*Outs[0], Ins[2], Ins[3]), Ins[1]).so;
		Ins[1].so = co_AND2(co_NOR3(*Outs[0], Ins[2], Ins[3]), Ins[0]).so;
		Ins[2].so = co_NOR2(*Outs[0], cc_AND2(Ins[0], Ins[1])).so;
		Ins[3].so = co_NOR2(*Outs[0], cc_AND2(Ins[0], Ins[1])).so;
	}
	else if (n->type >= AO22X1 && n->type <= AO22X2) {
		Ins[0].co = co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[2], Ins[3])), Ins[1]).co;
		Ins[1].co = co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[2], Ins[3])), Ins[0]).co;
		Ins[2].co = co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[0], Ins[1])), Ins[3]).co;
		Ins[3].co = co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[0], Ins[1])), Ins[2]).co;
		Ins[0].so = co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[2], Ins[3])), Ins[1]).so;
		Ins[1].so = co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[2], Ins[3])), Ins[0]).so;
		Ins[2].so = co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[0], Ins[1])), Ins[3]).so;
		Ins[3].so = co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[0], Ins[1])), Ins[2]).so;
	}
	else if (n->type >= AOI22XL && n->type <= AOI22X4) {
		Ins[0].co = co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[2], Ins[3])), Ins[1]).co;
		Ins[1].co = co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[2], Ins[3])), Ins[0]).co;
		Ins[2].co = co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[0], Ins[1])), Ins[3]).co;
		Ins[3].co = co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[0], Ins[1])), Ins[2]).co;
		Ins[0].so = co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[2], Ins[3])), Ins[1]).so;
		Ins[1].so = co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[2], Ins[3])), Ins[0]).so;
		Ins[2].so = co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[0], Ins[1])), Ins[3]).so;
		Ins[3].so = co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[0], Ins[1])), Ins[2]).so;
	}
	else if (n->type >= AO221X1 && n->type <= AO221X2) {
		Ins[0].co = co_AND2(co_OR3(*Outs[0], Ins[4], cc_AND2(Ins[2], Ins[3])), Ins[1]).co;
		Ins[1].co = co_AND2(co_OR3(*Outs[0], Ins[4], cc_AND2(Ins[2], Ins[3])), Ins[0]).co;
		Ins[2].co = co_AND2(co_OR3(*Outs[0], Ins[4], cc_AND2(Ins[0], Ins[1])), Ins[3]).co;
		Ins[3].co = co_AND2(co_OR3(*Outs[0], Ins[4], cc_AND2(Ins[0], Ins[1])), Ins[2]).co;
		Ins[4].co = co_OR3(*Outs[0], cc_AND2(Ins[0], Ins[1]), cc_AND2(Ins[2], Ins[3])).co;
		Ins[0].so = co_AND2(co_OR3(*Outs[0], Ins[4], cc_AND2(Ins[2], Ins[3])), Ins[1]).so;
		Ins[1].so = co_AND2(co_OR3(*Outs[0], Ins[4], cc_AND2(Ins[2], Ins[3])), Ins[0]).so;
		Ins[2].so = co_AND2(co_OR3(*Outs[0], Ins[4], cc_AND2(Ins[0], Ins[1])), Ins[3]).so;
		Ins[3].so = co_AND2(co_OR3(*Outs[0], Ins[4], cc_AND2(Ins[0], Ins[1])), Ins[2]).so;
		Ins[4].so = co_OR3(*Outs[0], cc_AND2(Ins[0], Ins[1]), cc_AND2(Ins[2], Ins[3])).so;
	}
	else if (n->type >= AOI221XL && n->type <= AOI221X4) {
		Ins[0].co = co_AND2(co_NOR3(*Outs[0], Ins[4], cc_AND2(Ins[2], Ins[3])), Ins[1]).co;
		Ins[1].co = co_AND2(co_NOR3(*Outs[0], Ins[4], cc_AND2(Ins[2], Ins[3])), Ins[0]).co;
		Ins[2].co = co_AND2(co_NOR3(*Outs[0], Ins[4], cc_AND2(Ins[0], Ins[1])), Ins[3]).co;
		Ins[3].co = co_AND2(co_NOR3(*Outs[0], Ins[4], cc_AND2(Ins[0], Ins[1])), Ins[2]).co;
		Ins[4].co = co_NOR3(*Outs[0], cc_AND2(Ins[0], Ins[1]), cc_AND2(Ins[2], Ins[3])).co;
		Ins[0].so = co_AND2(co_NOR3(*Outs[0], Ins[4], cc_AND2(Ins[2], Ins[3])), Ins[1]).so;
		Ins[1].so = co_AND2(co_NOR3(*Outs[0], Ins[4], cc_AND2(Ins[2], Ins[3])), Ins[0]).so;
		Ins[2].so = co_AND2(co_NOR3(*Outs[0], Ins[4], cc_AND2(Ins[0], Ins[1])), Ins[3]).so;
		Ins[3].so = co_AND2(co_NOR3(*Outs[0], Ins[4], cc_AND2(Ins[0], Ins[1])), Ins[2]).so;
		Ins[4].so = co_NOR3(*Outs[0], cc_AND2(Ins[0], Ins[1]), cc_AND2(Ins[2], Ins[3])).so;
	}
	else if (n->type >= AO222X1 && n->type <= AO222X2) {
		Ins[0].co = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[4], Ins[5]), cc_AND2(Ins[2], Ins[3])), Ins[1]).co;
		Ins[1].co = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[4], Ins[5]), cc_AND2(Ins[2], Ins[3])), Ins[0]).co;
		Ins[2].co = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[4], Ins[5]), cc_AND2(Ins[0], Ins[1])), Ins[3]).co;
		Ins[3].co = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[4], Ins[5]), cc_AND2(Ins[0], Ins[1])), Ins[2]).co;
		Ins[4].co = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[2], Ins[3]), cc_AND2(Ins[0], Ins[1])), Ins[5]).co;
		Ins[5].co = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[2], Ins[3]), cc_AND2(Ins[0], Ins[1])), Ins[4]).co;
		Ins[0].so = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[4], Ins[5]), cc_AND2(Ins[2], Ins[3])), Ins[1]).so;
		Ins[1].so = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[4], Ins[5]), cc_AND2(Ins[2], Ins[3])), Ins[0]).so;
		Ins[2].so = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[4], Ins[5]), cc_AND2(Ins[0], Ins[1])), Ins[3]).so;
		Ins[3].so = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[4], Ins[5]), cc_AND2(Ins[0], Ins[1])), Ins[2]).so;
		Ins[4].so = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[2], Ins[3]), cc_AND2(Ins[0], Ins[1])), Ins[5]).so;
		Ins[5].so = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[2], Ins[3]), cc_AND2(Ins[0], Ins[1])), Ins[4]).so;
	}
	else if (n->type >= AOI222XL && n->type <= AOI222X4) {
		Ins[0].co = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[4], Ins[5]), cc_AND2(Ins[2], Ins[3])), Ins[1]).co;
		Ins[1].co = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[4], Ins[5]), cc_AND2(Ins[2], Ins[3])), Ins[0]).co;
		Ins[2].co = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[4], Ins[5]), cc_AND2(Ins[0], Ins[1])), Ins[3]).co;
		Ins[3].co = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[4], Ins[5]), cc_AND2(Ins[0], Ins[1])), Ins[2]).co;
		Ins[4].co = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[2], Ins[3]), cc_AND2(Ins[0], Ins[1])), Ins[5]).co;
		Ins[5].co = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[2], Ins[3]), cc_AND2(Ins[0], Ins[1])), Ins[4]).co;
		Ins[0].so = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[4], Ins[5]), cc_AND2(Ins[2], Ins[3])), Ins[1]).so;
		Ins[1].so = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[4], Ins[5]), cc_AND2(Ins[2], Ins[3])), Ins[0]).so;
		Ins[2].so = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[4], Ins[5]), cc_AND2(Ins[0], Ins[1])), Ins[3]).so;
		Ins[3].so = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[4], Ins[5]), cc_AND2(Ins[0], Ins[1])), Ins[2]).so;
		Ins[4].so = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[2], Ins[3]), cc_AND2(Ins[0], Ins[1])), Ins[5]).so;
		Ins[5].so = co_AND2(co_OR3(*Outs[0], cc_AND2(Ins[2], Ins[3]), cc_AND2(Ins[0], Ins[1])), Ins[4]).so;
	}
	else if (n->type >= AOI2BB1XL && n->type <= AOI2BB1X4) {
		Ins[0].co = co_NOT(co_AND2(co_NOR2(*Outs[0], Ins[2]), cc_NOT(Ins[1]))).co;
		Ins[1].co = co_NOT(co_AND2(co_NOR2(*Outs[0], Ins[2]), cc_NOT(Ins[0]))).co;
		Ins[2].co = co_NOR2(*Outs[0], cc_AND2(cc_NOT(Ins[0]), cc_NOT(Ins[1]))).co;
		Ins[0].so = co_NOT(co_AND2(co_NOR2(*Outs[0], Ins[2]), cc_NOT(Ins[1]))).so;
		Ins[1].so = co_NOT(co_AND2(co_NOR2(*Outs[0], Ins[2]), cc_NOT(Ins[0]))).so;
		Ins[2].so = co_NOR2(*Outs[0], cc_AND2(cc_NOT(Ins[0]), cc_NOT(Ins[1]))).so;
	}
	else if (n->type >= AOI2BB2XL && n->type <= AOI2BB2X4) {
		Ins[0].co = co_NOT(co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[2], Ins[3])), cc_NOT(Ins[1]))).co;
		Ins[1].co = co_NOT(co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[2], Ins[3])), cc_NOT(Ins[0]))).co;
		Ins[2].co = co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[0], Ins[1])), Ins[3]).co;
		Ins[3].co = co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[0], Ins[1])), Ins[2]).co;
		Ins[0].so = co_NOT(co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[2], Ins[3])), cc_NOT(Ins[1]))).so;
		Ins[1].so = co_NOT(co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[2], Ins[3])), cc_NOT(Ins[0]))).so;
		Ins[2].so = co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[0], Ins[1])), Ins[3]).so;
		Ins[3].so = co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[0], Ins[1])), Ins[2]).so;
	}
	else if (n->type >= AOI31XL && n->type <= AOI31X4) {
		Ins[0].co = co_AND3(co_NOR2(*Outs[0], Ins[3]), Ins[1], Ins[2]).co;
		Ins[1].co = co_AND3(co_NOR2(*Outs[0], Ins[3]), Ins[0], Ins[2]).co;
		Ins[2].co = co_AND3(co_NOR2(*Outs[0], Ins[3]), Ins[0], Ins[1]).co;
		Ins[3].co = co_NOR2(*Outs[0], cc_AND3(Ins[0], Ins[1], Ins[2])).co;
		Ins[0].so = co_AND3(co_NOR2(*Outs[0], Ins[3]), Ins[1], Ins[2]).so;
		Ins[1].so = co_AND3(co_NOR2(*Outs[0], Ins[3]), Ins[0], Ins[2]).so;
		Ins[2].so = co_AND3(co_NOR2(*Outs[0], Ins[3]), Ins[0], Ins[1]).so;
		Ins[3].so = co_NOR2(*Outs[0], cc_AND3(Ins[0], Ins[1], Ins[2])).so;
	}
	else if (n->type >= AOI32XL && n->type <= AOI32X4) {
		Ins[0].co = co_AND3(co_NOR2(*Outs[0], cc_AND2(Ins[3], Ins[4])), Ins[1], Ins[2]).co;
		Ins[1].co = co_AND3(co_NOR2(*Outs[0], cc_AND2(Ins[3], Ins[4])), Ins[0], Ins[2]).co;
		Ins[2].co = co_AND3(co_NOR2(*Outs[0], cc_AND2(Ins[3], Ins[4])), Ins[0], Ins[1]).co;
		Ins[3].co = co_AND2(co_NOR2(*Outs[0], cc_AND3(Ins[0], Ins[1], Ins[2])), Ins[4]).co;
		Ins[4].co = co_AND2(co_NOR2(*Outs[0], cc_AND3(Ins[0], Ins[1], Ins[2])), Ins[3]).co;
		Ins[0].so = co_AND3(co_NOR2(*Outs[0], cc_AND2(Ins[3], Ins[4])), Ins[1], Ins[2]).so;
		Ins[1].so = co_AND3(co_NOR2(*Outs[0], cc_AND2(Ins[3], Ins[4])), Ins[0], Ins[2]).so;
		Ins[2].so = co_AND3(co_NOR2(*Outs[0], cc_AND2(Ins[3], Ins[4])), Ins[0], Ins[1]).so;
		Ins[3].so = co_AND2(co_NOR2(*Outs[0], cc_AND3(Ins[0], Ins[1], Ins[2])), Ins[4]).so;
		Ins[4].so = co_AND2(co_NOR2(*Outs[0], cc_AND3(Ins[0], Ins[1], Ins[2])), Ins[3]).so;
	}
	else if (n->type >= AOI33XL && n->type <= AOI33X4) {
		Ins[0].co = co_AND3(co_NOR2(*Outs[0], cc_AND3(Ins[3], Ins[4], Ins[5])), Ins[1], Ins[2]).co;
		Ins[1].co = co_AND3(co_NOR2(*Outs[0], cc_AND3(Ins[3], Ins[4], Ins[5])), Ins[0], Ins[2]).co;
		Ins[2].co = co_AND3(co_NOR2(*Outs[0], cc_AND3(Ins[3], Ins[4], Ins[5])), Ins[0], Ins[1]).co;
		Ins[3].co = co_AND3(co_NOR2(*Outs[0], cc_AND3(Ins[0], Ins[1], Ins[2])), Ins[4], Ins[5]).co;
		Ins[4].co = co_AND3(co_NOR2(*Outs[0], cc_AND3(Ins[0], Ins[1], Ins[2])), Ins[3], Ins[5]).co;
		Ins[5].co = co_AND3(co_NOR2(*Outs[0], cc_AND3(Ins[0], Ins[1], Ins[2])), Ins[3], Ins[4]).co;
		Ins[0].so = co_AND3(co_NOR2(*Outs[0], cc_AND3(Ins[3], Ins[4], Ins[5])), Ins[1], Ins[2]).so;
		Ins[1].so = co_AND3(co_NOR2(*Outs[0], cc_AND3(Ins[3], Ins[4], Ins[5])), Ins[0], Ins[2]).so;
		Ins[2].so = co_AND3(co_NOR2(*Outs[0], cc_AND3(Ins[3], Ins[4], Ins[5])), Ins[0], Ins[1]).so;
		Ins[3].so = co_AND3(co_NOR2(*Outs[0], cc_AND3(Ins[0], Ins[1], Ins[2])), Ins[4], Ins[5]).so;
		Ins[4].so = co_AND3(co_NOR2(*Outs[0], cc_AND3(Ins[0], Ins[1], Ins[2])), Ins[3], Ins[5]).so;
		Ins[5].so = co_AND3(co_NOR2(*Outs[0], cc_AND3(Ins[0], Ins[1], Ins[2])), Ins[3], Ins[4]).so;
	}
	else if (n->type >= OA21X1 && n->type <= OA21X2) {
		Ins[0].co = co_OR2(co_AND2(*Outs[0], Ins[2]), Ins[1]).co;
		Ins[1].co = co_OR2(co_AND2(*Outs[0], Ins[2]), Ins[0]).co;
		Ins[2].co = co_AND2(*Outs[0], cc_OR2(Ins[0], Ins[1])).co;
		Ins[0].so = co_OR2(co_AND2(*Outs[0], Ins[2]), Ins[1]).so;
		Ins[1].so = co_OR2(co_AND2(*Outs[0], Ins[2]), Ins[0]).so;
		Ins[2].so = co_AND2(*Outs[0], cc_OR2(Ins[0], Ins[1])).so;
	}
	else if (n->type >= OAI21XL && n->type <= OAI21X4) {
		Ins[0].co = co_OR2(co_NAND2(*Outs[0], Ins[2]), Ins[1]).co;
		Ins[1].co = co_OR2(co_NAND2(*Outs[0], Ins[2]), Ins[0]).co;
		Ins[2].co = co_NAND2(*Outs[0], cc_OR2(Ins[0], Ins[1])).co;
		Ins[0].so = co_OR2(co_NAND2(*Outs[0], Ins[2]), Ins[1]).so;
		Ins[1].so = co_OR2(co_NAND2(*Outs[0], Ins[2]), Ins[0]).so;
		Ins[2].so = co_NAND2(*Outs[0], cc_OR2(Ins[0], Ins[1])).so;
	}
	else if (n->type >= OAI211XL && n->type <= OAI211X4) {
		Ins[0].co = co_OR2(co_NAND3(*Outs[0], Ins[2], Ins[3]), Ins[1]).co;
		Ins[1].co = co_OR2(co_NAND3(*Outs[0], Ins[2], Ins[3]), Ins[0]).co;
		Ins[2].co = co_NAND2(*Outs[0], cc_OR2(Ins[0], Ins[1])).co;
		Ins[3].co = co_NAND2(*Outs[0], cc_OR2(Ins[0], Ins[1])).co;
		Ins[0].so = co_OR2(co_NAND3(*Outs[0], Ins[2], Ins[3]), Ins[1]).so;
		Ins[1].so = co_OR2(co_NAND3(*Outs[0], Ins[2], Ins[3]), Ins[0]).so;
		Ins[2].so = co_NAND2(*Outs[0], cc_OR2(Ins[0], Ins[1])).so;
		Ins[3].so = co_NAND2(*Outs[0], cc_OR2(Ins[0], Ins[1])).so;
	}
	else if (n->type >= OA22X1 && n->type <= OA22X2) {
		Ins[0].co = co_OR2(co_AND2(*Outs[0], cc_OR2(Ins[2], Ins[3])), Ins[1]).co;
		Ins[1].co = co_OR2(co_AND2(*Outs[0], cc_OR2(Ins[2], Ins[3])), Ins[0]).co;
		Ins[2].co = co_OR2(co_AND2(*Outs[0], cc_OR2(Ins[0], Ins[1])), Ins[3]).co;
		Ins[3].co = co_OR2(co_AND2(*Outs[0], cc_OR2(Ins[0], Ins[1])), Ins[2]).co;
		Ins[0].so = co_OR2(co_AND2(*Outs[0], cc_OR2(Ins[2], Ins[3])), Ins[1]).so;
		Ins[1].so = co_OR2(co_AND2(*Outs[0], cc_OR2(Ins[2], Ins[3])), Ins[0]).so;
		Ins[2].so = co_OR2(co_AND2(*Outs[0], cc_OR2(Ins[0], Ins[1])), Ins[3]).so;
		Ins[3].so = co_OR2(co_AND2(*Outs[0], cc_OR2(Ins[0], Ins[1])), Ins[2]).so;
	}
	else if (n->type >= OAI22XL && n->type <= OAI22X4) {
		Ins[0].co = co_OR2(co_NAND2(*Outs[0], cc_OR2(Ins[2], Ins[3])), Ins[1]).co;
		Ins[1].co = co_OR2(co_NAND2(*Outs[0], cc_OR2(Ins[2], Ins[3])), Ins[0]).co;
		Ins[2].co = co_OR2(co_NAND2(*Outs[0], cc_OR2(Ins[0], Ins[1])), Ins[3]).co;
		Ins[3].co = co_OR2(co_NAND2(*Outs[0], cc_OR2(Ins[0], Ins[1])), Ins[2]).co;
		Ins[0].so = co_OR2(co_NAND2(*Outs[0], cc_OR2(Ins[2], Ins[3])), Ins[1]).so;
		Ins[1].so = co_OR2(co_NAND2(*Outs[0], cc_OR2(Ins[2], Ins[3])), Ins[0]).so;
		Ins[2].so = co_OR2(co_NAND2(*Outs[0], cc_OR2(Ins[0], Ins[1])), Ins[3]).so;
		Ins[3].so = co_OR2(co_NAND2(*Outs[0], cc_OR2(Ins[0], Ins[1])), Ins[2]).so;
	}
	else if (n->type >= OA221X1 && n->type <= OA221X2) {
		Ins[0].co = co_OR2(co_AND3(*Outs[0], Ins[4], cc_OR2(Ins[2], Ins[3])), Ins[1]).co;
		Ins[1].co = co_OR2(co_AND3(*Outs[0], Ins[4], cc_OR2(Ins[2], Ins[3])), Ins[0]).co;
		Ins[2].co = co_OR2(co_AND3(*Outs[0], Ins[4], cc_OR2(Ins[0], Ins[1])), Ins[3]).co;
		Ins[3].co = co_OR2(co_AND3(*Outs[0], Ins[4], cc_OR2(Ins[0], Ins[1])), Ins[2]).co;
		Ins[4].co = co_AND3(*Outs[0], cc_OR2(Ins[0], Ins[1]), cc_OR2(Ins[2], Ins[3])).co;
		Ins[0].so = co_OR2(co_AND3(*Outs[0], Ins[4], cc_OR2(Ins[2], Ins[3])), Ins[1]).so;
		Ins[1].so = co_OR2(co_AND3(*Outs[0], Ins[4], cc_OR2(Ins[2], Ins[3])), Ins[0]).so;
		Ins[2].so = co_OR2(co_AND3(*Outs[0], Ins[4], cc_OR2(Ins[0], Ins[1])), Ins[3]).so;
		Ins[3].so = co_OR2(co_AND3(*Outs[0], Ins[4], cc_OR2(Ins[0], Ins[1])), Ins[2]).so;
		Ins[4].so = co_AND3(*Outs[0], cc_OR2(Ins[0], Ins[1]), cc_OR2(Ins[2], Ins[3])).so;
	}
	else if (n->type >= OAI221XL && n->type <= OAI221X4) {
		Ins[0].co = co_OR2(co_NAND3(*Outs[0], Ins[4], cc_OR2(Ins[2], Ins[3])), Ins[1]).co;
		Ins[1].co = co_OR2(co_NAND3(*Outs[0], Ins[4], cc_OR2(Ins[2], Ins[3])), Ins[0]).co;
		Ins[2].co = co_OR2(co_NAND3(*Outs[0], Ins[4], cc_OR2(Ins[0], Ins[1])), Ins[3]).co;
		Ins[3].co = co_OR2(co_NAND3(*Outs[0], Ins[4], cc_OR2(Ins[0], Ins[1])), Ins[2]).co;
		Ins[4].co = co_NAND3(*Outs[0], cc_OR2(Ins[0], Ins[1]), cc_OR2(Ins[2], Ins[3])).co;
		Ins[0].so = co_OR2(co_NAND3(*Outs[0], Ins[4], cc_OR2(Ins[2], Ins[3])), Ins[1]).so;
		Ins[1].so = co_OR2(co_NAND3(*Outs[0], Ins[4], cc_OR2(Ins[2], Ins[3])), Ins[0]).so;
		Ins[2].so = co_OR2(co_NAND3(*Outs[0], Ins[4], cc_OR2(Ins[0], Ins[1])), Ins[3]).so;
		Ins[3].so = co_OR2(co_NAND3(*Outs[0], Ins[4], cc_OR2(Ins[0], Ins[1])), Ins[2]).so;
		Ins[4].so = co_NAND3(*Outs[0], cc_OR2(Ins[0], Ins[1]), cc_OR2(Ins[2], Ins[3])).so;
	}
	else if (n->type >= OA222X1 && n->type <= OA222X2) {
		Ins[0].co = co_OR2(co_AND3(*Outs[0], cc_OR2(Ins[4], Ins[5]), cc_OR2(Ins[2], Ins[3])), Ins[1]).co;
		Ins[1].co = co_OR2(co_AND3(*Outs[0], cc_OR2(Ins[4], Ins[5]), cc_OR2(Ins[2], Ins[3])), Ins[0]).co;
		Ins[2].co = co_OR2(co_AND3(*Outs[0], cc_OR2(Ins[4], Ins[5]), cc_OR2(Ins[0], Ins[1])), Ins[3]).co;
		Ins[3].co = co_OR2(co_AND3(*Outs[0], cc_OR2(Ins[4], Ins[5]), cc_OR2(Ins[0], Ins[1])), Ins[2]).co;
		Ins[4].co = co_OR2(co_AND3(*Outs[0], cc_OR2(Ins[2], Ins[3]), cc_OR2(Ins[0], Ins[1])), Ins[5]).co;
		Ins[5].co = co_OR2(co_AND3(*Outs[0], cc_OR2(Ins[2], Ins[3]), cc_OR2(Ins[0], Ins[1])), Ins[4]).co;
		Ins[0].so = co_OR2(co_AND3(*Outs[0], cc_OR2(Ins[4], Ins[5]), cc_OR2(Ins[2], Ins[3])), Ins[1]).so;
		Ins[1].so = co_OR2(co_AND3(*Outs[0], cc_OR2(Ins[4], Ins[5]), cc_OR2(Ins[2], Ins[3])), Ins[0]).so;
		Ins[2].so = co_OR2(co_AND3(*Outs[0], cc_OR2(Ins[4], Ins[5]), cc_OR2(Ins[0], Ins[1])), Ins[3]).so;
		Ins[3].so = co_OR2(co_AND3(*Outs[0], cc_OR2(Ins[4], Ins[5]), cc_OR2(Ins[0], Ins[1])), Ins[2]).so;
		Ins[4].so = co_OR2(co_AND3(*Outs[0], cc_OR2(Ins[2], Ins[3]), cc_OR2(Ins[0], Ins[1])), Ins[5]).so;
		Ins[5].so = co_OR2(co_AND3(*Outs[0], cc_OR2(Ins[2], Ins[3]), cc_OR2(Ins[0], Ins[1])), Ins[4]).so;
	}
	else if (n->type >= OAI222XL && n->type <= OAI222X4) {
		Ins[0].co = co_OR2(co_NAND3(*Outs[0], cc_OR2(Ins[4], Ins[5]), cc_OR2(Ins[2], Ins[3])), Ins[1]).co;
		Ins[1].co = co_OR2(co_NAND3(*Outs[0], cc_OR2(Ins[4], Ins[5]), cc_OR2(Ins[2], Ins[3])), Ins[0]).co;
		Ins[2].co = co_OR2(co_NAND3(*Outs[0], cc_OR2(Ins[4], Ins[5]), cc_OR2(Ins[0], Ins[1])), Ins[3]).co;
		Ins[3].co = co_OR2(co_NAND3(*Outs[0], cc_OR2(Ins[4], Ins[5]), cc_OR2(Ins[0], Ins[1])), Ins[2]).co;
		Ins[4].co = co_OR2(co_NAND3(*Outs[0], cc_OR2(Ins[2], Ins[3]), cc_OR2(Ins[0], Ins[1])), Ins[5]).co;
		Ins[5].co = co_OR2(co_NAND3(*Outs[0], cc_OR2(Ins[2], Ins[3]), cc_OR2(Ins[0], Ins[1])), Ins[4]).co;
		Ins[0].so = co_OR2(co_NAND3(*Outs[0], cc_OR2(Ins[4], Ins[5]), cc_OR2(Ins[2], Ins[3])), Ins[1]).so;
		Ins[1].so = co_OR2(co_NAND3(*Outs[0], cc_OR2(Ins[4], Ins[5]), cc_OR2(Ins[2], Ins[3])), Ins[0]).so;
		Ins[2].so = co_OR2(co_NAND3(*Outs[0], cc_OR2(Ins[4], Ins[5]), cc_OR2(Ins[0], Ins[1])), Ins[3]).so;
		Ins[3].so = co_OR2(co_NAND3(*Outs[0], cc_OR2(Ins[4], Ins[5]), cc_OR2(Ins[0], Ins[1])), Ins[2]).so;
		Ins[4].so = co_OR2(co_NAND3(*Outs[0], cc_OR2(Ins[2], Ins[3]), cc_OR2(Ins[0], Ins[1])), Ins[5]).so;
		Ins[5].so = co_OR2(co_NAND3(*Outs[0], cc_OR2(Ins[2], Ins[3]), cc_OR2(Ins[0], Ins[1])), Ins[4]).so;
	}
	else if (n->type >= OAI2BB1XL && n->type <= OAI2BB1X4) {
		Ins[0].co = co_NOT(co_OR2(co_NAND2(*Outs[0], Ins[2]), cc_NOT(Ins[1]))).co;
		Ins[1].co = co_NOT(co_OR2(co_NAND2(*Outs[0], Ins[2]), cc_NOT(Ins[0]))).co;
		Ins[2].co = co_NAND2(*Outs[0], cc_OR2(cc_NOT(Ins[0]), cc_NOT(Ins[1]))).co;
		Ins[0].so = co_NOT(co_OR2(co_NAND2(*Outs[0], Ins[2]), cc_NOT(Ins[1]))).so;
		Ins[1].so = co_NOT(co_OR2(co_NAND2(*Outs[0], Ins[2]), cc_NOT(Ins[0]))).so;
		Ins[2].so = co_NAND2(*Outs[0], cc_OR2(cc_NOT(Ins[0]), cc_NOT(Ins[1]))).so;
	}
	else if (n->type >= OAI2BB2XL && n->type <= OAI2BB2X4) {
		Ins[0].co = co_NOT(co_OR2(co_NAND2(*Outs[0], cc_OR2(Ins[2], Ins[3])), cc_NOT(Ins[1]))).co;
		Ins[1].co = co_NOT(co_OR2(co_NAND2(*Outs[0], cc_OR2(Ins[2], Ins[3])), cc_NOT(Ins[0]))).co;
		Ins[2].co = co_OR2(co_NAND2(*Outs[0], cc_OR2(Ins[0], Ins[1])), Ins[3]).co;
		Ins[3].co = co_OR2(co_NAND2(*Outs[0], cc_OR2(Ins[0], Ins[1])), Ins[2]).co;
		Ins[0].so = co_NOT(co_OR2(co_NAND2(*Outs[0], cc_OR2(Ins[2], Ins[3])), cc_NOT(Ins[1]))).so;
		Ins[1].so = co_NOT(co_OR2(co_NAND2(*Outs[0], cc_OR2(Ins[2], Ins[3])), cc_NOT(Ins[0]))).so;
		Ins[2].so = co_OR2(co_NAND2(*Outs[0], cc_OR2(Ins[0], Ins[1])), Ins[3]).so;
		Ins[3].so = co_OR2(co_NAND2(*Outs[0], cc_OR2(Ins[0], Ins[1])), Ins[2]).so;
	}
	else if (n->type >= OAI31XL && n->type <= OAI31X4) {
		Ins[0].co = co_OR3(co_NAND2(*Outs[0], Ins[3]), Ins[1], Ins[2]).co;
		Ins[1].co = co_OR3(co_NAND2(*Outs[0], Ins[3]), Ins[0], Ins[2]).co;
		Ins[2].co = co_OR3(co_NAND2(*Outs[0], Ins[3]), Ins[0], Ins[1]).co;
		Ins[3].co = co_NAND2(*Outs[0], cc_OR3(Ins[0], Ins[1], Ins[2])).co;
		Ins[0].so = co_OR3(co_NAND2(*Outs[0], Ins[3]), Ins[1], Ins[2]).so;
		Ins[1].so = co_OR3(co_NAND2(*Outs[0], Ins[3]), Ins[0], Ins[2]).so;
		Ins[2].so = co_OR3(co_NAND2(*Outs[0], Ins[3]), Ins[0], Ins[1]).so;
		Ins[3].so = co_NAND2(*Outs[0], cc_OR3(Ins[0], Ins[1], Ins[2])).so;
	}
	else if (n->type >= OAI32XL && n->type <= OAI32X4) {
		Ins[0].co = co_OR3(co_NAND2(*Outs[0], cc_OR2(Ins[3], Ins[4])), Ins[1], Ins[2]).co;
		Ins[1].co = co_OR3(co_NAND2(*Outs[0], cc_OR2(Ins[3], Ins[4])), Ins[0], Ins[2]).co;
		Ins[2].co = co_OR3(co_NAND2(*Outs[0], cc_OR2(Ins[3], Ins[4])), Ins[0], Ins[1]).co;
		Ins[3].co = co_OR2(co_NAND2(*Outs[0], cc_OR3(Ins[0], Ins[1], Ins[2])), Ins[4]).co;
		Ins[4].co = co_OR2(co_NAND2(*Outs[0], cc_OR3(Ins[0], Ins[1], Ins[2])), Ins[3]).co;
		Ins[0].so = co_OR3(co_NAND2(*Outs[0], cc_OR2(Ins[3], Ins[4])), Ins[1], Ins[2]).so;
		Ins[1].so = co_OR3(co_NAND2(*Outs[0], cc_OR2(Ins[3], Ins[4])), Ins[0], Ins[2]).so;
		Ins[2].so = co_OR3(co_NAND2(*Outs[0], cc_OR2(Ins[3], Ins[4])), Ins[0], Ins[1]).so;
		Ins[3].so = co_OR2(co_NAND2(*Outs[0], cc_OR3(Ins[0], Ins[1], Ins[2])), Ins[4]).so;
		Ins[4].so = co_OR2(co_NAND2(*Outs[0], cc_OR3(Ins[0], Ins[1], Ins[2])), Ins[3]).so;
	}
	else if (n->type >= OAI33XL && n->type <= OAI33X4) {
		Ins[0].co = co_OR3(co_NAND2(*Outs[0], cc_OR3(Ins[3], Ins[4], Ins[5])), Ins[1], Ins[2]).co;
		Ins[1].co = co_OR3(co_NAND2(*Outs[0], cc_OR3(Ins[3], Ins[4], Ins[5])), Ins[0], Ins[2]).co;
		Ins[2].co = co_OR3(co_NAND2(*Outs[0], cc_OR3(Ins[3], Ins[4], Ins[5])), Ins[0], Ins[1]).co;
		Ins[3].co = co_OR3(co_NAND2(*Outs[0], cc_OR3(Ins[0], Ins[1], Ins[2])), Ins[4], Ins[5]).co;
		Ins[4].co = co_OR3(co_NAND2(*Outs[0], cc_OR3(Ins[0], Ins[1], Ins[2])), Ins[3], Ins[5]).co;
		Ins[5].co = co_OR3(co_NAND2(*Outs[0], cc_OR3(Ins[0], Ins[1], Ins[2])), Ins[3], Ins[4]).co;
		Ins[0].so = co_OR3(co_NAND2(*Outs[0], cc_OR3(Ins[3], Ins[4], Ins[5])), Ins[1], Ins[2]).so;
		Ins[1].so = co_OR3(co_NAND2(*Outs[0], cc_OR3(Ins[3], Ins[4], Ins[5])), Ins[0], Ins[2]).so;
		Ins[2].so = co_OR3(co_NAND2(*Outs[0], cc_OR3(Ins[3], Ins[4], Ins[5])), Ins[0], Ins[1]).so;
		Ins[3].so = co_OR3(co_NAND2(*Outs[0], cc_OR3(Ins[0], Ins[1], Ins[2])), Ins[4], Ins[5]).so;
		Ins[4].so = co_OR3(co_NAND2(*Outs[0], cc_OR3(Ins[0], Ins[1], Ins[2])), Ins[3], Ins[5]).so;
		Ins[5].so = co_OR3(co_NAND2(*Outs[0], cc_OR3(Ins[0], Ins[1], Ins[2])), Ins[3], Ins[4]).so;
	}
	else if (n->type >= MUX21X1 && n->type <= MX2X4) {
		Ins[0].co = co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[1], cc_NOT(cc_NOT(Ins[2])))), cc_NOT(Ins[2])).co;
		Ins[1].co = co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[0], cc_NOT(Ins[2]))), cc_NOT(cc_NOT(Ins[2]))).co;
		Ins[2].co = std::min({ co_NOT(co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[1], cc_NOT(cc_NOT(Ins[2])))), Ins[0])).co,
			co_NOT(co_NOT(co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[0], cc_NOT(Ins[2]))), Ins[1]))).co });
		Ins[0].so = co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[1], cc_NOT(cc_NOT(Ins[2])))), cc_NOT(Ins[2])).so;
		Ins[1].so = co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[0], cc_NOT(Ins[2]))), cc_NOT(cc_NOT(Ins[2]))).so;
		Ins[2].so = std::min({ co_NOT(co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[1], cc_NOT(cc_NOT(Ins[2])))), Ins[0])).so,
			co_NOT(co_NOT(co_AND2(co_OR2(*Outs[0], cc_AND2(Ins[0], cc_NOT(Ins[2]))), Ins[1]))).so });
	}
	else if (n->type >= MUX41X1 && n->type <= MX4X4) {
		SCOAP R = cc_OR2(cc_AND3(Ins[0], cc_NOT(Ins[4]), cc_NOT(Ins[5])), cc_AND3(Ins[1], cc_NOT(cc_NOT(Ins[4])), cc_NOT(Ins[5])));
		SCOAP H = cc_OR2(cc_AND3(Ins[2], cc_NOT(Ins[4]), cc_NOT(cc_NOT(Ins[5]))), cc_AND3(Ins[3], cc_NOT(cc_NOT(Ins[4])), cc_NOT(cc_NOT(Ins[5]))));
		R.cc[0] -= 2;
		H.cc[0] -= 2;
		Ins[0].co = co_AND3(co_OR2(co_OR2(*Outs[0], H), cc_AND3(Ins[1], cc_NOT(cc_NOT(Ins[4])), cc_NOT(Ins[5]))), cc_NOT(Ins[4]), cc_NOT(Ins[5])).co;
		Ins[1].co = co_AND3(co_OR2(co_OR2(*Outs[0], H), cc_AND3(Ins[0], cc_NOT(Ins[4]), cc_NOT(Ins[5]))), cc_NOT(cc_NOT(Ins[4])), cc_NOT(Ins[5])).co;
		Ins[2].co = co_AND3(co_OR2(co_OR2(*Outs[0], R), cc_AND3(Ins[3], cc_NOT(cc_NOT(Ins[4])), cc_NOT(cc_NOT(Ins[5])))), cc_NOT(Ins[4]), cc_NOT(cc_NOT(Ins[5]))).co;
		Ins[3].co = co_AND3(co_OR2(co_OR2(*Outs[0], R), cc_AND3(Ins[2], cc_NOT(Ins[4]), cc_NOT(cc_NOT(Ins[5])))), cc_NOT(cc_NOT(Ins[4])), cc_NOT(cc_NOT(Ins[5]))).co;

		Ins[4].co = std::min({
			co_AND3(co_OR2(co_OR2(*Outs[0], H), cc_AND3(Ins[1], cc_NOT(cc_NOT(Ins[4])), cc_NOT(Ins[5]))), Ins[0], cc_NOT(Ins[5])).co,
			co_AND3(co_OR2(co_OR2(*Outs[0], H), cc_AND3(Ins[0], cc_NOT(Ins[4]), cc_NOT(Ins[5]))), Ins[1], cc_NOT(Ins[5])).co,
			co_AND3(co_OR2(co_OR2(*Outs[0], R), cc_AND3(Ins[3], cc_NOT(cc_NOT(Ins[4])), cc_NOT(cc_NOT(Ins[5])))), Ins[2], cc_NOT(cc_NOT(Ins[5]))).co,
			co_AND3(co_OR2(co_OR2(*Outs[0], R), cc_AND3(Ins[2], cc_NOT(Ins[4]), cc_NOT(cc_NOT(Ins[5])))), Ins[3], cc_NOT(cc_NOT(Ins[5]))).co
		});
		Ins[5].co = std::min({
			co_AND3(co_OR2(co_OR2(*Outs[0], H), cc_AND3(Ins[1], cc_NOT(cc_NOT(Ins[4])), cc_NOT(Ins[5]))), cc_NOT(Ins[4]), Ins[0]).co,
			co_AND3(co_OR2(co_OR2(*Outs[0], H), cc_AND3(Ins[0], cc_NOT(Ins[4]), cc_NOT(Ins[5]))), cc_NOT(cc_NOT(Ins[4])), Ins[1]).co,
			co_AND3(co_OR2(co_OR2(*Outs[0], R), cc_AND3(Ins[3], cc_NOT(cc_NOT(Ins[4])), cc_NOT(cc_NOT(Ins[5])))), cc_NOT(Ins[4]), Ins[2]).co,
			co_AND3(co_OR2(co_OR2(*Outs[0], R), cc_AND3(Ins[2], cc_NOT(Ins[4]), cc_NOT(cc_NOT(Ins[5])))), cc_NOT(cc_NOT(Ins[4])), Ins[3]).co
		});
		
		R = cc_OR2(cc_AND3(Ins[0], cc_NOT(Ins[4]), cc_NOT(Ins[5])), cc_AND3(Ins[1], cc_NOT(cc_NOT(Ins[4])), cc_NOT(Ins[5])));
		H = cc_OR2(cc_AND3(Ins[2], cc_NOT(Ins[4]), cc_NOT(cc_NOT(Ins[5]))), cc_AND3(Ins[3], cc_NOT(cc_NOT(Ins[4])), cc_NOT(cc_NOT(Ins[5]))));
		R.cc[0] -= 2;
		H.cc[0] -= 2;
		Ins[0].so = co_AND3(co_OR2(co_OR2(*Outs[0], H), cc_AND3(Ins[1], cc_NOT(cc_NOT(Ins[4])), cc_NOT(Ins[5]))), cc_NOT(Ins[4]), cc_NOT(Ins[5])).so;
		Ins[1].so = co_AND3(co_OR2(co_OR2(*Outs[0], H), cc_AND3(Ins[0], cc_NOT(Ins[4]), cc_NOT(Ins[5]))), cc_NOT(cc_NOT(Ins[4])), cc_NOT(Ins[5])).so;
		Ins[2].so = co_AND3(co_OR2(co_OR2(*Outs[0], R), cc_AND3(Ins[3], cc_NOT(cc_NOT(Ins[4])), cc_NOT(cc_NOT(Ins[5])))), cc_NOT(Ins[4]), cc_NOT(cc_NOT(Ins[5]))).so;
		Ins[3].so = co_AND3(co_OR2(co_OR2(*Outs[0], R), cc_AND3(Ins[2], cc_NOT(Ins[4]), cc_NOT(cc_NOT(Ins[5])))), cc_NOT(cc_NOT(Ins[4])), cc_NOT(cc_NOT(Ins[5]))).so;

		Ins[4].so = std::min({
			co_AND3(co_OR2(co_OR2(*Outs[0], H), cc_AND3(Ins[1], cc_NOT(cc_NOT(Ins[4])), cc_NOT(Ins[5]))), Ins[0], cc_NOT(Ins[5])).so,
			co_AND3(co_OR2(co_OR2(*Outs[0], H), cc_AND3(Ins[0], cc_NOT(Ins[4]), cc_NOT(Ins[5]))), Ins[1], cc_NOT(Ins[5])).so,
			co_AND3(co_OR2(co_OR2(*Outs[0], R), cc_AND3(Ins[3], cc_NOT(cc_NOT(Ins[4])), cc_NOT(cc_NOT(Ins[5])))), Ins[2], cc_NOT(cc_NOT(Ins[5]))).so,
			co_AND3(co_OR2(co_OR2(*Outs[0], R), cc_AND3(Ins[2], cc_NOT(Ins[4]), cc_NOT(cc_NOT(Ins[5])))), Ins[3], cc_NOT(cc_NOT(Ins[5]))).so
		});
		Ins[5].so = std::min({
			co_AND3(co_OR2(co_OR2(*Outs[0], H), cc_AND3(Ins[1], cc_NOT(cc_NOT(Ins[4])), cc_NOT(Ins[5]))), cc_NOT(Ins[4]), Ins[0]).so,
			co_AND3(co_OR2(co_OR2(*Outs[0], H), cc_AND3(Ins[0], cc_NOT(Ins[4]), cc_NOT(Ins[5]))), cc_NOT(cc_NOT(Ins[4])), Ins[1]).so,
			co_AND3(co_OR2(co_OR2(*Outs[0], R), cc_AND3(Ins[3], cc_NOT(cc_NOT(Ins[4])), cc_NOT(cc_NOT(Ins[5])))), cc_NOT(Ins[4]), Ins[2]).so,
			co_AND3(co_OR2(co_OR2(*Outs[0], R), cc_AND3(Ins[2], cc_NOT(Ins[4]), cc_NOT(cc_NOT(Ins[5])))), cc_NOT(cc_NOT(Ins[4])), Ins[3]).so
		});
	}
	else if (n->type >= MXI2XL && n->type <= MXI2X4) {
		Ins[0].co = co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[1], cc_NOT(cc_NOT(Ins[2])))), cc_NOT(Ins[2])).co;
		Ins[1].co = co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[0], cc_NOT(Ins[2]))), cc_NOT(cc_NOT(Ins[2]))).co;
		Ins[2].co = std::min({ co_NOT(co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[1], cc_NOT(cc_NOT(Ins[2])))), Ins[0])).co,
			co_NOT(co_NOT(co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[0], cc_NOT(Ins[2]))), Ins[1]))).co });
		Ins[0].so = co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[1], cc_NOT(cc_NOT(Ins[2])))), cc_NOT(Ins[2])).so;
		Ins[1].so = co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[0], cc_NOT(Ins[2]))), cc_NOT(cc_NOT(Ins[2]))).so;
		Ins[2].so = std::min({ co_NOT(co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[1], cc_NOT(cc_NOT(Ins[2])))), Ins[0])).so,
			co_NOT(co_NOT(co_AND2(co_NOR2(*Outs[0], cc_AND2(Ins[0], cc_NOT(Ins[2]))), Ins[1]))).so });
	}
	else if (n->type >= MXI4XL && n->type <= MXI4X4) {
		SCOAP R = cc_OR2(cc_AND3(Ins[0], cc_NOT(Ins[4]), cc_NOT(Ins[5])), cc_AND3(Ins[1], cc_NOT(cc_NOT(Ins[4])), cc_NOT(Ins[5])));
		SCOAP H = cc_OR2(cc_AND3(Ins[2], cc_NOT(Ins[4]), cc_NOT(cc_NOT(Ins[5]))), cc_AND3(Ins[3], cc_NOT(cc_NOT(Ins[4])), cc_NOT(cc_NOT(Ins[5]))));
		R.cc[0] -= 2;
		H.cc[0] -= 2;
		Ins[0].co = co_AND3(co_OR2(co_NOR2(*Outs[0], H), cc_AND3(Ins[1], cc_NOT(cc_NOT(Ins[4])), cc_NOT(Ins[5]))), cc_NOT(Ins[4]), cc_NOT(Ins[5])).co;
		Ins[1].co = co_AND3(co_OR2(co_NOR2(*Outs[0], H), cc_AND3(Ins[0], cc_NOT(Ins[4]), cc_NOT(Ins[5]))), cc_NOT(cc_NOT(Ins[4])), cc_NOT(Ins[5])).co;
		Ins[2].co = co_AND3(co_OR2(co_NOR2(*Outs[0], R), cc_AND3(Ins[3], cc_NOT(cc_NOT(Ins[4])), cc_NOT(cc_NOT(Ins[5])))), cc_NOT(Ins[4]), cc_NOT(cc_NOT(Ins[5]))).co;
		Ins[3].co = co_AND3(co_OR2(co_NOR2(*Outs[0], R), cc_AND3(Ins[2], cc_NOT(Ins[4]), cc_NOT(cc_NOT(Ins[5])))), cc_NOT(cc_NOT(Ins[4])), cc_NOT(cc_NOT(Ins[5]))).co;

		Ins[4].co = std::min({
			co_AND3(co_OR2(co_NOR2(*Outs[0], H), cc_AND3(Ins[1], cc_NOT(cc_NOT(Ins[4])), cc_NOT(Ins[5]))), Ins[0], cc_NOT(Ins[5])).co,
			co_AND3(co_OR2(co_NOR2(*Outs[0], H), cc_AND3(Ins[0], cc_NOT(Ins[4]), cc_NOT(Ins[5]))), Ins[1], cc_NOT(Ins[5])).co,
			co_AND3(co_OR2(co_NOR2(*Outs[0], R), cc_AND3(Ins[3], cc_NOT(cc_NOT(Ins[4])), cc_NOT(cc_NOT(Ins[5])))), Ins[2], cc_NOT(cc_NOT(Ins[5]))).co,
			co_AND3(co_OR2(co_NOR2(*Outs[0], R), cc_AND3(Ins[2], cc_NOT(Ins[4]), cc_NOT(cc_NOT(Ins[5])))), Ins[3], cc_NOT(cc_NOT(Ins[5]))).co
		});
		Ins[5].co = std::min({
			co_AND3(co_OR2(co_NOR2(*Outs[0], H), cc_AND3(Ins[1], cc_NOT(cc_NOT(Ins[4])), cc_NOT(Ins[5]))), cc_NOT(Ins[4]), Ins[0]).co,
			co_AND3(co_OR2(co_NOR2(*Outs[0], H), cc_AND3(Ins[0], cc_NOT(Ins[4]), cc_NOT(Ins[5]))), cc_NOT(cc_NOT(Ins[4])), Ins[1]).co,
			co_AND3(co_OR2(co_NOR2(*Outs[0], R), cc_AND3(Ins[3], cc_NOT(cc_NOT(Ins[4])), cc_NOT(cc_NOT(Ins[5])))), cc_NOT(Ins[4]), Ins[2]).co,
			co_AND3(co_OR2(co_NOR2(*Outs[0], R), cc_AND3(Ins[2], cc_NOT(Ins[4]), cc_NOT(cc_NOT(Ins[5])))), cc_NOT(cc_NOT(Ins[4])), Ins[3]).co
		});
		R = cc_OR2(cc_AND3(Ins[0], cc_NOT(Ins[4]), cc_NOT(Ins[5])), cc_AND3(Ins[1], cc_NOT(cc_NOT(Ins[4])), cc_NOT(Ins[5])));
		H = cc_OR2(cc_AND3(Ins[2], cc_NOT(Ins[4]), cc_NOT(cc_NOT(Ins[5]))), cc_AND3(Ins[3], cc_NOT(cc_NOT(Ins[4])), cc_NOT(cc_NOT(Ins[5]))));
		R.cc[0] -= 2;
		H.cc[0] -= 2;
		Ins[0].so = co_AND3(co_OR2(co_NOR2(*Outs[0], H), cc_AND3(Ins[1], cc_NOT(cc_NOT(Ins[4])), cc_NOT(Ins[5]))), cc_NOT(Ins[4]), cc_NOT(Ins[5])).so;
		Ins[1].so = co_AND3(co_OR2(co_NOR2(*Outs[0], H), cc_AND3(Ins[0], cc_NOT(Ins[4]), cc_NOT(Ins[5]))), cc_NOT(cc_NOT(Ins[4])), cc_NOT(Ins[5])).so;
		Ins[2].so = co_AND3(co_OR2(co_NOR2(*Outs[0], R), cc_AND3(Ins[3], cc_NOT(cc_NOT(Ins[4])), cc_NOT(cc_NOT(Ins[5])))), cc_NOT(Ins[4]), cc_NOT(cc_NOT(Ins[5]))).so;
		Ins[3].so = co_AND3(co_OR2(co_NOR2(*Outs[0], R), cc_AND3(Ins[2], cc_NOT(Ins[4]), cc_NOT(cc_NOT(Ins[5])))), cc_NOT(cc_NOT(Ins[4])), cc_NOT(cc_NOT(Ins[5]))).so;

		Ins[4].so = std::min({
			co_AND3(co_OR2(co_NOR2(*Outs[0], H), cc_AND3(Ins[1], cc_NOT(cc_NOT(Ins[4])), cc_NOT(Ins[5]))), Ins[0], cc_NOT(Ins[5])).so,
			co_AND3(co_OR2(co_NOR2(*Outs[0], H), cc_AND3(Ins[0], cc_NOT(Ins[4]), cc_NOT(Ins[5]))), Ins[1], cc_NOT(Ins[5])).so,
			co_AND3(co_OR2(co_NOR2(*Outs[0], R), cc_AND3(Ins[3], cc_NOT(cc_NOT(Ins[4])), cc_NOT(cc_NOT(Ins[5])))), Ins[2], cc_NOT(cc_NOT(Ins[5]))).so,
			co_AND3(co_OR2(co_NOR2(*Outs[0], R), cc_AND3(Ins[2], cc_NOT(Ins[4]), cc_NOT(cc_NOT(Ins[5])))), Ins[3], cc_NOT(cc_NOT(Ins[5]))).so
		});
		Ins[5].so = std::min({
			co_AND3(co_OR2(co_NOR2(*Outs[0], H), cc_AND3(Ins[1], cc_NOT(cc_NOT(Ins[4])), cc_NOT(Ins[5]))), cc_NOT(Ins[4]), Ins[0]).so,
			co_AND3(co_OR2(co_NOR2(*Outs[0], H), cc_AND3(Ins[0], cc_NOT(Ins[4]), cc_NOT(Ins[5]))), cc_NOT(cc_NOT(Ins[4])), Ins[1]).so,
			co_AND3(co_OR2(co_NOR2(*Outs[0], R), cc_AND3(Ins[3], cc_NOT(cc_NOT(Ins[4])), cc_NOT(cc_NOT(Ins[5])))), cc_NOT(Ins[4]), Ins[2]).so,
			co_AND3(co_OR2(co_NOR2(*Outs[0], R), cc_AND3(Ins[2], cc_NOT(Ins[4]), cc_NOT(cc_NOT(Ins[5])))), cc_NOT(cc_NOT(Ins[4])), Ins[3]).so
		});
	}
	else if (n->type >= NAND2X0 && n->type <= NAND2X4) {
		Ins[0].co = co_NAND2(*Outs[0], Ins[1]).co;
		Ins[1].co = co_NAND2(*Outs[0], Ins[0]).co;
		Ins[0].so = co_NAND2(*Outs[0], Ins[1]).so;
		Ins[1].so = co_NAND2(*Outs[0], Ins[0]).so;
	}
	else if (n->type >= NAND2BXL && n->type <= NAND2BX4) {
		Ins[0].co = co_NOT(co_NAND2(*Outs[0], Ins[1])).co;
		Ins[1].co = co_NAND2(*Outs[0], cc_NOT(Ins[0])).co;
		Ins[0].so = co_NOT(co_NAND2(*Outs[0], Ins[1])).so;
		Ins[1].so = co_NAND2(*Outs[0], cc_NOT(Ins[0])).so;
	}
	else if (n->type >= NAND3X0 && n->type <= NAND3X4) {
		Ins[0].co = co_NAND3(*Outs[0], Ins[1], Ins[2]).co;
		Ins[1].co = co_NAND3(*Outs[0], Ins[0], Ins[2]).co;
		Ins[2].co = co_NAND3(*Outs[0], Ins[0], Ins[1]).co;
		Ins[0].so = co_NAND3(*Outs[0], Ins[1], Ins[2]).so;
		Ins[1].so = co_NAND3(*Outs[0], Ins[0], Ins[2]).so;
		Ins[2].so = co_NAND3(*Outs[0], Ins[0], Ins[1]).so;
	}
	else if (n->type >= NAND3BXL && n->type <= NAND3BX4) {
		Ins[0].co = co_NOT(co_NAND3(*Outs[0], Ins[1], Ins[2])).co;
		Ins[1].co = co_NAND3(*Outs[0], cc_NOT(Ins[0]), Ins[2]).co;
		Ins[2].co = co_NAND3(*Outs[0], cc_NOT(Ins[0]), Ins[1]).co;
		Ins[0].so = co_NOT(co_NAND3(*Outs[0], Ins[1], Ins[2])).so;
		Ins[1].so = co_NAND3(*Outs[0], cc_NOT(Ins[0]), Ins[2]).so;
		Ins[2].so = co_NAND3(*Outs[0], cc_NOT(Ins[0]), Ins[1]).so;
	}
	else if (n->type >= NAND4X0 && n->type <= NAND4X4) {
		memcpy(s.cc, cc_AND3(Ins[0], Ins[1], Ins[2]).cc, sizeof(s.cc));
		s.co = co_NAND2(*Outs[0], Ins[3]).co - 0;
		Ins[0].co = co_NAND3(s, Ins[1], Ins[2]).co;
		Ins[1].co = co_NAND3(s, Ins[0], Ins[2]).co;
		Ins[2].co = co_NAND3(s, Ins[1], Ins[0]).co;
		Ins[3].co = co_NAND2(*Outs[0], s).co - 0;
		memcpy(s.sc, cc_AND3(Ins[0], Ins[1], Ins[2]).sc, sizeof(s.sc));
		s.so = co_NAND2(*Outs[0], Ins[3]).so - 0;
		Ins[0].so = co_NAND3(s, Ins[1], Ins[2]).so;
		Ins[1].so = co_NAND3(s, Ins[0], Ins[2]).so;
		Ins[2].so = co_NAND3(s, Ins[1], Ins[0]).so;
		Ins[3].so = co_NAND2(*Outs[0], s).so - 0;
	}
	else if (n->type >= NAND4BXL && n->type <= NAND4BX4) {
		memcpy(s.cc, cc_AND3(cc_NOT(Ins[0]), Ins[1], Ins[2]).cc, sizeof(s.cc));
		s.co = co_NAND2(*Outs[0], Ins[3]).co - 0;
		Ins[0].co = co_NOT(co_NAND3(s, Ins[1], Ins[2])).co;
		Ins[1].co = co_NAND3(s, cc_NOT(Ins[0]), Ins[2]).co;
		Ins[2].co = co_NAND3(s, Ins[1], cc_NOT(Ins[0])).co;
		Ins[3].co = co_NAND2(*Outs[0], s).co - 0;
		memcpy(s.sc, cc_AND3(cc_NOT(Ins[0]), Ins[1], Ins[2]).sc, sizeof(s.sc));
		s.so = co_NAND2(*Outs[0], Ins[3]).so - 0;
		Ins[0].so = co_NOT(co_NAND3(s, Ins[1], Ins[2])).so;
		Ins[1].so = co_NAND3(s, cc_NOT(Ins[0]), Ins[2]).so;
		Ins[2].so = co_NAND3(s, Ins[1], cc_NOT(Ins[0])).so;
		Ins[3].so = co_NAND2(*Outs[0], s).so - 0;
	}
	else if (n->type >= NAND4BBXL && n->type <= NAND4BBX4) {
		memcpy(s.cc, cc_AND3(cc_NOT(Ins[0]), cc_NOT(Ins[1]), Ins[2]).cc, sizeof(s.cc));
		s.co = co_NAND2(*Outs[0], Ins[3]).co - 0;
		Ins[0].co = co_NOT(co_NAND3(s, Ins[1], Ins[2])).co;
		Ins[1].co = co_NOT(co_NAND3(s, cc_NOT(Ins[0]), Ins[2])).co;
		Ins[2].co = co_NAND3(s, cc_NOT(Ins[1]), cc_NOT(Ins[0])).co;
		Ins[3].co = co_NAND2(*Outs[0], s).co - 0;
		memcpy(s.sc, cc_AND3(cc_NOT(Ins[0]), cc_NOT(Ins[1]), Ins[2]).sc, sizeof(s.sc));
		s.so = co_NAND2(*Outs[0], Ins[3]).so - 0;
		Ins[0].so = co_NOT(co_NAND3(s, Ins[1], Ins[2])).so;
		Ins[1].so = co_NOT(co_NAND3(s, cc_NOT(Ins[0]), Ins[2])).so;
		Ins[2].so = co_NAND3(s, cc_NOT(Ins[1]), cc_NOT(Ins[0])).so;
		Ins[3].so = co_NAND2(*Outs[0], s).so - 0;
	}
	else if (n->type >= CLKINVXL && n->type <= IBUFFX32) {
		Ins[0].co = co_NOT(*Outs[0]).co;
		Ins[0].so = co_NOT(*Outs[0]).so;
	}
	else if (n->type >= NOR2X0 && n->type <= NOR2X4) {
		Ins[0].co = co_NOR2(*Outs[0], Ins[1]).co;
		Ins[1].co = co_NOR2(*Outs[0], Ins[0]).co;
		Ins[0].so = co_NOR2(*Outs[0], Ins[1]).so;
		Ins[1].so = co_NOR2(*Outs[0], Ins[0]).so;
	}
	else if (n->type >= NOR2BXL && n->type <= NOR2BX4) {
		Ins[0].co = co_NOT(co_NOR2(*Outs[0], Ins[1])).co;
		Ins[1].co = co_NOR2(*Outs[0], cc_NOT(Ins[0])).co;
		Ins[0].so = co_NOT(co_NOR2(*Outs[0], Ins[1])).so;
		Ins[1].so = co_NOR2(*Outs[0], cc_NOT(Ins[0])).so;
	}
	else if (n->type >= NOR3X0 && n->type <= NOR3X4) {
		Ins[0].co = co_NOR3(*Outs[0], Ins[1], Ins[2]).co;
		Ins[1].co = co_NOR3(*Outs[0], Ins[0], Ins[2]).co;
		Ins[2].co = co_NOR3(*Outs[0], Ins[0], Ins[1]).co;
		Ins[0].so = co_NOR3(*Outs[0], Ins[1], Ins[2]).so;
		Ins[1].so = co_NOR3(*Outs[0], Ins[0], Ins[2]).so;
		Ins[2].so = co_NOR3(*Outs[0], Ins[0], Ins[1]).so;
	}
	else if (n->type >= NOR3BXL && n->type <= NOR3BX4) {
		Ins[0].co = co_NOT(co_NOR3(*Outs[0], Ins[1], Ins[2])).co;
		Ins[1].co = co_NOR3(*Outs[0], cc_NOT(Ins[0]), Ins[2]).co;
		Ins[2].co = co_NOR3(*Outs[0], cc_NOT(Ins[0]), Ins[1]).co;
		Ins[0].so = co_NOT(co_NOR3(*Outs[0], Ins[1], Ins[2])).so;
		Ins[1].so = co_NOR3(*Outs[0], cc_NOT(Ins[0]), Ins[2]).so;
		Ins[2].so = co_NOR3(*Outs[0], cc_NOT(Ins[0]), Ins[1]).so;
	}
	else if (n->type >= NOR4X0 && n->type <= NOR4X4) {
		memcpy(s.cc, cc_OR3(Ins[0], Ins[1], Ins[2]).cc, sizeof(s.cc));
		s.co = co_NOR2(*Outs[0], Ins[3]).co - 0;
		Ins[0].co = co_NOR3(s, Ins[1], Ins[2]).co;
		Ins[1].co = co_NOR3(s, Ins[0], Ins[2]).co;
		Ins[2].co = co_NOR3(s, Ins[1], Ins[0]).co;
		Ins[3].co = co_NOR2(*Outs[0], s).co - 0;
		memcpy(s.sc, cc_OR3(Ins[0], Ins[1], Ins[2]).sc, sizeof(s.sc));
		s.so = co_NOR2(*Outs[0], Ins[3]).so - 0;
		Ins[0].so = co_NOR3(s, Ins[1], Ins[2]).so;
		Ins[1].so = co_NOR3(s, Ins[0], Ins[2]).so;
		Ins[2].so = co_NOR3(s, Ins[1], Ins[0]).so;
		Ins[3].so = co_NOR2(*Outs[0], s).so - 0;
	}
	else if (n->type >= NOR4BXL && n->type <= NOR4BX4) {
		memcpy(s.cc, cc_OR3(cc_NOT(Ins[0]), Ins[1], Ins[2]).cc, sizeof(s.cc));
		s.co = co_NOR2(*Outs[0], Ins[3]).co - 0;
		Ins[0].co = co_NOT(co_NAND3(s, Ins[1], Ins[2])).co;
		Ins[1].co = co_NOR3(s, cc_NOT(Ins[0]), Ins[2]).co;
		Ins[2].co = co_NOR3(s, Ins[1], cc_NOT(Ins[0])).co;
		Ins[3].co = co_NOR2(*Outs[0], s).co - 0;
		memcpy(s.sc, cc_OR3(cc_NOT(Ins[0]), Ins[1], Ins[2]).sc, sizeof(s.sc));
		s.so = co_NOR2(*Outs[0], Ins[3]).so - 0;
		Ins[0].so = co_NOT(co_NAND3(s, Ins[1], Ins[2])).so;
		Ins[1].so = co_NOR3(s, cc_NOT(Ins[0]), Ins[2]).so;
		Ins[2].so = co_NOR3(s, Ins[1], cc_NOT(Ins[0])).so;
		Ins[3].so = co_NOR2(*Outs[0], s).so - 0;
	}
	else if (n->type >= NOR4BBXL && n->type <= NOR4BBX4) {
		memcpy(s.cc, cc_OR3(cc_NOT(Ins[0]), cc_NOT(Ins[1]), Ins[2]).cc, sizeof(s.cc));
		s.co = co_NOR2(*Outs[0], Ins[3]).co - 0;
		Ins[0].co = co_NOT(co_NOR3(s, Ins[1], Ins[2])).co;
		Ins[1].co = co_NOT(co_NOR3(s, cc_NOT(Ins[0]), Ins[2])).co;
		Ins[2].co = co_NOR3(s, cc_NOT(Ins[1]), cc_NOT(Ins[0])).co;
		Ins[3].co = co_NOR2(*Outs[0], s).co - 0;
		memcpy(s.sc, cc_OR3(cc_NOT(Ins[0]), cc_NOT(Ins[1]), Ins[2]).sc, sizeof(s.sc));
		s.so = co_NOR2(*Outs[0], Ins[3]).so - 0;
		Ins[0].so = co_NOT(co_NOR3(s, Ins[1], Ins[2])).so;
		Ins[1].so = co_NOT(co_NOR3(s, cc_NOT(Ins[0]), Ins[2])).so;
		Ins[2].so = co_NOR3(s, cc_NOT(Ins[1]), cc_NOT(Ins[0])).so;
		Ins[3].so = co_NOR2(*Outs[0], s).so - 0;
	}
	else if (n->type >= OR2XL && n->type <= ISOLORX8) {
		Ins[0].co = co_OR2(*Outs[0], Ins[1]).co;
		Ins[1].co = co_OR2(*Outs[0], Ins[0]).co;
		Ins[0].so = co_OR2(*Outs[0], Ins[1]).so;
		Ins[1].so = co_OR2(*Outs[0], Ins[0]).so;
	}
	else if (n->type >= OR3XL && n->type <= OR3X4) {
		Ins[0].co = co_OR3(*Outs[0], Ins[1], Ins[2]).co;
		Ins[1].co = co_OR3(*Outs[0], Ins[0], Ins[2]).co;
		Ins[2].co = co_OR3(*Outs[0], Ins[0], Ins[1]).co;
		Ins[0].so = co_OR3(*Outs[0], Ins[1], Ins[2]).so;
		Ins[1].so = co_OR3(*Outs[0], Ins[0], Ins[2]).so;
		Ins[2].so = co_OR3(*Outs[0], Ins[0], Ins[1]).so;
	}
	else if (n->type >= OR4XL && n->type <= OR4X4) {
		memcpy(s.cc, cc_OR3(Ins[0], Ins[1], Ins[2]).cc, sizeof(s.cc));
		s.co = co_OR2(*Outs[0], Ins[3]).co - 0;
		Ins[0].co = co_OR3(s, Ins[1], Ins[2]).co;
		Ins[1].co = co_OR3(s, Ins[0], Ins[2]).co;
		Ins[2].co = co_OR3(s, Ins[1], Ins[0]).co;
		Ins[3].co = co_OR2(*Outs[0], s).co - 0;
		memcpy(s.sc, cc_OR3(Ins[0], Ins[1], Ins[2]).sc, sizeof(s.sc));
		s.so = co_OR2(*Outs[0], Ins[3]).so - 0;
		Ins[0].so = co_OR3(s, Ins[1], Ins[2]).so;
		Ins[1].so = co_OR3(s, Ins[0], Ins[2]).so;
		Ins[2].so = co_OR3(s, Ins[1], Ins[0]).so;
		Ins[3].so = co_OR2(*Outs[0], s).so - 0;
	}
	else if (n->type >= XOR2XL && n->type <= XOR2X4) {
		Ins[0].co = co_XNOR2(*Outs[0], Ins[1]).co;
		Ins[1].co = co_XNOR2(*Outs[0], Ins[0]).co;
		Ins[0].so = co_XNOR2(*Outs[0], Ins[1]).so;
		Ins[1].so = co_XNOR2(*Outs[0], Ins[0]).so;
	}
	else if (n->type >= XNOR2XL && n->type <= XNOR2X4) {
		Ins[0].co = co_XNOR2(*Outs[0], Ins[1]).co;
		Ins[1].co = co_XNOR2(*Outs[0], Ins[0]).co;
		Ins[0].so = co_XNOR2(*Outs[0], Ins[1]).so;
		Ins[1].so = co_XNOR2(*Outs[0], Ins[0]).so;
	}
	else if (n->type >= XOR3X1 && n->type <= XOR3X2) {
		Ins[0].co = co_XNOR3_v2(*Outs[0], Ins[1], Ins[2]).co;
		Ins[1].co = co_XNOR3_v2(*Outs[0], Ins[0], Ins[2]).co;
		Ins[2].co = co_XNOR3_v2(*Outs[0], Ins[0], Ins[1]).co;
		Ins[0].so = co_XNOR3_v2(*Outs[0], Ins[1], Ins[2]).so;
		Ins[1].so = co_XNOR3_v2(*Outs[0], Ins[0], Ins[2]).so;
		Ins[2].so = co_XNOR3_v2(*Outs[0], Ins[0], Ins[1]).so;
	}
	else if (n->type >= XNOR3X1 && n->type <= XNOR3X2) {
		Ins[0].co = co_XNOR3_v2(*Outs[0], Ins[1], Ins[2]).co;
		Ins[1].co = co_XNOR3_v2(*Outs[0], Ins[0], Ins[2]).co;
		Ins[2].co = co_XNOR3_v2(*Outs[0], Ins[0], Ins[1]).co;
		Ins[0].so = co_XNOR3_v2(*Outs[0], Ins[1], Ins[2]).so;
		Ins[1].so = co_XNOR3_v2(*Outs[0], Ins[0], Ins[2]).so;
		Ins[2].so = co_XNOR3_v2(*Outs[0], Ins[0], Ins[1]).so;
	}
	//sequential elements //TODO change to real formulations
	else if (n->type >= BUFXL && n->type <= CLKBUFX20) {
		Ins[0].co = co_NOT(co_NOT(*Outs[0])).co;
		Ins[0].so = co_NOT(co_NOT(*Outs[0])).so;
	}
	//ref: book->Design For Testability 
	else if (n->type >= DFFXL && n->type <= DFFNX4) {
		Ins[0].co = Outs[0]->co + Ins[1].cc[1] + Ins[1].cc[0];
		Ins[1].co = Outs[0]->co + Ins[1].cc[1] + Ins[1].cc[0] +
			std::min({ Ins[0].cc[0] + Outs[0]->cc[1],
				Outs[0]->cc[0] + Ins[0].cc[1] });
		Ins[0].so = Outs[0]->so + Ins[1].sc[1] + Ins[1].sc[0];
		Ins[1].so = Outs[0]->so + Ins[1].sc[1] + Ins[1].sc[0] +
			std::min({ Ins[0].sc[0] + Outs[0]->sc[1],
				Outs[0]->sc[0] + Ins[0].sc[1] });
	}
	else if (n->type >= SDFFXL && n->type <= SDFFX4) {
		Ins[0].co = Outs[0]->co + Ins[1].cc[1] + Ins[1].cc[0] + Ins[2].cc[0];
		Ins[1].co = Outs[0]->co + Ins[1].cc[1] + Ins[1].cc[0] +
			std::min({ Ins[0].cc[0] + Ins[3].cc[0] + Outs[0]->cc[1],
				Ins[0].cc[1] + Ins[3].cc[0] + Outs[0]->cc[0] });
		Ins[2].co = Outs[0]->co + Outs[0]->cc[1] + Ins[1].cc[0];
		Ins[3].co = Outs[0]->co + Ins[1].cc[1] + Ins[1].cc[0] + Ins[2].cc[1];

		Ins[0].so = Outs[0]->so + Ins[1].sc[1] + Ins[1].sc[0] + Ins[2].sc[0];
		Ins[1].so = Outs[0]->so + Ins[1].sc[1] + Ins[1].sc[0] +
			std::min({ Ins[0].sc[0] + Ins[3].sc[0] + Outs[0]->sc[1],
				Ins[0].sc[1] + Ins[3].sc[0] + Outs[0]->sc[0] });
		Ins[2].so = Outs[0]->so + Outs[0]->sc[1] + Ins[1].sc[0];
		Ins[3].so = Outs[0]->so + Ins[1].sc[1] + Ins[1].sc[0] + Ins[2].sc[1];

	}
	else if (n->type >= SDFFARX1 && n->type <= SDFFARX2) {
		Ins[0].co = Outs[0]->co + Ins[1].cc[1] + Ins[1].cc[0] + Ins[2].cc[0] + Ins[4].cc[1] ;
		Ins[1].co = Outs[0]->co + Ins[1].cc[1] + Ins[1].cc[0] +
			std::min({ Ins[0].cc[0] + Ins[3].cc[0] + Outs[0]->cc[1],
				Ins[0].cc[1] + Ins[3].cc[0] + Outs[0]->cc[0] });
		Ins[2].co = Outs[0]->co + Outs[0]->cc[1] + Ins[1].cc[0];
		Ins[3].co = Outs[0]->co + Ins[1].cc[1] + Ins[1].cc[0] + Ins[2].cc[1] + Ins[4].cc[1];
		Ins[4].co = Outs[0]->co + Outs[0]->cc[1] + Ins[1].cc[0];
		
		Ins[0].so = Outs[0]->so + Ins[1].sc[1] + Ins[1].sc[0] + Ins[2].sc[0] + Ins[4].sc[1];
		Ins[1].so = Outs[0]->so + Ins[1].sc[1] + Ins[1].sc[0] +
			std::min({ Ins[0].sc[0] + Ins[3].sc[0] + Outs[0]->sc[1],
				Ins[0].sc[1] + Ins[3].sc[0] + Outs[0]->sc[0] });
		Ins[2].so = Outs[0]->so + Outs[0]->sc[1] + Ins[1].sc[0];
		Ins[3].so = Outs[0]->so + Ins[1].sc[1] + Ins[1].sc[0] + Ins[2].sc[1] + Ins[4].sc[1];
		Ins[4].so = Outs[0]->so + Outs[0]->sc[1] + Ins[1].sc[0];
		
	}
	else if (n->type >= SDFFSRXL && n->type <= SDFFSRX4) {
		Ins[0].co = Outs[0]->co + Ins[1].cc[1] + Ins[1].cc[0] + Ins[2].cc[0] + Ins[4].cc[1] + Ins[5].cc[1];
		Ins[1].co = Outs[0]->co + Ins[1].cc[1] + Ins[1].cc[0] +
			std::min({ Ins[0].cc[0] + Ins[3].cc[0] + Outs[0]->cc[1],
				Ins[0].cc[1] + Ins[3].cc[0] + Outs[0]->cc[0] });
		Ins[2].co = Outs[0]->co + Outs[0]->cc[1] + Ins[1].cc[0];
		Ins[3].co = Outs[0]->co + Ins[1].cc[1] + Ins[1].cc[0] + Ins[2].cc[1] + Ins[4].cc[1] + Ins[5].cc[1];
		Ins[4].co = Outs[0]->co + Outs[0]->cc[1] + Ins[1].cc[0];
		Ins[5].co = Outs[0]->co + Outs[0]->cc[1] + Ins[1].cc[0];

		Ins[0].so = Outs[0]->so + Ins[1].sc[1] + Ins[1].sc[0] + Ins[2].sc[0] + Ins[4].sc[1] + Ins[5].sc[1];
		Ins[1].so = Outs[0]->so + Ins[1].sc[1] + Ins[1].sc[0] +
			std::min({ Ins[0].sc[0] + Ins[3].sc[0] + Outs[0]->sc[1],
				Ins[0].sc[1] + Ins[3].sc[0] + Outs[0]->sc[0] });
		Ins[2].so = Outs[0]->so + Outs[0]->sc[1] + Ins[1].sc[0];
		Ins[3].so = Outs[0]->so + Ins[1].sc[1] + Ins[1].sc[0] + Ins[2].sc[1] + Ins[4].sc[1] + Ins[5].sc[1];
		Ins[4].so = Outs[0]->so + Outs[0]->sc[1] + Ins[1].sc[0];
		Ins[5].so = Outs[0]->so + Outs[0]->sc[1] + Ins[1].sc[0];

	}
	else if (n->type >= RDFFNX1 && n->type <= RDFFNX2) {
		Ins[0].co = Outs[0]->co + Ins[1].cc[1] + Ins[1].cc[0] + Ins[2].cc[1];
		Ins[1].co = Outs[0]->co + Ins[1].cc[1] + Ins[1].cc[0] +
			std::min({ Ins[0].cc[0] + Outs[0]->cc[1],
				Ins[0].cc[1] + Outs[0]->cc[0] });
		Ins[2].co = Outs[0]->co + Outs[0]->cc[1] + Ins[1].cc[0];
		Ins[0].so = Outs[0]->so + Ins[1].sc[1] + Ins[1].sc[0] + Ins[2].sc[1];
		Ins[1].so = Outs[0]->so + Ins[1].sc[1] + Ins[1].sc[0] +
			std::min({ Ins[0].sc[0] + Outs[0]->sc[1],
				Ins[0].sc[1] + Outs[0]->sc[0] });
		Ins[2].so = Outs[0]->so + Outs[0]->sc[1] + Ins[1].sc[0];


	}
	else if (n->type >= DFFXL) {
		int min_co = INF;
		int min_so = INF;
		for (int i = 0; i < n->nOut; i++) {
			if (Outs[i]->co < min_co)
				min_co = Outs[i]->co;
			if (Outs[i]->so < min_so)
				min_so = Outs[i]->so;
		}
		for (int i = 0; i < n->nIn; i++)
		{
			Ins[i].co = min_co;
			Ins[i].so = min_so;
		}
	}

	for (int i = 0; i < n->nIn; i++)
	{
		if (Ins[i].co > INF)
			Ins[i].co = INF;
		if (Ins[i].so > INF)
			Ins[i].so = INF;
		if (n->type < DFFXL)
			Ins[i].co++;
		else
			Ins[i].so++;
		if (n->In[i]->type != PRIMARY_OUT && n->In[i]->st.s.co > Ins[i].co) {
			n->In[i]->st.s.co = Ins[i].co;
			n->In[i]->st_updated = true;
		}
		if (n->In[i]->type != PRIMARY_OUT && n->In[i]->st.s.so > Ins[i].so) {
			n->In[i]->st.s.so = Ins[i].so;
			n->In[i]->st_updated = true;
		}
	}
}

void EvaluateSCOAP_CC_v2(Module *m) {
	//step1
	//in seq nodes, for output signals, NOT_INITIALIZED should change to INF
	for (int k = 0; k < m->nNode; k++)
	{
		node *n = m->p_nlist[k];
		for (int i = 0; i < n->nIn; i++)
		{
			if (1 || n->In[i]->type == CLOCK) {
				for (int j = 0; j < n->nOut; j++)
				{
					if (n->Out[j] == NULL)
						continue;
					n->Out[j]->st.s.cc[0] = INF;
					n->Out[j]->st.s.cc[1] = INF;
					n->Out[j]->st.s.sc[0] = INF;
					n->Out[j]->st.s.sc[1] = INF;
				}
				break;
			}
		}
	}
	//step2
	bool Changing = true;
	int nstep = 0;
	//the number of outputs for nodes
	int nout = 0;
	while (Changing) {
		Changing = false;
		for (int i = 0; i < m->nNode; i++) {
			node *n = m->p_nlist[i];
			if (ReadyForCC(n)) {
				if (n->hModule != NULL) {
					Module *mp = hModule_v2(n);
					nout = mp->nOut;
					EvaluateSCOAP_CC_v2(mp);
					for (int k = 0; k < mp->nOut; k++)
						if (n->Out[k] != NULL) {
							memcpy(n->Out[k]->st.s.cc, mp->p_outputs[k]->st.s.cc, sizeof(SCOAP::cc));
							memcpy(n->Out[k]->st.s.sc, mp->p_outputs[k]->st.s.sc, sizeof(SCOAP::sc));
						}
				}
				else {
					Evaluate_SCOAP_CC_v2(n);
					nout = n->nOut;
				}
				n->CC_Evaluated = true;
				
				for (int l = 0; l < nout; l++)
				{
					if (n->Out[l] == NULL)
						continue;
					//countinue WHILE something changed in circuit 
					if( n->Out[l]->st_updated) 
						Changing = true;
					//EVALUATE while there is any INF output
					if (n->Out[l]->st.s.cc[0] == INF || n->Out[l]->st.s.cc[1] == INF || n->Out[l]->st.s.sc[0] == INF || n->Out[l]->st.s.sc[1] == INF)
						n->CC_Evaluated = false;
				}
			}
		}
		nstep++;
		if (m->bVerbose)
		{
			printf("\n---------------------\nnStep = %d\n", nstep);
			for (int i = 0; i < m->nIn; i++)
				if (nstep == 1)
				{
					m->p_inputs[i]->printCC();
				}
			for (int i = 0; i < m->nWires; i++)
				if (m->p_wires[i]->st_updated)
				{
					m->p_wires[i]->printCC();
				}
			for (int i = 0; i < m->nOut; i++)
				if (m->p_outputs[i]->st_updated)
				{
					m->p_outputs[i]->printCC();
				}
		}
	}

}

bool ReadyForCC(node *n) {
	//return true;
	if (n->CC_Evaluated)
		return false;
	for (int j = 0; j < n->nIn; j++)
		if ((n->In[j]->st.s.cc[0] == NOT_INITIALIZED && n->In[j]->st.s.cc[1] == NOT_INITIALIZED) ||
			(n->In[j]->st.s.sc[0] == NOT_INITIALIZED && n->In[j]->st.s.sc[1] == NOT_INITIALIZED)) {
			return false;
		}
	return true;
}
void EvaluateSCOAP_CC(Module *m) {
	ofstream myfile;
	node * n;
	bool Module_accept_INF = false;
	bool Node_accept_INF = false;
	for (int i = 0; i < m->nIn; i++)
		if (m->p_inputs[i]->st.s.cc[0] == INF || m->p_inputs[i]->st.s.cc[1] == INF || m->p_inputs[i]->st.s.sc[0] == INF || m->p_inputs[i]->st.s.sc[1] == INF) {
			Module_accept_INF = true;
			break;
		}
	//in seq nodes, for output signals, NOT_INITIALIZED should change to INF
	for (int j = 0; j < m->nNode; j++)
	{
		n = m->p_nlist[j];
		for (int i = 0; i < n->nIn; i++)
		{
			if (1 || n->In[i]->type == CLOCK) {
				for (int j = 0; j < n->nOut; j++)
				{
					if (n->Out[j] == NULL)
						continue;
					n->Out[j]->st.s.cc[0] = INF;
					n->Out[j]->st.s.cc[1] = INF;
					n->Out[j]->st.s.sc[0] = INF;
					n->Out[j]->st.s.sc[1] = INF;
				}
				break;
			}
		}
	}

	int NOT_Evaluated_Nodes = m->nNode;
	int Evaluated_Nodes = 0;
	int c = 0;
	while (NOT_Evaluated_Nodes > 0)
	{
		if (c == 20)
			break;
		c++;
		if (Evaluated_Nodes == 0 && NOT_Evaluated_Nodes != m->nNode)
			break;
		Evaluated_Nodes = 0;
		for (int i = 0; i < m->nNode; i++)
		{
			n = m->p_nlist[i];
			Node_accept_INF = true;
			if (!n->CC_Evaluated) {
				bool inputs_ready = true;
				//checking if the inputs are ready
				for (int j = 0; j < n->nIn; j++)
					if ((n->In[j]->st.s.cc[0] == NOT_INITIALIZED && n->In[j]->st.s.cc[1] == NOT_INITIALIZED)||
						(n->In[j]->st.s.sc[0] == NOT_INITIALIZED && n->In[j]->st.s.sc[1] == NOT_INITIALIZED) ){
						inputs_ready = false;
						break;
					}
				if (inputs_ready) {
					for (int j = 0; j < n->nIn; j++)
					for (int i = 0; i < n->nIn; i++)
						if (n->In[i]->type != PRIMARY_IN && !n->In[i]->st_updated) {
							Node_accept_INF = false;
							break;
						}
					n->CC_Evaluated = true;
					//user defined types
					if (n->hModule != NULL) {
						Module *mp = hModule(n);
						EvaluateSCOAP_CC(mp);
						for (int k = 0; k < mp->nOut; k++)
							if (n->Out[k] != NULL) {
								memcpy(n->Out[k]->st.s.cc, mp->p_outputs[k]->st.s.cc, sizeof(SCOAP::cc));
								memcpy(n->Out[k]->st.s.sc, mp->p_outputs[k]->st.s.sc, sizeof(SCOAP::sc));
							}
					}
					else
						Evaluate_SCOAP_CC(n);
				}
				if (n->CC_Evaluated) {
					NOT_Evaluated_Nodes--;
					Evaluated_Nodes++;
					if (!Module_accept_INF && !Node_accept_INF)
						for (int j = 0; j < n->nOut; j++)
							if (n->Out[j] != NULL)
								if (n->Out[j]->st.s.cc[0] == INF || n->Out[j]->st.s.cc[1] == INF ||
									n->Out[j]->st.s.sc[0] == INF || n->Out[j]->st.s.sc[1] == INF)
								{

									n->CC_Evaluated = false;
									NOT_Evaluated_Nodes++;
									Evaluated_Nodes--;
									break;
								}
				}
				if (n->CC_Evaluated) {
					for (int j = 0; j < n->nOut; j++)
						n->Out[j]->st_updated = true;
				}
			}
		}
	}
	for (int nodes = 0; nodes < m->nNode; nodes++)
		m->p_nlist[nodes]->CC_Evaluated = false;
}

void EvaluateSCOAP_CO_nonRec(Module *m) {
	for (int mi = 0; mi < m->nNode; mi++)
	{
		m->p_nlist[mi]->CC_Evaluated = false;
	}
	for (int i = 0; i < m->nOut; i++)
	{
		signal* s = m->p_outputs[i];
		node* g = s->generator;
		if (g != NULL) {
			g->ReadyForCO = true;
		}
	}
	bool Changing = true;
	int nstep = 0;
	while (Changing)
	{
		Changing = false;
		for (int i = 0; i < m->nNode; i++)
		{
			node* n = m->p_nlist[i];
			if (n->ReadyForCO) {
				if (n->hModule != NULL) {
					Module *mp = hModule_v2(n);
					EvaluateSCOAP_CC_v2(mp);
					EvaluateSCOAP_CO_nonRec(mp);
					for (int k = 0; k < mp->nIn; k++) {
						n->In[k]->st.s.co = mp->p_inputs[k]->st.s.co;
						n->In[k]->st.s.so = mp->p_inputs[k]->st.s.so;
						n->In[k]->st_updated = mp->p_inputs[k]->st_updated;
					}
				}
				else {
					Evaluate_SCOAP_CO(n);
				}
				for (int j = 0; j < n->nIn; j++)
				{
					signal* s_in = n->In[j];
					if (s_in->st_updated) {
						if (s_in->generator != NULL) {
							s_in->generator->ReadyForCO = true;
							Changing = true;
						}
					}
				}
				n->ReadyForCO = false;
			}
		}
		if (m->bVerbose)
		{
			printf("\n---------------------\nnStep = %d\n", nstep++);
			for (int i = 0; i < m->nOut; i++)
				if (nstep == 1)
				{
					m->p_outputs[i]->printCO();
				}
			for (int i = 0; i < m->nWires; i++)
				if (m->p_wires[i]->st_updated)
				{
					m->p_wires[i]->printCO();
				}
			for (int i = 0; i < m->nIn; i++)
				if (m->p_inputs[i]->st_updated)
				{
					m->p_inputs[i]->printCO();
				}
		}
	}
}

void EvaluateSCOAP_CO(Module *m) {
	for (int i = 0; i < m->nIn; i++) {
		m->p_inputs[i]->st_updated = false;
	}
	int *tmp_CO = new int[m->nIn];// [MAX_OUTPUT];
	for (int i = 0; i < m->nIn; i++) {
		tmp_CO[i] = m->p_inputs[i]->st.s.co;
	}
	for (int i = 0; i < m->nOut; i++)
	{
		Update_CO(m->p_outputs[i]);
	}
	for (int i = 0; i < m->nIn; i++)
		if (tmp_CO[i] != m->p_inputs[i]->st.s.co) {
			m->p_inputs[i]->st_updated = true;
		}
}

void Update_CO(signal *s) {
	node *g = s->generator;
	int genIn[100] = { 0 };
	if (g == NULL) {
		return;
	}
	if (g->hModule != NULL) {
		Module *mp = hModule_v2(g);
		EvaluateSCOAP_CC_v2(mp);
		EvaluateSCOAP_CO(mp);
		for (int k = 0; k < mp->nIn; k++) {
			g->In[k]->st.s.co = mp->p_inputs[k]->st.s.co;
			g->In[k]->st.s.so = mp->p_inputs[k]->st.s.so;
		}
	}
	else {
		Evaluate_SCOAP_CO(g);
	}
	for (int k = 0; k < g->nIn; k++)
	{
		if (g->In[k]->st_updated)
			genIn[k] = 1;
	}
	for (int i = 0; i < g->nIn; i++)
	{
		if (genIn[i] == 1) {
			Update_CO(g->In[i]);
		}
	}
}

Module * hModule_v2(node *n) {
	Module *mp = (Module*)n->hModule;
	//if (strcmp(mp->name, "aes_128") == 0)
	//{ }
	for (int k = 0; k < mp->nNode; k++)
		mp->p_nlist[k]->init();
	for (int k = 0; k < mp->nIn; k++) {
		memcpy(mp->p_inputs[k]->st.s.cc, n->In[k]->st.s.cc, sizeof(SCOAP::cc));
		memcpy(mp->p_inputs[k]->st.s.sc, n->In[k]->st.s.sc, sizeof(SCOAP::sc));
	}
	for (int k = 0; k < n->nOut; k++) {
		if (n->Out[k] != NULL) {
		mp->p_outputs[k]->st.s.co = n->Out[k]->st.s.co;
		mp->p_outputs[k]->st.s.so = n->Out[k]->st.s.so;
		}
	}
	return mp;
}

Module * hModule(node *n) {
	Module *mp = (Module*)n->hModule;
	
	for (int k = 0; k < mp->nNode; k++)
	{
		mp->p_nlist[k]->CC_Evaluated = false;
		for (int g = 0; g < mp->p_nlist[k]->nIn; g++) {
			if (strcmp(mp->p_nlist[k]->In[g]->name, "GND") || strcmp(mp->p_nlist[k]->In[g]->name, "VCC"))
				continue;
			mp->p_nlist[k]->In[g]->st.s.cc[0] = NOT_INITIALIZED;
			mp->p_nlist[k]->In[g]->st.s.cc[1] = NOT_INITIALIZED;
			mp->p_nlist[k]->In[g]->st.s.co = INF;
			mp->p_nlist[k]->In[g]->st.s.sc[0] = NOT_INITIALIZED;
			mp->p_nlist[k]->In[g]->st.s.sc[1] = NOT_INITIALIZED;
			mp->p_nlist[k]->In[g]->st.s.so = INF;
			mp->p_nlist[k]->In[g]->st_updated = false;
		}
		for (int g = 0; g < mp->p_nlist[k]->nOut; g++) {
			if (strcmp(mp->p_nlist[k]->Out[g]->name, "GND") || strcmp(mp->p_nlist[k]->Out[g]->name, "VCC"))
				continue;
			mp->p_nlist[k]->Out[g]->st.s.cc[0] = NOT_INITIALIZED;
			mp->p_nlist[k]->Out[g]->st.s.cc[1] = NOT_INITIALIZED;
			mp->p_nlist[k]->Out[g]->st.s.co = INF;
			mp->p_nlist[k]->Out[g]->st.s.sc[0] = NOT_INITIALIZED;
			mp->p_nlist[k]->Out[g]->st.s.sc[1] = NOT_INITIALIZED;
			mp->p_nlist[k]->Out[g]->st.s.so = INF;
			mp->p_nlist[k]->Out[g]->st_updated = false;
		}
	}
	for (int k = 0; k < mp->nIn; k++) {
		memcpy(mp->p_inputs[k]->st.s.cc, n->In[k]->st.s.cc, sizeof(SCOAP::cc));
		memcpy(mp->p_inputs[k]->st.s.sc, n->In[k]->st.s.sc, sizeof(SCOAP::sc));
	}
	for (int k = 0; k < n->nOut; k++) {
		mp->p_outputs[k]->st.s.co = n->Out[k]->st.s.co;
		mp->p_outputs[k]->st.s.so = n->Out[k]->st.s.so;
	}
	return mp;
}

void EvaluateSCOAP(Module *m) {
	node *n;
	signal *s;
	SCOAP *scoap;
	for (int i = 0; i < m->nIn; i++)
	{
		scoap = & m->p_inputs[i]->st.s;
		scoap->total_cc = sqrt(sqr(scoap->cc[0]) + sqr(scoap->cc[1]));
		scoap->total_sc = sqrt(sqr(scoap->sc[0]) + sqr(scoap->sc[1]));
	}
	for (int i = 0; i < m->nOut; i++)
	{
		scoap =& m->p_outputs[i]->st.s;
		scoap->total_cc = sqrt(sqr(scoap->cc[0]) + sqr(scoap->cc[1]));
		scoap->total_sc = sqrt(sqr(scoap->sc[0]) + sqr(scoap->sc[1]));
	}
	for (int i = 0; i < m->nWires; i++)
	{
		scoap =& m->p_wires[i]->st.s;
		scoap->total_cc = sqrt(sqr(scoap->cc[0]) + sqr(scoap->cc[1]));
		scoap->total_sc = sqrt(sqr(scoap->sc[0]) + sqr(scoap->sc[1]));
	}
}

void PrintSCOAP(char *name, void *hModules[], int nModule) {
	double max_cc = 0;
	int max_co = 0;
	ofstream myfile;
	myfile.open(name);
	ofstream matlab;
	matlab.open("matlabData.txt");
	myfile << ",CC[0],CC[1],CC,CO,SC0,SC1,SC,SO\n";
	int nNode = 0;
		Module *m = (Module *)hModules[nModule - 1];
		matlab << std::fixed << std::setprecision(4) << m->nNode << "," << m->nSeqNode << "\n";
		nNode += m->nNode;
	for (int i = 0; i < m->nIn; i++)
	{
		if (m->p_inputs[i]->st.s.total_cc == 1.4142135623730951 && m->p_inputs[i]->st.s.co == INF)
			continue;
		if (m->p_inputs[i]->st.s.total_cc > max_cc)
			max_cc = m->p_inputs[i]->st.s.total_cc;
		if (m->p_inputs[i]->st.s.co > max_co)
			max_co = m->p_inputs[i]->st.s.co;

		myfile << m->p_inputs[i]->name << ",";
		myfile << m->p_inputs[i]->st.s.cc[0] << ",";
		myfile << m->p_inputs[i]->st.s.cc[1] << ",";
		myfile << std::fixed << std::setprecision(4) << m->p_inputs[i]->st.s.total_cc << ",";
		myfile << m->p_inputs[i]->st.s.co << ",";
		myfile << m->p_inputs[i]->st.s.sc[0] << ",";
		myfile << m->p_inputs[i]->st.s.sc[1] << ",";
		myfile << std::fixed << std::setprecision(4) << m->p_inputs[i]->st.s.total_sc << ",";
		myfile << m->p_inputs[i]->st.s.so << "\n";

		matlab << std::fixed << std::setprecision(4) << m->p_inputs[i]->st.s.total_cc << ",";
		matlab << m->p_inputs[i]->st.s.co << ",";
		matlab << std::fixed << std::setprecision(4) << m->p_inputs[i]->st.s.total_sc << ",";
		matlab << m->p_inputs[i]->st.s.so << "\n";
	}
	for (int i = 0; i < m->nOut; i++)
	{
		if (m->p_outputs[i]->st.s.total_cc == 1.4142135623730951 && m->p_outputs[i]->st.s.co == INF)
			continue;
		if (m->p_outputs[i]->st.s.total_cc > max_cc)
			max_cc = m->p_outputs[i]->st.s.total_cc;
		if (m->p_outputs[i]->st.s.co > max_co)
			max_co = m->p_outputs[i]->st.s.co;

		myfile << m->p_outputs[i]->name << ",";
		myfile << m->p_outputs[i]->st.s.cc[0] << ",";
		myfile << m->p_outputs[i]->st.s.cc[1] << ",";
		myfile << std::fixed << std::setprecision(4) << m->p_outputs[i]->st.s.total_cc << ",";
		myfile << m->p_outputs[i]->st.s.co << ",";
		myfile << m->p_outputs[i]->st.s.sc[0] << ",";
		myfile << m->p_outputs[i]->st.s.sc[1] << ",";
		myfile << std::fixed << std::setprecision(4) << m->p_outputs[i]->st.s.total_sc << ",";
		myfile << m->p_outputs[i]->st.s.so << "\n";

		matlab << std::fixed << std::setprecision(4) << m->p_outputs[i]->st.s.total_cc << ",";
		matlab << m->p_outputs[i]->st.s.co << ",";
		matlab << std::fixed << std::setprecision(4) << m->p_outputs[i]->st.s.total_sc << ",";
		matlab << m->p_outputs[i]->st.s.so << "\n";
	}
	for (int i = 0; i < m->nWires; i++)
	{
		if (m->p_wires[i]->st.s.total_cc == 1.4142135623730951	&& m->p_wires[i]->st.s.co == INF)
			continue;
		if (m->p_wires[i]->st.s.total_cc > max_cc)
			max_cc = m->p_wires[i]->st.s.total_cc;
		if (m->p_wires[i]->st.s.co > max_co)
			max_co = m->p_wires[i]->st.s.co;
		myfile << m->p_wires[i]->name << ",";
		myfile << m->p_wires[i]->st.s.cc[0] << ",";
		myfile << m->p_wires[i]->st.s.cc[1] << ",";
		myfile << std::fixed << std::setprecision(4) << m->p_wires[i]->st.s.total_cc << ",";
		myfile << m->p_wires[i]->st.s.co << ",";
		myfile << m->p_wires[i]->st.s.sc[0] << ",";
		myfile << m->p_wires[i]->st.s.sc[1] << ",";
		myfile << std::fixed << std::setprecision(4) << m->p_wires[i]->st.s.total_sc << ",";
		myfile << m->p_wires[i]->st.s.so << "\n";

		matlab << std::fixed << std::setprecision(4) << m->p_wires[i]->st.s.total_cc << ",";
		matlab << m->p_wires[i]->st.s.co << ",";
		matlab << std::fixed << std::setprecision(4) << m->p_wires[i]->st.s.total_sc << ",";
		matlab << m->p_wires[i]->st.s.so << "\n";
	}
	myfile.close();
}

void Evaluate_SCOAP(struct parse_state *state) {
	time_t seconds;

		Module *m = (Module *)state->hModules[state->nModule - 1];
		m->bVerbose = (state->bVerbose == 1);
		printf("hmodule: %s\nBegining of CC Evaluation ...", m->name);
		EvaluateSCOAP_CC_v2(m);
		printf("End of CC Evaluation ...\nBegining of CO Evaluation ...");
		EvaluateSCOAP_CO_nonRec(m);
		printf("End of CO Evaluation\n");
		//EvaluateSCOAP_CO(m);
//// this block is used for time calculationg, if uncomment, also uncomment defines and pragma at the first of the code
//		LARGE_INTEGER  tBegin, tEnd, tFreq;
//		
//		QueryPerformanceCounter(&tBegin);
//		QueryPerformanceFrequency(&tFreq);
//		// your process
//		EvaluateSCOAP_CC_v2(m);
//		QueryPerformanceCounter(&tEnd);
//		__int64 tDiff = (tEnd.QuadPart - tBegin.QuadPart) / tFreq.QuadPart;
//		float  tDiff1 = ((float)(tEnd.QuadPart - tBegin.QuadPart) / (float)tFreq.QuadPart);
//		printf(" ==> Time of CC evaluation: %0.6f <==\n", tDiff1);
//
//		QueryPerformanceCounter(&tBegin);
//		// your process
//		EvaluateSCOAP_CO_nonRec(m);
//		QueryPerformanceCounter(&tEnd);
//		tDiff1 = ((float)(tEnd.QuadPart - tBegin.QuadPart) / (float)tFreq.QuadPart);
//		printf(" ==> Time of CO evaluation: %0.6f <==\n", tDiff1);
		
		//TO DO evaluate scoao just for last module or ...
	for (int iM = 0; iM < state->nModule; iM++)
	{
		Module *m = (Module *)state->hModules[iM];
		m->bVerbose = (state->bVerbose == 1);
		EvaluateSCOAP(m);
	}
}
void print_SCOAP(struct parse_state *state) {
	/*for (int iM = 0; iM < state->nModule; iM++)
	{
		Module *m = (Module *)state->hModules[iM];*/
		PrintSCOAP("sout.v", state->hModules, state->nModule);
		PrintSCOAP("sout.csv", state->hModules, state->nModule);
		//PrintSCOAP("sout.csv", m);
//	}
}

extern "C" {
	void eval_scoap(struct parse_state *state)
	{
		Evaluate_SCOAP(state);
		printf("eval_scoap\n");
	}
	void print_scoap(struct parse_state *state)
	{
		print_SCOAP(state);
		printf("print_scoap\n");
	}
}


SCOAP cc_NOT(SCOAP in1) {
	SCOAP out;
	int t_cc0 = in1.cc[1] + 0;
	int t_cc1 = in1.cc[0] + 0;
	out.cc[0] = t_cc0;
	out.cc[1] = t_cc1;
	t_cc0 = in1.sc[1] + 0;
	t_cc1 = in1.sc[0] + 0;
	out.sc[0] = t_cc0;
	out.sc[1] = t_cc1;
	return out;
}
SCOAP cc_NAND2(SCOAP in1, SCOAP in2) {
	SCOAP out;
	int t_cc0 = in1.cc[1] + in2.cc[1] + 0;
	int t_cc1 = std::min(in1.cc[0], in2.cc[0]) + 0;
	out.cc[0] = t_cc0;
	out.cc[1] = t_cc1;
	int t_sc0 = in1.sc[1] + in2.sc[1] + 0;
	int t_sc1 = std::min(in1.sc[0], in2.sc[0]) + 0;
	out.sc[0] = t_sc0;
	out.sc[1] = t_sc1;
	return out;
}
SCOAP cc_NOR2(SCOAP in1, SCOAP in2) {
	SCOAP out;
	int t_cc0 = std::min(in1.cc[1], in2.cc[1]) + 0;
	int t_cc1 = in1.cc[0] + in2.cc[0] + 0;
	out.cc[0] = t_cc0;
	out.cc[1] = t_cc1;
	int t_sc0 = std::min(in1.sc[1], in2.sc[1]) + 0;
	int t_sc1 = in1.sc[0] + in2.sc[0] + 0;
	out.sc[0] = t_sc0;
	out.sc[1] = t_sc1;
	return out;
}
SCOAP cc_XOR2(SCOAP in1, SCOAP in2) {
	SCOAP out;
	int t_cc0 = std::min(in1.cc[0] + in2.cc[0], in1.cc[1] + in2.cc[1]) + 0;
	int t_cc1 = std::min(in1.cc[1] + in2.cc[0], in1.cc[0] + in2.cc[1]) + 0;
	out.cc[0] = t_cc0;
	out.cc[1] = t_cc1;
	int t_sc0 = std::min(in1.sc[0] + in2.sc[0], in1.sc[1] + in2.sc[1]) + 0;
	int t_sc1 = std::min(in1.sc[1] + in2.sc[0], in1.sc[0] + in2.sc[1]) + 0;
	out.sc[0] = t_sc0;
	out.sc[1] = t_sc1;
	return out;
}
SCOAP cc_NAND3(SCOAP in1, SCOAP in2, SCOAP in3) {
	SCOAP out;
	int t_cc0 = in1.cc[1] + in2.cc[1] + in3.cc[1] + 0;
	int t_cc1 = std::min({ in1.cc[0], in2.cc[0], in3.cc[0] }) + 0;
	out.cc[0] = t_cc0;
	out.cc[1] = t_cc1;
	int t_sc0 = in1.sc[1] + in2.sc[1] + in3.sc[1] + 0;
	int t_sc1 = std::min({ in1.sc[0], in2.sc[0], in3.sc[0] }) + 0;
	out.sc[0] = t_sc0;
	out.sc[1] = t_sc1;
	return out;
}
SCOAP cc_NOR3(SCOAP in1, SCOAP in2, SCOAP in3) {
	SCOAP out;
	int t_cc0 = std::min({ in1.cc[1], in2.cc[1], in3.cc[1] }) + 0;
	int t_cc1 = in1.cc[0] + in2.cc[0] + in3.cc[0] + 0;
	out.cc[0] = t_cc0;
	out.cc[1] = t_cc1;
	int t_sc0 = std::min({ in1.sc[1], in2.sc[1], in3.sc[1] }) + 0;
	int t_sc1 = in1.sc[0] + in2.sc[0] + in3.sc[0] + 0;
	out.sc[0] = t_sc0;
	out.sc[1] = t_sc1;
	return out;
}
SCOAP cc_AND2(SCOAP in1, SCOAP in2) {
	return cc_NOT(cc_NAND2(in1, in2));
}
SCOAP cc_AND3(SCOAP in1, SCOAP in2, SCOAP in3) {
	return cc_NOT(cc_NAND3(in1, in2, in3));
}
SCOAP cc_OR2(SCOAP in1, SCOAP in2) {
	return cc_NOT(cc_NOR2(in1, in2));
}
SCOAP cc_OR3(SCOAP in1, SCOAP in2, SCOAP in3) {
	return cc_NOT(cc_NOR3(in1, in2, in3));
}
SCOAP cc_XOR3(SCOAP in1, SCOAP in2, SCOAP in3) {
	SCOAP x = cc_XOR2(in1, in2);
	return cc_XOR2(x, in3);
}

SCOAP co_NOT(SCOAP out) {
	SCOAP res;
	int res_co = out.co + 0;
	res.co = res_co;
	int res_so = out.so + 0;
	res.so = res_so;
	return res;
}
SCOAP co_NAND2(SCOAP out, SCOAP in1) {
	SCOAP res;
	int res_co = out.co + in1.cc[1] + 0;
	res.co = res_co;
	int res_so = out.so + in1.sc[1] + 0;
	res.so = res_so;
	return res;
}
SCOAP co_NOR2(SCOAP out, SCOAP in1) {
	SCOAP res;
	int res_co = out.co + in1.cc[0] + 0;
	res.co = res_co;
	int res_so = out.so + in1.sc[0] + 0;
	res.so = res_so;
	return res;
}
SCOAP co_XNOR2(SCOAP out, SCOAP in1) {
	SCOAP res;
	int res_co = out.co + std::min(in1.cc[0], in1.cc[1]) + 0;
	res.co = res_co;
	int res_so = out.so + std::min(in1.sc[0], in1.sc[1]) + 0;
	res.so = res_so;
	return res;
}
SCOAP co_NAND3(SCOAP out, SCOAP in1, SCOAP in2) {
	SCOAP res;
	int res_co = out.co + in1.cc[1] + in2.cc[1] + 0;
	res.co = res_co;
	int res_so = out.so + in1.sc[1] + in2.sc[1] + 0;
	res.so = res_so;
	return res;
}
SCOAP co_NOR3(SCOAP out, SCOAP in1, SCOAP in2) {
	SCOAP res;
	int res_co = out.co + in1.cc[0] + in2.cc[0] + 0;
	res.co = res_co;
	int res_so = out.so + in1.sc[0] + in2.sc[0] + 0;
	res.so = res_so;
	return res;
}
SCOAP co_XNOR3(SCOAP out, SCOAP in1, SCOAP in2, SCOAP in3, int select) {
	SCOAP x = cc_XOR2(in1, in2);
	SCOAP y = cc_NOT(cc_XOR2(in1, in2));
	SCOAP z = cc_AND2(x, cc_NOT(in3));
	SCOAP t = cc_AND2(y, in3);
	z.co = co_NOR2(out, t).co;
	t.co = co_NOR2(out, z).co;
	x.co = co_NAND2(z, cc_NOT(in3)).co;
	y.co = co_NAND2(t, in3).co;

	z.so = co_NOR2(out, t).so;
	t.so = co_NOR2(out, z).so;
	x.so = co_NAND2(z, cc_NOT(in3)).so;
	y.so = co_NAND2(t, in3).so;

	SCOAP res;
	switch (select)
	{
	case 1:
		res.co = std::min(co_XNOR2(x, in2).co, co_XNOR2(y, in2).co);
		res.so = std::min(co_XNOR2(x, in2).so, co_XNOR2(y, in2).so);
		break;
	case 2:
		res.co = std::min(co_XNOR2(x, in1).co, co_XNOR2(y, in1).co);
		res.co = std::min(co_XNOR2(x, in1).so, co_XNOR2(y, in1).so);
		break;
	case 3:
		res.co = std::min({ co_NOT(co_NAND2(z,x)).co, co_NAND2(t, y).co });
		res.co = std::min({ co_NOT(co_NAND2(z,x)).so, co_NAND2(t, y).so });
		break;
	default:
		printf("ERROR: select of XNOR3 is not correctly defined");
		break;
	}
	return res;
}
SCOAP co_XNOR3_v2(SCOAP out, SCOAP in1, SCOAP in2) {
	SCOAP res;
	int res_co = out.co + std::min(in1.cc[0], in1.cc[1]) + std::min(in2.cc[0], in2.cc[1]) + 0;
	res.co = res_co;
	int res_so = out.so + std::min(in1.sc[0], in1.sc[1]) + std::min(in2.sc[0], in2.sc[1]) + 0;
	res.so = res_so;
	return res;
}
SCOAP co_AND2(SCOAP out, SCOAP in1) {
	return(co_NAND2(out, in1));
}
SCOAP co_OR2(SCOAP out, SCOAP in1) {
	return co_NOR2(out, in1);
}
SCOAP co_AND3(SCOAP out, SCOAP in1, SCOAP in2) {
	return co_NAND3(out, in1, in2);
}
SCOAP co_OR3(SCOAP out, SCOAP in1, SCOAP in2) {
	return co_NOR3(out, in1, in2);
}