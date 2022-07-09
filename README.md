

# 基于跳表结构的KV存储引擎设计

​		该项目是作者学习**数据库技术**时整理的项目，非关系型数据库Redis的核心存储引擎使用的数据结构就是**跳表**，该项目采用**C++语言**基于**跳表结构**设计了一个**轻量级键值型存储引擎**，支持**插入**、**查询**、**删除**、**显示**、**导出**、**重加载**6种基本操作。为了测试引擎效率，作者开发了一个使用和web bench相似技术的压力测试工具**Stress Bench**。在**随机读写**情况下，该引擎每秒在**多个并发线程**下的**累计执行成功次数**都可以保持在30000左右。

## 技术栈

* 基于**跳表结构**和**面向对象思想**的节点类、跳表类的开发与封装；
* 基于**父子多进程**和**匿名管道技术**的**Stress Bench**测试工具；
* 使用**互斥锁**，防止并发插入数据时发生冲突；
* 基于**MakeFile**的多文件编译方式；

## 对外接口

* insert_element(int,string)  				// 插入记录
* srase_element(int)                              // 删除记录
* search_element(int)                            // 查询记录
* displayList(int)                                     // 显示结构
* dumpFile(string)                                  // 导出数据
* loadFile(string)                                    // 加载数据
* size()                                                     // 获取数据规模

## 环境要求

* Linux操作系统
* C++11编程语言
* MakeFIle项目编译工具

## 目录树

```tex
.
├── bin            
│   └── main	   			二进制文件
├── build           
│   └── Makefile   			编译指令
├── code           			源代码
│   ├── node.h     		 	定义跳表节点    
│   ├── skiplist.h       	定义跳表结构
│   ├── skiplist.cpp     	实现跳表结构
│   ├── stress_bench.h   	定义压测工具
│   ├── stress_bench.cpp 	实现压测工具
│   └── main.cpp   		 	入口函数
├── data            
│   └── maindumpFile		数据存储文件
├── LICENSE					使用协议
├── Makefile       			启动指令  
└── readme.md				项目简介
```


## 项目启动

#### 1、进行压力测试

```bash
make            
```

#### 2、测试引擎功能

调用main.cpp中的test_dump()和test_load()函数，执行如下指令：

```bash
make            
```

#### 3、使用跳表引擎

引入node.h、skiplist.h和skiplist.cpp即可。

## 压力测试

* 测试环境: Ubuntu:19.10 cpu:i5-8400 内存:8G 

在跳表最大高度为18， 最大随机数据为100000时， 插入数据的测试结果如下：

| 并发线程数 | 每秒执行成功次数 | 每秒执行失败次数 |
| ---------- | ---------------- | ---------------- |
| 1          | 27229            | 0                |
| 2          | 34204            | 0                |
| 3          | 32799            | 0                |
| 6          | 29815            | 0                |
| 12         | 33026            | 0                |
| 24         | 33116            | 0                |
| 100        | 34737            | 0                |

可见，在串行或者并行实验下，该引擎在多个并发线程数下的累计执行成功次数都可以保持在30000左右。

## 未来展望 

* delete的时候没有释放内存
* 引擎的key和value必须是int型和string型，可以考虑优化
* 可以考虑进一步改造为分布式存储服务

## 参考资料

- https://github.com/youngyangyang04/Skiplist-CPP
-  [Java手写实现跳表 - 设计跳表 - 力扣（LeetCode）](https://leetcode.cn/problems/design-skiplist/solution/javashou-xie-shi-xian-tiao-biao-by-feng-omdm0/) .



