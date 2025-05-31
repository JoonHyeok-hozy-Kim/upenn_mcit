# TCP Socket Programming
- [Client TCP Connection](#concept-client-tcp-connection)
- [Server TCP Connection](#concept-server-tcp-connection)

<br><br>

## Concept) Client TCP Connection
- 5 Steps
  1. [Figure out the IP address and port to which to connect.](#1-figure-out-the-ip-address-and-port)
  2. [Create a socket](#2-creating-a-socket)
  3. [Connect the socket to the remote server](#3-connect-to-the-server)
  4. [`read()` and `write()` data using the socket](#4-read-and-write-data-using-the-socket)
  5. [Close the socket.]()

#### 1. Figure out the IP address and port
- Using DNS.
  - `getaddrinfo()` function helps getting IP address using the DNS.
    - Library : `#include <netdb.h>`
    - Def.)
      ```cpp
      int getaddrinfo(const char* hostname, // Domain name or IP address string
                      const char* service,  // Port No or Service name
                      const struct addrinfo* hints, 
                      struct addrinfo** res);
      ```
      - Use `struct addrinfo`.
        - Set up hint object and pass its pointer to `getaddrinfo()`.
        - Output will be assigned at the res object.
        - Def.)
          ```cpp
          struct addrinfo {
            int ai_flags;
            int ai_family;  
            int ai_socktype;
            int ai_protocol;
            size_t ai_addrlen;
            struct sockaddr* ai_addr;   // Pointer to socket addr
            char* ai_canonname;
            struct addrinfo* ai_next;   // Next node!
          }
          ```
          - Has the field of [`sockaddr` (generic socket address)](./lec18.md#concept-generic-address-structures)
      - e.g.)
        ```cpp
        struct addrinfo hints, *res;
        int status;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        status = getaddrinfo("www.google.com", "http", &hints, &res);
        freeaddrinfo(res);

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        status = getaddrinfo("localhost", "8080", &hints, &res);
        freeaddrinfo(res);
        ```
    - Returns
      - `0` if success
      - `-1` otherwise.
    - Free `addrinfo`
      - `getaddrinfo` allocates memory for `res`.
      - The user must free `res` after using `freeaddrinfo()` manually.
  - e.g.) `dnsresolve.cpp`
    ```cpp
    #include <arpa/inet.h>
    #include <netdb.h>      // For getaddrinfo()
    #include <stdlib.h>
    #include <string.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <iostream>
    
    using namespace std;
    
    void Usage(char* prog_name) {
            std::cerr << "usage: " << prog_name  << " hostname" << std::endl;
            exit(EXIT_FAILURE);
    }
    
    int main(int argc, char** argv) {
            int retval;
            struct addrinfo hints;
            struct addrinfo* results;
            //struct addrinfo* r;
    
            if (argc != 2) {
                    Usage(argv[0]);
            }
    
            memset(&hints, 0, sizeof(addrinfo));
    
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
    
            if ((retval = getaddrinfo(argv[1], nullptr, &hints, &results)) != 0) {
                    cerr << "getaddrinfo failed";
                    std::cerr << gai_strerror(retval) << std::endl;
                    return EXIT_FAILURE;
            }
    
    
            for (auto r = results; r != nullptr; r = r->ai_next) {
              // Treat the IPv4 and IPv6 cases differently.
              if (r->ai_family == AF_INET) {
                char ipstring[INET_ADDRSTRLEN];
                struct sockaddr_in *v4addr =
                        reinterpret_cast<struct sockaddr_in *>(r->ai_addr);
                inet_ntop(r->ai_family,
                          &(v4addr->sin_addr),
                          ipstring,
                          INET_ADDRSTRLEN);
                std::cout << "  IPv4: " << ipstring << std::endl;
    
              } else if (r->ai_family == AF_INET6) {
                char ipstring[INET6_ADDRSTRLEN];
                struct sockaddr_in6 *v6addr =
                        reinterpret_cast<struct sockaddr_in6 *>(r->ai_addr);
                inet_ntop(r->ai_family,
                          &(v6addr->sin6_addr),
                          ipstring,
                          INET6_ADDRSTRLEN);
                std::cout << "  IPv6: " << ipstring << std::endl;
    
              } else {
                std::cout << "  unknown address family " << r->ai_family << std::endl;
              }
            }
    
            freeaddrinfo(results);
            return EXIT_SUCCESS;
    }
    ```

#### 2. Creating a socket
- Def.)
  ```cpp
  int socket(int domain, int type, int protocol);
  ```
- e.g.)
  ```cpp
  #include <arpa/inet.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <iostream>
  
  using namespace std;
  
  int main(int argc, char** argv) {
          int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
          if (socket_fd == -1) {
                  cerr << strerror(errno) << endl;
                  return EXIT_FAILURE;
          }
  
          close(socket_fd);
          return EXIT_SUCCESS;
  }
  ```

#### 3. Connect to the Server
- Def.)
  ```cpp
  int connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
  ```
