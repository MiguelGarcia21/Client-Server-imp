#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <iostream>
#include <thread>
#include <chrono>

std::vector<char> vBuffer(20 * 1024);

//debug & testing

void GrabSomeData(boost::asio::ip::tcp::socket &socket){
    socket.async_read_some(boost::asio::buffer(vBuffer.data(), vBuffer.size()),
        [&](std::error_code ec, std::size_t length){
            if (!ec){
                std::cout << "\n\nRead " << length << " bytes\n\n";

            for (int i = 0; i < length; i++){
                std::cout << vBuffer[i];
            }
            GrabSomeData(socket);
            }
        }
    );
}
/*
int main() {
    boost::asio::io_context context;
    boost::system::error_code ec;

    boost::asio::io_context::work idleWork(context);

    std::thread thrContext = std::thread([&](){context.run();});

    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address("51.38.81.49", ec), 80);

    boost::asio::ip::tcp::socket socket(context);

    socket.connect(endpoint, ec);

    if(!ec){
        std::cout << "Connected" << std::endl;
    }else{
        std::cout << "Not conencted: " << ec.message() << std::endl;  
    }

    if(socket.is_open()){

        GrabSomeData(socket);

        std::string sRequest =
            "GET /index.html HTTP/1.1\r\n"
            "Host: example.com\r\n"
            "Connection: close\r\n\r\n";
        
        socket.write_some(boost::asio::buffer(sRequest.data(), sRequest.size()), ec);

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2000ms);

        context.stop();
        if(thrContext.joinable()) thrContext.join();
    }
    system("pause");
    return 0;
}*/