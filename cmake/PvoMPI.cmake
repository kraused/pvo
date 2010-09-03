
OPTION( TPL_ENABLE_MPI "Enable MPI" ON )

IF( "${TPL_ENABLE_MPI}" STREQUAL "ON" )
	MESSAGE( STATUS "Compiling parallel version." )
ELSE()
	MESSAGE( STATUS "Compiling serial version." )
	RETURN()
ENDIF()

SET( MPI_BINARY_DIRS "" CACHE STRING "Directory containing the binaries in the MPI package" )

# We set the compiler to the mpicc and mpic++ wrappers.
# This is the easiest way to ensure that all necessary
# libraries are linked and that the include paths are
# correct

FIND_PROGRAM( MPI_MPICC_EXE
              mpicc
              PATHS ${MPI_BINARY_DIRS}
              DOC "MPI C compiler wrapper." )

IF( NOT MPI_MPICC_EXE )
	MESSAGE( FATAL_ERROR "Could not find MPI C compiler wrapper." )
ENDIF()

FIND_PROGRAM( MPI_MPICXX_EXE
              mpic++ | NAMES mpiCC
              PATHS ${MPI_BINARY_DIRS}
              DOC "MPI C++ compiler wrapper." )

IF( NOT MPI_MPICXX_EXE )
	MESSAGE( FATAL_ERROR "Could not find MPI C++ compiler wrapper." )
ENDIF()

SET( CMAKE_C_COMPILER   ${MPI_MPICC_EXE}  )
SET( CMAKE_CXX_COMPILER ${MPI_MPICXX_EXE} )

