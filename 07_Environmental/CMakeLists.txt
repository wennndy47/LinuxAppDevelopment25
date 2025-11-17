cmake_minimum_required(VERSION 3.20)
project(rhasher)

option(USE_READLINE "Build with readline" ON)

find_library(RHASH_LIBRARY NAMES rhash)

find_path(RHASH_INCLUDE_DIR NAMES rhash.h)

if(NOT RHASH_INCLUDE_DIR OR NOT RHASH_LIBRARY)
    message(FATAL_ERROR "librhash not found. librhash-dev is required.")
endif()

message(STATUS "rhash found: ${RHASH_LIBRARY}")

if(USE_READLINE)
    message(STATUS "readline ENABLED")
    find_path(READLINE_INCLUDE_DIR NAMES readline/readline.h)
    find_library(READLINE_LIBRARY NAMES readline)
    if(READLINE_INCLUDE_DIR AND READLINE_LIBRARY)
        set(READLINE_AVAILABLE TRUE)
        message(STATUS "readline found: ${READLINE_LIBRARY}")
    else()
        set(READLINE_AVAILABLE FALSE)
        message(STATUS "readline not found, building without readline support")
    endif()
else()
    set(READLINE_AVAILABLE FALSE)
    message(STATUS "readline DISABLED")
endif()

add_executable(rhasher rhasher.c)

if(READLINE_AVAILABLE)
    target_compile_definitions(rhasher PRIVATE USE_READLINE)
endif()
target_link_libraries(rhasher ${RHASH_LIBRARY})
if(READLINE_AVAILABLE)
    target_link_libraries(rhasher ${READLINE_LIBRARY})
endif()

# testing segment

enable_testing()

execute_process(
    COMMAND sh -c "openssl rand -base64 32 | tr -d '\\n'"
    OUTPUT_VARIABLE RANDOM_STRING
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND sh -c "echo -n '${RANDOM_STRING}' | md5sum | cut -d' ' -f1"
    OUTPUT_VARIABLE REFERENCE_MD5
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND sh -c "echo -n '${RANDOM_STRING}' | sha1sum | cut -d' ' -f1"
    OUTPUT_VARIABLE REFERENCE_SHA1
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

file(WRITE ${CMAKE_BINARY_DIR}/test_string "${RANDOM_STRING}")
file(WRITE ${CMAKE_BINARY_DIR}/expected_md5 "${REFERENCE_MD5}")
file(WRITE ${CMAKE_BINARY_DIR}/expected_sha1 "${REFERENCE_SHA1}")

add_test(NAME test_md5_comparison
    COMMAND sh -c "
        RHASHER_OUTPUT=$(echo 'MD5 \\\"${RANDOM_STRING}\\\"' | ./rhasher | grep -oE '[0-9a-fA-F]{32}');
        echo \\\"Rhasher MD5: \\${RHASHER_OUTPUT}\\\";
        echo \\\"Reference MD5: ${REFERENCE_MD5}\\\";
        [ \\\"\\$RHASHER_OUTPUT\\\" = \\\"${REFERENCE_MD5}\\\" ]
    "
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_test(NAME test_sha1_comparison
    COMMAND sh -c "
        RHASHER_OUTPUT=$(echo 'SHA1 \\\"${RANDOM_STRING}\\\"' | ./rhasher | tr -d '\\n' | tr '[:upper:]' '[:lower:]');
        echo \\\"Rhasher SHA1: \\$RHASHER_OUTPUT\\\";
        echo \\\"Reference SHA1: ${REFERENCE_SHA1}\\\";
        [ \\\"\\$RHASHER_OUTPUT\\\" = \\\"${REFERENCE_SHA1}\\\" ]
    "
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
