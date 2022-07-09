/*
 * @Author: WJ
 * @Date: 2022-06-09 20:43:14
 * @Description: 压力测试工作台（针对KV存储引擎）:测试n个进程运行t秒可以执行的插入\搜索操作的数量
 * @LastEditTime: 2022-07-09 17:56:28
 * @FilePath: \code\stress_bench.h
 */
#include<iostream>
#include<chrono>
#include<cstdlib>
#include<time.h>
#include <unistd.h>
#include <sys/param.h>
#include <rpc/types.h>
#include <getopt.h>
#include <strings.h>
#include <signal.h>
#include"../code/skiplist.h"

// 压力测试类
class StressBench{
public:
    // 构造函数
    StressBench(int levels=18,int threads=1,int runtime=1,int maxCount=10000){
        srand (time(NULL)) ;
        _skiplist=new SkipList(levels);
        _thread_count=threads;
        _benchtime=runtime;
        _test_count=maxCount;
        // 创建匿名管道
        if(pipe(_pipe)){
            perror("pipe failed.");
        }
    }
    // 运行压力测试功能
    void run(int test_time,int thread_count);
private:
    // 测试的目标功能
    void test_target_function(int& count,int& failed);
    // 设置定时信号
    void set_alarm_signal();
    // 到期执行程序
    static void alarm_handler(int signal){_timerexpired=1;}
private:
    int _thread_count; // 测试进程数
    int _benchtime; // 测试时间，单位：秒
    int _test_count; // 插入数据的最大取值
    SkipList* _skiplist; // 用于测试的跳表
    int _pipe[2]; // 匿名管道，用于父子进程间的数据通信
    static int _timerexpired; // 是否到期（到达规定时间）
};

