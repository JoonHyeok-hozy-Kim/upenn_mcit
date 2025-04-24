# OSI 7 Layers
|No.|Layer|Desc.|
|:-:|:-:|:-|
|1|Physical|- wire, cable, NIC(Network Interface Controller) <br> - How bits are encoded in signal level|
|2|Data Link|- LAN, MAC address <br> - How computers cooperate in a local network. <br> - How bits are packetized. <br> -- Encapsulation : Ethernet Frame (MAC address)  <br> - How NICs are addressed.|
|3|Network|- IP, router <br> - IP routes packets across multiple networks. <br> -- Encapsulation : IP Packet (MAC address)|
|4|Transport|- Port <br> - Protocols between source and destination <br> - TCP, UDP <br> -- Encapsulation : TCP/UDP Segment|
|5|Session|- Establishing/terminating application sessions|
|6|Presentation|- Network neutral representations. <br> - Encryptions : SSL|
|7|Application|- DNS, HTTP, HTTPS, SSH <br> - Form and meaning of messages between application entries|

<br><br>

### Concept) Transmission Control Protocol (TCP)
- Prop.)
  - Reliable, ordered, congestion-controlled byte stream
  - Typically requires 3 round trips.

<br><br>

### Concept) Datagram Protocol (UDP)