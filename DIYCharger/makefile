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


#------------------------------------------------------------------------------ 

build : makeDir
	$(COMPILER) $(CPPFLAGS) -I$(SRC_PATH) -L$(LIB_PATH) DIYCharger.cpp -o platforms/bin/$(PROGRAM)
	#$(COMPILER) $(CPPFLAGS) -I$(SRC_PATH) -L$(LIB_PATH) DIYCharger.cpp -llitihiumIon -o $(PROGRAM)

makeDir:
	mkdir platforms/bin

#------------------------------------------------------------------------------ 
