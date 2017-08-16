CXX=g++
CXXFLAGS=-std=c++11 
CPPFLAGS=-Iinclude
LDFLAGS=-Llib -Xlinker -rpath=lib 
LDLIBS=-lboost_program_options -lboost_filesystem -lboost_system -lboost_regex -lmagic -lzip
objects=main.o command_line_args_options.o files_list.o grep_pptx.o

grep-pptx : $(objects)
	$(CXX) $(CXXFLAGS) $(objects) $(LDFLAGS) -o $@ $(LDLIBS)

main.o : 
command_line_args_options.o : command_line_args_options.h
files_list.o : files_list.h
grep_pptx.o : grep_pptx.h

.PHONY : clean
clean :
	-rm -f grep-pptx $(objects)
