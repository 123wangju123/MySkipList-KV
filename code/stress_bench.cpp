/*
 * @Author: WJ
 * @Date: 2022-06-09 20:43:14
 * @Description: 压力测试工作台（针对KV存储引擎）:测试n个进程运行t秒可以执行的插入\搜索操作的数量
 * @LastEditTime: 2022-07-09 17:56:28
 * @FilePath: \code\stress_bench.cpp
 */
#include"stress_bench.h"

// 类的静态成员变量的类外声明
int StressBench::_timerexpired=0;


void StressBench::run(int test_time,int thread_count){
    this->_benchtime=test_time;
    this->_thread_count=thread_count;
    // 打印运行的关键信息
    std::cout<<("\nBegin testing: ")<<_thread_count<<" threads"<<", running "<<_benchtime<<" sec"<<std::endl;
    
    pid_t pid=0;
    FILE *f;
    // 拷贝clients个进程
    for(int i=0;i<_thread_count;i++){
        pid=fork();
        // 如果是子进程或者出错
        if(pid <= (pid_t) 0){
            sleep(1); /* make childs faster */
            break;// 退出循环
        }
    }
    // 如果出错
    if(pid<(pid_t)0){
        perror("fork failed.");
    }
    // 子进程将要执行的代码
    if(pid== (pid_t) 0){
        int count=0;
        int failed=0;
        this->test_target_function(count,failed);// 子进程进行测试
        // 将该进程的测试结果写入匿名管道
        f=fdopen(_pipe[1],"w");// 打开管道的写端
        if(f==NULL){
            perror("open pipe for writing failed.");
        }
        fprintf(f,"%d %d\n",count,failed);//每次写入一行格式化的数据
        fclose(f);
    }
    // 父进程执行的代码
    else{
        f=fdopen(_pipe[0],"r");// 打开匿名管道的读端
        if(f==NULL) {
            perror("open pipe for reading failed.");
        }
        setvbuf(f,NULL,_IONBF,0);
        int count=0;
        int failed=0;
        int i,j,client=_thread_count;
        while(1){
            pid=fscanf(f,"%d %d",&i,&j);
            if(pid<2){
                fprintf(stderr,"Some of our childrens died.\n");
                break;
            }
            count+=i;
            failed+=j;
            if(--client==0) break;
        }
        fclose(f);
        printf("\n-----------------------------------------------------\n");
        printf("this stress test used: threads= %d runtime= %d sec\n",_thread_count,_benchtime);
        //std::cout<<("this stress test used ")<<_thread_count<<" threads"<<", run "<<_benchtime<<" sec"<<endl;
        printf("your final  result is: success= %d failed= %d",count,failed);
        printf("\n-----------------------------------------------------\n");
    }
}

void StressBench::test_target_function(int& count,int& failed){
    this->set_alarm_signal();
    int n=10;
    while(1){
        try{
            if(_timerexpired){
                break;
            }
            _skiplist->insert_element(rand() % _test_count, "a"); // 插入数据
            count++;
        }
        catch(const std::exception& e){
            std::cerr << e.what() << '\n';
            failed++;
            break;
        }
    }
}

void StressBench::set_alarm_signal(){
    struct sigaction sa;
    sa.sa_handler=alarm_handler;
    sa.sa_flags=0;
    if(sigaction(SIGALRM,&sa,NULL))
        exit(3);
    alarm(_benchtime);
}




   
