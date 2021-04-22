#pragma once
#include "scoap.h"

int Evaluate_SCOAP_CC(node *n);
int Evaluate_SCOAP_CC_v2(node *n);
void Evaluate_SCOAP_CO(node *n);
void EvaluateSCOAP_CC(Module *m);
void EvaluateSCOAP_CC_v2(Module *m);
void EvaluateSCOAP_CO(Module *m);
void Update_CO(signal *s);
void PrintSCOAP(ofstream myfile, Module *m);
void EvaluateSCOAP(Module *m);
void print_SCOAP(struct parse_state *state);
void Evaluate_SCOAP(struct parse_state *state);

bool ReadyForCC(node *n);
Module * hModule(node *n);
Module * hModule_v2(node *n);

void cc_NAND2(node *n);
void cc_NOT(node *n);
SCOAP cc_NAND2(SCOAP in1, SCOAP in2);
SCOAP cc_NOT(SCOAP in1);
SCOAP cc_NOR2(SCOAP in1, SCOAP in2);
SCOAP cc_XOR2(SCOAP in1, SCOAP in2);
SCOAP cc_NAND3(SCOAP in1, SCOAP in2, SCOAP in3);
SCOAP cc_NOR3(SCOAP in1, SCOAP in2, SCOAP in3);
SCOAP cc_AND2(SCOAP in1, SCOAP in2);
SCOAP cc_AND3(SCOAP in1, SCOAP in2, SCOAP in3);
SCOAP cc_OR2(SCOAP in1, SCOAP in2);
SCOAP cc_OR3(SCOAP in1, SCOAP in2, SCOAP in3);
SCOAP cc_XOR3(SCOAP in1, SCOAP in2, SCOAP in3);

SCOAP co_NOT(SCOAP out);
SCOAP co_NAND2(SCOAP out, SCOAP in1);
SCOAP co_NOR2(SCOAP out, SCOAP in1);
SCOAP co_XNOR2(SCOAP out, SCOAP in1);
SCOAP co_NAND3(SCOAP out, SCOAP in1, SCOAP in2);
SCOAP co_NOR3(SCOAP out, SCOAP in1, SCOAP in2);
SCOAP co_XNOR3(SCOAP out, SCOAP in1, SCOAP in2, SCOAP in3, int select);
SCOAP co_AND2(SCOAP out, SCOAP in1);
SCOAP co_OR2(SCOAP out, SCOAP in1);
SCOAP co_AND3(SCOAP out, SCOAP in1, SCOAP in2);
SCOAP co_OR3(SCOAP out, SCOAP in1, SCOAP in2);
SCOAP co_XNOR3_v2(SCOAP out, SCOAP in1, SCOAP in2);