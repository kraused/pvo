
# vi: tabstop=4:expandtab:hlsearch

OPTION( PVO_ENABLE_FORTRAN "Enable Fortran Interface" ON )

IF( "${PVO_ENABLE_FORTRAN}" STREQUAL "ON" )
	MESSAGE( STATUS "Compiling with Fortran support." )
ELSE()
	MESSAGE( STATUS "Disabled Fortran support." )
	RETURN()
ENDIF()

