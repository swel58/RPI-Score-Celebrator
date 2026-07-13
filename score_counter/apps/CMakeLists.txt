add_executable(score_counter main.cpp)

# Link against your core library, which already carries the includes and cpr dependency
target_link_libraries(score_counter PRIVATE core_lib)