#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <string>
#include <string.h>
#include <functional>

#include "muduo/TcpServer.h"
#include "muduo/Logger.h"

class HttpServer {
public:
  HttpServer(EventLoop *loop, const InetAddress &addr, const std::string &name);
  void start();

private:
  void onConnection(const TcpConnectionPtr &conn);
  void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time);

  EventLoop *loop_;
  TcpServer server_;
};








#endif