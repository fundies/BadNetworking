#include <enet/enet.h>

#include <iostream>
#include <cstdio>

class Server {
public:
  Server() {}
  
  ~Server() {
    std::cout << "Shutting down the sever" << std::endl;
    enet_host_destroy(_server);
    enet_deinitialize();
  }
  
  bool Init(const char* host, unsigned port, unsigned max_clients) {
    std::cout << "Intializing Server" << std::endl;
    
    if (enet_initialize () != 0) {
      std::cerr << "An error occurred while initializing ENet" << std::endl;
      return false;
    }
    
    enet_address_set_host(&_address, host);
    _address.port = port;

    _server = enet_host_create (& _address /* the address to bind the server host to */, 
                               max_clients /* allow up to 32 clients and/or outgoing connections */,
                                2          /* allow up to 2 channels to be used, 0 and 1 */,
                                0          /* assume any amount of incoming bandwidth */,
                                0          /* assume any amount of outgoing bandwidth */);

    if (_server == nullptr) {
      std::cerr << "An error occurred while trying to create an ENet server host" << std::endl;
      return false;
    }

    _client = enet_host_create (NULL /* create a client host */,
                1 /* only allow 1 outgoing connection */,
                2 /* allow up 2 channels to be used, 0 and 1 */,
                0 /* assume any amount of incoming bandwidth */,
                0 /* assume any amount of outgoing bandwidth */);

    if (_client == nullptr) {
      std::cerr << "An error occurred while trying to create an ENet client host" << std::endl;
      return false; 
    }

    std::cout << "Listening on " << host << ":" << port << std::endl;

    return true;
  }

  void Poll() {
    ENetEvent event;
    /* Wait up to 1000 milliseconds for an event. */
    while (enet_host_service (_client, & event, 1000) > 0) {
        switch (event.type) {
          case ENET_EVENT_TYPE_CONNECT:
              printf ("A new client connected from %x:%u.\n", 
                      event.peer -> address.host,
                      event.peer -> address.port);
              /* Store any relevant client information here. */
              event.peer -> data = const_cast<char*>("Client information");
              break;
          case ENET_EVENT_TYPE_RECEIVE:
              printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
                      event.packet -> dataLength,
                      event.packet -> data,
                      event.peer -> data,
                      event.channelID);
              /* Clean up the packet now that we're done using it. */
              enet_packet_destroy (event.packet);
              
              break;
             
          case ENET_EVENT_TYPE_DISCONNECT:
              printf ("%s disconnected.\n", event.peer -> data);
              /* Reset the peer's client information. */
              event.peer -> data = NULL;
        }
      }
    }

protected:
  ENetAddress _address;
  ENetHost* _server;
  ENetHost* _client;
};


int main() {

  Server server;
  if (!server.Init("127.0.0.1", 1234, 32))
    return EXIT_FAILURE;

  while(true) server.Poll();

  return EXIT_SUCCESS;
}
