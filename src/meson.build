inc += include_directories('.')

subdir('thread_pool')
deps += thread_pool_dep

lib = library(
  'actfm',
  include_directories : inc,
  dependencies : deps,
  cpp_args: args
)

actfm_dep = declare_dependency(
  include_directories: inc, 
  link_with: lib, 
)
