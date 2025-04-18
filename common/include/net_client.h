#pragma once
#include "net_tsqueue.h"
#include "net_message.h"
#include "net_common.h"
#include "net_connection.h"

namespace olc{
    namespace net{
        template<typename T>
        class client_interface{
            client_interface() : msocket(m_context){

            }

            virtual ~client_interface(){
                disconnect();
            }

            protected:
            boost::asio::io_context m_context;
            std::thread thrContext;
            boost::asio::ip::tcp::socket m_socket;
            std::unique_ptr<connection<T>> m_connection;

            private:
            tsqueue<owned_message<T>> m_qMessages_in;

            public:
            bool connect(const std::string& host, const uint16_t port){
                try{
                    m_connection = std::make_unique<connection<T>>();
                    boost::asio::ip::tcp::resolver resolver(m_context);
                    m_endpoints = resolver.resolve(host, std::to_string(port));
                    m_connection->ConnectToServer(m_endpoints);
                    thrContext = std::thread([this]() { m_connection.run(); });
                } catch(std::exception& e){
                    std::cerr << "Client Exception: " << e.what() << "\n";
                    return false;
                }
                return true;
            }

            void disconnect(){
                if(isConnected){
                    m_connection->disconnect();
                }
                m_context.stop();
                if(thrContext.joinable()){
                    thrContext.join();
                }
                m_connection.release();
            }

            bool isConnected(){
                if(m_connection){
                    return m_connection->isConnected();
                } else{
                    return false;
                }
            }

            tsqueue<owned_message<T>>& Incoming(){
                return m_qMessages_in;
            }
        };
    }
}