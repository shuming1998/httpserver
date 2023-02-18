# HTTP_Server

#### 1. Environment:

```shell
$ sudo apt-get install php-cgi
```



#### 2. Build:

```shell
$ cd Http_Server
$ chmod +x ./autobuild.sh
$ ./autobuild.sh
```



#### 3.1. Run server:

```shell
$ cd bin
# port can be set arbitrarily, if not specified, the default value is 3000
$ ./http_server 8888   
```

#### 3.2. Http test:

Open a browser and visit the URL: 

```
127.0.0.1:8888/index1.html
```

```
127.0.0.1:8888/index.php?id=1&name=csm
```



#### 4. About moduo:

https://github.com/chenshuo/muduo



#### 5. Handsolve muduo(this repo used):

https://github.com/shuming1998/muduo-cpp11

