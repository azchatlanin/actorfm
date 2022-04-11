headers = ['thread_pool.hpp']
sources = ['thread_pool.cpp']

lib = library(
  'thread_pool',
  include_directories : inc,
  sources: [headers, sources],
  dependencies : deps,
  cpp_args: args
)

thread_pool_dep = declare_dependency(
  include_directories: inc, 
  link_with: lib, 
)
