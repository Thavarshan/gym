add_library(app "")

target_sources(
    app
    PRIVATE
        bill.cpp
        details.cpp
        menus.cpp
        utils.cpp
        invoice.cpp
        fort.cpp
        main.cpp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/includes/bill.h
        ${CMAKE_CURRENT_LIST_DIR}/includes/menus.h
        ${CMAKE_CURRENT_LIST_DIR}/includes/details.h
        ${CMAKE_CURRENT_LIST_DIR}/includes/utils.h
        ${CMAKE_CURRENT_LIST_DIR}/includes/invoice.h
        ${CMAKE_CURRENT_LIST_DIR}/includes/fort.h
        ${CMAKE_CURRENT_LIST_DIR}/includes/fort.hpp
    )

target_include_directories(
    app
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
    )

set_property(TARGET app PROPERTY CXX_STANDARD 17)
