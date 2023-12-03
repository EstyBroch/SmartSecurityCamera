cmake_minimum_required(VERSION 3.16)
project(ImageProcessingTest)
set(PROJECT_NAME ImageProcessingTest)
set(SOURCES test.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
list(APPEND CMAKE_PREFIX_PATH ${OpenCV_BIN_DIR})
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(OpenCV_DIR "C:/opencv/build")
endif()

# Find OpenCV package
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Catch2
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(Catch2_DIR C:/Catch2/build/x64)
endif()
find_package(Catch2 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Cath2_INCLUDE_DIRS})

# Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${ADDITIONAL_LIBRARY_DEPENDENCIES}
    ${OpenCV_LIBS}
    Catch2::Catch2WithMain
    #${CURSES_LIBRARIES}
    $<$<BOOL:${MINGW}>:ws2_32>
    ImageProject
    opencv_dnn
)

# Compile definitions
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

# Compile and link options (if needed)
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /std:c++17;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /std:c++17;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()
