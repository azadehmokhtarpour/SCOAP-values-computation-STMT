#ifndef _GENERAL_
#define _GENERAL_
#define _CRT_SECURE_NO_WARNINGS

enum SIG_TYPE
{
	ST_IN,
	ST_OUT,
	ST_WIRE,
	ST_REG
};
enum __STATEMENT_STATE
{
	MODULE_DEFINE,
	SIGNAL_DEFINE,
	ASSIGN,
	COMPONENT_INSTANTIATE,
};
struct vector_range
{
	int begin, end;
};
int diff(struct vector_range r);// { return (r.begin - r.end); }

struct signal_type
{
	int type;
	int is_vector;
	struct vector_range range;
};
struct signal_ref
{
	char name[100];
	int is_const;
	int width;
	int is_vector;
	struct vector_range range;
};

#define MAX_MODULE  1000
#define MAX_SIGLIST 10000

#define STD180 0
#define STD90  1

struct parse_state
{
	enum __STATEMENT_STATE state;
	struct signal_type sig_type;
	struct signal_ref sig_ref;
	struct signal_ref siglist[MAX_SIGLIST];
	char modName[100];
	int nSignal;
	int is_array;
	void *h_module;
	void *h_current_node;
	void *hModules[MAX_MODULE];
	int nModule;
	char outFileName[100];
	int typeSTD;
	int bVerbose;
};

#endif