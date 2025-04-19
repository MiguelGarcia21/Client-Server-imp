#pragma once
#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"

namespace olc{
    namespace net{
        template<typename T>
        class connection : public std::enable_shared_from_this<connection<T>>{
            public:
            enum class owner
			{
				server,
				client
			};
            connection(owner parent, boost::asio::io_context& asioContext, boost::asio::ip::tcp::socket socket, tsqueue<owned_message<T>>& qIn) : m_asioContext(asioContext), m_socket(std::move(socket)), m_qMessages_in(qIn)
			{
				m_nOwnerType = parent;
			}
            virtual ~connection(){

            }

            uint32_t GetID() const { return id; }

            void ConnectToClient(uint32_t uid = 0){
                if(m_nOwnerType == owner::server){
                    if(m_socket.is_open()){
                        id = uid;
                    }
                }
            }
            bool ConnectToServer() {return true;}
            bool Disconnect() {
                if (isConnected())
                    boost::asio::post(m_asioContext, [this]() { m_socket.close(); });
                    return true;
                }

            bool isConnected() const { return m_socket.is_open(); }

            void Send(const message<T>& msg) {

            };

            protected:
            boost::asio::ip::tcp::socket m_socket;
            boost::asio::io_context& m_asioContext;
            tsqueue<message<T>> m_qMessages_out;
            tsqueue<owned_message<T>>& m_qMessages_in;
            message<T> m_msgTemporary_in;
            owner m_nOwnerType = owner::server;
            uint32_t id = 0;
        };
    }
}