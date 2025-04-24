### Concept) Socket API
- Types)
  |Socket API|Desc|
  |:-:|:-|
  |Berkeley Socket|- Written in C|
  |POSIX Socket|- Multi-threading added|

<br><br>

### Concept) IPv4
- Props.)
  - 4 Bytes (1 Byte = 0 ~ 255)
- C struct
  ```cpp
  struct in_addr {
    uint32_t s_addr;    // 32 bit = 4 * 1 Byte
  }
  ```
  - In socket usage
    ```cpp
    struct sockaddr_in {
        sa_family_t     sin_family;
        in_port_t       sin_port;
        struct in_addr  sin_addr;    // Here!
        unsigned char   sin_zero[8];
    }
    ```
    - This should be type casted to `struct sockaddr` or `struct sockaddr_storage` to use in function.
    - `sin_zero` pads 8 byte zeros to sync the size with `sockaddr`.
      - `sin_family` : 2 Byte
      - `sin_port` : 2 Byte
      - `sin_addr` : 4 Byte (IPv4)
      - `sin_zero` : 8 Bytes of zeros!

<br><br>

### Concept) IPv6
- Props.)
  - 16 Bytes
  - 8 group of 4 hex-digits
    - Why?)
      - 16 Bytes = 8 * 16 bits = 8 * (4 * 4) bits = 8 * (4 * hex)
        - where hex : 0 ~ 16 = 2^4
    - e.g.) 2d01:0db8:f188:0000:0000:0000:0000:1f33
  - Can omit leading zeros
- C struct
  ```cpp
  struct in6_addr {
    uint8_t s6_addr[16];    // 16 * 8bits = 16 Bytes
  }
  ```
  - In socket usage
    ```cpp
    struct sockaddr_in6 {
        sa_family_t     sin6_family;
        in_port_t       sin6_port;
        uint32_t        sin6_flowinfo;
        struct in6_addr sin6_addr;      // Here!
        uint32_t        sin6_scope_id;
    }
    ```
    - This should be type casted to `struct sockaddr` or `struct sockaddr_storage` to use in function.

<br><br>

### Concept) Generic Address Structures
- Desc.)
  - A mostly-protocol-independent address structure.
  - Pointer to this is parameter type for socket system calls.
- Types)
  - `sockaddr`
    - Def.)
      ```cpp
      struct sockaddr {
        sa_family_t sa_family;
        char        sa_data[14];
      }
      ```
    - Props.)
      - 16 Bytes
        - `sa_family` : 2 Bytes
        - `sa_data` : 14 Bytes
      - Used when we know the IP version.
        - e.g.)
          - `bind()`
          - `connect()`
  - `sockaddr_storage`
    - Def.)
      ```cpp
      struct sockaddr_storage {
        sa_family_t ss_family;
        char __ss_pad1[_SS_PAD1SIZE];
        int64_t __ss_align;
        char __ss_pad2[_SS_PAD2SIZE];
      }
      ```
    - Props.)
      - 10 Bytes + $`\alpha`$ : big enough!
        - `sa_family` : 2 Bytes
        - `__ss_align` : 8 Bytes
        - `__ss_pad1` and `__ss_pad2` differs by the platform.
      - Used when we do NOT know the IP version.
        - e.g.)
          - `accept()`
          - `recvfrom()`
- How to Use)
  - Type cast the `sockaddr_in` and `sockaddr_in6` struct when passing them as the args of functions.

<br><br>

### Concept) Address Conversion
- Desc.)
  - `sockaddr_in` and `sockaddr_in6` are the network address.
  - `inet_ntop` converts network address into presentation.
  - `inet_pton` converts presentation into network address.
    - e.g.) IPv4, IPv6
      ```cpp
      #include <stdlib.h>
      #include <arpa/inet.h>
      #include <iostream>
      
      using namespace std;
      
      int main(int argc, char** argv) {
              // IPv4
              struct sockaddr_in sa;
              char zstring[INET_ADDRSTRLEN];
      
              inet_pton(AF_INET, "192.255.255.255", &(sa.sin_addr));
              inet_ntop(AF_INET, &(sa.sin_addr), zstring, INET_ADDRSTRLEN);
      
              cout << zstring << endl;
      
              // IPv6
              struct sockaddr_in6 sa6;
              char astring[INET6_ADDRSTRLEN];
      
              inet_pton(AF_INET6, "2001:0db8:63b3:1::3490", &(sa6.sin6_addr));
              inet_ntop(AF_INET6, &(sa6.sin6_addr), astring, INET6_ADDRSTRLEN);
      
              cout << astring << endl;
      
              return EXIT_SUCCESS;
      
      }
      ```

<br><br>

### Concept) Socket
- Desc.)
  - Endpoint for a specific connection
  - File descriptor for network communications.   
    ![](../images/exam_prep/final/18_001.png)
    - It identifies...
      - Client's
        - IP address
        - Port number
      - Server's
        - IP address
        - Port number
- Types)
  |Type|Desc.|Procedures|
  |:-:|:-|:-|
  |Stream Socket|- Point to Point, reliable byte streams <br> - TCP, SCTP<br> - Typical client-server communication.|1. Establish connection<br>2. Communicate<br>3. Close connection.|
  |Datagram Socket|- One to many, unreliable byte streams <br> - UDP<br>- Often used as a building block|1. Create socket.<br>2. Communicate|
  |Raw Socket|- Layer 3 communication||
  - Diagrams)
    |Stream Socket|Datagram Socket
    |:-:|:-:|
    |![](../images/exam_prep/final/18_002.png)|![](../images/exam_prep/final/18_003.png)|

<br><br>

### Concept) Client TCP Connection
- 5 Steps
  1. [Figure out the IP address and port to which to connect.]()
  2. [Create a socket]()
  3. [Connect the socket to the remote server]()
  4. [`read()` and `write()` data using the socket]()
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