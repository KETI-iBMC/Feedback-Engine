
# CMake version
cmake_minimum_required(
    VERSION 3.2
)

# Project information
# CMake system name must be something like "Linux".
# This is important for cross-compiling.
# Project information

# Variable definition
## Execute file
set(
    OUTPUT
    KETI-FeedBack
)
set (
    SOURCE_FILES
    foflManager.cpp
    policyController.cpp
    sensorMonitoring.cpp
    main.cpp
    feedbackDbus.cpp
)

include_directories(
   ${POPULATE_SDK_INCLUDE_DIR}
   ${INC_DIR}
)
## Source
link_directories(
    ${POPULATE_SDK_LIBRARY_DIR}
)
# Build
add_executable(${OUTPUT} ${SOURCE_FILES})
target_link_libraries( ${OUTPUT}
    crypto
    ssl 
    m
    rt
    uuid
    pthread
    pam
    pam_misc
    boost_system
    boost_log
    boost_log_setup
    boost_thread
    cpprest
    sqlite3
    stdc++fs
    gpiod
    libdbus-c++-1.so.0
)

