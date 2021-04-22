#include "scoap.h"
//#include "stdafx.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>    // std::min
#include <vector>
#include "Evaluate_SCOAP.h"
using namespace std;
//#define typeSTD180 180;
//#define typeSTD90 90;

//these codes creates modules in C from verilog file,

//search in eInfo[] and return the correct EnrirtyInfo
int diff(struct vector_range r) 
{ 
	return (r.begin - r.end); 
}

nodePort Module::findPortInfo(char *port_name) {
	nodePort res;
	res.found = res.is_vector = false;
	strcpy(res.name, port_name);
	for (int i = 0; i < nIn; i++)
		if (strcmp(p_inputs[i]->name, port_name) == 0)
		{
			res.index = i;
			res.type = p_inputs[i]->type;
			res.found = true;
			return res;
		}
	for (int i = 0; i < nOut; i++)
		if (strcmp(p_outputs[i]->name, port_name) == 0)
		{
			res.index = i;
			res.type = p_outputs[i]->type;
			res.found = true;
			return res;
		}
	for (int i = 0; i < nWires; i++)
		if (strcmp(p_wires[i]->name, port_name) == 0)
		{
			res.index = i;
			res.type = p_wires[i]->type;
			res.found = true;
			return res;
		}
	// second try : finding vector
	char port_Brace[100];
	sprintf(port_Brace, "%s[", port_name);
	for (int i = 0; i < nIn; i++)
		if (strstr(p_inputs[i]->name, port_Brace) != NULL)
		{
			res.index = i;
			res.type = PRIMARY_IN;
			if (res.found == false)
			{
				res.r.begin = i;
				res.r2.begin = p_inputs[i]->vec_index;
			}
			res.found = true;
			res.r2.end = p_inputs[i]->vec_index;
			res.r.end = i;
			res.is_vector = true;
		}
	if (res.found)
		return res;
	for (int i = 0; i < nOut; i++)
		if (strstr(p_outputs[i]->name, port_Brace) != NULL)
		{
			res.index = i;
			res.type = PRIMARY_OUT;
			if (res.found == false)
			{
				res.r.begin = i;
				res.r2.begin = p_outputs[i]->vec_index;
			}
			res.found = true;
			res.r.end = i;
			res.r2.end = p_outputs[i]->vec_index;
			res.is_vector = true;
		}
	if (res.found)
		return res;
	for (int i = 0; i < nWires; i++)
		if (strstr(p_wires[i]->name, port_Brace) != NULL)
		{
			res.index = i;
			res.type = INTERMEDIATE;
			if (res.found == false)
			{
				res.r.begin = i;
				res.r2.begin = p_wires[i]->vec_index;
			}
			res.found = true;
			res.r.end = i;
			res.r2.end = p_wires[i]->vec_index;
			res.is_vector = true;
		}
	return res;
}
nodePort node::findPortInfo(char *port_name)
{
	nodePort res;
	res.found = res.is_vector = false;
	strcpy(res.name, port_name);
	if (hModule != NULL)
	{
		Module *m = (Module*)hModule;
		return m->findPortInfo(port_name);
	}
	StdCellInfo *pEInfo = FindStdCellInfo(type, stdType);
	if (pEInfo == NULL)
		return res;
	portInfo* pInfo = pEInfo->findPortInfo(port_name);
	if (pInfo == NULL)
		return res;
	res.type = pInfo->type;
	res.index = pEInfo->findPortIndex(port_name);
	res.found = true;
	return res;
}
char* node::findPortName(bool bIn, int index)
{
	if (hModule == NULL)
	{
		StdCellInfo *pEInfo = FindStdCellInfo(type, stdType);
		if (pEInfo == NULL)
			return NULL;
		if (bIn)
			return pEInfo->in_ports[index].name;
		else
			return pEInfo->out_ports[index].name;
	}
	else
	{
		Module *m = (Module*)hModule;
		if (bIn)
			return m->p_inputs[index]->name;
		else
			return m->p_outputs[index]->name;

	}
	return NULL;
}

