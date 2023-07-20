cmake_minimum_required(VERSION 3.2)

project(SqlCpp7Lesson5)
set(PostgreSQL_ADDITIONAL_VERSIONS "15")
add_subdirectory("d:\\Учёба\\CPP\\GIT-Repos\\libpqxx-7.7.5" libpqxx-build)

add_executable(SqlCpp7Lesson5 main.cpp)

target_compile_features(SqlCpp7Lesson5 PRIVATE cxx_std_17)

target_link_libraries(SqlCpp7Lesson5 pqxx)
