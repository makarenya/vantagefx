cmake_minimum_required(VERSION 3.3)
project(vantagefx)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (MSVC)
    set(CMAKE_PREFIX_PATH "c:/msvc_2015")
    set(Boost_USE_STATIC_LIBS ON) # only find static libs
	set(MAKE_GUI "1")
    add_definitions(-D_WIN32_WINNT=0x0502 -DWINVER=0x0502)
    add_definitions(-D_UNICODE -DUNICODE)
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif ()
if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "-mwindows -municode")
endif ()

if (MAKE_GUI) 
    set(CMAKE_EXE_LINKER_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:\"wWinMainCRTStartup\"")
    add_definitions(-DGUI_MAIN)
endif()

add_definitions(-DDATA_DIR="${CMAKE_SOURCE_DIR}/data/")

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)


# Find the QtWidgets library
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Boost 1.55 REQUIRED COMPONENTS date_time filesystem regex atomic system locale)
find_package(OpenSSL 1.0.2 REQUIRED)

set(SOURCE_FILES
        src/main.cpp
        src/model/MainModel.cpp
        src/model/MainModel.h
        src/viewmodel/MainViewModel.cpp
        src/viewmodel/MainViewModel.h
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
        src/api/GwtRequestParser.cpp
        src/api/GwtRequestParser.h
        src/api/GwtIterator.cpp
        src/api/GwtIterator.h
        src/api/GwtPathExpression.cpp
        src/api/GwtPathExpression.h
        src/http/HttpContext.cpp
        src/http/HttpContext.h
        src/http/HttpRequest.cpp
        src/http/HttpRequest.h
        src/http/HttpResponse.cpp
        src/http/HttpResponse.h
        src/http/HttpResponseParser.cpp
        src/http/HttpResponseParser.h
        src/http/HttpConnection.cpp
        src/http/HttpConnection.h
        src/analyzer/FiddlerLogEntry.cpp
        src/analyzer/FiddlerLogEntry.h
        src/analyzer/FiddlerLogParser.cpp
        src/analyzer/FiddlerLogParser.h
        src/analyzer/GwtAnalyzer.cpp
        src/analyzer/GwtAnalyzer.h
        src/http/Url.cpp
        src/http/Url.h
        src/GwtHttpRequest.cpp
        src/GwtHttpRequest.h
        src/viewmodel/VantageFxService.cpp
        src/viewmodel/VantageFxService.h
        src/viewmodel/OptionsListModel.cpp
        src/viewmodel/OptionsListModel.h
        src/model/OptionModel.cpp
        src/model/OptionModel.h
        src/api/GwtHttpRequestContext.cpp
        src/api/GwtHttpRequestContext.h
        src/viewmodel/ComboBoxModel.cpp
        src/viewmodel/ComboBoxModel.h
        src/model/VantageFxModel.cpp
		src/model/VantageFxModel.h
		src/model/AssetModel.cpp
		src/model/AssetModel.h
		src/model/CurrentSettings.cpp
		src/model/CurrentSettings.h
		src/model/TransactionModel.cpp
		src/model/TransactionModel.h
		)


qt5_add_resources(RESOURCES layout/qml.qrc)
add_executable(vantagefx ${SOURCE_FILES} ${RESOURCES})
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(vantagefx Qt5::Core Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Xml ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES})
if(WIN32)
    target_link_libraries(vantagefx wsock32 ws2_32 Shell32 Winhttp)
    add_definitions(-D_WIN32_WINNT=0x0501)
endif()