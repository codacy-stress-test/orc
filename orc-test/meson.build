orc_test_headers = ['orctest.h', 'orcarray.h', 'orcrandom.h', 'orcprofile.h']
orc_test_sources = ['orctest.c', 'orcarray.c', 'orcrandom.c', 'orcprofile.c']

install_headers(orc_test_headers, subdir : 'orc-' + orc_api + '/orc-test')

orc_test_lib = library ('orc-test-' + orc_api,
  orc_test_sources,
  version : libversion,
  soversion : soversion,
  darwin_versions : osxversion,
  include_directories : orc_inc,
  c_args : orc_c_args + ['-DBUILDING_ORC_TEST'],
  dependencies : [libm, orc_dep],
  install : true)

orc_test_dep_cargs = []

if get_option('default_library') == 'static'
  orc_test_dep_cargs += ['-DORC_STATIC_COMPILATION']
endif

# pkg-config file
pkg.generate(orc_test_lib,
  name: 'orc-test',
  filebase: 'orc-test-' + orc_api,
  subdirs: 'orc-' + orc_api,
  description: 'Test Library of Optimized Inner Loops Runtime Compiler',
  libraries: [libm, orc_dep])

orc_test_dep = declare_dependency(include_directories : orc_inc,
  link_with : orc_test_lib)
