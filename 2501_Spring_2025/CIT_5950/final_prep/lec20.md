# Concept) Protocol
- Desc.)
  - A set of rules governing the format and exchange of messages in a computing system
    - What messages?
      - Syntax of the message
      - Meanings of the message
      - Legal replies
    - What sequence of messages?
      - How to convey errors?

<br><br>

# Concept) HTTP
- Desc.)
  - Request / Response protocol
    - **Client** sends a **request** to **Server**
      - Resource
        - An object or a document client asks with a request
        - Named by Uniform Resource Identifier (URI)
    - **Serer** processes the **request** and sends a **response**
      - Response indicates whether or not the server succeeded.

<br>

### Syntax) HTTP Request
- General Form
  ```
  [METHOD] [request-uri] HTTP/[version]\r\n
  [header_field1]: [value_field1]\r\n
  [header_field2]: [value_field2]\r\n
  ...
  [header_fieldN]: [value_fieldN]\r\n
  \r\n
  [request body, if any]
  ```
- Components)
  - Methods
    - `GET`
      - Desc.)
        - Requesting the named resource as a response
    - `POST`
      - Desc.)
        - Submitting the data to the server
    - `HEAD`
      - Desc.)
        - Request only the headers
    - `PUT`, `DELETE`, `TRACE`, `OPTIONS`, `CONNECT`, `PATCH`
  - URI (Uniform Resource Identifier)
    - Types)
      - Absolute URI
        - Composition) 
          - `scheme:[//authority]path[?query]`
          - e.g.)
            ```
            /static/test_tree/books/artofwar.txt?terms=hello
            ------------<path>------------------+++<query>++
             
            ```
        - Props.)
          - Mainly used for communicating via proxy
  - Headers (Client)
    - Desc.)
      - Client can provide one or more headers
    - Types)
      |Header|Desc.|
      |:-:|:-|
      |Host|the DNS name of the server|
      |User-Agent|an identifying string naming the browser|
      |Accept|the content types the client prefers or can accept|
      |Cookie|an HTTP cookie previously set by the server|

<br>

### Concept) HTTP Versions
- HTTP/1.1
- HTTP/2

<br>

### Syntax) HTTP Response
- General Form
  ```
  HTTP/[version] [status code] [reason]\r\n
  [header_field1]: [value_field1]\r\n
  [header_field2]: [value_field2]\r\n
  ...
  [header_fieldN]: [value_fieldN]\r\n
  \r\n
  [request body, if any]
  ```
- Components)
  - Status Code
    - Desc.)
      - Numeric outcome of the request
      - 3-digit integer with the first digit indicating a response category   
        |Code|Desc.|e.g.|
        |:-:|:-|:-|
        |`1xx`|Information message|- |
        |`2xx`|Success|- `200` Success!|
        |`3xx`|Redirect to a different URL|- `301` Moved permanently|
        |`4xx`|Error in the client's request|- `404` Not found|
        |`5xx`|Error experienced by the server|- `500` Server Error|
  - Reason
    - Desc.)
      - Human-readable explanation
  - Headers
    - Desc.)
      - Server can provide 0 or more headers
    - Types)
      |Header|Desc.|
      |:-:|:-|
      |Server|a string identifying the server software|
      |Content-Type|the type of the requested object|
      |Content-Length|size of requested object|
      |Last-Modified|a date indicating the last time the request object was modified|