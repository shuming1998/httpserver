#include "http_server.hpp"

void HttpServer::onConnection(const TcpConnectionPtr &conn) {
  if (conn->connected()) {
    LOG_INFO("Connection success : %s\n", conn->peerAddress().toIpPort().c_str());
  } else {
    LOG_INFO("Connection failed : %s\n", conn->peerAddress().toIpPort().c_str());
  }
}

void HttpServer::onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time) {
  for (;;) {
    std::string request = buf->retrieveAllAsString();
    if (!response_.solveRequest(request)) {
      break;
    }

    std::string header = response_.getHeader();
    conn->send(header);

    char buf[1024] = {0};
    int size = sizeof(buf);
    for (;;) {
      int len = response_.readFile(buf, size);
      if (len <= 0) {
        break;
      }
      conn->send(std::string(buf));
    }
  }
  conn->shutdown();
}

HttpServer::HttpServer(EventLoop *loop, const InetAddress &addr, const std::string &name)
    : server_(loop, addr, name)
    , loop_(loop)
    , response_(HttpResponse()) {
  server_.setConnectionCallback(std::bind(&HttpServer::onConnection, this, std::placeholders::_1));
  server_.setMessageCallback(std::bind(&HttpServer::onMessage, this, std::placeholders::_1, std::placeholders::_2,
      std::placeholders::_3));
  server_.setThreadNum(7);
}

void HttpServer::start() { server_.start(); }

int main(int argc, char *argv[]) {
  EventLoop loop;
  InetAddress addr(3000);
  HttpServer server(&loop, addr, "HttpServer-01");
  // 开启子线程 loop，注册 wakeupfd
  server.start(); // listen loopthread listenfd => acceptChannel => mainLoop
  loop.loop(); // 启动 mainLoop Poller
  return 0;
}