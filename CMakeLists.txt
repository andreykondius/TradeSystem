cmake_minimum_required(VERSION 3.0) 
project(TradeSystem)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Core Network Widgets Gui REQUIRED)
if (Qt6Core_FOUND AND Qt6Network_FOUND AND Qt6Widgets_FOUND)
    set(QT6_FOUND true)
        message("-- Package Qt6Core and Qt6Network and Qt6Widgets was found !!!")
	else ()
        message("-- Package Qt6Core or Qt6etwork or Qt6Widgets was not found")
		set(QT6_FOUND false)
endif()

set(CMAKE_CXX_STANDARD 17)

set(SRC_TRADE trade_src)
add_subdirectory(src)

set(SRC
    main.cpp
)

qt6_wrap_ui(UI_FORM src/mainwindow.ui)

add_executable(TradeSystem WIN32
    ${SRC}
    ${UI_FORM}
)

target_link_libraries(TradeSystem
    ${SRC_TRADE}
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} CACHE INTERNAL "" FORCE)
