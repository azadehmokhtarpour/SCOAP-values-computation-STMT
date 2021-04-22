/******************************************************************************

Online C++ Compiler.
Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#ifndef __SCOAP
#define __SCOAP
#include <iostream>

#include <stdio.h>
#include <string.h>
#include <string>
#include <fstream>
#include <vector> 
#include "def.h"

using namespace std;



#define INF 10000
#define NOT_INITIALIZED -1
const int MAX = 254;
//const int MAX_INPUT  = 2000;// 2000;
//const int MAX_OUTPUT = 2000;//2000;
//const int MAX_WIRES  = 30000;//3000000;
//const int MAX_NODE   = 20000;//2000000;

#include"general.h"

struct signal;
struct node;
struct Module;
struct StdCellInfo;
struct TestabilityState;
struct SCOAP;


enum EType;
enum SType;

signal * findSignal(vector<signal*> sig, int nCount,const char *sig_name);
int Evaluate_SCOAP_CC(node *n);
void ImportFromFile(char *fname, Module *m);
int PrintModule(char *name, Module *m);

enum SType
{
	PRIMARY_IN = 0,
	PRIMARY_OUT,
	INTERMEDIATE,
	//PRIMARY_INOUT,
	CLOCK,
	ASYNC_RESET
};

struct portInfo
{
	SType type; //for entity ports, it could only be PRIMARY_IN, PRIMARY_OUT, CLOCK or ASYNC_RESET
	char name[MAX];
};

struct nodePort
{
	char name[MAX];
	SType type; //for entity ports, it could only be PRIMARY_IN, PRIMARY_OUT, CLOCK or ASYNC_RESET
	int index;
	bool found;
	bool is_vector;
	struct vector_range r, r2;
};
#define MAX_PORT 20
struct StdCell
{
	EType e;
	char name[64];
};
struct StdCellInfo
{
	int nStdType;
	StdCell cells[80];
	int nIn, nOut;
	// clk and reset are determined in FF SC and SO 
	portInfo in_ports[MAX_PORT], out_ports[MAX_PORT];

public:
	portInfo* findPortInfo(const char* port_name) {
		for (int i = 0; i < nIn; i++)
		{
			if (strcmp(in_ports[i].name, port_name) == 0)
				return &in_ports[i];
		}
		for (int i = 0; i < nOut; i++)
		{
			if (strcmp(out_ports[i].name, port_name) == 0)
				return &out_ports[i];
		}
		return NULL;
	}
	int findPortIndex(const char* port_name) {
		for (int i = 0; i < nIn; i++)
		{
			if (strcmp(in_ports[i].name, port_name) == 0)
				return i;
		}
		for (int i = 0; i < nOut; i++)
		{
			if (strcmp(out_ports[i].name, port_name) == 0)
				return i;
		}
		return -1;
	}

};

StdCellInfo* FindStdCellInfo(EType e, int stdType);
StdCellInfo* FindStdCellInfo(char *node_name, int stdType);



struct SCOAP
{
	int cc[2];
	double total_cc;
	int co;
	int sc[2];
	double total_sc;
	int so;
public:
	SCOAP()
	{
		//these parameters should set to infinity because of Min function
		cc[0] = cc[1] = NOT_INITIALIZED;
		co = INF;
		sc[0] = sc[1] = NOT_INITIALIZED;
		so = INF;
	}
};

struct	TestabilityState
{
	SCOAP s;
	// FANCI
};
struct signal
{
	char name[MAX];
	SType type;
	node *generator;
	TestabilityState st;
	int vec_index;
	bool st_updated;

public:
	signal()
	{
		generator = NULL;
		st_updated = false;
		vec_index = 0;
	}
	void setName(const char *n) {
		strcpy_s(name, n);
	}
	void printCC()
	{
		printf("%10s : cc0 = %3d, cc1 = %3d, sc0 = %3d, sc1 = %3d\n",
			name, st.s.cc[0], st.s.cc[1], st.s.sc[0], st.s.sc[1]);

	}
	void printCO()
	{
		printf("%10s : co = %3d, so = %3d\n",
			name, st.s.co, st.s.so);

	}
};
#include <algorithm>    // std::min
struct Module;
struct node
{
	int stdType;
	char name[MAX];
	EType type;
	// handle for user-defined module
	void *hModule;
	int nIn, nOut;
	//signal *In[MAX_INPUT];
	//signal *Out[MAX_OUTPUT];
	vector<signal*> In;
	vector<signal*> Out;
	bool CC_Evaluated;
	bool ReadyForCO;

public:
	node(char* n_name, EType tStdCell, void *handle)
	{
		type = tStdCell;
		hModule = handle;
		strcpy_s(name, n_name);
		CC_Evaluated = false;
		ReadyForCO = false;
		// to do
		nIn = nOut = 0;
	}
	void init() {
		CC_Evaluated = false;
		ReadyForCO = false;
		for (int i = 0; i < nIn; i++) {
			if (In[i] == NULL)
				continue;
			if (strcmp(In[i]->name, "GND") || strcmp(In[i]->name, "VCC"))
				continue;
			In[i]->st.s.cc[0] = NOT_INITIALIZED;
			In[i]->st.s.cc[1] = NOT_INITIALIZED;
			In[i]->st.s.co = INF;
			In[i]->st.s.sc[0] = NOT_INITIALIZED;
			In[i]->st.s.sc[1] = NOT_INITIALIZED;
			In[i]->st.s.so = INF;
			In[i]->st_updated = false;
		}
		for (int j = 0; j < nOut; j++) {
			if (Out[j] == NULL)
				continue;
			if (strcmp(Out[j]->name, "GND") || strcmp(Out[j]->name, "VCC"))
				continue;
			Out[j]->st.s.cc[0] = NOT_INITIALIZED;
			Out[j]->st.s.cc[1] = NOT_INITIALIZED;
			Out[j]->st.s.co = INF;
			Out[j]->st.s.sc[0] = NOT_INITIALIZED;
			Out[j]->st.s.sc[1] = NOT_INITIALIZED;
			Out[j]->st.s.so = INF;
			Out[j]->st_updated = false;
		}

	}
	void connectSignal(char *portname, char*signame)
	{
	}
	void addInput(signal *s, int index) {
		//In[index] = s;
		if (index == In.size())
			In.push_back(s);
		else if (index < In.size())
		{
			In[index] = s;
		}
		else
		{
			In.resize(index+1);
			In[index] = s;
		}
		nIn++;
	}
	void addOutput(signal *s, int index) {
		//Out[index] = s;
		if (index == Out.size())
			Out.push_back(s);
		else if (index < Out.size())
		{
			Out[index] = s;
		}
		else
		{
			Out.resize(index + 1);
			Out[index] = s;
		}
		s->generator = this;
		nOut++;
	}

	int min(int x, int y, int z = 10000, int t = 10000)
	{
		return std::min({ x, y, z, t });
	};
	int sum_cc(int x0, int x1)
	{
		return In[0]->st.s.cc[x0] + In[1]->st.s.cc[x1];
	}
	int sum_cc(int x0, int x1, int x2)
	{
		return In[0]->st.s.cc[x0] + In[1]->st.s.cc[x1] + In[1]->st.s.cc[x2];
	}
	nodePort findPortInfo(char *sig_name);
	char* findPortName(bool bIn, int index);
	char* findCellName();
};


struct Module
{
public:
	char name[MAX];
	int nIn, nOut, nWires, nNode, nSeqNode;
	vector<signal*> p_inputs;
	vector<signal*> p_outputs;
	vector<signal*> p_wires;
	vector<node*> p_nlist;
	signal  gnd, vcc;
	//node
	bool bVerbose;
public:
	Module(const char *n)
	{
		strcpy_s(name, n);
		bVerbose = false;
		nNode = nIn = nOut = nWires = nSeqNode = 0;
		strcpy_s(vcc.name, "VCC");
		strcpy_s(gnd.name, "GND");
		strcpy_s(vcc.name, "VCC");
		vcc.st.s.cc[0] = 1;
		vcc.st.s.cc[1] = 1;
		vcc.st.s.sc[0] = 0;
		vcc.st.s.sc[1] = 0;
		strcpy_s(gnd.name, "GND");
		gnd.st.s.cc[0] = 1;
		gnd.st.s.cc[1] = 1;
		gnd.st.s.sc[0] = 0;
		gnd.st.s.sc[1] = 0;
	}

	void setName(const char *n) {
		strcpy_s(name, n);
	}

	signal * find_signal(const char *sig_name)
	{
		signal * sig;
		sig = findSignal(p_inputs, nIn, sig_name);
			if (sig != NULL)
				return sig;
		sig = findSignal(p_outputs, nOut, sig_name);
			if (sig != NULL)
				return sig;
		sig = findSignal(p_wires, nWires, sig_name);
			if (sig != NULL)
				return sig;
		return NULL;
	}
	SType getType(const char *name)
	{
		char *clock_sig[] = { "clk", "CK", "clock", "CLOCK"};
		char *reset_sig[] = { "rst", "RESET"};
		for (int i = 0; i < 3; i++)
			if (strstr(name, clock_sig[i]))
				return CLOCK;
		for (int i = 0; i < 2; i++)
			if (strstr(name, reset_sig[i]))
				return ASYNC_RESET;
		return PRIMARY_IN;
	}
	void addInput(const char *n, int idx) {

		signal *sig = new signal();
		sig->setName(n);
		sig->type = getType(n);
		switch (sig->type)
		{
		case CLOCK:
			printf("Warning : consider %s as Clock\n", n);
			break;
		case ASYNC_RESET:
			printf("Warning : consider %s as Async Reset\n", n);
			break;
		default :
			;
		}
		sig->st.s.cc[0] = 1;
		sig->st.s.cc[1] = 1;
		sig->st.s.sc[0] = 0;
		sig->st.s.sc[1] = 0;
		sig->vec_index = idx;
		p_inputs.push_back(sig);
		//p_inputs[nIn] = sig;
		nIn++;
	}
	void addOutput(const char *n, int idx) {
		signal *sig = new signal();
		sig->setName(n);
		sig->type = PRIMARY_OUT;
		sig->st.s.co = 0;
		sig->st.s.so = 0;
		p_outputs.push_back(sig);
		//p_outputs[nOut] = sig;
		sig->vec_index = idx;
		nOut++;
	}
	void addWire(const char *n, int idx) {
		signal *sig = new signal();
		sig->setName(n);
		SType s = getType(n);
		switch (s)
		{
		case CLOCK:
			sig->type = s;
			printf("Warning : consider %s as Clock\n", n);
			break;
		case ASYNC_RESET:
			sig->type = s;
			printf("Warning : consider %s as Async Reset\n", n);
			break;
		default:
			sig->type = INTERMEDIATE;
			break;
		}
		p_wires.push_back(sig);
		//p_wires[nWires] = sig;
		sig->vec_index = idx;
		nWires++;
	}
	void addNode(node *n) {
		//p_nlist[nNode] = n;
		p_nlist.push_back(n);
		nNode++;
		if (n->type == __MAX_ENTITY) {
			Module *mp = (Module*)n->hModule;
			nSeqNode += mp->nSeqNode;
		}
		else if (n->type >= DFFX1)
			nSeqNode++;
	}
	nodePort Module::findPortInfo(char *port_name);
};
#endif