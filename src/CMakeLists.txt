set(client_src
        client.cpp
    )
set(server_src
        server.cpp
    )

set(raw_tcp_client_src
        raw_tcp_client.cpp
    )

set(raw_udp_client_src
        raw_udp_client.cpp
    )

set(router_src
    ospf/Router.cpp
    ospf/Router.h
    ospf/main.cpp
)

#add_executable(tcp_echo_client.out ${router_src})
#add_executable(tcp_echo_server.out ${router_src})
add_executable(router.out ${router_src})
#add_executable(raw_tcp_client.out ${raw_tcp_client_src})
#add_executable(udp_client.out ${raw_udp_client_src})
#add_executable(server.out ${server_src})
#add_executable(client.out ${client_src})

#target_link_libraries(${Boost_SYSTEM_LIBRARY})
target_link_libraries(router.out)

#target_link_libraries(tcp_echo_client.out ${Boost_LIBRARIES} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})

#target_link_libraries(server.out ${Boost_LIBRARIES} )
