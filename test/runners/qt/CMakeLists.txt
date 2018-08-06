cmake_minimum_required (VERSION 2.8.9)

project (ZXingTest)

set (ENABLE_ENCODERS OFF CACHE BOOL "Check to include encoders")
set (ENABLE_DECODERS ON CACHE BOOL "Check to include decoders")

add_definitions (-DUNICODE -D_UNICODE)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

set(CMAKE_AUTOMOC ON)

if (MSVC)
    set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Oi /GS-")
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Oi /GS-")
else()
    set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
    if (APPLE)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
    elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    endif()
endif()

add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/../../../wrappers/qt ${CMAKE_BINARY_DIR}/ZXingQt)

if (ENABLE_DECODERS)
    #set(EXECUTABLE_FLAG )
    #if(WIN32)
    #    set(EXECUTABLE_FLAG WIN32)
    #endif()

    add_executable (QtReaderTest ${EXECUTABLE_FLAG}
        TestReaderMain
    )

    target_link_libraries (QtReaderTest PRIVATE Qt5::WinMain Qt5::Core Qt5::Gui ZXingQt)
endif()

if (ENABLE_ENCODERS)
    add_executable (QtWriterTest
        TestWriterMain.cpp
    )

	target_include_directories (QtWriterTest
		PRIVATE ../../common
	)

    #set_target_properties (WriterTest PROPERTIES
    #   LINK_FLAGS "/link setargv.obj"
    #)

    target_link_libraries (QtWriterTest PRIVATE Qt5::WinMain Qt5::Core Qt5::Gui ZXingQt)
endif()
