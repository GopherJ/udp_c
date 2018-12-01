cmake_minimum_required(VERSION 3.0)

project(udp_transport)

add_executable(udp_client udp_client.c utils.c)

add_executable(udp_server udp_server.c utils.c)
