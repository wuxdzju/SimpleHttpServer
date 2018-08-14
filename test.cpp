#include "base/noncopyable.h"
#include "base/Thread.h"
#include <iostream>
#include "base/CurrentThread.h"
#include "EventLoop.h"
#include "base/TimeUnit.h"
#include "TimerManager.h"
#include "EventLoopThread.h"
#include "Socket.h"
#include "SocketHand.h"
#include "Acceptor.h"
#include "InetAddr.h"
#include "Connection.h"
#include "Server.h"

#include <sys/timerfd.h>
#include "Channel.h"
#include "Poller.h"
#include <string.h>
#include "test/echo.h"


#include <utility>
#include <set>

using namespace std;

void threadfunc()
{
    cout<<"CurrentThread::tid():"<<CurrentThread::tid()<<endl;
}

void test_thread()
{
    typedef Thread::ThreadFunc ThreadFunc;
    ThreadFunc fuc=threadfunc;
    Thread* threads[4];
    for(int i=0;i<4;i++)
    {
        threads[i]=new Thread(fuc);
    }
    for(int i=0;i<4;i++)
    {
        threads[i]->start();
        threads[i]->join();//这一句主要用来处理主线程和新线程之间的竞争，如果没有这一句，主线程不会等待工作线程的执行，
        // 可能工作线程还没执行，整个进程就已经终止了，这样工作可能没有机会运行。
    }

    for(int i=0;i<4;i++)
    {
        //threads[i]->start();
        //threads[i]->join();//这一句主要用来处理主线程和新线程之间的竞争，如果没有这一句，主线程不会等待工作线程的执行，
        // 可能工作线程还没执行，整个进程就已经终止了，这样工作可能没有机会运行。
    }
}



void threadFuncEventLoop()
{
    cout<<"threadFuncEventLoop(): pid="<<getpt()<<" tid="<<CurrentThread::tid()<<endl;

    EventLoop loop;
    loop.loop();
}

void test_EventLoop()
{
    cout<<"main(): pid="<<getpt()<<" tid="<<CurrentThread::tid()<<endl;

    EventLoop loop;
    Thread thread(threadFuncEventLoop);
    thread.start();

    loop.loop();
    pthread_exit(nullptr);
}

EventLoop *g_loop;//全局变量

void threadFuncEventLoop2()
{

    g_loop->loop();
}

void test_EventLoop2()
{

    EventLoop loop;
    g_loop=&loop;
    Thread thread(threadFuncEventLoop2);
    thread.start();
    thread.join();
}

void test_timeunit()
{
    TimeUnit tu1=TimeUnit::now();
    string nowtime=tu1.ToFormatString();
    string nowtimestr=tu1.ToString();
    cout<<nowtime<<endl;
    cout<<nowtimestr<<endl;
    sleep(5);
    TimeUnit tu2=TimeUnit::now();
    nowtime=tu2.ToFormatString();
    cout<<nowtime<<endl;
    cout<<(tu1<tu2)<<endl;
    cout<<(tu1==tu2)<<endl;
    cout<<(tu1==tu1)<<endl;
}

void timeout()
{
    cout<<"timeout"<<endl;
    g_loop->quit();
}

void test_pair()
{
    pair<int,string> pair1=make_pair(1,"hr");
    pair<int,string> pair2=make_pair(1,"he");

    set<pair<int,string> > set1;
    set1.insert(pair1);
    set1.insert(pair2);

    cout<<(pair1<pair2)<<endl;
}

void test3()
{
//    EventLoop loop;
//    g_loop=&loop;
//
//    assert(g_loop->isInLoopThread());
//
//    int timerfd=timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);
//    Channel channel(g_loop,timerfd);
//    channel.setReadCallBack(timeout);
//    channel.enableReading();
//
//    struct itimerspec howlong;
//    bzero(&howlong,sizeof(howlong));
//    howlong.it_value.tv_sec=5;
//    timerfd_settime(timerfd,0,&howlong, nullptr);
//
//    loop.loop();
//
//    close(timerfd);
}

int cnt=0;

void print(const char* msg)
{
    printf("msg %s %s\n",TimeUnit::now().ToString().c_str(),msg);
    if(++cnt==20)
    {
        g_loop->quit();
    }
}

void test_timequeue()
{
    EventLoop loop;
    g_loop=&loop;

    print("main");
    loop.runAfter(1,std::bind(print,"once1"));
    loop.runAfter(1.5,std::bind(print,"once1.5"));
    loop.runAfter(2.5,std::bind(print,"once2.5"));
    loop.runAfter(3.5,std::bind(print,"once3.5"));
    loop.runEvery(2,std::bind(print,"every2"));
    loop.runEvery(3,std::bind(print,"every3"));

    loop.loop();
    print("main loop exit");
}


int g_flag=0;

void run4()
{
    printf("run4():tid=%d,flag=%d\n",CurrentThread::tid(),g_flag);
    g_loop->quit();
}

