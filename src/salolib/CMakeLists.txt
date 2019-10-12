set(target salolib)


set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(source_path  "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(headers
    ${include_path}/quicksort.hpp
)

set(sources
    ${source_path}/quicksort.cpp
)

add_library(${target}
    ${sources}
    ${headers}
)
# Create namespaced alias
add_library(${META_PROJECT_NAME}::${target} ALIAS ${target})

# Export library for downstream projects
export(TARGETS ${target} NAMESPACE ${META_PROJECT_NAME}:: FILE ${PROJECT_BINARY_DIR}/cmake/${target}/${target}-export.cmake)