char* node::findCellName()
{
	if (hModule)
		return ((Module*)hModule)->name;
	else
	{
		StdCellInfo *pInfo = FindStdCellInfo(type, stdType);
		for (int j = 0; pInfo->cells[j].e < __MAX_ENTITY; j++)
		{
			if (pInfo->cells[j].e == type)
				return pInfo->cells[j].name;
		}
	}
	return NULL;
}
signal * findSignal(vector<signal *> sig, int nCount, const char *sig_name)
{
	for (signal* res : sig) {
		if (strcmp(res->name, sig_name) == 0)
			return res;
	}
	return NULL;
}
int findSignalIndex(signal *sig[], int nCount, const char *sig_name)
{
	for (int i = 0; i < nCount; i++) {
		if (strcmp(sig[i]->name, sig_name) == 0)
			return i;
	}
	return -1;
}

int PrintModules(char *name, struct parse_state *state)
{
	ofstream myfile;
	myfile.open(name);

	for (int iM = 0; iM < state->nModule; iM++)
	{
		Module *m = (Module *)state->hModules[iM];
		myfile << "\n// module " << iM << "\n";
		myfile << "nIn = " << m->nIn << " nOut = " << m->nOut << " nNode = " << m->nNode << " nWire = " << m->nWires << "\n";
		myfile << "module " << m->name << "\n";
		myfile << "input ";
		for (int i = 0; i < m->nIn; i++)
		{
			myfile << m->p_inputs[i]->name << ", ";
			if (i % 10 == 9)
				myfile << "\n";
		}
		myfile << ";\n";
		myfile << "output ";
		for (int i = 0; i < m->nOut; i++)
		{
			myfile << m->p_outputs[i]->name << ", ";
			if (i % 10 == 9)
				myfile << "\n";
		}
		myfile << ";\nwire ";
		for (int i = 0; i < m->nWires; i++)
		{
			myfile << m->p_wires[i]->name << ", ";
			if (i % 10 == 9)
				myfile << "\n";
		}
		myfile << ";\n";
		for (int i = 0; i < m->nNode; i++)
		{
			char *cellName = m->p_nlist[i]->findCellName();
			if (cellName)
				myfile << i << ". " << cellName << " " << m->p_nlist[i]->name << " ( ";
			else
				continue;
			for (int j = 0; j < m->p_nlist[i]->nIn; j++)
			{
				char *portname = m->p_nlist[i]->findPortName(true, j);
				if (m->p_nlist[i]->In[j] && portname)
					myfile << " ." << portname << "(" << (m->p_nlist[i]->In[j])->name << "), ";
				else
					myfile << ".No Info for in-Port " << j;
			}
			for (int j = 0; j < m->p_nlist[i]->nOut; j++)
			{
				char *portname = m->p_nlist[i]->findPortName(false, j);
				if (m->p_nlist[i]->Out[j] && portname)
					myfile << " ." << portname << "(" << m->p_nlist[i]->Out[j]->name << "), ";
				else
					myfile << ".No Info for out-Port " << j;
			}
			myfile << ") ; \n ";
		}
		myfile << "\n endmodule;";
	}
	myfile.close();
	return 0;
}
void* CreateModule(char *name)
{
	Module* pM;
	pM = new Module(name);
	printf("creating module %s\n.", name);
	return pM;
}
int AddSingleSignal(struct parse_state *state, char *name, int idx = 0)
{
	Module* pM = (Module*)state->h_module;
	if (state->sig_type.type == ST_IN)
	{
		pM->addInput(name, idx);
		return 0;
	}
	if (state->sig_type.type == ST_OUT)
	{
		pM->addOutput(name, idx);
		return 0;
	}
	if (state->sig_type.type == ST_WIRE)
	{
		pM->addWire(name, idx);
		return 0;
	}
	return 1;
}
int AddSignal(struct parse_state *st, char *name)
{
	Module* pM = (Module*)st->h_module;
	if (st->sig_type.is_vector == 0)
		return AddSingleSignal(st, name);
	int bidx = st->sig_type.range.begin;
	int eidx = st->sig_type.range.end;
	int step = (bidx < eidx) ? 1 : -1;
	for (int i = bidx; ; i += step)
	{
		char temp[100];
		sprintf(temp, "%s[%d]", name, i);
		AddSingleSignal(st, temp, i);
		if (i == eidx)
			break;
	}
	return 0;
}
int AddVectorSignal(struct parse_state *state, char* name, struct vector_range r)
{
	Module* pM = (Module*)state->h_module;
	char temp[100];
	for (int i = r.begin; i <= r.end; i++)
	{
		sprintf(temp, "%s[%d]", name, i);
		AddSignal(state, temp);
	}
	return 0;
}
void * CreateNode(struct parse_state *state, char* type, char* name)
{
	Module* pM = (Module*)state->h_module;
	EType stdType = __MAX_ENTITY;
	void *hModule = NULL;
	int g = 0;
	if (strcmp(type, "expand_key_128_1") == 0 && strcmp(name, "a10") == 0)
	{
		g = 1;
	}
	StdCellInfo* p = FindStdCellInfo(type, state->typeSTD);
	if (p != NULL)
	{
		for (int j = 0; p->cells[j].e < __MAX_ENTITY; j++)
		{
			if (strcmp(p->cells[j].name, type) == 0)
				stdType = p->cells[j].e;
		}
	}
	else
	{
		// seach for user defined module
		for (int iM = 0; iM < state->nModule; iM++)
		{
			Module *m = (Module *)state->hModules[iM];
			if (strcmp(m->name, type) == 0)
			{
				hModule = state->hModules[iM];
			}
		}
	}
	if (hModule == NULL && stdType == __MAX_ENTITY)
	{
		printf("Error in module %s :  %s is not found neither in stdCell or userDefined modules.\n", pM->name, type);
		exit(1);
		return NULL;
	}
	node *pN = new node(name, stdType, hModule);
	pN->stdType = state->typeSTD;
	pM->addNode(pN);
	return pN;
}
int BindPort(parse_state *state, Module *pM, node *pNode, nodePort port)
{
	if (port.is_vector  && diff(port.r) !=0 && state->sig_ref.is_vector == false)
	{
		printf("type mismatch in %s: %s and %s\n", pM->name, port.name, state->sig_ref.name);
		exit(1);
	}
	char tname[100];
	strcpy_s(tname, state->sig_ref.name);
	if (state->sig_ref.is_vector == 1)
	{
		int range1 = diff(state->sig_ref.range);
		int range2 = diff(port.r);
		if (abs(range1) != abs(range2))
		{
			printf("range mismatch in %s: %s and %s\n", pM->name, port.name, tname);
			exit(1);
		}
		int step1 = (range1 <= 0) ? 1 : -1;
		int step2 = (range2 <= 0) ? 1 : -1;
		if (range1 != 0)
			port.index = port.r.begin;
		for (int i = state->sig_ref.range.begin; ; i += step1)
		{
			sprintf(tname, "%s[%d]", state->sig_ref.name, i);
			signal *sig = pM->find_signal(tname);
			if (sig == NULL)
			{
				printf("Error in module %s : %s is not found in current module as input, wire or output.\n", pM->name, tname);
				exit(1);
				return 3;
			}
			//////////////////////////////////////////////////////////////
			if (port.type == PRIMARY_OUT)
				pNode->addOutput(sig, port.index);
			else
				pNode->addInput(sig, port.index);
			if (i == state->sig_ref.range.end)
				break;
			port.index += step2;
		}
	};
	if (state->sig_ref.is_vector == 0)
	{
		if (state->sig_ref.is_const == 1)
		{
			if (port.is_vector)
				port.index = port.r.begin;
			if (strcmp(state->sig_ref.name, "b1") == 0)
				if (port.type == PRIMARY_OUT)
					pNode->addOutput(&pM->vcc, port.index);
				else
					pNode->addInput(&pM->vcc, port.index);
			if (strcmp(state->sig_ref.name, "b0") == 0)
				if (port.type == PRIMARY_OUT)
					pNode->addOutput(&pM->gnd, port.index);
				else
					pNode->addInput(&pM->gnd, port.index);
		}
		else
		{
			char tname[100];
			strcpy_s(tname, state->sig_ref.name);

			signal *sig = pM->find_signal(tname);
			if (sig == NULL)
			{
				printf("Error in module %s : %s is not found in current module as input, wire or output.\n", pM->name, tname);
				exit(1);
				return 3;
			}
			if (port.type == PRIMARY_OUT)
				pNode->addOutput(sig, port.index);
			else
				pNode->addInput(sig, port.index);
		}
	}

	if (state->sig_ref.is_vector == 1 && state->sig_ref.is_const == 1)
	{
		printf("Error in module %s : Constant Vector is not implemented YET.\n", pM->name);
		exit(1);
	}
	return 0;
}
void CheckandSetSrcType(Module* pM, struct parse_state *state, nodePort dstPort)
{
	if (dstPort.is_vector  && state->sig_ref.is_vector == false)
	{
		nodePort srcPort = pM->findPortInfo(state->sig_ref.name);
		if (srcPort.is_vector)
		{
			state->sig_ref.is_vector = true;
			state->sig_ref.range = srcPort.r2;
		}
	}
}

