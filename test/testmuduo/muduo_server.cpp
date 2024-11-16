/*
muduo网络库给用户提供了两个主要的类
TcpServer：用来编写服务器程序的
TcpClient：用来编写客户端程序的

epoll+线程池
好处：能够把网络I/O的代码和业务区代码分开
我们只管用户的连接与断开，用户的可读写事件就行
*/
#include<muduo/net/TcpServer.h>
#include<muduo/net/EventLoop.h>
#include<iostream>
#include<functional>//绑定器
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;//参数占位符
/*基于muduo网络库开发服务器程序
1.组合Tcpserver对象
2.创建EventLoop事件循环对象的指针
3.明确TcpServer构造函数需要的参数，输出ChatServer的参数
4.在当前服务器类的构造函数中，注册处理连接的回调函数和处理读写事件的回调函数
5.设置合适的服务器端线程数量，nuduo库会自己分配io线程和work线程
*/
class ChatServer{
public:
     ChatServer(EventLoop* loop,//事件循环
            const InetAddress& listenAddr,//IP+port
            const string& nameArg)//服务器名
            :_server(loop,listenAddr,nameArg),_loop(loop)
            {
                //给服务器注册用户连接的创建和断开回调
                _server.setConnectionCallback(std::bind(&ChatServer::onConnection,this,_1));
                
                //给服务器注册用户读写事件回调
                _server.setMessageCallback(std::bind(&ChatServer::onMessage,this,_1,_2,_3));    
                //设置服务器端的线程数量 1个I/O线程， 3个work线程
                _server.setThreadNum(4);
            }
            //开启事件循环
            void start(){
                _server.start();
            }
private:
    //我们只专注这个专门处理用户的连接创建和断开 epoll listenfd accept
    void onConnection(const TcpConnectionPtr &conn){
        if(conn->connected()){
            cout<<conn->peerAddress().toIpPort()<<" -> "<<conn->localAddress().toIpPort()<<"state:online"<<endl;
        }
        else {
            cout<<conn->peerAddress().toIpPort()<<" -> "<<conn->localAddress().toIpPort()<<"state:offline"<<endl;
            conn->shutdown();//close(fd)
            //_loop->quit();//连接断开
        }
    }
    //专门处理用户的读写事件
    void onMessage(const TcpConnectionPtr &conn,//连接
                    Buffer *buffer, //缓冲区
                    Timestamp time) //接收到数据的时间信息
                    {            
                    string buf=buffer->retrieveAllAsString();
                    cout<<"recv data:"<<buf<<"time:"<<time.toString()<<endl;
                    conn->send(buf);
                    }
    TcpServer _server;//必须构造，没有默认构造
    EventLoop *_loop;
};
int main(){
    EventLoop loop;//epoll
    InetAddress addr("192.168.126.144",6000);
    ChatServer server(&loop,addr,"ChatServer");//循环事件，addr对象，名字
    server.start();
    loop.loop();//epoll_wait以阻塞的方式等待新用户连接，已连接用户的读写事件等
    return 0;
}