add_executable(expected
  expected.cpp)
target_include_directories(expected
  PRIVATE ${PROJECT_SOURCE_DIR}/variant/include
  PRIVATE ${PROJECT_SOURCE_DIR}/include
  )
