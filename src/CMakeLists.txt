set(client_src
        client.cpp
    )
set(server_src
        server.cpp
    )

#add_executable(client.out ${client_src})
add_executable(server.out ${server_src})

#target_link_libraries(${Boost_SYSTEM_LIBRARY})
#target_link_libraries(server.out LINK_PUBLIC ${Boost_LIBRARIES} )

target_link_libraries(server.out ${Boost_LIBRARIES} )
