
SET ( SRC "" )
LIST( APPEND SRC
      pvo_f.c
      pvo_f.f90 )

ADD_LIBRARY( pvof ${SRC} )

