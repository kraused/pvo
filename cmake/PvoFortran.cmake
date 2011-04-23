
# vi: tabstop=4:expandtab:hlsearch

OPTION( PVO_ENABLE_FORTRAN "Enable Fortran Interface" ON )

IF( "${PVO_ENABLE_FORTRAN}" STREQUAL "ON" )
	MESSAGE( STATUS "Compiling with Fortran support." )
ELSE()
	MESSAGE( STATUS "Disabled Fortran support." )
	RETURN()
ENDIF()

SET( CMAKE_Fortran_MODULE_DIRECTORY
     ${PROJECT_BINARY_DIR}/fmods CACHE PATH "Single directory for all fortran modules" )
# This is needed to find the pvo module file
INCLUDE_DIRECTORIES( ${CMAKE_Fortran_MODULE_DIRECTORY} )

