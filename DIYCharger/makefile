#------------------------------------------------------------------------------
# LIION -- MAKEFILE
#------------------------------------------------------------------------------
# 
# Dr. mont. Tobias Holzmann
# October 2024
#
# Description
#     Nono
#
#------------------------------------------------------------------------------ 

 include src/.compilerFlags

 PROGRAM=LithiumIonCharger
 COMPILER=g++
 SRC_PATH=src/gcc/lnInclude
 LIB_PATH=platforms/libs/
 WORK_PATH=$(PWD)


#------------------------------------------------------------------------------ 

all: makeDir libs exec

libs: 
	$(MAKE) -C src/

exec:
	$(COMPILER) $(CPPFLAGS) -I$(SRC_PATH) -L$(LIB_PATH) LiIonCharger.cpp -llithiumIon -o $(WORK_PATH)/platforms/bin/$(PROGRAM)

makeDir:
	mkdir -p platforms/bin platforms/libs

clean: 
	rm -rf platforms gcc
	$(MAKE) -C src/ clean


#------------------------------------------------------------------------------ 
