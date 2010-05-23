
#vim: tabstop=4:expandtab:hlsearch

PROJECT( pvo NONE )
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

# ------------------------------------------------------------------------------
# Enable MPI
# ------------------------------------------------------------------------------
# On my local system I don't need to use the wrapper explicitly
# SET( CMAKE_C_COMPILERR  "mpicc"  )
# SET( CMAKE_CXX_COMPILER "mpic++" )

ENABLE_LANGUAGE( C )
ENABLE_LANGUAGE( CXX )

# Soure files
SET ( SRC "" )
LIST( APPEND SRC
      pvo_report.c
      pvo_memory.c
      pvo_init.c
      pvo_quit.c
      pvo_physical_node_uid.c
      pvo_MPI.c
      pvo_low_io.c
      pvo_low_io_single.c
      pvo_low_io_MPI.c
      pvo_low_io_posix.c
      pvo_island.c
      pvo_xml_file.c )

#      pvo_MPI.c
#      pvo_report.c
#      pvo_memory.c
#      pvo_low_io_MPI.c
#      pvo_init.c
#      pvo_quit.c
#      pvo_island.c
#      pvo_xml_file.c
#      pvo_var.c
#      pvo_cookie.c
#      pvo_file.c )

# Temporary
ADD_DEFINITIONS( -DPVO_USE_MPI_IO=1 -O0 -ggdb )

ADD_LIBRARY( pvo ${SRC} )


# Testing
ENABLE_TESTING   ()
ADD_CUSTOM_TARGET( check COMMAND ${CMAKE_CTEST_COMMAND} )
 
ADD_SUBDIRECTORY ( test )

