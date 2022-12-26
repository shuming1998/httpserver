#include "http_server.hpp"

void HttpServer::onConnection(const TcpConnectionPtr &conn) {
  if (conn->connected()) {
    LOG_INFO("Connection success : %s\n", conn->peerAddress().toIpPort().c_str());
  } else {
    LOG_INFO("Connection failed : %s\n", conn->peerAddress().toIpPort().c_str());
  }
}

void HttpServer::onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time) {
  // std::string msg = buf->retrieveAllAsString();
  // conn->send(msg);
  // conn->shutdown(); // 关闭写端
  //* 获取 http 请求并分析：这里使用正则表达式
  std::string msg = buf->retrieveAllAsString();
  std::cout << "==============recv==============\n" << msg
            << "==============recvEnd==============\n";

  //* 回应 http GET 请求
  std::string reMsg = "";
  //* 消息头
  reMsg.append("HTTP/1.1 200 OK\r\n");
  reMsg.append("Server:MDHttp\r\n");
  reMsg.append("Content-Type: text/html\r\n");
  reMsg.append("Content-Length: ");
  reMsg.append("10\r\n");
  reMsg.append("\r\n");
  //* 消息内容
  reMsg.append("0123456789");
  conn->send(reMsg);
  conn->shutdown();
}

HttpServer::HttpServer(EventLoop *loop, const InetAddress &addr, const std::string &name)
    : server_(loop, addr, name)
    , loop_(loop) {
  server_.setConnectionCallback(std::bind(&HttpServer::onConnection, this, std::placeholders::_1));
  server_.setMessageCallback(std::bind(&HttpServer::onMessage, this, std::placeholders::_1, std::placeholders::_2,
      std::placeholders::_3));
  server_.setThreadNum(7);
}

void HttpServer::start() { server_.start(); }

int main(int argc, char *argv[]) {
  EventLoop loop;
  InetAddress addr(8000);
  HttpServer server(&loop, addr, "HttpServer-01");
  // 开启子线程 loop，注册 wakeupfd
  server.start(); // listen loopthread listenfd => acceptChannel => mainLoop
  loop.loop(); // 启动 mainLoop Poller
  return 0;
}