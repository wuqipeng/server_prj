#ifndef SYSTEM_FUNC_H_
#define SYSTEM_FUNC_H_

//this file package linux system api ,so that those system function is more friendly.
//
namespace lib { system  {

const int32_t PROCESS_NAME_LEN = 256;
struct ProcessInfo
{
	pid_t pid;                              //���̵�PID
	char ProName[PROCESS_NAME_LEN];         //������
};


//ͨ��getpid��ȡ����pid
//ͨ����ȡ����pid����ȡ��/proc/$(pid)/exe��Ӧ�������ӣ�Ҳ������Ӧ�Ŀ�ִ���ļ����ļ�·��

int32_t GetProcessInfo(ProcessInfo *pro_info);


}}



#endif
