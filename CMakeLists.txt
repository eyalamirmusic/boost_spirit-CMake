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



function(add_lex_example name)
    set(exampleDir ${spirit_SOURCE_DIR}/example/lex)
    add_executable(${name} ${exampleDir}/${name}.cpp)

    target_include_directories(${name} PRIVATE
            ${spirit_SOURCE_DIR}/include
            ${Boost_INCLUDE_DIR})

    add_custom_command(TARGET ${name}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${exampleDir}/${name}.input
            ${CMAKE_CURRENT_BINARY_DIR}/${name}.input)
endfunction()

add_lex_example(example1)
add_lex_example(example2)
add_lex_example(example3)
add_lex_example(example5)

add_lex_example(strip_comments)
add_lex_example(word_count)
