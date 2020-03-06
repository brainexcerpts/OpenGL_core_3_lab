find_package(Doxygen)

if (NOT DOXYGEN_FOUND)
    message(WARNING "Doxygen is needed to build the documentation.")
else()
    # this will run doxygen at compilation time (every single time we run 'make')
#    add_custom_target(
#        user_documentation ALL
#        ${PROJECT_SOURCE_DIR}/00_INSTRUCTIONS/build_doc.sh
#        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/00_INSTRUCTIONS
#        COMMENT "Generating API documentation with Doxygen"
#     )

    # this will run doxygen at compilation time (every single time we run 'make')
   SET( doxy_html_index_file
        ${PROJECT_SOURCE_DIR}/00_INSTRUCTIONS/html_english/html/index.html
        ${PROJECT_SOURCE_DIR}/00_INSTRUCTIONS/html_french/html/index.html
        )

    file(GLOB_RECURSE
         dependency_files
         ${PROJECT_SOURCE_DIR}/00_INSTRUCTIONS/lab_instructions/*.h
         ${PROJECT_SOURCE_DIR}/src/fileloaders/*.cpp
         ${PROJECT_SOURCE_DIR}/src/fileloaders/*.h
         ${PROJECT_SOURCE_DIR}/src/gl_utils/*.cpp
         ${PROJECT_SOURCE_DIR}/src/gl_utils/*.h
         ${PROJECT_SOURCE_DIR}/src/qt_gui/*.cpp
         ${PROJECT_SOURCE_DIR}/src/qt_gui/*.h
        )

   ADD_CUSTOM_COMMAND(
     OUTPUT ${doxy_html_index_file}
     COMMAND ${PROJECT_SOURCE_DIR}/00_INSTRUCTIONS/build_doc.sh
     WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/00_INSTRUCTIONS
     MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/00_INSTRUCTIONS/doxygen_config.txt
     DEPENDS ${dependency_files}
     COMMENT "Generating API documentation with Doxygen"
   )

   ADD_CUSTOM_TARGET( doc ALL DEPENDS ${doxy_html_index_file} )


endif()
