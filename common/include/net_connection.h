#pragma once
#include "net_tsqueue.h"
#include "net_message.h"

namespace olc{
    namespace net{
        template<typename T>
        class connection : public std::enable_shared_from_this<connection<T>>{
            public:
            connection(){

            }
            virtual ~connection(){

            }

            bool ConnectToServer();
            bool Disconnect();
            bool isConnected() cont;

            bool Send(const message<T>& msg);

            protected:
            boost::asio::ip::tcp::socket m_socket;
            boost::asio::io_context& m_asio_context;
            tsqueue<message<T>> m_qMessages_out;
            tsqueue<owned_message>& m_qMessages_in;
        };
    }
}