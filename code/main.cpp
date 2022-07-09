/*
 * @Author: WJ
 * @Date: 2022-06-09 20:43:14
 * @Description: KV存储引擎的使用示范
 * @LastEditTime: 2022-07-09 17:56:28
 * @FilePath: \code\main.cpp
 */
#include <iostream>
#include"stress_bench.h"

#define FILE_PATH "./data/dumpFile" // 数据保存文件路径

// 函数声明
void test_load(SkipList& skipList);
void test_dump(SkipList& skipList);
// 主函数
int main() {
    std::cout<<"-----------------------"<<std::endl;
    std::cout<<"storage_engine begins running..."<<std::endl;

    
    // SkipList skipList(5);//初始化跳表存储引擎
    // // 测试功能
    // if(true){
    //     test_dump(skipList);
    // }
    // else{
    //     test_load(skipList);
    // }

    // 压力测试台
    StressBench bench;
    bench.run(1,2);
}
// 测试保存功能
void test_dump(SkipList& skipList){
    // 插入若干K-V
    skipList.insert_element(1, "北京"); 
    skipList.insert_element(3, "上海"); 
	skipList.insert_element(3, "上海"); 
    skipList.insert_element(6, "广州"); 
    skipList.insert_element(7, "深圳"); 
    skipList.insert_element(8, "苏州"); 
    skipList.insert_element(9, "南京"); 
    skipList.insert_element(10, "武汉"); 
    skipList.insert_element(11, "天津"); 
    skipList.insert_element(12, "西安"); 
    skipList.insert_element(13, "成都"); 
    // 搜索若干K-V
    skipList.search_element(3);
    //skipList.search_element(3);
    // 打印存储结果
    std::cout << "skipList size:" << skipList.size() << std::endl;
    skipList.display_list();
    // 删除若干K-V
    skipList.erase_element(3);
    // 打印存储结果
    std::cout << "skipList size:" << skipList.size() << std::endl;
    skipList.display_list();
    // 保存存储数据
    skipList.dump_file(FILE_PATH);
}
// 测试加载功能
void test_load(SkipList& skipList){
    skipList.load_file(FILE_PATH);
    std::cout << "skipList size:" << skipList.size() << std::endl;
    skipList.display_list();
}
