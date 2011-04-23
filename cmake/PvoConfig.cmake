
# vi: tabstop=4:expandtab:hlsearch

INCLUDE( CheckIncludeFiles )
INCLUDE( TestBigEndian )

# Include files
IF( TPL_ENABLE_MPI )
    CHECK_INCLUDE_FILES( mpi.h PVO_HAVE_MPI_H )
ENDIF()
CHECK_INCLUDE_FILES( unistd.h PVO_HAVE_UNISTD_H )

# Endianess
TEST_BIG_ENDIAN( PVO_HAVE_SYSTEM_BE )
IF( NOT PVO_HAVE_SYSTEM_BE )
    SET( PVO_HAVE_SYSTEM_LE 1 CACHE INTERNAL "Result of endianess test" FORCE )
ENDIF()

IF( TPL_ENABLE_MPI )
    SET( PVO_HAVE_MPI 1 )
ENDIF()

# The default transport layer
IF( TPL_ENABLE_MPI )
    SET( PVO_DEFAULT_LOW_IO_LAYER "PVO_LOW_IO_MPI"    CACHE INTERNAL "Default low I/O layer" )
ELSE()
    SET( PVO_DEFAULT_LOW_IO_LAYER "PVO_LOW_IO_SINGLE" CACHE INTERNAL "Default low I/O layer" )
ENDIF()

# Fortran naming scheme
IF( PVO_ENABLE_FORTRAN )
    # FIXME Fixed for now. This is fine for many environments so it's okay to start
    #       with those but eventually we should move to running real test. However, we
    #       need to make sure that we don't run into problems on system with cross-
    #       compilers (Cray a bit, especially BlueGene)
    SET( PVO_FORTRAN_LC         1 )
    SET( PVO_FORTRAN_UNDERSCORE 1 )
ENDIF()

CONFIGURE_FILE( pvo_config.h.in
                pvo_config.h )

