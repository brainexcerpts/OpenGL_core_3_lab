find_package(Doxygen)

if (NOT DOXYGEN_FOUND)
    message(WARNING "Doxygen is needed to build the documentation.")
else()
    # this will run doxygen at compilation time
    add_custom_target(
        user_documentation ALL
        ${PROJECT_SOURCE_DIR}/00_INSTRUCTIONS/build_doc.sh
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/00_INSTRUCTIONS
     )

endif()
