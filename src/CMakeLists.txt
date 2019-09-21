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

add_executable(raw_tcp_client.out ${raw_tcp_client_src})
#add_executable(udp_client.out ${raw_udp_client_src})
add_executable(server.out ${server_src})
add_executable(client.out ${client_src})

#target_link_libraries(${Boost_SYSTEM_LIBRARY})
#target_link_libraries(server.out LINK_PUBLIC ${Boost_LIBRARIES} )

#target_link_libraries(server.out ${Boost_LIBRARIES} )
