sumer WebServer
---
#### Introduction
本项目采用reactor+线程池模式搭建了一个Web服务器，项目基于C++11实现，解析了GET请求，可处理静态资源，支持HTTP长连接。

| Part Ⅰ | Part Ⅱ | Part Ⅲ | Part Ⅳ | Part Ⅴ |
| :--------: | :---------: | :---------: | :---------: | :---------: |
| [项目目的]()|[并发模型](https://github.com/wuxdzju/sumer/并发模型.md)|[核心结构]() | [测试及改进]() | [遇到的困难]()

#### Environment
- 操作系统：Ubuntu 16.04
- 编译器 ： g++ 5.4.0
- 版本控制：git
- 集成环境：Clion
- 压测工具：WebBench

#### Usage
```shell
cd sumer

mkdir build

cmake ..

make

./sumer [-t threadNum] [-p  port]
```

#### other
本项主要参考了陈硕老师的muduo网络库



