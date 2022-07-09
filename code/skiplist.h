/*
 * @Author: WJ
 * @Date: 2022-06-07 20:43:14
 * @Description: 跳表声明
 * @LastEditTime: 2022-06-08 17:56:28
 * @FilePath: \code\skiplist.h
 */
#include <iostream> 
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <mutex>
#include <fstream>
#include <string>
#include <vector>
#include"node.h"

using namespace std;

// 跳表类
class SkipList {
public: // 对外接口
    SkipList(int); // 构造函数
    ~SkipList(); // 析沟函数
    int insert_element(int, string); // 插入某条记录
    void display_list(); // 显示跳表结构
    bool search_element(int); // 搜索某条记录
    bool erase_element(int key); // 删除某条记录  
    void dump_file(string path); // 导出跳表结构
    void load_file(string path); // 加载跳表结构
    int size(){ return _element_count;} // 返回存储记录数量
private: // 对内方法
    Node* create_node(Node* right,Node* down,const int k, const string v){return new Node(right,down,k, v);}
private: // 内部数据   
    int _max_level;// 最大层数
    int _skip_list_level; // 当前层数
    int _element_count; // 存储的记录数
    Node* _header; // 跳表头节点
    vector<Node*> pathList;// 临时存储每一层的插入位置
    // 文件流对象
    std::ofstream _file_writer;
    std::ifstream _file_reader;
    // 互斥锁
    std::mutex mtx;     // 互斥锁
    std::string delimiter = ":"; // 导出文件中K-V的分割符
};

