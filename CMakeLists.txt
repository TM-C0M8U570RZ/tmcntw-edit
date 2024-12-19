cmake_minimum_required(VERSION 3.16)

project(tmcntw-edit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tmcntw-edit main.cpp
    tmc_application.h tmc_application.cpp)

target_include_directories(tmcntw-edit PRIVATE $ENV{TMC_SDK_PATH}/current/helpers/include  $ENV{TMC_SDK_PATH}/current/libTmcNdbTagging/include)
target_link_directories(tmcntw-edit PRIVATE $ENV{TMC_SDK_PATH}/current/lib/${CMAKE_HOST_SYSTEM_PROCESSOR})
target_link_libraries(tmcntw-edit PRIVATE -lTmcNdbTagging -lTmcHelpers)

include(GNUInstallDirs)
install(TARGETS tmcntw-edit
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