- e.g.)
  ```cpp
  #include <arpa/inet.h>  // ip address
  #include <assert.h>     // assert()
  #include <errno.h>      // Error constants
  #include <netdb.h>
  #include <unistd.h>     // close()
  #include <stdio.h>      // sscanf for argv
  #include <stdlib.h>     // EXIT_SUCCESS
  #include <string.h>
  #include <sys/socket.h> // socket
  #include <sys/types.h>  // Types _t
  #include <iostream>
  
  using namespace std;
  
  void Usage(char *progname, string& message);
  
  bool LookupName(char *name,
                  unsigned short port,
                  struct sockaddr_storage *ret_addr,
                  size_t *ret_addrlen);
  
  
  int main(int argc, char** argv) {
          if (argc != 3) {
                  string err_msg = "not 3 args";
                  Usage(argv[0], err_msg);
          }
  
          unsigned short port = 0;
          if (sscanf(argv[2], "%hu", &port) != 1) {
                  string err_msg = "port not number";
                  Usage(argv[0], err_msg);
          }
  
          struct sockaddr_storage addr;
          size_t addrlen;
          if (!LookupName(argv[1], port, &addr, &addrlen)) {
                  string err_msg = "invalid addr and port";
                  Usage(argv[0], err_msg);
          }
  
          // Create the socket.
          int socket_fd = socket(addr.ss_family, SOCK_STREAM, 0);
          if (socket_fd == -1) {
                  cerr << "socket() failed: " << strerror(errno) << endl;
                  return EXIT_FAILURE;
          }
  
          // Connect (Finally!)
          int res = connect(socket_fd, reinterpret_cast<sockaddr*>(&addr), addrlen);
          if (res == -1) {
                  cerr << "connect() failed: " << strerror(errno) << endl;
          }
  
          // Clean up
          close(socket_fd);
  
          return EXIT_SUCCESS;
  }
  
  
  void Usage(char *progname, string& message) {
          cerr << "usage: " << progname << " hostname port" << endl;
          cerr << "message : " << message << endl;
          exit(EXIT_FAILURE);
  }
  
  bool LookupName(char *name,
                  unsigned short port,
                  struct sockaddr_storage *ret_addr,
                  size_t *ret_addrlen) {
  
          struct addrinfo hints, *results;
          int retval;
  
          memset(&hints, 0, sizeof(addrinfo));
          hints.ai_family = AF_UNSPEC;
          hints.ai_socktype = SOCK_STREAM;
  
          if ((retval = getaddrinfo(name, nullptr, &hints, &results)) != 0) {
                  cerr << "getaddrinfo failed: ";
                  cerr << gai_strerror(retval) << endl;
                  return false;
          }
  
          if (results->ai_family == AF_INET) {
                  struct sockaddr_in *v4addr = reinterpret_cast<sockaddr_in*>(results->ai_addr);
                  v4addr->sin_port = htons(port); // Network byte order conversion : Big Endian!
          } else if (results->ai_family == AF_INET6) {
                  struct sockaddr_in6 *v6addr =  reinterpret_cast<sockaddr_in6*>(results->ai_addr);
                  v6addr->sin6_port = htons(port);
          } else {
                  cerr << "getaddrinfo failed to provide an IPv4 or IPv6 address";
                  cerr << endl;
                  freeaddrinfo(results);
                  return false;
          }
  
          assert(results != nullptr);
          memcpy(ret_addr, results->ai_addr, results->ai_addrlen);
          *ret_addrlen = results->ai_addrlen;
  
          freeaddrinfo(results);
          return true;
  }
  ```

