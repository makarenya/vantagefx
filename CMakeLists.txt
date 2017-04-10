cmake_minimum_required(VERSION 3.3)
project(vantagefx)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (MSVC)
    set(CMAKE_PREFIX_PATH "c:/msvc_2017")
    set(BOOST_ROOT ${CMAKE_PREFIX_PATH})
    set(Boost_USE_STATIC_LIBS ON) # only find static libs
    add_definitions(-D_WIN32_WINNT=0x0502 -DWINVER=0x0502)
    add_definitions(-D_UNICODE -DUNICODE)
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif ()


set(MAKE_GUI "1")

if (MAKE_GUI)
	if (MSVC)
		set(CMAKE_EXE_LINKER_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:\"wWinMainCRTStartup\"")
		add_definitions(-DGUI_MAIN)
	endif()
	if (MINGW)
		set(CMAKE_EXE_LINKER_FLAGS "-mwindows -municode")
		add_definitions(-DGUI_MAIN)
	endif ()
endif()

if (MINGW)
	add_definitions(-DBOOST_USE_WINDOWS_H)
endif()

if (APPLE)
    set(OPENSSL_ROOT_DIR /opt/local)
endif()

add_definitions(-DDATA_DIR="${CMAKE_SOURCE_DIR}/res/")

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

# Find the QtWidgets library
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5OpenGLExtensions REQUIRED)
find_package(Boost 1.55 REQUIRED COMPONENTS date_time filesystem regex atomic system locale)
find_package(OpenSSL 1.0.2 REQUIRED)
find_package(log4cplus REQUIRED)


set(SOURCE_FILES
        src/main.cpp
        src/GwtHttpRequest.cpp
        src/GwtHttpRequest.h
		)

set(SOURCE_FILES_API 
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
		src/api/GwtHttpRequestContext.cpp
		src/api/GwtHttpRequestContext.h
		)

set(SOURCE_FILES_HTTP
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
        src/http/Url.cpp
        src/http/Url.h
		)

set(SOURCE_FILES_ANALYZER
        src/analyzer/FiddlerLogEntry.cpp
        src/analyzer/FiddlerLogEntry.h
        src/analyzer/FiddlerLogParser.cpp
        src/analyzer/FiddlerLogParser.h
        src/analyzer/GwtAnalyzer.cpp
        src/analyzer/GwtAnalyzer.h
		)

set(SOURCE_FILES_VIEWMODEL
		src/viewmodel/MainViewModel.cpp
		src/viewmodel/MainViewModel.h
        src/viewmodel/VantageFxService.cpp
        src/viewmodel/VantageFxService.h
        src/viewmodel/OptionsListModel.cpp
        src/viewmodel/OptionsListModel.h
		src/viewmodel/AssetsViewModel.cpp
		src/viewmodel/AssetsViewModel.h
		src/viewmodel/AssetListModel.cpp
		src/viewmodel/AssetListModel.h
		src/viewmodel/OptionItem.cpp
		src/viewmodel/OptionItem.h
		src/viewmodel/ComboBoxModel.cpp
		src/viewmodel/ComboBoxModel.h
		)

set(SOURCE_FILES_MODEL
        src/model/OptionModel.cpp
        src/model/OptionModel.h
        src/model/VantageFxModel.cpp
		src/model/VantageFxModel.h
		src/model/AssetModel.cpp
		src/model/AssetModel.h
		src/model/CurrentSettings.cpp
		src/model/CurrentSettings.h
		src/model/TransactionModel.cpp
		src/model/TransactionModel.h
		)

set(SOURCE_FILES_CONTROLLER
		src/controller/StatisticRenderer.cpp
        src/controller/StatisticRenderer.h
		)

source_group("" FILES ${SOURCE_FILES} ${RESOURCES})
source_group("api" FILES ${SOURCE_FILES_API})
source_group("http" FILES ${SOURCE_FILES_HTTP})
source_group("analyzer" FILES ${SOURCE_FILES_ANALYZER})
source_group("viewmodel" FILES ${SOURCE_FILES_VIEWMODEL})
source_group("model" FILES ${SOURCE_FILES_MODEL})
source_group("controller" FILES ${SOURCE_FILES_CONTROLLER})

qt5_add_resources(RESOURCES layout/qml.qrc)
add_executable(vantagefx 
	${SOURCE_FILES} 
	${SOURCE_FILES_API}
	${SOURCE_FILES_HTTP}
	${SOURCE_FILES_ANALYZER}
	${SOURCE_FILES_VIEWMODEL}
	${SOURCE_FILES_MODEL}
	${SOURCE_FILES_CONTROLLER}
	${RESOURCES}
	)
include_directories(. ${Boost_INCLUDE_DIRS})
target_link_libraries(vantagefx Qt5::Core Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Xml log4cplus::log4cplusU ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES})
if(WIN32)
    target_link_libraries(vantagefx wsock32 ws2_32 Shell32 Winhttp)
    add_definitions(-D_WIN32_WINNT=0x0501)
endif()