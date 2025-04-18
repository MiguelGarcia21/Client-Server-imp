#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include ".../olc_net.h"

enum class CustomMsgTypes : uint32_t{
    FireBullet,
    MovePlayer
};

class CustomClient : public olc::net::client_interface<CustomMsgTypes>{
    public:
    bool FireBullet(float x, float y){
        olc::net::message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::FireBullet;
        msg << x << y;
        Send(msg);
    }
};

int main(){
    CustomClient c;
    c.connect("website.com", 60000);
}