#------------------------------------------------------------------------------
# LIION -- MAKEFILE
#------------------------------------------------------------------------------
#
# Dr. mont. Tobias Holzmann
# October 2024
#
# Description
#     This makefile compiles all source files and builds the libraries that
#     can be used for your personal program or individual usage
#
#------------------------------------------------------------------------------

include .makefile
include .compilerFlags

#------------------------------------------------------------------------------

PROGRAM=LithiumIonCharger


#------------------------------------------------------------------------------
# Linux commands
#------------------------------------------------------------------------------

MAKE_DIR=mkdir -p
LINK=ln -sf
CLEAR=clear
CLEAN_DIR=rm -rfv
WORK_DIR=$(PWD)
DIR_OBJ=gcc/obj
DIR_LIB=$(WORK_DIR)/../platforms/libs
DIR_INCLUDE=$(WORK_DIR)/gcc/lnInclude


#------------------------------------------------------------------------------
# Stuff for symbolic links (all header files) and libraries (folder generation)
#------------------------------------------------------------------------------
HEADER_SYM_LINK=$(shell find . -type f -iname '*.hpp')
DIR_OBJECTS=$(shell find . -type d -not -path "./gcc*" | sed "s|\.||" | sed "s|^/||")



#------------------------------------------------------------------------------
# Stuff for the library
#------------------------------------------------------------------------------
SOURCE_FILES=$(shell find . -type f -iname '*.cpp')
OBJECT_FILES=$(addprefix gcc/obj/, $(addsuffix .o, $(basename $(SOURCE_FILES))))


#------------------------------------------------------------------------------
# Make Commands | Probably could be done much nicer - I am not a makefile guru
#------------------------------------------------------------------------------

buildLIION: pre linkHeaders liblithiumIon
	$(SPACE_MSG)


#------------------------------------------------------------------------------
pre: clear
	$(info $(PRE_MSG))
	$(info Creating directories...)
	$(shell $(MAKE_DIR) $(DIR_OBJ) $(DIR_INCLUDE) $(DIR_LIB) $(addprefix gcc/obj/, $(DIR_OBJECTS)))
	$(shell sleep 0.5)


#------------------------------------------------------------------------------
clear:
	$(CLEAR)


#------------------------------------------------------------------------------
linkHeaders:
	$(info Linking header files...)
	$(foreach file, $(HEADER_SYM_LINK), $(info $(file) ))
	$(foreach file, $(HEADER_SYM_LINK), $(shell $(LINK) $(WORK_DIR)/$(file) $(DIR_INCLUDE)))
	$(info Linking done...)
	$(info )
	$(info )


#------------------------------------------------------------------------------
liblithiumIon: $(OBJECT_FILES)
	$(SPACE_MSG)
	$(info Create lithiumIon library...)
	ar rvs $(DIR_LIB)/$(addsuffix .so, $(basename $@)) $(OBJECT_FILES)


#------------------------------------------------------------------------------
gcc/obj/%.o : %.cpp
	$(COMPILER) $(CPPFLAGS) -I$(DIR_INCLUDE) -c $< -o $@


#------------------------------------------------------------------------------
clean: clear
	$(info $(CLEAN_MSG))
	$(info )
	$(CLEAN_DIR) gcc $(PROGRAM)


#------------------------------------------------------------------------------
rebuild: clean buildLIION


#------------------------------------------------------------------------------
post:
	$(POST_MSG)


#------------------------------------------------------------------------------
help:
	@echo "Targets:"
	@echo "buildLIION           - build all libraries"
	@echo "clean        		- clean all libraries"
	@echo "pre          		- build directory tree for libraries"
	@echo "rebuild      		- execute clean, pre and buildLIION"


#------------------------------------------------------------------------------
