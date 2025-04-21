# 🧠 Client-Server Implementation

A lightweight, client-server network framework built in modern C++ using Boost.Asio. This project includes both client and server implementations, message serialization, a thread-safe queue system, and asynchronous communication primitives. Built to learn and experiment with networking concepts.

---
## 🖥️ Design Goals:
- Modularity: Code is structured in clearly separated components (client, server, messaging, threading, etc.).
- Test-Driven Development (TDD): Key components are unit-tested using Google Test.
- Continuous Improvement: The architecture allows easy extension, refactoring, and addition of new features.
- Lifecycle-Oriented Documentation: This project simulates a real-world software development lifecycle (SDLC), starting from analysis of Software Requirements Specifications (SRS) and evolving into implementation and testing.

---
## 🚀 Features

- ✅ Asynchronous communication with Boost.Asio
- 📦 Message system using templates and custom headers
- 🔁 Thread-safe queue for inbound/outbound communication
- 🔌 TCP client & server interfaces
- 🧪 Google Test-based unit tests for core serialization logic
- 🧵 Multithreaded I/O handling with automatic cleanup
- 🔧 Easily extendable using enum-based message routing

---

## 🛠️ Technologies and Concepts

| Tool/Library       | Purpose |
|--------------------|---------|
| **C++17**          | Core programming language |
| **Boost.Asio**     | TCP networking (non-blocking, async IO) |
| **Google Test**    | Unit testing framework |
| **Threads / std::thread** | Running Boost IO context in background |
| **std::deque / std::vector** | Internal message buffering |
| **Serialization**  | POD-safe templated message encoding |
| **Design Patterns**| CRTP-style client/server inheritance, RAII |

---

## 🔧Requirements:

- Boost (header-only Asio or full Boost.Asio, build)
- GoogleTest (installed or included as submodule, build)

## Testing

### Test 1: Client-Server Connection (Manual test)

📄 [View Test Details and Screenshot](tests/test1/test1_notes.md)

The first test verifies that a client can successfully establish a connection with the server. This was done using [PuTTY](https://www.putty.org/) to simulate a client. When a connection is established, the server prints the client's IP address, confirming a successful handshake.

🧪 **Result:** Connection established successfully and logged on the server side.

---
### Test 2: Automated Tests - Client/Server Communication (Google Test / GTest)

📄 [View Test Details and Output](tests/test2_gtest/TEST2_NOTES.md)

This project includes automated tests using **Google Test (gtest)** to verify client-server communication over Boost.Asio.

- ✔️ Connection and disconnection
- 📤 Message sending (`MessageAll`)
- 🔁 Ping-pong message echoing

🧪 **Result:** 3 tests successfuly passed without ASSERT or EXPECT failure.


> 🚧 **Work in Progress**  
> This project is actively being developed and is not yet complete.


## 📌 Notes
- Network code uses Boost.Asio TCP sockets (non-blocking)
- Messages are serialized using POD-type-safe operator<< / operator>>
- Each message includes a templated header (enum class) for routing
- Incoming messages are stored in a thread-safe queue (tsqueue)
- The client/server run their own I/O context in dedicated threads
