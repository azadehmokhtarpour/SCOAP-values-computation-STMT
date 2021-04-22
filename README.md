# SCOAP Values 
Hardware testing is needed to ensure that every component of a system is operating as it should. In the last decade, increasing the density of transistors at the chip surface makes this testing process difficult, so vendors try to design and fabricate circuits in a way that their testability facilitated. To do this they adopt a testable design style imposes constraints on the designer. The advantage of this method is that the resulting digital circuit will have features to facilitate the production of test sequences and test processes. Testability is related to the difficulty of controlling and observing the logical values of internal nodes from circuit inputs and outputs, respectively. One way to calculate this testability is using SCOAP values. Controllability and observability are two most important measurements of SCOAP which can be calculated in two structural designs: Sequential and combinational. Controllability is difficulty of setting and controlling value of signal to 0 or 1. Observability is difficulty of observing changes in signal value at output. If all signals and nodes in the circuit have less controllability and observability that’s mean circuit is testable. To calculate this measurements, we develop a project named: STMT SCOAP value calculation. You can do “build and usage’ steps to build and run this project. Output of observability and controllability will be produced in CSV format. Enjoy it!
# Build and usage
folow next steps to build and run codes to calculate SCOAP values:
1.	Create a solution in visual studio (Enterprise 2015):
i.	Win32 Console Application
ii.	Not using Precompiled header
iii.	Create an empty project
iv.	Set the solution folder to D:\program\scoap
2.	Copy the source files from cloud into d:\program\scoap\scoap\ folder
3.	Add the source to project (Add Existing Item)
•	Add all *.cpp and *.h files 
•	Add Verilog_s.l 
•	Add Verilog_p.y
4.	Download win_flex_bison-latest.zip and extract it into D:\flex\
5.	 Right click on Project => Build dependencies => Build Customization => Find Existing => 
•	Select D:\flex\ custom_build_rules\win_flex_bison_custom_build.targets
•	Check True win_flex_bison_custom_build.targets
6.	Right click on Project\Properties\VC++ Directories\Executable Directories Select Edit and add new line as “D:\flex”
7.	Right click on Verilog_s.l => Properties\General : set ItemType as “Flex files”
8.	Right click on Verilog_p.y => Properties\General : set ItemType as “Bison files”
9.	
10.	Right click on Verilog_s.l / Verilog_p.y select compile => For both “The compile must be successful”
11.	Right click on main.cpp select Properties\C/C++\Advanced Set “Compile As” to “Compile as C Code /TC”
12.	Increase the stack reserved size to 100,000,000 via Linker\System\Stack Reserved Size (100MB)
13.	 Now you can run the project 
•	Right click on Project\ Configuration Properties => Debugging\Command Line Argument
•	Wire =>  D:\test.v -n180
•	You  can choose test.v from benchmark.rar located in cloud
•	The results are written to current directory (d:\program\scoap\scoap)
i.	Sout.v => scoap result in text format
ii.	Sout.csv => scoap result in csv format (you can open in EXCEL)
