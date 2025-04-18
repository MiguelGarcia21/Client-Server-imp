#pragma once
#include "net_tsqueue.h"
#include "net_message.h"
#include "net_common.h"
#include "net_connection.h"

namespace olc{
    namespace net{
        template<typename T>
        class server_interface{
            public:
            server_interface(uint16_t port) 
                :m_asioAcceptor(m_asioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
            {

            }

            virtual ~server_interface(){
                Stop();
            }

            bool Start(){
                try{
                    WaitForClientConnection();
                    m_threadContext = std::thread([this](){ m_asioContext.run(); });
                } catch(std::exception& e){
                    std::cerr << "[SERVER] exception: " << e.what() << "\n";
                    return false;
                }

                std::cout << "[SERVER] Started\n";
                return true;
            }

            void Stop(){
                m_asioContext.stop();
                if(m_threadContext.joinable()){
                    m_threadContext.join();
                }
                std::cout << "[SERVER] Stop\n";
            }

            //ASINC function
            void WaitForClientConnection(){
                m_asioAcceptor.async_accept(
                    [this](std::error_code ec, boost::asio::tcp::socket socket){
                        if(!ec){
                            std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";
                            std::shared_ptr<connection<T>> newconn =
                                std::make_shared<connection<T>>(connection<T>::owner::server,
                                    m_asioContext, std::move(socket), m_qMessages_in);
                        } else{
                            std::cout << "[SERVER] New Connection Error: " << ex.message() << "\n";
                        }
                    }
                )
            }

            void MessageAllClients(const message<T>& msg, std::shared_ptr<connection<T>> pIgnoreClient = nullptr){

            }

            protected:
            virtual bool OnClientConnect(std::shared_ptr<connection<T>> client){
                return false;
            }

            virtual void OnClientDisconnect(std::shared_ptr<connection<T>> client, message<T>& msg){

            }

            tsqueue<owned_message<T>> m_qMessages_in;
            boost::asio::iocontext m_asioContext;
            std::thread m_threadContext;

            boost::asio::ip::tcp::acceptor m_asioAcceptor;
            uint32_t nIDCounter = 10000;
        };
    }
}