void run3()
{
    printf("run3():tid=%d,flag=%d\n",CurrentThread::tid(),g_flag);
    g_loop->runInLoopThread(run4);
    g_flag=3;
}

void run2()
{
    printf("run2():tid=%d,flag=%d\n",CurrentThread::tid(),g_flag);
    g_loop->runInLoopThread(run3);
    g_flag=3;
}

void run1()
{
    g_flag=1;
    printf("run1():tid=%d,flag=%d\n",CurrentThread::tid(),g_flag);
    g_loop->runInLoopThread(run2);
    g_flag=2;
}

void test5()
{
    printf("main():tid=%d,flag=%d\n",CurrentThread::tid(),g_flag);

    EventLoop loop;
    g_loop=&loop;

    loop.runAfter(2,run1);

    loop.loop();
    printf("main():tid=%d,flag=%d\n",CurrentThread::tid(),g_flag);
}

void threadFunc()
{
    g_loop->runAfter(1,bind(print,"ThreadFunc"));
    sleep(5);
    g_loop->quit();
}

void test5_5()
{
    EventLoop loop;
    g_loop=&loop;
    Thread thread(threadFunc);
    thread.start();
    loop.loop();
}

void runInThread()
{
    printf("runInThread():tid=%d\n",CurrentThread::tid());
}

void test6()
{
    printf("main():tid=%d\n",CurrentThread::tid());

    EventLoopThread loopThread;
    EventLoop *loop=loopThread.startLoop();

    loop->runInLoopThread(runInThread);
    sleep(1);
    loop->runAfter(2,runInThread);
    sleep(3);
    loop->quit();

    printf("exit main");
}

void test_socket()
{
    string ip="127.0.0.1";
    int port=5070;

    InetAddr listenAddr(port);

    Socket listenSocket(SocketHand::createNonBlockingOrDie());
    listenSocket.setReuseAddr(true);
    listenSocket.setReusePort(true);
    listenSocket.bindAddrr(listenAddr);
    listenSocket.listen();

    while(true)
    {
        InetAddr peerAddr(0);
        Socket acceptSocket(listenSocket.acceptConn(&peerAddr));
        char buf[32]="hello\n";
        write(acceptSocket.fd(),buf, sizeof(buf));
    }
}

void newConnection(int sockfd,const InetAddr& peerAddr)
{
    cout<<"NewConnection():accept a new connection from "<<peerAddr.toHostPort()<<endl;
    ::write(sockfd,"Hello\n",6);
    SocketHand::closeFd(sockfd);
}

void newConnection2(int sockfd,const InetAddr& peerAddr)
{
    cout<<"NewConnection():accept a new connection from "<<peerAddr.toHostPort()<<endl;
    ::write(sockfd,"Hello2\n",7);
    SocketHand::closeFd(sockfd);
}


void test_acceptor()
{
    cout<<"main(): tid="<<CurrentThread::tid()<<endl;


    EventLoop loop;

    InetAddr listenAddr(5070);
    Acceptor acceptor(&loop,listenAddr);
    acceptor.setNewConnCallBack(newConnection);
    acceptor.listen();

    InetAddr listenAddr2(5080);
    Acceptor acceptor2(&loop,listenAddr2);
    acceptor2.setNewConnCallBack(newConnection2);
    acceptor2.listen();


    loop.loop();

}

void OnConnection(const ConnectionPtr& conn)
{
    if(conn->Connected())
    {
        printf("OnConnection():new connection [%s] form %s\n",conn->getName().c_str(),conn->getPeerAddr().toHostPort().c_str());
    }
    else
    {
        printf("OnConnection():connection [%s] is down\n",conn->getName().c_str());
    }
}

void OnMessage(const ConnectionPtr&conn,const char*data,ssize_t len)
{
    printf("OnMessage():received %zd bytes form connection [%s]\n",len,conn->getName().c_str());
}

void test_connection()
{
//    printf("main():tid=%d\n",CurrentThread::tid());
//
//    InetAddr listenAddr(9981);
//    EventLoop loop;
//
//    Server server(&loop,listenAddr);
//    server.setConnectionCallBack(OnConnection);
//    server.setMessageCallBack(OnMessage);
//    server.start();
//
//    loop.loop();
}

void timeout2(TimeUnit receiveTime)
{
    printf("%s TimeOut\n",receiveTime.ToFormatString().c_str());
    g_loop->quit();
}

void  test3_3()
{
    printf("%s started\n",TimeUnit::now().ToFormatString().c_str());
    EventLoop loop;
    g_loop=&loop;


    assert(g_loop->isInLoopThread());

    int timerfd=timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);
    Channel channel(g_loop,timerfd);
    channel.setReadCallBack(timeout2);
    channel.enableReading();

    struct itimerspec howlong;
    bzero(&howlong,sizeof(howlong));
    howlong.it_value.tv_sec=5;
    timerfd_settime(timerfd,0,&howlong, nullptr);

    loop.loop();

    close(timerfd);

}

