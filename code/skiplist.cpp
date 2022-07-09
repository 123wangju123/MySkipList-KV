/*
 * @Author: WJ
 * @Date: 2022-06-07 20:43:14
 * @Description: 跳表实现
 * @LastEditTime: 2022-06-08 17:56:28
 * @FilePath: \code\skiplist.cpp
 */

#include"skiplist.h"
// 构造函数
SkipList::SkipList(int max_level) {
    this->_max_level = max_level;
    this->_skip_list_level = 0;
    this->_element_count = 0;
    // 创建虚拟头节点
    int k;
    string v;
    this->_header = new Node(NULL,NULL,-1, "0head");
};
// 析构函数
SkipList::~SkipList() {
    if (_file_writer.is_open()) {
        _file_writer.close();
    }
    if (_file_reader.is_open()) {
        _file_reader.close();
    }
    delete _header;
}

// 插入某条记录
int SkipList::insert_element(const int key, const string value) {
    mtx.lock();
    //从上至下记录搜索路径
    pathList.clear();
    Node *p = this->_header;
    // 从上到下去搜索，搜索后p指向次小于key的位置，应在p后新插入
    while(p){
        // 从左向右查找
        while(p->right&&p->right->get_key()<key)p=p->right;
        pathList.push_back(p);// 此时的p是最后一个小于num的节点
        p = p->down;// 下降一层
    }
    // 如果current存在,放弃插入
    Node* current=pathList.back()->right;
    if (current!= NULL && current->get_key() == key) {
        std::cout << "key: " << key << ", exists" << std::endl;
        mtx.unlock();
        return 1;
    }
    // 此时p指向最底层中插入位置前
    bool  insertUp = true;
    Node* downNode = NULL;// 下一个节点为NULL
    // 从下至上搜索路径回溯，50%概率
    // 这里实现是会保证不会超过当前的层数的，然后靠头结点去额外加层， 即每次新增一层
    int level=0;
    while (insertUp && pathList.size() > 0){
        Node *insert = pathList.back();
        pathList.pop_back();
        // 添加新结点
        insert->right = new Node(insert->right,downNode,key,value); 
        // 把新结点赋值为downNode
        downNode = insert->right; 
        // 50%概率   
        insertUp = (rand()&1)==0;
        level++;
    }
    // 说明pathList已经不为空，插入新的头结点，加层
    if(insertUp){  
        this->_header = new Node(new Node(NULL,downNode,key,value), this->_header, -5,to_string(level)+"head");
        this->_skip_list_level++;
    }

    this->_element_count ++;
    std::cout << "Successfully inserted key:" << key << ", value:" << value << std::endl;
    mtx.unlock();
    return 0;
}
// 显示跳表结构
void SkipList::display_list() {
    std::cout << "\n*****【Skip List】*****"<<"\n"; 
    Node *p_header = this->_header;
    Node *p= nullptr;
    int level=this->_skip_list_level;
    // 从上到下搜索
    while(p_header){
        std::cout << "Level " << level << ": ";
        p=p_header;
        // 从左向右查找,打印当前层
        while(p->right){
            p=p->right;
            std::cout <<p->get_key() << ":" << p->get_value() << ";";
        }
        std::cout << std::endl;
        p_header = p_header->down;// 下降一层
        level--;
    }
    std::cout << "\n*********************"<<"\n"; 
}
// 搜索某条记录
bool SkipList::search_element(int key) {
    mtx.lock();
    Node *p = this->_header;
    while(p){
        // 从左向右寻找目标区间
        while(p->right && p->right->get_key() < key){
            p = p->right;
        }
        // 没找到目标值，则继续往下走
        if(!p->right || p->right->get_key()>key){
            p = p->down;
        }
        else{   
            std::cout << "成功找到key: " << key << ":"<<p->right->get_value()<<endl;
            return true;////找到目标值，结束
        }
    }
    std::cout << "无法找到key: "<<key<<endl;
    mtx.unlock();
    return false;
}
// 删除某条记录  
bool SkipList::erase_element(int key) {
    Node *p = this->_header;
    bool seen = false;
    while (p){
        // 当前层向右找到次小的点
        while (p->right && p->right->get_key() < key)p = p->right;
        // 无效点，或者 太大， 则到下一层去
        if (!p->right||p->right->get_key()> key){  
            p = p->down;
        }
        else{
            // 搜索到目标结点，进行删除操作，结果记录为true
            // 继续往下层搜索，删除一组目标结点
            seen = true;
            p->right = p->right->right;
            p = p->down;
        }
    }
    if(seen) {
        this->_element_count--;
        std::cout << "成功删除key: "<<key<<endl;
    }
    else std::cout << "无法删除key: "<<key<<endl;
    return seen;
}
// 导出跳表结构
void SkipList::dump_file(string path) {
    std::cout << "engine is dumping-----------------" << std::endl;
    _file_writer.open(path);// 打开文件
    Node *p_header = this->_header;
    Node *p=nullptr;
    while (p_header!= NULL) {
        p=p_header;
        while(p->right){
            p=p->right;
            _file_writer << p->get_key() << ":" << p->get_value() << "\n";
        }
        p_header = p_header->down;// 下降一层
    }
    _file_writer.flush();
    _file_writer.close(); 
    return ;
}
// 加载跳表结构
void SkipList::load_file(string path) {
    std::cout << "engine is loading-----------------" << std::endl;
    _file_reader.open(path);
    std::string line;
    std::string* key = new std::string();
    std::string* value = new std::string();
    while (getline(_file_reader, line)) {
        //get_key_value_from_string(line, key, value);
        *key = line.substr(0, line.find(delimiter));
        *value = line.substr(line.find(delimiter)+1, line.length());
        if (key->empty() || value->empty()) {
            std::cout << "无法识别的key或value"<< std::endl;
            continue;
        }
        this->insert_element(stoi(*key), *value);
        //std::cout << "key:" << *key << "value:" << *value << std::endl;
    }
    _file_reader.close();
}

