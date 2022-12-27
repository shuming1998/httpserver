#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H
#include <string>

class HttpResponse {
public:
  HttpResponse();
  ~HttpResponse();

  std::string getHeader();
  int read(char *msg, int msgLen);
  bool solveRequest(std::string request);

private:
  FILE *fp_;
  size_t fileSize_;

};

#endif