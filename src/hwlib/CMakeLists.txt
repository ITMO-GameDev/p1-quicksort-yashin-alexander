set(target hwlib)

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(source_path  "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(headers
    ${include_path}/sort.hpp
    ${include_path}/containers.hpp
    ${include_path}/dictonary.hpp
)

add_library(${target}
    ${headers}
)

set_target_properties(${target} PROPERTIES LINKER_LANGUAGE CXX)

# Create namespaced alias
add_library(${META_PROJECT_NAME}::${target} ALIAS ${target})

# Export library for downstream projects
export(TARGETS ${target} NAMESPACE ${META_PROJECT_NAME}:: FILE ${PROJECT_BINARY_DIR}/cmake/${target}/${target}-export.cmake)


