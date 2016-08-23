// MyThreadPoolTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <windows.h>
#include "../inc/Job.h"
#include "../inc/ThreadManage.h"

// �̳߳�ʹ��ʾ��
// �������Ǹ�����һ���򵥵�����������޹ص��̳߳ؿ�ܡ�ʹ�øÿ�ܷǳ��ļ򵥣���������Ҫ�����ľ�������CJob�࣬
// ����Ҫ��ɵ�����ʵ����Run�����С�Ȼ�󽫸�Job����CThreadManageȥִ�С��������Ǹ���һ���򵥵�ʾ������
//
// CXJob��CYJob���Ǵ�Job��̳ж������䶼ʵ����Run�ӿڡ�CXJobֻ�Ǽ򵥵Ĵ�ӡһ�䡱The Job comes from CXJob����
// CYJobҲֻ��ӡ��The Job comes from CYJob����Ȼ�������2���ӡ��������������ǳ�ʼ����10�������̡߳�Ȼ��ֱ�ִ��40��CXJob��һ��CYJob��
// 
// �̳߳�ʹ�ú��
// �̳߳��ʺϳ���
// ��ʵ�ϣ��̳߳ز��������ܵġ��������ض���ʹ�ó��ϡ��̳߳������ڼ����̱߳���Ŀ�����Ӧ����������Ӱ�죬������ǰ��ģ�
// ǰ������̱߳��������߳�ִ��������Ȳ��ɺ��ԡ�����̱߳���Ŀ���������߳�����ִ�п��������ǿ��Ժ��Բ��Ƶģ�
// ��ô��ʱ�̳߳��������ĺô��ǲ����Եģ��������FTP�������Լ�Telnet��������ͨ�������ļ���ʱ��ϳ��������ϴ���ô��ʱ��
// ���ǲ����̳߳�δ��������ķ��������ǿ���ѡ�񡰼�ʱ��������ʱ���١��Ĳ��ԡ�
// 
// ��֮�̳߳�ͨ���ʺ�����ļ������ϣ�
// (1)  ��λʱ���ڴ�������Ƶ������������ʱ���
// (2)  ��ʵʱ��Ҫ��ϸߡ�������ܵ�������ڴ����̣߳��������㲻��ʵʱҪ����˱�������̳߳ؽ���Ԥ������
// (3)  ���뾭����Ը�ͻ�����¼�������Web�����������������ת������������������޴�ĳ������ʱ�����ȡ��ͳ������
//		����벻ͣ�Ĵ��������̣߳������̡߳���ʱ���ö�̬�̳߳ؿ��Ա�����������ķ�����
//
class CXJob : public CJob
{
public:
	CXJob() {}
	~CXJob() {}
	void Run(void* jobdata) 
	{
		printf("The Job comes from CXJOB/n");
		Sleep(2);
	}
};

class CYJob :public CJob
{
public:
	CYJob() {}
	~CYJob() {}
	void Run(void* jobdata) 
	{
		printf("The Job comes from CYJob/n");
	}
};

int main()
{
	CThreadManage* manage = new CThreadManage(10);
	for (int i = 0;i < 40;i++)
	{
		CXJob* job = new CXJob();
		manage->Run(job, NULL);
	}

	Sleep(2);
	CYJob* job = new CYJob();
	manage->Run(job, NULL);
	manage->TerminateAll();
    return 0;
}

