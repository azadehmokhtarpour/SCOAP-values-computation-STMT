%{
#include <stdio.h>
#include <stdlib.h>
extern int yylex();
extern int yyparse();
extern FILE* yyin;


void yyerror(const char* s);
#include "general.h"


void* create_module(char* name);
int add_signal(struct parse_state *state, char *name);
int add_vector_signal(struct parse_state *state, char* name, struct vector_range r);
void * create_node(struct parse_state *state, char* name, char* type);
int set_port_map(struct parse_state *state, char* sig_name);
void print_module(struct parse_state *state);
void check_module(struct parse_state *state);
void release_context(struct parse_state *state);
void eval_scoap(struct parse_state *state);
void print_scoap(struct parse_state *state);
void eval_Kmeans(struct parse_state *state);
void assign_statement(struct parse_state *state);
//hkhk
struct parse_state st;
int nNode = 0;
void push()
{
		if (st.is_array)
		{
			if (st.nSignal >= MAX_SIGLIST)
			{
				printf("push Error\n");
				getchar();
			}
			st.siglist[st.nSignal++] = st.sig_ref;
		}
}

%}

%union {
	int ival;
	float fval;
	char str[100];
}

%token<str> T_IDENTIFIER
%token<ival> T_SIG_TYPE
%token<ival> T_INT
%token<fval> T_FLOAT

%token  T_ASSIGN_STR T_MODULE T_ENDMODULE	

%token T_COMMA T_SINGLEQOUT T_POINT T_PLUS T_MINUS T_MULTIPLY T_DIVIDE T_LEFT T_RIGHT T_SEMICOLON  T_RANGE T_BRACE_O T_BRACE_C T_BLOCK_O T_BLOCK_C
																			 
%left T_ASSIGN T_PLUS T_MINUS 
%left T_COMMA T_MULTIPLY T_DIVIDE T_MODULE
%nonassoc  T_INPUT T_OUTPUT T_WIRE 


%start start-var

%%
start-var:  
		| module-statement start-var
;
module-statement :  module-def-statement statement-list T_ENDMODULE 
			{
				printf("matching end of module %s nCount=%d \n", st.modName, st.nModule);
				check_module(&st);
			}
;
module-def-statement :  T_MODULE T_IDENTIFIER {
				st.h_module = create_module($2);
				st.hModules[st.nModule++] = st.h_module;
				st.state = MODULE_DEFINE; 
				strcpy(st.modName, $2);
				nNode = 0;
				printf("matching module %s definition : nModule = %d\n", $2, st.nModule); 
			}
						T_LEFT sig_list T_RIGHT T_SEMICOLON
			
;
statement-list :
		| statement T_SEMICOLON statement-list
;
statement :
	   | signal-assign-statement
	   | signal-def-statement 
	   | component-instantiate-statement 
;
signal-def-statement :  
	signal-type sig_list  
;	
signal-type : 
	   | T_SIG_TYPE		
	   {
			st.state = SIGNAL_DEFINE; 
			st.sig_type.type = $1;
			st.sig_type.is_vector = 0;
			printf("matching type of %d\n", $1);
		}
	   | T_SIG_TYPE T_BRACE_O T_INT T_RANGE T_INT T_BRACE_C
	   {
			st.state = SIGNAL_DEFINE;
			st.sig_type.type = $1;
			st.sig_type.is_vector = 1;
			st.sig_type.range.begin = $3;
			st.sig_type.range.end   = $5;
	   }
;

sig_list : 
    | sig_name  
    | sig_name T_COMMA sig_list    
;

sig_name :
	| T_IDENTIFIER  
	{	
		if (st.state == SIGNAL_DEFINE) add_signal(&st, $1);
		printf("\t matching signal %s \n", $1); 
	}
	| T_IDENTIFIER T_BRACE_O T_INT T_RANGE T_INT T_BRACE_C 
	{ 
		struct vector_range r;
		r.begin = $3;
		r.end = $5;
		if (st.state == SIGNAL_DEFINE) add_vector_signal(&st, $1, r);
		printf("\t matching signal %s[%d..%d]\n ", $1, $3, $5); 
	}
;
signal-assign-statement :
	T_ASSIGN_STR sig_ref 
	{ 
			st.siglist[0] = st.sig_ref;
	} 
	T_ASSIGN sig_ref 
	{ 
		st.state = ASSIGN;
		assign_statement(&st);
		printf("%d Matching assign Statement \n ", ++nNode);
	}
