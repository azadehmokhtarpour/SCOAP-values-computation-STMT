// scoap.cpp : Defines the entry point for the console application.
//
// start /B /WAIT /D "D:\program\scoap\scoap\" win_bison.exe --output="verilog_p.tab.cpp" --defines="verilog_p.tab.h"  "verilog_p.y"

#include "string.h"
#define YYINITDEPTH 1000000
#define YYMAXDEPTH  1000000
#include "targetver.h"
#include <stdio.h>
#include <tchar.h>

#include "verilog_s.flex.cpp"

#include "verilog_p.tab.cpp"
//start /B /WAIT /D "D:\program\scoap\scoap\" win_bison.exe --define=YYINITDEPTH=2000000 --define=YYMAXDEPTH=2000000 --output="verilog_p.tab.cpp" --defines="verilog_p.tab.h"  "verilog_p.y"






