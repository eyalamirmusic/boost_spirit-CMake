cmake_minimum_required(VERSION 3.25)
project(BoostSpiritExample)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(spirit
        GIT_REPOSITORY https://github.com/boostorg/spirit.git
        GIT_TAG        develop)
        FetchContent_MakeAvailable(spirit)

FetchContent_MakeAvailable(spirit)

find_package(Boost REQUIRED)

function(add_example dir name)

    set(exampleDir ${spirit_SOURCE_DIR}/example)
    add_executable(${name} ${exampleDir}/${dir}/${name}.cpp)
    target_include_directories(${name} PRIVATE ${spirit_SOURCE_DIR}/include ${Boost_INCLUDE_DIR})

    add_custom_command(TARGET ${name}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${exampleDir}/${dir}/${name}.input
            ${CMAKE_CURRENT_BINARY_DIR}/${name}.input)
endfunction()

add_example(lex example1)
add_example(lex example2)
add_example(lex example3)
add_example(lex example5)

add_example(lex strip_comments)
add_example(lex word_count)
