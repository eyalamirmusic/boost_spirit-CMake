set(exampleDir ${spirit_SOURCE_DIR}/example/lex)
add_executable(MyExample Main.cpp)

target_include_directories(MyExample PRIVATE
        ${spirit_SOURCE_DIR}/include
        ${spirit_SOURCE_DIR}/example/lex
        ${Boost_INCLUDE_DIR})

add_custom_command(TARGET MyExample
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${exampleDir}/example5.input
        ${CMAKE_CURRENT_BINARY_DIR}/${name}.input)