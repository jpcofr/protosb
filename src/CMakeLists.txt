set(client_src
        client.cpp
    )
set(server_src
        server.cpp
    )

add_executable(client.out ${client_src})
add_executable(server.out ${server_src})