void OnMessage2(const ConnectionPtr&conn,Buffer* buffer,TimeUnit reciveTime)
{
    printf("OnMessage():received %zd bytes form connection [%s] at %s\n",
           buffer->readableBytes(),
           conn->getName().c_str(),
           reciveTime.ToFormatString().c_str());

    printf("OnMessage():[%s]\n",buffer->retrieveAsString().c_str());
}

void test_connection2()
{
    printf("main():tid=%d\n",CurrentThread::tid());

    InetAddr listenAddr(9982);
    EventLoop loop;

    Server server(&loop,listenAddr);
    server.setConnectionCallBack(OnConnection);
    server.setMessageCallBack(OnMessage2);
    server.start();

    loop.loop();
}

std::string message1;
std::string message2;

void onConnection2(const ConnectionPtr& conn)
{
    if(conn->Connected())
    {
        printf("OnConnection():new connection [%s] from %s\n",
               conn->getName().c_str(),
               conn->getPeerAddr().toHostPort().c_str());
        conn->send(message1);
        conn->send(message2);
        conn->shutdown();
    }
    else
    {
        printf("onConnection():connection [%s] is down\n",
               conn->getName().c_str());
    }
}

void test10()
{
    printf("main():tid= %d\n",CurrentThread::tid());

    int len1=10000;
    int len2=200;

    message1.resize(len1);
    message2.resize(len2);
    std::fill(message1.begin(),message1.end(),'A');
    std::fill(message2.begin(),message2.end(),'B');

    InetAddr listenAddr(9983);
    EventLoop loop;

    Server server(&loop,listenAddr);
    server.setConnectionCallBack(onConnection2);
    server.setMessageCallBack(OnMessage2);
    server.start();

    loop.loop();
}



void OnConnectionEcho(const ConnectionPtr &conn)
{
    std::cout<<"EchoServer - "<<conn->getPeerAddr().toHostPort()<<" -> "
             <<conn->getLocalAddr().toHostPort()<<" is "<<(conn->Connected()?"UP":"DOWN")<<std::endl;
    //printf("EchoServer::OnConnection\n");
}

void OnMessageEcho(const ConnectionPtr &conn, Buffer *buffer, TimeUnit timeUnit)
{
    std::string msg(buffer->retrieveAsString());
    //printf("EchoServer::OnMessage");
    std::cout<<conn->getName()<<" echo "<<msg.size()<<" bytes,"
             <<"data receive at "<<timeUnit.ToString()<<std::endl;
    conn->send(msg);
}

void test_echo2()
{
    printf("main():tid= %d\n",CurrentThread::tid());
    EventLoop loop;
    InetAddr listenAddr(9982);
    Server server(&loop,listenAddr);
    server.setConnectionCallBack(OnConnectionEcho);
    server.setMessageCallBack(OnMessageEcho);
    server.start();

    loop.loop();
}

void test_echo()
{
    printf("main():tid= %d\n",CurrentThread::tid());
    EventLoop loop;
    InetAddr listenAddr(9981);
    EchoServer echoServer(&loop,listenAddr);
    echoServer.start();

    loop.loop();
}

void OnConnectionThreadPool(const ConnectionPtr &conn)
{
    if(conn->Connected())
    {
        printf("OnConnection(): tid=%d new connection [%s] from %s\n",
               CurrentThread::tid(),
               conn->getName().c_str(),
               conn->getPeerAddr().toHostPort().c_str());
    }
    else
    {
        printf("OnConnection(): tid=%d  connection [%s] is down\n",
               CurrentThread::tid(),
               conn->getName().c_str());
    }
}

void OnMessageThreadPool(const ConnectionPtr &conn,
                         Buffer* buf,
                         TimeUnit receiveTime)
{
    printf("OnMessage(): tid=%d received %zd bytes from connection [%s] at %s\n",
           CurrentThread::tid(),
           buf->readableBytes(),
           receiveTime.ToFormatString().c_str());
    printf("OnMessage(): [%s]\n",buf->retrieveAsString().c_str());
}

void testThreadPool()
{
    printf("main():tid= %d\n",CurrentThread::tid());

    InetAddr listenAddr(9981);
    EventLoop loop;

    Server server(&loop,listenAddr);
    server.setMessageCallBack(OnMessageThreadPool);
    server.setConnectionCallBack(OnConnectionThreadPool);
    server.setThreadNum(4);
    server.start();

    loop.loop();
}

int main()
{
    //test_timeunit();
    //test3();
    //test_pair();
    //test_timequeue();
    //test5();
    // test6();
    //test_socket();
    //test_acceptor();
    //test_connection();
    //test3_3();
    //test_connection2();
    //test10();
    //test_echo();
    testThreadPool();
}