########################################################
# Makefile for CSCI 435 
# Author: Dr. Zoppetti
########################################################

########################################################
# Variable definitions
########################################################
# C++ compiler
CXX := g++
#CXX := clang++

# Include directories, prefaced with "-I"
INCDIRS  := 

# C++ compiler flags
# Use the first for debugging, the second for release
# this is the good one for linux CXXFLAGS := -g -Wall -std=c++17 $(INCDIRS)
CXXFLAGS := -g -Wall -Wno-register -std=gnu++17 $(INCDIRS)
#CXXFLAGS := -O3 -Wall -std=c++17 $(INCDIRS)

# Linker. For C++ should be $(CXX).
LINK := $(CXX)

# Linker flags. Usually none.
LDFLAGS := 

# Library paths, prefaced with "-L". Usually none.
LDPATHS := 

# Executable name. 
EXEC := CMinus

# Libraries used, prefaced with "-l".
# LDLIBS := -lfl
LDLIBS :=  -L/Library/Developer/CommandLineTools/SDKs/MacOSX10.15.sdk/usr/lib -ll


#############################################################
# Rules
#   Rules have the form
#   target : prerequisites
#         recipe
#############################################################

$(EXEC) : CMinus.o Lexer.o Parser.o
	$(LINK) $(LDFLAGS) $(LDPATHS) $^ -o $@ $(LDLIBS)

%.o : %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.cc : %.l
	flex --nounput -o $@ $<

#############################################################

.PHONY : clean
clean :
	$(RM) $(EXEC) a.out core
	$(RM) *.o *.d *~

#############################################################