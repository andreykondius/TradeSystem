add_library(
    ${LOGREC_LIB}
    logrec.cpp
    SingltoneCollection.cpp
    putlog.hpp
)

target_link_libraries(
    ${LOGREC_LIB}
    PUBLIC
    Qt6::Widgets
)
