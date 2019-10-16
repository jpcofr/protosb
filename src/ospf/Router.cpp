#include "Router.h"
#include <iostream>
//#include <boost/asio.hpp>
using namespace std;
// using namespace boost::asio;

Router::Router() { print_if_ip(); }
Router::~Router() { std::cout << "Adieu!" << std::endl; }

void Router::print_if_ip() {
  std::cout << "I can print my inteface IP!" << std::endl;
  delete_me.print_IP();
}

void Router::receive_updates() {
  // receive updates per interface
}

void Router::update_neighbors() {
  // Send updates to the neigbors through all the interfaces
}

void Router::update_routing_table() {
  // Use the information from the tables to generate the updates
}

Interface::Interface() {
  interface_ip = ip::make_address("34.24.123.2");
}

void Interface::print_IP() {
  std::cout << interface_ip.to_string() << std::endl;
}


