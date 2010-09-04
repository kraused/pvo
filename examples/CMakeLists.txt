
# Macro to create a new benchmark
MACRO( PVO_ADD_BENCH FILE )
	STRING( REGEX REPLACE "[.](c|cxx|cc)" "" TARGET "${FILE}" )

	ADD_EXECUTABLE       ( ${TARGET} ${FILE} )
	TARGET_LINK_LIBRARIES( ${TARGET} pvo )
	ADD_TEST             ( ${TARGET} ${EXECUTABLE_OUTPUT_PATH}/${TARGET} )

ENDMACRO()

PVO_ADD_BENCH( rand.c )

