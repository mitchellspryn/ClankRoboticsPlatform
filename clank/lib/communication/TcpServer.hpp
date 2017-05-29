#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include <errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

#include "messages/messages.hpp"
#include "Command.hpp"
#include <functional>
#include <vector>

#include "configuration/Configuration.hpp"
#include <string>
#include <stdexcept>

/** Max string size of a TCP port number (5 digits + '\0') */
#define TCPSRV_PORT_SIZE 6


namespace Clank
{
  namespace Communication
  {
    class TcpServer
    {

      public:
        TcpServer(Clank::Configuration::Configuration* config);
        TcpServer(const char* port);
        ~TcpServer();
        int Start();
        int Send(const char* str);
        int Receive(char* str, size_t size);
        int KillServer();
        int isConnected();
        bool isAlive();
        int AddListener(std::function<const char*(Command*)> _target);
        std::vector<const char*> RaiseEvent(Command *C);

        int wait_for_connection();
        
        
      private:
      /**
       * Create the server's listen socket.
       *
       * @param	s	Server
       *
       * @return -1 on failure
       */
      int create_listen_socket();
      void close_connection();
      bool killAll;
      std::vector<std::function<const char*(Command*)> > commandListeners;
      pthread_t serverThread;
      pthread_mutex_t serverMutex;

      char port[TCPSRV_PORT_SIZE]; /**< Port number */

      int lsocket;	/**< Listen socket descriptor */
      int islsock;  /**< Is the listen socket active */

      int csocket;	/**< Connection socket descriptor */
      int iscsock;  /**< Is the connection socket active */
    };
  }
}
#endif
