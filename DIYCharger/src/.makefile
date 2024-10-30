#------------------------------------------------------------------------------
# LIION -- MAKEFILE
#------------------------------------------------------------------------------
#
# Dr. mont. Tobias Holzmann
# October 2024
#
# Description
#     Simple make file to create and build the project
#
#------------------------------------------------------------------------------

.PHONEY = buildLIION pre linkHeaders libLIION

PRE_MSG=\
	$(info ) \
	$(info ************************ LIION MAKE ***************************) \
	$(info ) \
	$(info l-I Starting compiling the libraries) \
	$(info l-I Build by Tobias Holzmann) \
	$(info l-I Makefile update 30.10.2024) \
	$(info ) \
	$(info ***************************************************************) \
	$(info )



POST_MSG=\
	$(info ) \
	$(info ) \
	$(info ***************************************************************) \
	$(info ) \
	$(info l-I Compiling of the libraries done) \
	$(info l-I You are ready to use the LIION application or the libraries) \
	$(info l-I If you have any problems comment on git or write an email) \
	$(info l-I Makefile update 30.10.2024) \
	$(info ) \
	$(info ***************************************************************)


CLEAN_MSG=\
	$(info ) \
	$(info ************************ LIION MAKE ***************************) \
	$(info ) \
	$(info l-I Cleaning the libraries and folder) \
	$(info ) \
	$(info ***************************************************************) \
	$(info )


SPACE_MSG=\
	$(info ) \
	$(info ) \
	$(info ***************************************************************) \
	$(info ) \
	$(info )

#------------------------------------------------------------------------------
