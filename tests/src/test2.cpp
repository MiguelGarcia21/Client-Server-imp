#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <atomic>
#include "olc_net.h"

enum class CustomMsgTypes : uint32_t {
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
};

// Mock Server que guarda los mensajes recibidos
class TestServer : public olc::net::server_interface<CustomMsgTypes> {
public:
	TestServer(uint16_t port) : server_interface(port) {}

protected:
	bool OnClientConnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client) override {
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerAccept;
		client->Send(msg);
		return true;
	}

	void OnMessage(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, olc::net::message<CustomMsgTypes>& msg) override {
		last_msg_id = msg.header.id;
		last_received = std::chrono::system_clock::now();
		received_any = true;

		if (msg.header.id == CustomMsgTypes::ServerPing) {
			client->Send(msg);
		}
	}

public:
	CustomMsgTypes last_msg_id = CustomMsgTypes::ServerDeny;
	std::chrono::system_clock::time_point last_received;
	bool received_any = false;
};

// Cliente extendido
class TestClient : public olc::net::client_interface<CustomMsgTypes> {
public:
	bool ConnectToLocalhost(uint16_t port) {
		return Connect("127.0.0.1", port);
	}

	void PingServer() {
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerPing;
		std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
		msg << timeNow;
		Send(msg);
	}

	void MessageAll() {
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::MessageAll;
		Send(msg);
	}
};

// 🔁 Servidor persistente entre pruebas
TestServer* persistent_server = nullptr;
std::thread server_thread;
std::atomic<bool> server_running{true};
constexpr uint16_t test_port = 6002;

// Fixture de pruebas
class ClientServerTest : public ::testing::Test {
protected:
	void SetUp() override {
		persistent_server->last_msg_id = CustomMsgTypes::ServerDeny;
		persistent_server->received_any = false;
	}
};

// 🧪 Tests
TEST_F(ClientServerTest, ClientCanConnectAndDisconnect) {
	TestClient client;
	ASSERT_TRUE(client.ConnectToLocalhost(test_port));
	EXPECT_TRUE(client.IsConnected());
	client.Disconnect();
	EXPECT_FALSE(client.IsConnected());
}

TEST_F(ClientServerTest, ClientCanSendMessageAll) {
	TestClient client;
	ASSERT_TRUE(client.ConnectToLocalhost(test_port));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	client.MessageAll();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	EXPECT_TRUE(persistent_server->received_any);
	EXPECT_EQ(persistent_server->last_msg_id, CustomMsgTypes::MessageAll);
}

TEST_F(ClientServerTest, ClientCanPingServer) {
	TestClient client;
	ASSERT_TRUE(client.ConnectToLocalhost(test_port));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	client.PingServer();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	EXPECT_TRUE(persistent_server->received_any);
	EXPECT_EQ(persistent_server->last_msg_id, CustomMsgTypes::ServerPing);
}

// 🔧 Setup global del servidor
int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);

	persistent_server = new TestServer(test_port);
	persistent_server->Start();

	server_thread = std::thread([]() {
		while (server_running) {
			persistent_server->Update(10);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	});

	int result = RUN_ALL_TESTS();

	server_running = false;
	persistent_server->Stop();
	if (server_thread.joinable()) server_thread.join();
	delete persistent_server;

	return result;
}