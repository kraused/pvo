
# vi: tabstop=4:expandtab:hlsearch

# Macro to create a new benchmark
MACRO( PVO_ADD_C_BENCH FILE )
    STRING( REGEX REPLACE "[.](c|cxx|cc)" "_c" TARGET "${FILE}"   )

	ADD_EXECUTABLE       ( ${TARGET} ${FILE} )
	TARGET_LINK_LIBRARIES( ${TARGET} pvo m )
ENDMACRO()

MACRO( PVO_ADD_F_BENCH FILE )
    STRING( REGEX REPLACE "[.](F90|f)" "_f" TARGET "${FILE}" )

    ADD_EXECUTABLE       ( ${TARGET} ${FILE} )
    # This is needed to find the pvo module file
    INCLUDE_DIRECTORIES  ( ${CMAKE_BINARY_DIR}/fortran )
    TARGET_LINK_LIBRARIES( ${TARGET} pvof pvo )
ENDMACRO()


PVO_ADD_C_BENCH( rand.c )
PVO_ADD_C_BENCH( chunk.c )

IF( PVO_ENABLE_FORTRAN )
    PVO_ADD_F_BENCH( rand.F90 )
    PVO_ADD_F_BENCH( chunk.F90 )
ENDIF()


