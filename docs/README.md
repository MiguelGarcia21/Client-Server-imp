# 🧩 Networking Client-Server System – Scrum Backlog

This README outlines the workflow from a Scrum perspective, based on the requirements of the project using Boost.Asio.

---

## Epic: Asynchronous Client-Server Communication System

---

### 📘 User Story 1: As a developer, I want to set up the core infrastructure for client-server communication so that data can be exchanged asynchronously.

#### 🔧 Tasks:
- Set up `asio::io_context` and use `asio::ip::tcp::acceptor` to listen for incoming connections (server).
- Implement `connection<T>` class to manage socket logic.
- Use `asio::ip::tcp::resolver` and `asio::async_connect` to connect client to server.
- Run the ASIO context in dedicated threads on both server and client.
- Define `message<T>` and `owned_message<T>` to structure messages.
- Create a thread-safe queue `tsqueue<T>` to manage concurrent message processing.

### 🧪 Test Result: `Story1_CoreNetworking`: Passed ✅

---

### 📘 User Story 2: As a server, I want to assign unique IDs to connected clients so they can be identified in the system.

#### 🔧 Tasks:
- Maintain an ID counter `nIDCounter` in the server class.
- Assign IDs during the call to `ConnectToClient(uid)` in each new connection.
- Expose the ID through `GetID()`.
### 🧪 Test Result: `Story2_ClientIDManagement`: Passed ✅
---

### 📘 User Story 3: As a client, I want to ping the server and receive a timestamp response so I can measure latency.

#### 🔧 Tasks:
- Create and send a message with ID `ServerPing` containing a timestamp (`std::chrono::system_clock::time_point`).
- Echo the same message back from the server to the originating client.
- Compute round-trip time on the client and display the result.
### 🧪 Test Result: `Story3_Ping`: Passed ✅
---

### 📘 User Story 4: As a client, I want to send a broadcast message so all connected clients (except me) receive it.

#### 🔧 Tasks:
- Send a `MessageAll` message from the client to the server.
- The server receives the message and creates a `ServerMessage` that includes the sender’s client ID.
- The server sends this message to all clients except the originator.
- Clients print the received broadcast message with the sender’s ID.
### 🧪 Test Result: `Story4_Broadcast`: Passed ✅
---

### 📘 User Story 5: As a system, I want to validate client connection states to avoid communication failures.

#### 🔧 Tasks:
- Use `IsConnected()` to validate client state before sending messages.
- If a client is disconnected, remove it from the list (`m_deqConnections`).
- Always restart the acceptor using `WaitForClientConnection()` to accept new connections.
### 🧪 Test Result: `Story5_ConnectionValidation`: Passed ✅
---


### 📘 User Story 6: As a developer, I want to log system activity so that I can monitor performance and debug issues.

#### 🔧 Tasks:
- Print relevant events to `std::cout`: connection established, disconnected, messages received, and errors.
- Include client IDs in logs to make debugging easier.
### 🧪 Test Result: `Story6_Logging`: Passed ✅
---


