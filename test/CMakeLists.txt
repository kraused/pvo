
# vi: tabstop=4:expandtab:hlsearch

# Macro to create a new test
MACRO( PVO_ADD_C_TEST FILE )
	STRING( REGEX REPLACE "[.](c|cxx|cc)" "_c" TARGET "${FILE}"   )
	SET   ( TARGET "test_${TARGET}" )

	ADD_EXECUTABLE       ( ${TARGET} ${FILE} )
	TARGET_LINK_LIBRARIES( ${TARGET} pvo )
	ADD_TEST             ( ${TARGET} ${EXECUTABLE_OUTPUT_PATH}/${TARGET} )

ENDMACRO()

MACRO( PVO_ADD_F_TEST FILE )
    STRING( REGEX REPLACE "[.](F90|f)" "_f" TARGET "${FILE}" )
    SET   ( TARGET "test_${TARGET}" )

    ADD_EXECUTABLE       ( ${TARGET} ${FILE} )
    TARGET_LINK_LIBRARIES( ${TARGET} pvof pvo )
    ADD_TEST             ( ${TARGET} ${EXECUTABLE_OUTPUT_PATH}/${TARGET} )

ENDMACRO()


PVO_ADD_C_TEST( pvo_low_io.c )
PVO_ADD_C_TEST( pvo_island.c )
PVO_ADD_C_TEST( pvo_xml_file.c )
PVO_ADD_C_TEST( pvo_cookie.c )
PVO_ADD_C_TEST( pvo_file.c )
PVO_ADD_C_TEST( pvo_vtu_file.c )
PVO_ADD_C_TEST( pvo_vtp_file.c )
PVO_ADD_C_TEST( pvo_vti_file.c )

IF( PVO_ENABLE_FORTRAN )
    PVO_ADD_F_TEST( pvo_init.F90 )
    PVO_ADD_F_TEST( pvo_cookie.F90 )
    PVO_ADD_F_TEST( pvo_file.F90 )
    PVO_ADD_F_TEST( pvo_vtu_file.F90 )
    PVO_ADD_F_TEST( pvo_vtp_file.F90 )
    PVO_ADD_F_TEST( pvo_vti_file.F90 )
ENDIF()

