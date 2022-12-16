/*
moduo网络库给用户提供了两个主要的类
TcpServer：用于编写服务器程序的
TcpClient：用于编写客户端程序的
epoll+线程池
好处：能够将网络I/O的代码和业务代码区分开
用户的连接和断开 用户的可读写事件
*/
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <string>
#include <functional> //绑定器，用于绑定
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders; // 占位符
/*
基于muduo网络库开发服务器程序
1.组合TcpServer对象
2.创建EventLoop事件循环对象的指针
3.明确TcpServer构造函数需要什么参数，输出ChatServer的构造函数
4.在当前服务器类的构造函数中，注册处理连接的回调函数和处理读写事件的回调函数
5.设置合适的服务端线程数量，muduo库会自己划分I/O线程和worker线程
*/
class ChatServer
{
public:
    ChatServer(EventLoop *loop,               // 事件循环，也是reactor反应堆
               const InetAddress &listenAddr, // IP+Port
               const string &nameArg)         // 服务器的名字，也就是给这个线程绑定一个名字
        : _server(loop, listenAddr, nameArg), _loop(loop)
    {
        // 给服务器注册用户连接的创建和断开回调
        _server.setConnectionCallback(bind(&ChatServer::onConnection, this, _1)); // 方法只有1个参数，所以_1参数占位符
        // 给服务器注册用户读写事件回调
        _server.setMessageCallback(bind(&ChatServer::onMessage, this, _1, _2, _3));
        //_1,代表第一个占位符，_2代表第二个，使用的时候依次递增使用即可
        // 默认是单线程的，这样epoll既要处理新用户的连接 又要处理事件
        // 设置服务器端的线程数量 1个I/O线程，3个worker线程
        _server.setThreadNum(4);
    }
    // 开启事件循环
    void start()
    {
        _server.start();
    }

private:
    // 专门处理用户的连接创建和断开 epoll listenfd accept
    // 只要有用户的连接，这个方法就会响应
    void onConnection(const TcpConnectionPtr &conn)
    {
        // 成员方法会有this指针，多一个参数
        if (conn->connected())
        {
            // peerAddress对端的地址
            cout << conn->peerAddress().toIpPort() << "->" << conn->localAddress().toIpPort() << "state:online" << endl;
        } // toIpPort把ip地址和端口号都打印出来
        else
        {
            cout << conn->peerAddress().toIpPort() << "->" << conn->localAddress().toIpPort() << "state:offline" << endl;
            conn->shutdown(); // close(fd)
            //_loop->quit();
        }
    }
    // 专门处理用户的读写事件
    void onMessage(const TcpConnectionPtr &conn, // 连接
                   Buffer *buffer,               // 缓冲区
                   Timestamp time)               // 接收到数据的时间信息
    {
        string buf = buffer->retrieveAllAsString();
        cout << "recv data:" << buf << "time:" << time.toString() << endl;
        conn->send(buf);
    }

    TcpServer _server; // 1.定义一个server
    // 因为_server没有默认构造，就没办法创建对象，需要三个参数
    EventLoop *_loop; // 2. epoll
};

int main()
{
    EventLoop loop; // epoll
    InetAddress addr("127.0.0.1", 6000);
    ChatServer server(&loop, addr, "ChatServer");
    server.start(); // listenfd epoll_ctl=>epoll
    loop.loop();    // epoll_wait以阻塞方式等待新用户连接，已连接用户的读写事件等
    return 0;
}