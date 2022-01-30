cmake_minimum_required(VERSION 3.21.1)
project(IngameIME-Common VERSION 0.1.0)

FIND_PACKAGE(SWIG)

if(SWIG_FOUND)
    INCLUDE(${SWIG_USE_FILE})
    INCLUDE_DIRECTORIES(src/include)
    SET_SOURCE_FILES_PROPERTIES(src/IngameIME.i PROPERTIES CPLUSPLUS ON)

    #Java Binding
    find_package(JNI)
    if(JNI_FOUND)
        swig_add_library(IngameIME_Java LANGUAGE java SOURCES src/IngameIME.i)
        target_include_directories(IngameIME_Java PUBLIC ${JNI_INCLUDE_DIRS})
        target_link_libraries(IngameIME_Java ${JNI_LIBRARIES})
        # Generate document in java codes
        set_property(TARGET IngameIME_Java PROPERTY SWIG_COMPILE_OPTIONS -doxygen)
    else(JNI_FOUND)
        message(WARNING "No JDK found skip Java binding generation")
    endif(JNI_FOUND)
    
    #CSharp Binding
    swig_add_library(IngameIME_CSharp LANGUAGE csharp SOURCES src/IngameIME.i)

    #Python Binding
    find_package (Python3 COMPONENTS Interpreter Development)
    if(Python3_FOUND)
        swig_add_library(IngameIME_Python LANGUAGE python SOURCES src/IngameIME.i)
        target_include_directories(IngameIME_Python PUBLIC ${Python3_INCLUDE_DIRS})
        target_link_libraries(IngameIME_Python ${Python3_LIBRARIES})
        # Generate document in python codes
        set_property(TARGET IngameIME_Python PROPERTY SWIG_COMPILE_OPTIONS -doxygen)
    else(Python3_FOUND)
        message(WARNING "No Python3 environment found skip Python3 binding generation")
    endif(Python3_FOUND)
endif(SWIG_FOUND)