#### 4. `read()` and `write()` data using the socket
- Desc.)
  - Read/Write just like files.
- Read)
  - If data already exists, `read()` will return immediately.
  - Else, `read()` will block until something arrives.
    - Deadlock available!
- Write)
  - `write()` queues your data in a send buffer in the OS and the `return`s
  - If there is no more space left in the send buffer, by default `write()` will block
- e.g.) `sendreceive.cpp`
  ```cpp
  #include <arpa/inet.h>
  #include <assert.h>
  #include <errno.h>
  #include <netdb.h>
  #include <unistd.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/socket.h>
  #include <sys/types.h>
  #include <iostream>
  
  using namespace std;
  
  #define BUF 256
  
  void Usage(char* progname);
  
  bool LookupName(char* name,
                  unsigned short port,
                  struct sockaddr_storage* ret_addr,
                  size_t* ret_addrlen);
  
  bool Connect(const struct sockaddr_storage &addr,
               const size_t &addrlen,
               int* ret_fd);
  
  int main(int argc, char** argv) {
          if (argc != 3) {
                  Usage(argv[0]);
          }
  
          unsigned short port = 0;
          if (sscanf(argv[2], "&hu", &port) != 1) {
                  Usage(argv[0]);
          }
  
          // Get an appropriate sockaddr structure.
          struct sockaddr_storage addr;
          size_t addrlen;
          if (!LookupName(argv[1], port, &addr, &addrlen)) {
                  Usage(argv[0]);
          }
  
          // Connect to the remote host.
          int socket_fd;
          if (!Connect(addr, addrlen, &socket_fd)) {
                  Usage(argv[0]);
          }
  
          // Read from the remote host.
          char readbuf[BUF];
          int res;
          while (1) {
                  res = read(socket_fd, readbuf, BUF-1);
  
                  if (res == 0) {
                          cerr << "socket closed prematurely" << endl;
                          close(socket_fd);
                          return EXIT_FAILURE;
                  }
  
                  if (res == -1) {
                          if (errno == EINTR) {
                                  continue;
                          }
                          cerr << "socket read failure: " << strerror(errno) << endl;
                          close(socket_fd);
                          return EXIT_FAILURE;
                  }
  
                  readbuf[res] = '\0';
                  cout << readbuf;
                  break;
          }
  
          // Write something to the remote host.
          while (1) {
                  int wres = write(socket_fd, readbuf, res);
  
                  if (wres == 0) {
                          cerr << "socket closed prematurely" << endl;
                          close(socket_fd);
                          return EXIT_FAILURE;
                  }
  
                  if (wres == -1) {
                          if (errno == EINTR) {
                                  continue;
                          }
                          cerr << "socket write failure: " << strerror(errno) << endl;
                          close(socket_fd);
                          return EXIT_FAILURE;
                  }
                  break;
          }
  
          // Clean up
          close(socket_fd);
          return EXIT_SUCCESS;
  }
  
  void Usage(char* progname) {
    std::cerr << "usage: " << progname << " hostname port" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  bool LookupName(char* name,
                  unsigned short port,
                  struct sockaddr_storage* ret_addr,
                  size_t* ret_addrlen) {
    struct addrinfo hints, *results;
    int retval;
  
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
  
    // Do the lookup by invoking getaddrinfo().
    if ((retval = getaddrinfo(name, nullptr, &hints, &results)) != 0) {
      std::cerr << "getaddrinfo failed: ";
      std::cerr << gai_strerror(retval) << std::endl;
      return false;
    }
  
    // Set the port in the first result.
    if (results->ai_family == AF_INET) {
      struct sockaddr_in *v4addr = reinterpret_cast<struct sockaddr_in *>(results->ai_addr);
      v4addr->sin_port = htons(port);
  
    } else if (results->ai_family == AF_INET6) {
      struct sockaddr_in6 *v6addr = reinterpret_cast<struct sockaddr_in6 *>(results->ai_addr);
      v6addr->sin6_port = htons(port);
  
    } else {
      std::cerr << "getaddrinfo failed to provide an IPv4 or IPv6 address";
      std::cerr << std::endl;
      freeaddrinfo(results);
      return false;
    }
  
    // Return the first result.
    assert(results != nullptr);
    memcpy(ret_addr, results->ai_addr, results->ai_addrlen);
    *ret_addrlen = results->ai_addrlen;
  
    // Clean up.
    freeaddrinfo(results);
    return true;
  }
  
  bool Connect(const struct sockaddr_storage &addr,
               const size_t &addrlen,
               int* ret_fd) {
    // Create the socket.
    int socket_fd = socket(addr.ss_family, SOCK_STREAM, 0);
    if (socket_fd == -1) {
      std::cerr << "socket() failed: " << strerror(errno) << std::endl;
      return false;
    }
  
    // Connect the socket to the remote host.
    int res = connect(socket_fd,
                      reinterpret_cast<const sockaddr *>(&addr),
                      addrlen);
    if (res == -1) {
      std::cerr << "connect() failed: " << strerror(errno) << std::endl;
      return false;
    }
  
    *ret_fd = socket_fd;
    return true;
  }
  ```


