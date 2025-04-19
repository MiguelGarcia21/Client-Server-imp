# 🧪 Test Case 1: Simulate Client Connection with PuTTY

**📅 Date:** 2025-04-19 
**🧰 Environment:**
- **OS:** Windows 11
- **Compiler:** g++ (MinGW, C++23)
- **Boost Version:** 1.82
- **Tool:** [PuTTY](https://www.putty.org/)
- **Server Run Command:** `.\bin\main.exe`

---

## 🔍 Objective  
Simulate a TCP client connection to the server using PuTTY and verify that the server accepts it and prints the client's IP address.

---

## 🧵 Steps  
1. Start the server:  
   ```bash
   .\bin\main.exe
   ```
2. Open PuTTY and configure a **Raw** or **Telnet** connection:
   - Hostname: `127.0.0.1`
   - Port: `60000`
   - Connection Type: **Raw**
3. Click **Open** to connect.

---

## ✅ Expected Result  
- Server prints the client IP address (e.g., `Connection from: 127.0.0.1`)
- PuTTY shows an open session (may be blank)

---

## 📸 Screenshot  
![test2_putty_connection](https://github.com/user-attachments/assets/108159fa-a975-4a1c-ba18-c50243e0d8c0)
---
![test2_putty_connection2](https://github.com/user-attachments/assets/ec555b72-b265-468a-a220-a68d505f76d7)


## 🧾 Server Log Output  
```
[SERVER] Started
[SERVER] New Connection: 127.0.0.1:XXXXX
[10000] Connection Approved
```

---

## 🧠 Notes  
- No message exchange was performed — this only verifies the connection acceptance.
- PuTTY works great for raw TCP testing.
