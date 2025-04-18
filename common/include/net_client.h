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
                return false;
            }

            void disconnect(){

            }

            bool isConnected(){

            }

            tsqueue<owned_message<T>>& Incoming(){
                return m_qMessages_in;
            }
        };
    }
}