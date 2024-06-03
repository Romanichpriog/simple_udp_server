project('udp_server', 'cpp')
asio_proj = subproject('asio')
asio_dep = asio_proj.get_variable('asio_dep')
executable('udp_server', 'src/main.cpp', dependencies : asio_dep, install : true)