int SetPortMap(struct parse_state *state, char* port_name)
{
	Module* pM = (Module*)state->h_module;
	node* pNode = (node*)state->h_current_node;
	if (pNode == NULL)
		return 2;
	if (pM == NULL)
		return 1;
	int g = 0;
	if (strcmp(pM->name, "aes_128") == 0)
		g = 1;
	nodePort dstPort = pNode->findPortInfo(port_name);
	CheckandSetSrcType(pM, state, dstPort);
	if (dstPort.is_vector)
	{
		//dstPort.r = dstPort.r2;
	}
	if (!dstPort.found)
	{
		printf("Error in module %s : port %s is not found in %s\n", pM->name, port_name, pNode->name);
		exit(1);
		return 1;
	}
	if (state->is_array == 0)
		BindPort(state, pM, pNode, dstPort);
	else
	{
		int nCount = 0;
		int step1 = (diff(dstPort.r) <= 0) ? 1 : -1;
		dstPort.is_vector = true;
		for (nCount = 0; nCount < state->nSignal; nCount++)
		{
			state->sig_ref = state->siglist[nCount];
			CheckandSetSrcType(pM, state, dstPort);
			int range2 = (state->sig_ref.is_vector) ? diff(state->sig_ref.range) : 0;
			dstPort.r.end = dstPort.r.begin + step1 * abs(range2);
			BindPort(state, pM, pNode, dstPort);
			// update range
			dstPort.r.begin += step1 * (1 + abs(range2));
		}
	}
	return 0;
}
int DoAssign(struct parse_state *state)
{
	static int nDummy = 0;
	Module* pM = (Module*)state->h_module;
	char lSigName[100], tmp[100];
	signal_ref *lsig = &state->siglist[0];
	if (state->siglist[0].is_vector)
		sprintf(lSigName, "%s[%d]", lsig->name, lsig->range.begin);
	else
		sprintf(lSigName, "%s", lsig->name);
	nodePort dstPort = pM->findPortInfo(lSigName);

	if (dstPort.found == false)
	{
		printf("Error in module %s : signal '%s' in left-expr of assing is not found.\n", pM->name, lSigName);
		exit(1);
	}
	if (dstPort.is_vector == false)
	{
		signal *out = pM->find_signal(lSigName);
		sprintf(tmp, "dummy_%d", ++nDummy);
		node *dummy = new node(tmp, DUMMY, NULL);
		dummy->stdType = state->typeSTD;
		pM->addNode(dummy);
		dummy->addOutput(out, 0);
		nodePort p = dummy->findPortInfo("IN");
		BindPort(state, pM, dummy, p);
		dummy->Out[0]->type = dummy->In[0]->type;
		return 0;
	}
	dstPort.r = dstPort.r2;
	int step1 = (diff(dstPort.r) <= 0) ? 1 : -1;
	CheckandSetSrcType(pM, state, dstPort);
	int step2 = (diff(state->sig_ref.range) <= 0) ? 1 : -1;
	int len = abs(diff(dstPort.r)) + 1;;
	int idx1 = dstPort.r.begin ;
	int idx2 = state->sig_ref.range.begin;
	for (int i = 0; i < len; i++)
	{
		dstPort.r.begin = dstPort.r.end = idx1;
		state->sig_ref.range.begin = state->sig_ref.range.end = idx2;
		sprintf(lSigName, "%s[%d]", dstPort.name, idx1);
		signal *out = pM->find_signal(lSigName);
		sprintf(tmp, "dummy_%d", ++nDummy);
		node *dummy = new node(tmp, DUMMY, NULL);
		dummy->stdType = state->typeSTD;
		pM->addNode(dummy);
		dummy->addOutput(out, 0);
		nodePort p = dummy->findPortInfo("IN");
		BindPort(state, pM, dummy, p);
		dummy->Out[0]->type = dummy->In[0]->type;
		idx1 += step1;
		idx2 += step2;
	}
	return 0;
}
void Check(Module *pM)
{
	if (pM->nIn != pM->p_inputs.size())
	{
		printf("%s : check module error => nIn = %d in.size()=%d\n",
			pM->name, pM->nIn, pM->p_inputs.size());
		exit(1);
	}
	for (int i = 0; i < pM->nIn; i++)
	{
		if (pM->p_inputs[i] == NULL)
		{
			printf("%s : check module error => input %d == NULL\n", pM->name, i);
			exit(1);
		}
	}
	if (pM->nOut != pM->p_outputs.size())
	{
		printf("%s : check module error => nOut = %d out.size()=%d\n",
			pM->name, pM->nOut, pM->p_outputs.size());
		exit(1);
	}
	if (pM->nWires != pM->p_wires.size())
	{
		printf("%s : check module error => nWire = %d wire.size()=%d\n",
			pM->name, pM->nWires, pM->p_wires.size());
		exit(1);
	}
	if (pM->nNode != pM->p_nlist.size())
	{
		printf("%s : check module error => nNode = %d nlist.size()=%d\n",
			pM->name, pM->nNode, pM->p_nlist.size());
		exit(1);
	}
	for (int i = 0; i < pM->nNode; i++)
	{
		if (pM->p_nlist[i]->type == __MAX_ENTITY && pM->p_nlist[i]->hModule != NULL)
		{
			Module* m = (Module*)pM->p_nlist[i]->hModule;
			Check(m);
		}
	}
}
void CheckModule(struct parse_state *state)
{
	Module* pM = (Module*)state->h_module;
	int g = 0;
	if (strcmp(pM->name, "aes_128") == 0)
		g = 1;
	Check(pM);
	return;
}
void PrintModule(struct parse_state *state)
{
	PrintModules(state->outFileName, state);

}
void ReleaseContext(struct parse_state *state)
{
	for (int iM = 0; iM < state->nModule; iM++)
	{
		Module* pM = (Module*)state->hModules[iM];
		for (int i = 0; i < pM->nIn; i++)
			delete pM->p_inputs[i];
		for (int i = 0; i < pM->nOut; i++)
			delete pM->p_outputs[i];
		for (int i = 0; i < pM->nWires; i++)
			delete pM->p_wires[i];
		for (int i = 0; i < pM->nNode; i++)
			delete pM->p_nlist[i];
		delete pM;
	}
}

