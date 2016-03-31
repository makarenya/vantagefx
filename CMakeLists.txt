cmake_minimum_required(VERSION 3.3)
project(vantagefx)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (MSVC)
    set(CMAKE_PREFIX_PATH "c:/msvc_2015")
    set(Boost_USE_STATIC_LIBS ON) # only find static libs
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif ()

add_definitions(-DDATA_DIR="${CMAKE_SOURCE_DIR}/data/")

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)


# Find the QtWidgets library
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Boost 1.55 REQUIRED COMPONENTS date_time filesystem system)

set(SOURCE_FILES
        src/main.cpp
        src/model/MainModel.cpp
        src/model/MainModel.h
        src/model/PersonModel.cpp
        src/model/PersonModel.h
        src/viewmodel/MainViewModel.cpp
        src/viewmodel/MainViewModel.h
        src/viewmodel/PersonViewModel.cpp
        src/viewmodel/PersonViewModel.h
        src/api/GwtRpcResponse.cpp
        src/api/GwtRpcResponse.h
        src/api/GwtResponseData.h
        src/api/GwtResponseParser.cpp
        src/api/GwtResponseParser.h
        src/api/GwtResponseData.h
        src/api/GwtRpcRequest.cpp
        src/api/GwtRpcRequest.h
        src/api/GwtParseContext.cpp
        src/api/GwtParseContext.h
        src/api/GwtReflectedType.h
        src/api/ParseError.h
        src/api/GwtObjectWrapper.h
        src/api/CheckedType.h
        src/api/GwtType.cpp
        src/api/GwtType.h
        src/api/GwtField.cpp
        src/api/GwtField.h
        src/api/GwtObject.cpp
        src/api/GwtObject.h
        src/api/GwtParser.cpp
        src/api/GwtParser.h
        src/api/GwtValue.cpp
        src/api/GwtValue.h
        src/api/GwtBundle.cpp
        src/api/GwtBundle.h
        src/api/FiddlerLogEntry.cpp
        src/api/FiddlerLogEntry.h
        src/api/FiddlerLogParser.cpp
        src/api/FiddlerLogParser.h
        src/api/GwtRequestParser.cpp
        src/api/GwtRequestParser.h
        )


qt5_add_resources(RESOURCES layout/qml.qrc)
add_executable(vantagefx ${SOURCE_FILES} ${RESOURCES})
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(vantagefx Qt5::Core Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Xml ${Boost_LIBRARIES})
