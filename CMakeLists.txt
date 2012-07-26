
# vi: tabstop=4:expandtab:hlsearch

PROJECT( pvo NONE )
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

# Include path
SET( CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

# Check whether the user wants Fortran support.
INCLUDE(PvoFortran)

# Check for MPI. We need to cal this before we enable the
# languages because these files might set CMAKE_C_COMPILER
# and CMAKE_CXX_COMPILER to enforce usage of compiler
# wrappers
INCLUDE(PvoMPI)

# Now we can enable the languages. This must be done here because
# PvoConfig.cmake checks for headers
ENABLE_LANGUAGE( C )
ENABLE_LANGUAGE( CXX )

SET(CMAKE_SHARED_LIBRARY_LINK_Fortran_FLAGS "")

IF( PVO_ENABLE_FORTRAN )
    ENABLE_LANGUAGE( Fortran )
ENDIF()

INCLUDE( PvoConfig )

# To be able to find pvo_config.h ...
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )

# Source files
SET ( SRC "" )
LIST( APPEND SRC
      pvo_report.c
      pvo_memory.c
      pvo_utils.c
      pvo_init.c
      pvo_quit.c
      pvo_physical_node_uid.c
      pvo_mpi.c
      pvo_low_io.c
      pvo_low_io_single.c
      pvo_low_io_posix.c
      pvo_island.c
      pvo_xml_file.c
      pvo_var.c
      pvo_cookie.c
      pvo_file.c )

# MPI
IF( TPL_ENABLE_MPI )
	LIST( APPEND SRC
	      pvo_low_io_mpi.c )
ENDIF()

# vtu files
LIST( APPEND SRC
      pvo_vtu_file.c )
# vtp files
LIST( APPEND SRC
      pvo_vtp_file.c )
# vti files
LIST( APPEND SRC
      pvo_vti_file.c )

ADD_LIBRARY( pvo ${SRC} )

# Fortran interface
IF( PVO_ENABLE_FORTRAN )
    ADD_SUBDIRECTORY( fortran )
ENDIF()

# Testing
ENABLE_TESTING  ()
ADD_SUBDIRECTORY( test )

# Examples and benchmarks
ADD_SUBDIRECTORY( examples )

# Installation
SET( LIB_INSTALL_DIR		${CMAKE_INSTALL_PREFIX}/lib )
SET( INCLUDE_INSTALL_DIR	${CMAKE_INSTALL_PREFIX}/include )
SET( BIN_INSTALL_DIR		${CMAKE_INSTALL_PREFIX}/bin )

INSTALL( TARGETS	pvo
	 DESTINATION	lib )

INSTALL( FILES
         ${CMAKE_CURRENT_BINARY_DIR}/pvo_config.h
         ${CMAKE_CURRENT_SOURCE_DIR}/pvo.h
         ${CMAKE_CURRENT_SOURCE_DIR}/pvof.h
         ${CMAKE_CURRENT_SOURCE_DIR}/pvo_cookie.h
         ${CMAKE_CURRENT_SOURCE_DIR}/pvo_decls.h
         ${CMAKE_CURRENT_SOURCE_DIR}/pvo_file.h
         ${CMAKE_CURRENT_SOURCE_DIR}/pvo_island.h
         ${CMAKE_CURRENT_SOURCE_DIR}/pvo_low_io.h
         ${CMAKE_CURRENT_SOURCE_DIR}/pvo_low_io_mpi.h
         ${CMAKE_CURRENT_SOURCE_DIR}/pvo_low_io_posix.h
         ${CMAKE_CURRENT_SOURCE_DIR}/pvo_low_io_single.h
         ${CMAKE_CURRENT_SOURCE_DIR}/pvo_memory.h
         ${CMAKE_CURRENT_SOURCE_DIR}/pvo_mpi.h
         ${CMAKE_CURRENT_SOURCE_DIR}/pvo_mpi_stubs.h
         ${CMAKE_CURRENT_SOURCE_DIR}/pvo_report.h
         ${CMAKE_CURRENT_SOURCE_DIR}/pvo_types.h
         ${CMAKE_CURRENT_SOURCE_DIR}/pvo_var.h
         ${CMAKE_CURRENT_SOURCE_DIR}/pvo_vtp_file.h
         ${CMAKE_CURRENT_SOURCE_DIR}/pvo_vtu_file.h
         ${CMAKE_CURRENT_SOURCE_DIR}/pvo_vti_file.h
         ${CMAKE_CURRENT_SOURCE_DIR}/pvo_xml_file.h 
         DESTINATION include )

IF( PVO_ENABLE_FORTRAN )
    INSTALL( CODE "EXECUTE_PROCESS( COMMAND \"${CMAKE_COMMAND}\" -E copy_directory \"${CMAKE_Fortran_MODULE_DIRECTORY}/\${BUILD_TYPE}\" \"${CMAKE_INSTALL_PREFIX}/lib\" )")
ENDIF()