<br><br>

## Concept) Server TCP Connection
- 7 Steps
  1. [Figure out the IP address and port on which to listen]()
  1. [Create a socket]()
  1. [`bind()`]()
  2. [`listen()`]()
  3. [`accept()`]()
  4. [`read()` and `write()`]()
  5. [`close()`]()
- Props.)
  - Servers can have multiple IP addresses
    - 127.0.0.1 : local home
- Goal)
  - `bind()` a socket to a particular port
  - Allow multiple clients to connect to the same port

#### 1. Figure out IP address(es) & Port
- Desc.)
  - Use `getaddrinfo()` with the following args.
    ```cpp
    struct sockaddr hint, *results;
    hint.ai_flags = AI_PASSIVE; // MUST: 

    int res = getaddrinfo(NULL  // MUST: NULL for the hostname
                         ,nullptr
                         ,&hint
                         ,&results)
    ```

#### 2. Create a socket
- Same as the [client TCP connection](#2-creating-a-socket)

#### 3. Bind the socket
- Def.)
  ```cpp
  int bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
  ```
  - Returns
    - `0` on success
    - `-1` on error
- Desc.)
  - Just associating, not actually connecting.

#### 4. Listen for Incoming Clients
- Def.)
  ```cpp
  int listen(int sockfd, int backlog);
  ```
