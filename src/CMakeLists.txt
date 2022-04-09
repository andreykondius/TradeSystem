set(CUDNN_DIR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cudnn)
set(CUDNN_INCLUDE_PATH "${CUDNN_DIR_PATH}/include")
set(CUDNN_LIBRARY_PATH "${CUDNN_DIR_PATH}/lib")
LINK_DIRECTORIES(${CUDNN_LIBRARY_PATH})

set(LIBTORCH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libtorch/${CMAKE_BUILD_TYPE})
set(LIBPATH "${LIBPATH} ${LIBTORCH_DIR}/lib")
list(APPEND CMAKE_PREFIX_PATH ${LIBTORCH_DIR})
find_package(Torch REQUIRED CONFIG)
message(STATUS "Torch_FOUND: ${Torch_FOUND}")
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT ")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

set(TORCH_INCLUDE_DIRS
    ${LIBTORCH_DIR}/include
    ${LIBTORCH_DIR}/include/torch/csrc/api/include
)

LINK_DIRECTORIES(${LIBTORCH_DIR}/lib)

set(SRC
    mainwindow.cpp
    trading/tcpserver.cpp
    trading/trademanager.cpp
    database/data.cpp
    database/database.cpp
    database/thrpushtofile.cpp
    neurocpu/cell.cpp
    neurocpu/neuron.cpp
    neurocpu/perceptron.cpp
    logrec/logrec.cpp
    logrec/SingltoneCollection.cpp
    tests/test.cpp
)

set(INC
    logrec
)

add_library(${SRC_TRADE}
    ${SRC}
)

target_link_libraries(${SRC_TRADE}
    PUBLIC
    ${LOGREC_LIB}
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
)

target_include_directories(${SRC_TRADE}
    PUBLIC
    ${INC}
    ${TORCH_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Network_INCLUDE_DIRS}
)

set(LOGREC_LIB logrec_lib)
add_subdirectory(logrec)
