cmake_minimum_required(VERSION 3.21.1)
project(IngameIME-Common VERSION 0.1.0)

set(IngameIME_PUBLIC_HEADER ${IngameIME_PUBLIC_HEADER} ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
set(IngameIME_PRIVATE_HEADER ${IngameIME_PRIVATE_HEADER})

set(IngameIME_SOURCE ${IngameIME_SOURCE})
set(IngameIME_PUBLIC_SOURCES ${IngameIME_PUBLIC_SOURCES})
set(IngameIME_PRIVATE_SOURCES ${IngameIME_PRIVATE_HEADER})

set(IngameIME_PUBLIC_LIBRARIES ${IngameIME_PUBLIC_LIBRARIES})
set(IngameIME_PRIVATE_LIBRARIES ${IngameIME_PRIVATE_LIBRARIES})

# Target
add_library(IngameIME_Static STATIC ${IngameIME_SOURCE})
target_include_directories(IngameIME_Static PUBLIC ${IngameIME_PUBLIC_HEADER})
target_include_directories(IngameIME_Static PRIVATE ${IngameIME_PRIVATE_HEADER})
target_link_directories(IngameIME_Static PUBLIC ${IngameIME_PUBLIC_LIBRARIES})
target_link_directories(IngameIME_Static PRIVATE ${IngameIME_PRIVATE_LIBRARIES})
target_sources(IngameIME_Static PUBLIC ${IngameIME_PUBLIC_SOURCES})
target_sources(IngameIME_Static PRIVATE ${IngameIME_PRIVATE_SOURCES})

add_library(IngameIME_Shared SHARED ${IngameIME_SOURCE})
target_include_directories(IngameIME_Shared PUBLIC ${IngameIME_PUBLIC_HEADER})
target_include_directories(IngameIME_Shared PRIVATE ${IngameIME_PRIVATE_HEADER})
target_link_directories(IngameIME_Shared PUBLIC ${IngameIME_PUBLIC_LIBRARIES})
target_link_directories(IngameIME_Shared PRIVATE ${IngameIME_PRIVATE_LIBRARIES})
target_sources(IngameIME_Shared PUBLIC ${IngameIME_PUBLIC_SOURCES})
target_sources(IngameIME_Shared PRIVATE ${IngameIME_PRIVATE_SOURCES})

find_package(SWIG)

if(SWIG_FOUND)
    INCLUDE(${SWIG_USE_FILE})
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src/include)
    SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/src/IngameIME.i PROPERTIES CPLUSPLUS ON)

    #Java Binding
    find_package(JNI)
    if(JNI_FOUND)
        swig_add_library(IngameIME_Java LANGUAGE java SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/IngameIME.i)
        target_include_directories(IngameIME_Java PUBLIC ${IngameIME_PUBLIC_HEADER} ${JNI_INCLUDE_DIRS})
        target_include_directories(IngameIME_Java PRIVATE ${IngameIME_PRIVATE_HEADER})
        target_link_directories(IngameIME_Java PUBLIC ${IngameIME_PUBLIC_LIBRARIES} ${JNI_LIBRARIES})
        target_link_directories(IngameIME_Java PRIVATE ${IngameIME_PRIVATE_LIBRARIES})
        target_sources(IngameIME_Java PUBLIC ${IngameIME_PUBLIC_SOURCES})
        target_sources(IngameIME_Java PRIVATE ${IngameIME_PRIVATE_SOURCES})
        # Generate document in java codes
        set_property(TARGET IngameIME_Java PROPERTY SWIG_COMPILE_OPTIONS -doxygen)
    else(JNI_FOUND)
        message(WARNING "No JDK found skip Java binding generation")
    endif(JNI_FOUND)
    
    #CSharp Binding
    swig_add_library(IngameIME_CSharp LANGUAGE csharp SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/IngameIME.i)
    target_include_directories(IngameIME_CSharp PUBLIC ${IngameIME_PUBLIC_HEADER})
    target_include_directories(IngameIME_CSharp PRIVATE ${IngameIME_PRIVATE_HEADER})
    target_link_directories(IngameIME_CSharp PUBLIC ${IngameIME_PUBLIC_LIBRARIES})
    target_link_directories(IngameIME_CSharp PRIVATE ${IngameIME_PRIVATE_LIBRARIES})
    target_sources(IngameIME_CSharp PUBLIC ${IngameIME_PUBLIC_SOURCES})
    target_sources(IngameIME_CSharp PRIVATE ${IngameIME_PRIVATE_SOURCES})

    #Python Binding
    find_package (Python3 COMPONENTS Interpreter Development)
    if(Python3_FOUND)
        swig_add_library(IngameIME_Python LANGUAGE python SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/IngameIME.i)
        target_include_directories(IngameIME_Python PUBLIC ${IngameIME_PUBLIC_HEADER} ${Python3_INCLUDE_DIRS})
        target_include_directories(IngameIME_Python PRIVATE ${IngameIME_PRIVATE_HEADER})
        target_link_directories(IngameIME_Python PUBLIC ${IngameIME_PUBLIC_LIBRARIES} ${Python3_LIBRARIES})
        target_link_directories(IngameIME_Python PRIVATE ${IngameIME_PRIVATE_LIBRARIES})
        target_sources(IngameIME_Python PUBLIC ${IngameIME_PUBLIC_SOURCES})
        target_sources(IngameIME_Python PRIVATE ${IngameIME_PRIVATE_SOURCES})
        # Generate document in python codes
        set_property(TARGET IngameIME_Python PROPERTY SWIG_COMPILE_OPTIONS -doxygen)
    else(Python3_FOUND)
        message(WARNING "No Python3 environment found skip Python3 binding generation")
    endif(Python3_FOUND)
endif(SWIG_FOUND)

# Install
install(TARGETS IngameIME_Static IngameIME_Shared)
install(DIRECTORY ${IngameIME_PUBLIC_HEADER} TYPE INCLUDE)

# Package
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "Windmill_City")
set(CPACK_GENERATOR "ZIP")
include(CPack)