- Desc.)
  - Tells the OS that the socket is a listening socket
    - Clients can start [connecting](#3-connect-to-the-server) to the socket as soon as `listen()` returns.
    - Server can’t use a connection until you [`accept()`](#5-accept-a-client-connection) it
- e.g.)
  ```cpp
  #include <arpa/inet.h>
  #include <assert.h>
  #include <errno.h>
  #include <netdb.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/socket.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <iostream>
  
  using namespace std;
  
  void Usage(char* progname);
  void PrintOut(int fd, struct sockaddr *addr, size_t addrlen);
  
  int main(int argc, char** argv) {
  
          if (argc != 2) {
                  Usage(argv[0]);
          }
  
          struct addrinfo hints, *result;
          memset(&hints, 0, sizeof(struct addrinfo));
          hints.ai_family = AF_INET6;
          hints.ai_socktype = SOCK_STREAM;
          hints.ai_flags = AI_PASSIVE;    // A must for Binding!
          hints.ai_flags |= AI_V4MAPPED;  // use v4-mapped v6 if no v6 found
          hints.ai_protocol = IPPROTO_TCP;
          hints.ai_canonname = nullptr;
          hints.ai_addr = nullptr;
          hints.ai_next = nullptr;
  
          int res = getaddrinfo(nullptr, argv[1], &hints, &result);
  
          if (res != 0) {
                  cerr << "getaddrinfo() failed: ";
                  cerr << gai_strerror(res) << endl;
                  return EXIT_FAILURE;
          }
  
          // Loop through the returned address structures, try to bind, and break if succeeded.
          int listen_fd = -1;
          for (struct addrinfo *rp = result; rp!=nullptr; rp=rp->ai_next) {
                  // Create Socket
                  listen_fd = socket(rp->ai_family,
                                     rp->ai_socktype,
                                     rp->ai_protocol);
  
                  if (listen_fd == -1) {
                          std::cerr << "socket() failed: " << strerror(errno) << std::endl;
                          listen_fd = 0;
                          continue;
                  }
  
                  // Configure socket option : SO_REUSEADDR
                  // WHY?) Tell TCP stack to make port we bind available after we exit.
                  int optval = 1;
                  setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  
                  // Bind!
                  if (bind(listen_fd, rp->ai_addr, rp->ai_addrlen) == 0) {
                          // Success! Break!
                          PrintOut(listen_fd, rp->ai_addr, rp->ai_addrlen);
                          break;
                  }
  
                  close(listen_fd);
                  listen_fd = -1;
          }
  
          freeaddrinfo(result);
  
          if (listen_fd == -1) {
                  // Binding Failed.
                  std::cerr << "Couldn't bind to any addresses." << std::endl;
                  return EXIT_FAILURE;
          }
  
          // Listen!
          if (listen(listen_fd, SOMAXCONN) != 0) {
                  std::cerr << "Failed to mark socket as listening: ";
                  std::cerr << strerror(errno) << std::endl;
                  close(listen_fd);
                  return EXIT_FAILURE;
          }
  
          sleep(20);
          close(listen_fd);
          return EXIT_SUCCESS;
  }
  
  
  
  void Usage(char *progname) {
    std::cerr << "usage: " << progname << " port" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  void PrintOut(int fd, struct sockaddr *addr, size_t addrlen) {
    std::cout << "Socket [" << fd << "] is bound to:" << std::endl;
    if (addr->sa_family == AF_INET) {
      // Print out the IPV4 address and port
  
      char astring[INET_ADDRSTRLEN];
      struct sockaddr_in *in4 = reinterpret_cast<struct sockaddr_in *>(addr);
      inet_ntop(AF_INET, &(in4->sin_addr), astring, INET_ADDRSTRLEN);
      std::cout << " IPv4 address " << astring;
      std::cout << " and port " << ntohs(in4->sin_port) << std::endl;
  
    } else if (addr->sa_family == AF_INET6) {
      // Print out the IPV6 address and port
  
      char astring[INET6_ADDRSTRLEN];
      struct sockaddr_in6 *in6 = reinterpret_cast<struct sockaddr_in6 *>(addr);
      inet_ntop(AF_INET6, &(in6->sin6_addr), astring, INET6_ADDRSTRLEN);
      std::cout << " IPv6 address " << astring;
      std::cout << " and port " << ntohs(in6->sin6_port) << std::endl;
  
    } else {
      std::cout << " ???? address and port ????" << std::endl;
    }
  }
  ```

#### 5. Accept a Client Connection
- Def.)
  ```cpp
  int accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen);
  ```
  - Returns
    - `-1` on error
    - `addr` and `addrlen` are output parameters.
      - Clent's address information is written.
        - Use `inet_ntop()` to get the presentation version of the IP
        - Use `getnameinfo()` to do a reverse DNS lookup on the client.
