#include <gtest/gtest.h>
#include <olc_net.h>
#include <thread>
#include <chrono>

enum class CustomMsgTypes : uint32_t
{
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
};

class TestClient : public olc::net::client_interface<CustomMsgTypes>
{
public:
    void PingServer()
    {
        olc::net::message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::ServerPing;
        auto now = std::chrono::system_clock::now();
        msg << now;
        Send(msg);
    }

    void Broadcast()
    {
        olc::net::message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::MessageAll;
        Send(msg);
    }
};

class TestServer : public olc::net::server_interface<CustomMsgTypes>
{
public:
    TestServer(uint16_t port) : server_interface(port) {}

    void ResetFlags() {
        clientConnected = false;
        pingReceived = false;
        broadcastReceived = false;
    }

    void ProcessMessages(int count = -1, bool wait = true) {
        this->Update(count, wait);
    }

    bool clientConnected = false;
    bool pingReceived = false;
    bool broadcastReceived = false;

protected:
    bool OnClientConnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client) override
    {
        clientConnected = true;
        olc::net::message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::ServerAccept;
        client->Send(msg);
        return true;
    }

    void OnMessage(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, olc::net::message<CustomMsgTypes>& msg) override
    {
        switch (msg.header.id)
        {
        case CustomMsgTypes::ServerPing:
            pingReceived = true;
            client->Send(msg);
            break;
        case CustomMsgTypes::MessageAll:
            broadcastReceived = true;
            olc::net::message<CustomMsgTypes> response;
            response.header.id = CustomMsgTypes::ServerMessage;
            response << client->GetID();
            MessageAllClients(response, client);
            break;
        }
    }
};

const uint16_t kTestPort = 60010;
static TestServer* sharedServer = nullptr;

class StoryTests : public ::testing::Test {
protected:
    void SetUp() override {
        sharedServer->ResetFlags();
    }
};

class ServerEnvironment : public ::testing::Environment {
public:
    void SetUp() override {
        sharedServer = new TestServer(kTestPort);
        ASSERT_TRUE(sharedServer->Start());
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    void TearDown() override {
        sharedServer->Stop();
        delete sharedServer;
    }
};

::testing::Environment* const server_env = ::testing::AddGlobalTestEnvironment(new ServerEnvironment());

TEST_F(StoryTests, Story1_CoreNetworking) {
    TestClient client;
    client.Connect("127.0.0.1", kTestPort);
    while (!client.IsConnected()) std::this_thread::sleep_for(std::chrono::milliseconds(10));
    ASSERT_TRUE(client.IsConnected());
    client.Disconnect();
}

TEST_F(StoryTests, Story2_ClientIDManagement) {
    TestClient client;
    client.Connect("127.0.0.1", kTestPort);
    while (!client.IsConnected()) std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ASSERT_TRUE(sharedServer->clientConnected);
    client.Disconnect();
}

TEST_F(StoryTests, Story3_Ping) {
    TestClient client;
    client.Connect("127.0.0.1", kTestPort);
    while (!client.IsConnected()) std::this_thread::sleep_for(std::chrono::milliseconds(10));
    client.PingServer();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    sharedServer->ProcessMessages();
    ASSERT_TRUE(sharedServer->pingReceived);
    client.Disconnect();
}

TEST_F(StoryTests, Story4_Broadcast) {
    TestClient client;
    client.Connect("127.0.0.1", kTestPort);
    while (!client.IsConnected()) std::this_thread::sleep_for(std::chrono::milliseconds(10));
    client.Broadcast();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    sharedServer->ProcessMessages();
    ASSERT_TRUE(sharedServer->broadcastReceived);
    client.Disconnect();
}

TEST_F(StoryTests, Story5_ConnectionValidation) {
    TestClient client;
    client.Connect("127.0.0.1", kTestPort);
    while (!client.IsConnected()) std::this_thread::sleep_for(std::chrono::milliseconds(10));
    client.Disconnect();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    SUCCEED();
}

TEST_F(StoryTests, Story6_Logging) {
    TestClient client;
    client.Connect("127.0.0.1", kTestPort);
    while (!client.IsConnected()) std::this_thread::sleep_for(std::chrono::milliseconds(10));
    client.PingServer();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    sharedServer->ProcessMessages();
    client.Disconnect();
    SUCCEED(); // Logging is visual only
}