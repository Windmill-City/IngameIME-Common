
#[[
Add a new IngameIME C++ Library target

@param Target - Target name of the library
@param Type - Library type<SHARED|STATIC|MODULE>
@param Sources - Source files
]]
function(IngameIME_add_library Target Type)
    message(STATUS "Adding ${Target} as ${Type} Library with Sources: ${ARGN}")
    add_library(${Target} ${Type} ${ARGN})
    target_include_directories(${Target} PUBLIC ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/src/include)
endfunction()

#[[
Add a new IngameIME SWIG language binding library target

Currently supported language:
- CSharp
- Java
- Python

@param Target - Target name of the library
@param Type - Library type<SHARED|STATIC|MODULE>
@param Language - Target language of the binding
@param Sources - Source files

@note The path of the generated binding files can be retrieved from the SWIG_SUPPORT_FILES_DIRECTORY target property.
@code{
    get_property(support_files_dir TARGET targetName PROPERTY SWIG_SUPPORT_FILES_DIRECTORY)
}
]]
function(IngameIME_add_swig_library Target Type Language)
    message(STATUS "Adding ${Target} as SWIG ${Type} Library for Language ${Language} with Sources: ${ARGN}")
    find_package(SWIG 4.0.2 REQUIRED)
    set(UseSWIG_MODULE_VERSION 2)

    INCLUDE(${SWIG_USE_FILE})
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/src/IngameIME.i PROPERTIES CPLUSPLUS ON)

    # Language Name must in lower case
    string(TOLOWER ${Language} ${Language})

    swig_add_library(${Target} TYPE ${Type} LANGUAGE ${Language} SOURCES ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/src/IngameIME.i)

    set_property(TARGET ${Target} PROPERTY SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON)
    target_include_directories(${Target} PUBLIC ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/src/include)
    target_sources(${Target} PRIVATE ${ARGN})
    
    # Log about support files
    get_property(SUPPORT_FILES_DIR TARGET ${Target} PROPERTY SWIG_SUPPORT_FILES_DIRECTORY)
    message(STATUS "[${Target}] Support files will be place at: ${SUPPORT_FILES_DIR}")
    
    #Java Binding
    if(${Language} STREQUAL "java")
        find_package(JNI REQUIRED)

        target_include_directories(${Target} PUBLIC ${JNI_INCLUDE_DIRS})
        target_link_directories(${Target} PUBLIC ${JNI_LIBRARIES})
        # Generate document in generated codes
        set_property(TARGET ${Target} PROPERTY SWIG_COMPILE_OPTIONS -doxygen)
    endif()

    #Python Binding
    if(${Language} STREQUAL "python")
        find_package (Python3 REQUIRED COMPONENTS Interpreter Development)

        target_include_directories(${Target} PUBLIC ${Python3_INCLUDE_DIRS})
        target_link_directories(${Target} PUBLIC ${Python3_LIBRARIES})
        # Generate document in generated codes
        set_property(TARGET ${Target} PROPERTY SWIG_COMPILE_OPTIONS -doxygen)
    endif()
endfunction()