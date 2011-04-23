
# vi: tabstop=4:expandtab:hlsearch

# Macro to create a new benchmark
MACRO( PVO_ADD_BENCH FILE )
    STRING( REGEX REPLACE "[.](c|cxx|cc)" "_c" T "${FILE}"   )
    STRING( REGEX REPLACE "[.](f|f90)"    "_f" TARGET "${T}" )

	ADD_EXECUTABLE       ( ${TARGET} ${FILE} )
	TARGET_LINK_LIBRARIES( ${TARGET} pvo )

ENDMACRO()

PVO_ADD_BENCH( rand.c )
PVO_ADD_BENCH( chunk.c )

