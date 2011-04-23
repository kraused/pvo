
# vi: tabstop=4:expandtab

SET ( SRC "" )

IF( NOT TPL_ENABLE_MPI )
    LIST( APPEND SRC
          pvo_mpi_stubs_f.F90 )
ENDIF()
LIST( APPEND SRC
      pvo_f.c
      pvo_f.F90 )

ADD_LIBRARY( pvof ${SRC} )

