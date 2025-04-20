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
                        ReadHeader();
                    }
                }
            }
            void ConnectToServer(const boost::asio::ip::tcp::resolver::results_type& endpoints) { //corrected to void
                if (m_nOwnerType == owner::client){
                    boost::asio::async_connect(m_socket,endpoints,
                    [this](std::error_code ec, boost::asio::ip::tcp::endpoint endpoint){
                        if(!ec){
                            ReadHeader();
                        }
                    });
                }
            }

            void Disconnect() { // corrected to void
                if (isConnected())
                    boost::asio::post(m_asioContext, [this]() { m_socket.close(); });
                }

            bool isConnected() const { return m_socket.is_open(); }

            void Send(const message<T>& msg) {
                boost::asio::post(m_asioContext, 
                [this, msg](){
                    bool bWritingMessage = !m_qMessages_out.empty();
                    m_qMessages_out.push_back(msg);
                    if (!bWritingMessage){
                        WriteHeader();
                    }
                });
            }

            private:
            //ASYNC FUNCTIONS
            void ReadHeader(){
                boost::asio::async_read(m_socket, boost::asio::buffer(&m_msgTemporary_in.header, sizeof(message_header<T>)),
                                        [this](std::error_code ec, std::size_t length){
                                            if(!ec){
                                                if(m_msgTemporary_in.header.size > 0){
                                                    m_msgTemporary_in.body.resize(m_msgTemporary_in.header.size);
                                                    ReadBody();
                                                } else{
                                                    AddToIncomingMessageQueue();
                                                }
                                            }else{
                                                std::cout << "[" << id << "] Read Header Fail.\n";
                                                m_socket.close();
                                            }
                                        });
            }

            void ReadBody(){
                boost::asio::async_read(m_socket, boost::asio::buffer(m_msgTemporary_in.body.data(), m_msgTemporary_in.body.size()),
                [this](std::error_code ec, std::size_t length){
                
                    if(!ec){
                        AddToIncomingMessageQueue();
                    } else{
                        std::cout << "[" << id << "] Read Header Fail.\n";
                        m_socket.close();
                    }
                }
                );
            }

            void WriteBody(){
                boost::asio::async_write(m_socket, boost::asio::buffer(m_qMessages_out.front().body.data(), m_qMessages_out.front().body.size()),
                [this](std::error_code ec, std::size_t length){
                    if(!ec){
                        m_qMessages_out.pop_front();
                        if(!m_qMessages_out.empty()){
                            WriteHeader();
                        }
                    } else {
                        std::cout << "[" << id << "] Write Body Fail. \n";
                        m_socket.close();
                    }
                });
            }

            void WriteHeader(){
                boost::asio::async_write(m_socket, boost::asio::buffer(&m_qMessages_out.front().header, sizeof(message_header<T>)),
                [this](std::error_code ec, std::size_t length){
                    if (!ec){
                        if(m_qMessages_out.front().body.size() > 0){
                            WriteBody();
                        } else{
                            m_qMessages_out.pop_front();
                            if (!m_qMessages_out.empty()){
                                WriteHeader();
                            }
                        }
                    } else {
                        std::cout << "[" << id << "] Write Header Fail.\n";
                        m_socket.close();
                    }
                });
            }

            void AddToIncomingMessageQueue(){
                if(m_nOwnerType == owner::server){
                    m_qMessages_in.push_back({this->shared_from_this(), m_msgTemporary_in});
                } else{
                    m_qMessages_in.push_back({nullptr, m_msgTemporary_in});
                }
                ReadHeader();
            }

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