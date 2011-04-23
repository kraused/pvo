
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
    STRING( REGEX REPLACE "[.](f90|f)" "_f" TARGET "${FILE}" )
    SET   ( TARGET "test_${TARGET}" )

    ADD_EXECUTABLE       ( ${TARGET} ${FILE} )
    # This is needed to find the pvo module file
    INCLUDE_DIRECTORIES  ( ${CMAKE_BINARY_DIR}/fortran )
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

IF( PVO_ENABLE_FORTRAN )
    PVO_ADD_F_TEST( pvo_init.f90 )
    PVO_ADD_F_TEST( pvo_cookie.f90 )
    PVO_ADD_F_TEST( pvo_file.f90 )
    PVO_ADD_F_TEST( pvo_vtu_file.f90 )
    PVO_ADD_F_TEST( pvo_vtp_file.f90 )
ENDIF()

