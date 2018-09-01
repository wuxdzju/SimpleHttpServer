sumer WebServer
---
#### Introduction
本项目采用reactor+线程池模式搭建了一个Web服务器，项目基于C++11实现，解析了GET请求，可处理静态资源，支持HTTP长连接。

| Part Ⅰ | Part Ⅱ | Part Ⅲ | Part Ⅳ | Part Ⅴ |
| :--------: | :---------: | :---------: | :---------: | :---------: |
| [项目目的](https://github.com/wuxdzju/sumer/blob/fixb/%E9%A1%B9%E7%9B%AE%E7%9B%AE%E7%9A%84.md)|[并发模型](https://github.com/wuxdzju/sumer/blob/fixb/%E5%B9%B6%E5%8F%91%E6%A8%A1%E5%9E%8B.md)|[核心结构](https://github.com/wuxdzju/sumer/blob/fixb/%E6%A0%B8%E5%BF%83%E7%BB%93%E6%9E%84.md) | [测试及改进](https://github.com/wuxdzju/sumer/blob/fixb/%E6%B5%8B%E8%AF%95%E5%8F%8A%E6%94%B9%E8%BF%9B.md) | [遇到的困难]()

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



