/*
 * @Author: WJ
 * @Date: 2022-06-07 20:43:14
 * @Description: 跳表节点类
 * @LastEditTime: 2022-06-08 17:56:28
 * @FilePath: \code\node.h
 */
#include <string>
using std::string;

// 节点类
class Node {
public:
    // 构造函数
    Node(Node* right,Node* down,const int k, const string v):right(right),down(down),key(k),value(v){}
    int get_key() const{return key;};
    string get_value() const{return value;}
    void set_value(string){this->value=value;}
public:
    Node* right;// 右节点指针
    Node* down;// 下节点指针
private:
    int key; // 索引
    string value;// 数据
};