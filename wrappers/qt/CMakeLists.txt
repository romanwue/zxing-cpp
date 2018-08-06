if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    # this is the top level project
    cmake_minimum_required (VERSION 2.8.12)

    project (ZXingQt)

    set (ENABLE_ENCODERS OFF CACHE BOOL "Check to include encoders")
    set (ENABLE_DECODERS ON CACHE BOOL "Check to include decoders")
    set (LINK_CPP_STATICALLY OFF CACHE BOOL "MSVC only, check to link statically standard library (/MT and /MTd)")

    add_definitions (-DUNICODE -D_UNICODE)
    
    if (MSVC AND LINK_CPP_STATICALLY)
        set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
        set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
        set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
        set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    endif()
endif()

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

set(CMAKE_AUTOMOC ON)

if (NOT DEFINED ENABLE_ENCODERS)
    set (ENABLE_ENCODERS OFF)
endif()

if (NOT DEFINED ENABLE_DECODERS)
    set (ENABLE_DECODERS ON)
endif()

add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/../../core ${CMAKE_BINARY_DIR}/ZXingCore)

set (LOCAL_COMMON_FILES
    #GdiplusInit.h
    #GdiplusInit.cpp
)
if (ENABLE_DECODERS)
    set (DECODERS_FILES
        BarcodeReader.h
        BarcodeReader.cpp
    )
endif()
if (ENABLE_ENCODERS)
	set (ENCODERS_FILES
		ImageWriter.h
		ImageWriter.cpp
        BarcodeGenerator.h
        BarcodeGenerator.cpp
	)
endif()

source_group (Sources FILES
    ${LOCAL_COMMON_FILES}
	${DECODERS_FILES}
	${ENCODERS_FILES}
)

add_library (ZXingQt
    ${LOCAL_COMMON_FILES}
    ${DECODERS_FILES}
    ${ENCODERS_FILES}
)

target_include_directories (ZXingQt
    PUBLIC .
)

target_link_libraries (ZXingQt
    PRIVATE ZXingCore Qt5::Core Qt5::Gui #gdiplus.lib 
)

target_compile_options (ZXingQt
    PUBLIC -DGDIPVER=0x0110
)