;
sig_ref_list : sig_ref 
			 | sig_ref T_COMMA sig_ref_list
;
sig_ref :
	T_IDENTIFIER  
	{ 
		st.sig_ref.is_vector = st.sig_ref.is_const = 0;
		strcpy(st.sig_ref.name, $1);
		st.sig_ref.range.begin = st.sig_ref.range.end = 0;
		push();
		//printf("\t refering %s\n ", $1);
	}
	| T_IDENTIFIER T_BRACE_O T_INT T_BRACE_C 
	{ 
		st.sig_ref.is_vector = 1;
		st.sig_ref.is_const = 0;
		st.sig_ref.range.begin = st.sig_ref.range.end = $3;
		strcpy(st.sig_ref.name, $1);
		push();
		printf("refering %s[%d]\n ", $1, $3); 
	}
	| T_IDENTIFIER T_BRACE_O T_INT T_RANGE T_INT T_BRACE_C 
	{ 
		st.sig_ref.is_vector = 1;
		st.sig_ref.is_const = 0;
		st.sig_ref.range.begin = $3;
		st.sig_ref.range.end = $5;
		strcpy(st.sig_ref.name, $1);
		push();
		printf("refering %s[%d:%d]\n ", $1, $3, $5); 
	}
	| T_INT T_SINGLEQOUT T_IDENTIFIER 
	{ 
		st.sig_ref.is_const = 1;
		st.sig_ref.is_vector = 0;
		st.sig_ref.width = $1;
		strcpy(st.sig_ref.name, $3);
		push();
		printf("refering constant %d bit[%s\n ", $1, $3); 
	}
;
component_def :
		T_IDENTIFIER T_IDENTIFIER  
		{
			st.state = COMPONENT_INSTANTIATE;
			st.h_current_node = create_node(&st, $1, $2);
			printf("%d instantiate %s of type %s \n ", ++nNode, $2, $1);
		}
		| T_IDENTIFIER T_IDENTIFIER T_BRACE_O T_INT T_BRACE_C
		{
			printf("TEST");
			st.state = COMPONENT_INSTANTIATE;
			char str[100];
			sprintf(str, "%s[%d]", $2, $4);
			st.h_current_node = create_node(&st, $1, str);
			printf("%d instantiate %s of type %s \n ", ++nNode, str, $1);
		}
;
component-instantiate-statement : component_def T_LEFT port-mapping T_RIGHT
;
port-mapping :
      port-map  
    | port-map T_COMMA port-mapping    
;
port-map : T_IDENTIFIER 
		 | T_POINT T_IDENTIFIER T_LEFT sig_ref T_RIGHT
		 {
			set_port_map(&st, $2);
		 }
		 | T_POINT T_IDENTIFIER T_LEFT T_BLOCK_O  
		 {
			 st.is_array = 1;
			 st.nSignal = 0;
		 }
		 sig_ref_list T_BLOCK_C T_RIGHT
		 {
			set_port_map(&st, $2);
			st.is_array = 0;
		 }
		 | T_POINT T_IDENTIFIER T_LEFT T_RIGHT
		 {
			printf("Warning : Unconnected in/out %s\n", $2);
		 }

;

%%
int main(int argc, char* argv[]) {
	st.nModule = 0;
	if (argc < 2)
	{
		printf("Please sepcify the input file.\nUsage : scoap infile [-n90|-n180] ");
		return 1;
	}
	fopen_s(&yyin, argv[1], "r");
	st.typeSTD = STD90;
	if (yyin == NULL)
	{
		printf("unable to open %s\n", argv[1]);
		return 1;
	}
	strcpy(st.outFileName, "out.v");
	int i;
	for (i = 2; i < argc; i++)
	{
		if (strcmp(argv[i], "-n90") == 0)
			st.typeSTD = STD90;
		if (strcmp(argv[i], "-n180") == 0)
			st.typeSTD = STD180;
		if (strcmp(argv[i], "-verbose") == 0)
			st.bVerbose = 1;
	}
	do
	{ 
		yyparse();
	} while(!feof(yyin));
	fclose(yyin);
	print_module(&st);
	eval_scoap(&st);
	print_scoap(&st);
	//eval_Kmeans(&st);
	release_context(&st);
	return 0;
}
void yyerror(const char* s) {
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}
