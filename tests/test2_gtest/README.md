## ✅ Automated Tests: Client-Server Communication

This project includes a set of automated unit tests using **Google Test (gtest)** to verify the basic functionality of a custom Boost.Asio-based client-server architecture. These tests simulate real network interactions between a server and a client.

### 🔧 Setup
- The tests spin up a persistent `TestServer` on port `6002`.
- Each test uses a `TestClient` to connect, send messages, and verify responses from the server.
- The server runs in a separate thread and is gracefully shut down after all tests complete.

### 🧪 Tests Overview

| Test Name                        | Description                                                        |
|----------------------------------|--------------------------------------------------------------------|
| `ClientCanConnectAndDisconnect` | Verifies that a client can connect and disconnect cleanly.        |
| `ClientCanSendMessageAll`       | Checks that the client can send a `MessageAll` and the server receives it. |
| `ClientCanPingServer`           | Sends a `ServerPing` message and expects the server to echo it back.       |

### 🖥️ Test Output Example

```
[SERVER] Started
[==========] Running 3 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 3 tests from ClientServerTest
[ RUN      ] ClientServerTest.ClientCanConnectAndDisconnect
[SERVER] New Connection: 127.0.0.1:50737
[       OK ] ClientServerTest.ClientCanConnectAndDisconnect (0 ms)
[ RUN      ] ClientServerTest.ClientCanSendMessageAll

[10000] Connection Approved
[10000] Read Header Fail.
[SERVER] New Connection: 127.0.0.1:50738
[10001] Connection Approved
[10001] Read Header Fail.
[0] Read Header Fail.
[       OK ] ClientServerTest.ClientCanSendMessageAll (321 ms)
[ RUN      ] ClientServerTest.ClientCanPingServer
[SERVER] New Connection: 127.0.0.1:50739
[10002] Connection Approved
[10002] Read Header Fail.
[0] Read Header Fail.
[       OK ] ClientServerTest.ClientCanPingServer (314 ms)
[----------] 3 tests from ClientServerTest (657 ms total)

[----------] Global test environment tear-down
[==========] 3 tests from 1 test suite ran. (667 ms total)
[  PASSED  ] 3 tests.
[SERVER] Stop
[SERVER] Stop
```

> ⚠️ **Note:** The `Read Header Fail.` logs are expected in this mock setup and do not affect test pass/fail results. This means the server closed the connection of a disconnected client.
