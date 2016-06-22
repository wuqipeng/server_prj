#ifndef SYSTEM_FUNC_H_
#define SYSTEM_FUNC_H_

//this file package linux system api ,so that those system function is more friendly.
//
namespace lib { system  {

const int32_t PROCESS_NAME_LEN = 256;
struct ProcessInfo
{
	pid_t pid;                              //进程的PID
	char ProName[PROCESS_NAME_LEN];         //进程名
};


//通过getpid获取进程pid
//通过获取到的pid，来取得/proc/$(pid)/exe对应的软连接，也就是相应的可执行文件的文件路径

int32_t GetProcessInfo(ProcessInfo *pro_info);


}}



#endif