- e.g.)
  ```cpp
  #include <arpa/inet.h>
  #include <assert.h>
  #include <errno.h>
  #include <netdb.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/socket.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <iostream>
  
  using namespace std;
  
  void Usage(char *progname);
  void PrintOut(int fd, struct sockaddr *addr, size_t addrlen);
  void PrintReverseDNS(struct sockaddr *addr, size_t addrlen);
  void PrintServerSide(int client_fd, int sock_family);
  int Listen(char *portnum, int* sock_family);
  void HandleClient(int c_fd, struct sockaddr* addr, size_t addrlen, int sock_family);
  
  int main(int argc, char** argv) {
          if (argc != 2) {
                  Usage(argv[0]);
          }
  
          int sock_family;
          int listen_fd = Listen(argv[1], &sock_family);
  
          if (listen_fd <= 0) {
                  std::cerr << "Couldn't bind to any addresses." << std::endl;
                  return EXIT_FAILURE;
          }
  
          while (1) {
  
                  // Accept client!
                  struct sockaddr_storage caddr;
                  socklen_t caddr_len = sizeof(caddr);
                  int client_fd = accept(listen_fd,
                                         reinterpret_cast<struct sockaddr *>(&caddr),
                                         &caddr_len);
  
                  if (client_fd < 0) {
                          if ((errno == EINTR) || (errno == EAGAIN) || (errno == EWOULDBLOCK)){
                                  continue;
                          }
                          cerr << "Failure on accept: " << strerror(errno) << endl;
                          break;
                  }
  
                  HandleClient(client_fd,
                               reinterpret_cast<struct sockaddr*>(&caddr),
                               caddr_len,
                               sock_family);
          }
  
          close(listen_fd);
          return EXIT_SUCCESS;
  }
  
  
  void Usage(char *progname) {
    std::cerr << "usage: " << progname << " port" << std::endl;
    exit(EXIT_FAILURE);
  }
  
  void PrintOut(int fd, struct sockaddr *addr, size_t addrlen) {
    std::cout << "Socket [" << fd << "] is bound to:" << std::endl;
    if (addr->sa_family == AF_INET) {
      // Print out the IPV4 address and port
  
      char astring[INET_ADDRSTRLEN];
      struct sockaddr_in *in4 = reinterpret_cast<struct sockaddr_in *>(addr);
      inet_ntop(AF_INET, &(in4->sin_addr), astring, INET_ADDRSTRLEN);
      std::cout << " IPv4 address " << astring;
      std::cout << " and port " << ntohs(in4->sin_port) << std::endl;
  
    } else if (addr->sa_family == AF_INET6) {
      // Print out the IPV6 address and port
  
      char astring[INET6_ADDRSTRLEN];
      struct sockaddr_in6 *in6 = reinterpret_cast<struct sockaddr_in6 *>(addr);
      inet_ntop(AF_INET6, &(in6->sin6_addr), astring, INET6_ADDRSTRLEN);
      std::cout << " IPv6 address " << astring;
      std::cout << " and port " << ntohs(in6->sin6_port) << std::endl;
  
    } else {
      std::cout << " ???? address and port ????" << std::endl;
    }
  }
  
  void PrintReverseDNS(struct sockaddr *addr, size_t addrlen) {
          char hostname[1024];
          if (getnameinfo(addr, addrlen, hostname, 1024, nullptr, 0, 0) != 0) {
                  sprintf(hostname, "[reverse DNS failed");
          }
          cout << " DNS name : " << hostname << endl;
  }
  
  void PrintServerSide(int client_fd, int sock_family) {
          char hname[1024];
          hname[0] = '\0';
  
          cout << "Server side interface is ";
          if (sock_family == AF_INET) {
                  // The server is using an IPv4 address.
                  struct sockaddr_in srvr;
                  socklen_t srvrlen = sizeof(srvr);
                  char addrbuf[INET_ADDRSTRLEN];
  
                  getsockname(client_fd, (struct sockaddr *) &srvr, &srvrlen);
                  inet_ntop(AF_INET, &srvr.sin_addr, addrbuf, INET_ADDRSTRLEN);
                  std::cout << addrbuf;
  
                  // Get the server's dns name, or return it's IP address as
                  // a substitute if the dns lookup fails.
                  getnameinfo((const struct sockaddr *) &srvr, srvrlen, hname, 1024, nullptr, 0, 0);
                  std::cout << " [" << hname << "]" << std::endl;
  
          } else {
                  // The server is using an IPv6 address.
                  struct sockaddr_in6 srvr;
                  socklen_t srvrlen = sizeof(srvr);
                  char addrbuf[INET6_ADDRSTRLEN];
  
                  getsockname(client_fd, (struct sockaddr *) &srvr, &srvrlen);
                  inet_ntop(AF_INET6, &srvr.sin6_addr, addrbuf, INET6_ADDRSTRLEN);
                  std::cout << addrbuf;
  
                  // Get the server's dns name, or return it's IP address as
                  // a substitute if the dns lookup fails.
                  getnameinfo((const struct sockaddr *) &srvr, srvrlen, hname, 1024, nullptr, 0, 0);
                  std::cout << " [" << hname << "]" << std::endl;
          }
  }
  
  int Listen(char *portnum, int* sock_family) {
    // Populate the "hints" addrinfo structure for getaddrinfo().
    // ("man addrinfo")
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET6;       // IPv6 (also handles IPv4 clients)
    hints.ai_socktype = SOCK_STREAM;  // stream
    hints.ai_flags = AI_PASSIVE;      // use an address we can bind to a socket and accept client connections   on
    hints.ai_flags |= AI_V4MAPPED;    // use v4-mapped v6 if no v6 found
    hints.ai_protocol = IPPROTO_TCP;  // tcp protocol
    hints.ai_canonname = nullptr;
    hints.ai_addr = nullptr;
    hints.ai_next = nullptr;
  
    // Use argv[1] as the string representation of our portnumber to
    // pass in to getaddrinfo().  getaddrinfo() returns a list of
    // address structures via the output parameter "result".
    struct addrinfo *result;
    int res = getaddrinfo(nullptr, portnum, &hints, &result);
  
    // Did addrinfo() fail?
    if (res != 0) {
      std::cerr << "getaddrinfo() failed: ";
      std::cerr << gai_strerror(res) << std::endl;
      return -1;
    }
  
    // Loop through the returned address structures until we are able
    // to create a socket and bind to one.  The address structures are
    // linked in a list through the "ai_next" field of result.
    int listen_fd = -1;
    for (struct addrinfo *rp = result; rp != nullptr; rp = rp->ai_next) {
      listen_fd = socket(rp->ai_family,
                         rp->ai_socktype,
                         rp->ai_protocol);
      if (listen_fd == -1) {
        // Creating this socket failed.  So, loop to the next returned
        // result and try again.
        std::cerr << "socket() failed: " << strerror(errno) << std::endl;
        listen_fd = -1;
        continue;
      }
  
      // Configure the socket; we're setting a socket "option."  In
      // particular, we set "SO_REUSEADDR", which tells the TCP stack
      // so make the port we bind to available again as soon as we
      // exit, rather than waiting for a few tens of seconds to recycle it.
      int optval = 1;
      setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR,
                 &optval, sizeof(optval));
  
      // Try binding the socket to the address and port number returned
      // by getaddrinfo().
      if (bind(listen_fd, rp->ai_addr, rp->ai_addrlen) == 0) {
        // Bind worked!  Print out the information about what
        // we bound to.
        PrintOut(listen_fd, rp->ai_addr, rp->ai_addrlen);
  
        // Return to the caller the address family.
        *sock_family = rp->ai_family;
        break;
      }
  
      // The bind failed.  Close the socket, then loop back around and
      // try the next address/port returned by getaddrinfo().
      close(listen_fd);
      listen_fd = -1;
    }
  
    // Free the structure returned by getaddrinfo().
    freeaddrinfo(result);
  
    // If we failed to bind, return failure.
    if (listen_fd == -1)
      return listen_fd;
  
    // Success. Tell the OS that we want this to be a listening socket.
    if (listen(listen_fd, SOMAXCONN) != 0) {
      std::cerr << "Failed to mark socket as listening: ";
      std::cerr << strerror(errno) << std::endl;
      close(listen_fd);
      return -1;
    }
  
    // Return to the client the listening file descriptor.
    return listen_fd;
  }
  
  void HandleClient(int c_fd, struct sockaddr* addr, size_t addrlen, int sock_family) {
    // Print out information about the client.
    std::cout << std::endl;
    std::cout << "New client connection" << std::endl;
    PrintOut(c_fd, addr, addrlen);
    PrintReverseDNS(addr, addrlen);
    PrintServerSide(c_fd, sock_family);
  
    // Loop, reading data and echo'ing it back, until the client
    // closes the connection.
    while (1) {
      char clientbuf[1024];
      ssize_t res = read(c_fd, clientbuf, 1023);
      if (res == 0) {
        std::cout << " [The client disconnected.]" << std::endl;
        break;
      }
  
      if (res == -1) {
        if ((errno == EAGAIN) || (errno == EINTR))
          continue;
  
        std::cout << " [Error on client socket: ";
        std::cout << strerror(errno) << "]" << std::endl;
        break;
      }
      clientbuf[res] = '\0';
      std::cout << " the client sent: " << clientbuf;
  
      // Really should do this in a loop in case of EAGAIN, EINTR,
      // or short write, but I'm lazy.  Don't be like me. ;)
      write(c_fd, "You typed: ", strlen("You typed: "));
      write(c_fd, clientbuf, strlen(clientbuf));
    }
  
    close(c_fd);
  }
  ```