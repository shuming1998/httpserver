#include "Logger.h"
#include "http_response.hpp"

#include <regex>
#include <iostream>

HttpResponse::HttpResponse() : fileSize_(0), fp_(nullptr) {}
HttpResponse::~HttpResponse() {}

bool HttpResponse::solveRequest(std::string request) {
  // ==============recv==============
  // GET / HTTP/1.1
  // Host: 127.0.0.1:8000
  // User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:102.0) Gecko/20100101 Firefox/102.0
  // Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
  // Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2
  // Accept-Encoding: gzip, deflate, br
  // Connection: keep-alive
  // Upgrade-Insecure-Requests: 1
  // Sec-Fetch-Dest: document
  // Sec-Fetch-Mode: navigate
  // Sec-Fetch-Site: none
  // Sec-Fetch-User: ?1
  // ==============recvEnd==============

  //* 获取 http 请求并分析：这里使用正则表达式
  std::cout << "==============recv==============\n" << request
            << "==============recvEnd==============\n";
  //* 正则表达式规则：获取 操作类型 和 路径
  // std::string pattern = "^([A-Z]+) (.+) HTTP/1";
  //* /   /index.html   /ff/index.php   /ff
  // std::string pattern = "^([A-Z]+) /([a-zA-Z0-9]*([.].*)?) HTTP/1";
  //* GET /index.php?id=1&name=csm HTTP/1.1
  std::string pattern = "^([A-Z]+) /([a-zA-Z0-9]*([.][a-zA-Z]*)?)[?]?(.*) HTTP/1";
  std::regex r(pattern);
  std::smatch mas;
  std::regex_search(request, mas, r);

  if (0 == mas.size()) {
    LOG_ERROR("%s failed!", pattern.c_str());
    return false;
  }

  //* 取出匹配成功的 操作类型 路径 文件后缀名
  std::string type = mas[1];
  // std::string path = mas[2];
  std::string path = "/";
  path.append(mas[2]);
  std::string filetype = mas[3];
  std::string query = mas[4];

  if (filetype.size() > 0) {
    filetype = filetype.substr(1, filetype.size() - 1);
  }
  std::cout << "type: [" << type << "]\npath: [" << path << "]\nfiletype: [" << filetype << "]\nquery: ["
  << query << "]\n";

  if (type != "GET") {
    LOG_ERROR("NOT GET!");
    return false;
  }

  std::string fileName = path;
  if (path == "/") {
    fileName = "/index.html";
  }

  std::string filePath = "../html";
  filePath.append(fileName);

  if (filetype == "php") {
    //* php-cgi test/index.php id=1 name=csm > test/index.php.html
    std::string cmd = "php-cgi ";
    cmd.append(filePath);
    cmd.append(" ");
    //* query id=1&name=csm  =>  id=1 name=csm
    replace(query.begin(), query.end(), '&', ' ');
    cmd.append(query);
    cmd.append(" > ");
    cmd.append(filePath.append(".html"));
    LOG_INFO("======query======:\n%s", query.c_str());
    LOG_INFO("======cmd======:\n%s", cmd.c_str());
    system(cmd.c_str());
    int idx = filePath.find_first_not_of("/");
  }

  fp_ = fopen(filePath.c_str(), "rb");
  if (!fp_) {
    LOG_ERROR("OPEN FILE %s FAILED!", filePath.c_str());
    return false;
  }

  //* 将文件指针移动到结尾
  fseek(fp_, 0, SEEK_END);
  //* 根据结尾指针位置获取文件大小
  int fileSize = ftell(fp_);
  //* 恢复指针位置
  fseek(fp_, 0, 0);
  LOG_INFO("FILE size is: %d", fileSize);

  if (filetype == "php") {
    char c = 0;
    int headSize = 0;
    while (fread(&c, 1, 1, fp_) > 0) {
      ++headSize;
      if (c == '\r') {
        fseek(fp_, 3, SEEK_CUR);
        headSize += 3;
        break;
      }
    }
    fileSize = fileSize - headSize;
  }

  //* 回应 http GET 请求
  std::string reMsg = "";
  //* 消息头
  reMsg.append("HTTP/1.1 200 OK\r\n");
  reMsg.append("Server:MDHttp\r\n");
  reMsg.append("Content-Type: text/html\r\n");
  reMsg.append("Content-Length: ");
  char size[128] = {0};
  sprintf(size, "%d", fileSize);
  reMsg.append("size\r\n");
  reMsg.append("\r\n");
  return true;
}

std::string HttpResponse::getHeader() {
  //* 回应 http GET 请求
  std::string header = "";
  //* 消息头
  header.append("HTTP/1.1 200 OK\r\n");
  header.append("Server:MDHttp\r\n");
  header.append("Content-Type: text/html\r\n");
  header.append("Content-Length: ");
  char size[128] = {0};
  sprintf(size, "%ld", fileSize_);
  header.append("size\r\n");
  header.append("\r\n");
  return header;
}

int HttpResponse::read(char *msg, int msgLen) {
  return fread(msg, 1, sizeof(msg), fp_);
}

