
INCLUDE( CheckIncludeFiles )
INCLUDE( TestBigEndian )

# Include files
IF( TPL_ENABLE_MPI )
	CHECK_INCLUDE_FILES( mpi.h HAVE_MPI_H )
ENDIF()
CHECK_INCLUDE_FILES( unistd.h HAVE_UNISTD_H )

# Endianess
TEST_BIG_ENDIAN( HAVE_SYSTEM_BE )
IF( NOT HAVE_SYSTEM_BE )
	SET( HAVE_SYSTEM_LE 1 CACHE INTERNAL "Result of endianess test" FORCE )
ENDIF()

IF( TPL_ENABLE_MPI )
	SET( HAVE_MPI 1 )
ENDIF()

# The default transport layer
IF( TPL_ENABLE_MPI )
	SET( PVO_DEFAULT_LOW_IO_LAYER "PVO_LOW_IO_MPI"    CACHE INTERNAL "Default low I/O layer" )
ELSE()
	SET( PVO_DEFAULT_LOW_IO_LAYER "PVO_LOW_IO_SINGLE" CACHE INTERNAL "Default low I/O layer" )
ENDIF()

CONFIGURE_FILE( pvo_config.h.in
                pvo_config.h )

