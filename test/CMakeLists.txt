
# Macro to create a new test
MACRO( PVO_ADD_TEST FILE )
	STRING( REGEX REPLACE "[.](c|cxx|cc)" "" TARGET "${FILE}" )
	SET   ( TARGET "test_${TARGET}" )

	ADD_EXECUTABLE       ( ${TARGET} ${FILE} )
	TARGET_LINK_LIBRARIES( ${TARGET} pvo )
	ADD_TEST             ( ${TARGET} ${EXECUTABLE_OUTPUT_PATH}/${TARGET} )

ENDMACRO()

PVO_ADD_TEST( pvo_low_io.c )
PVO_ADD_TEST( pvo_island.c )
PVO_ADD_TEST( pvo_xml_file.c )
# PVO_ADD_TEST( pvo_cookie.c )
# PVO_ADD_TEST( pvo_file.c )