extern "C"
{
	void* create_module(char* name)
	{
		void *h = CreateModule(name);
		if (h == NULL)
		{
			printf("Error : create_module %s \n", name);
			exit(1);
		}
		return h;
	}

	int add_signal(struct parse_state *state, char *name)
	{
		return AddSignal(state, name);
	}
	int add_vector_signal(struct parse_state *state, char* name, struct vector_range r)
	{
		return AddVectorSignal(state, name, r);
	}
	void * create_node(struct parse_state *state, char* name, char* type)
	{
		return CreateNode(state, name, type);
	}
	int set_port_map(struct parse_state *state, char* sig_name)
	{
		return SetPortMap(state, sig_name);
	}
	void assign_statement(struct parse_state *state)
	{
		//return;
		DoAssign(state);
	}
	void check_module(struct parse_state *state)
	{
		//return;
		CheckModule(state);
	}	
	void print_module(struct parse_state *state)
	{
		//return;
		PrintModule(state);
	}
	void release_context(struct parse_state *state)
	{
		//return;
		ReleaseContext(state);
	}
/*
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
*/


}
//start /B /WAIT /D "D:\program\scoap\scoap\" win_bison.exe --output="verilog_p.tab.cpp" --defines="verilog_p.tab.h"  "verilog_p.y"