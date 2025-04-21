# Client-Server Implementation

This is a C++ client-server application using TCP sockets, built to learn and experiment with networking concepts.

## ⚙️ Requirements

✅ Thread-safe message queues (tsqueue) for asynchronous communication.
✅ Custom message types (enum-based) for flexible protocol design.
✅ Client & Server implementations with connection management.
✅ Ping/Message broadcasting (example use case).
(WIP) Correct disconnection handling.

## 🔄 Currently working on:
- Improve message handling
- Add proper logging
- Handle disconnects
- Add multiclient support
- 
## Testing

### Test 1: Client-Server Connection

📄 [View Test Details and Screenshot](docs/test1_notes.md)

The first test verifies that a client can successfully establish a connection with the server. This was done using [PuTTY](https://www.putty.org/) to simulate a client. When a connection is established, the server prints the client's IP address, confirming a successful handshake.

🧪 **Result:** Connection established successfully and logged on the server side.

---
### Test 2: Client-Server & Client-Client communication output

> 🚧 **Work in Progress**  
> This project is actively being developed and is not yet complete.

