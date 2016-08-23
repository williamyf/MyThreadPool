// MyThreadPoolTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include "../inc/Job.h"
#include "../inc/ThreadManage.h"

// 线程池使用示例
// 至此我们给出了一个简单的与具体任务无关的线程池框架。使用该框架非常的简单，我们所需要的做的就是派生CJob类，
// 将需要完成的任务实现在Run方法中。然后将该Job交由CThreadManage去执行。下面我们给出一个简单的示例程序
//
// CXJob和CYJob都是从Job类继承而来，其都实现了Run接口。CXJob只是简单的打印一句”The Job comes from CXJob”，
// CYJob也只打印”The Job comes from CYJob”，然后均休眠2秒钟。在主程序中我们初始创建10个工作线程。然后分别执行40次CXJob和一次CYJob。
// 
// 线程池使用后记
// 线程池适合场合
// 事实上，线程池并不是万能的。它有其特定的使用场合。线程池致力于减少线程本身的开销对应用所产生的影响，这是有前提的，
// 前提就是线程本身开销与线程执行任务相比不可忽略。如果线程本身的开销相对于线程任务执行开销而言是可以忽略不计的，
// 那么此时线程池所带来的好处是不明显的，比如对于FTP服务器以及Telnet服务器，通常传送文件的时间较长，开销较大，那么此时，
// 我们采用线程池未必是理想的方法，我们可以选择“即时创建，即时销毁”的策略。
// 
// 总之线程池通常适合下面的几个场合：
// (1)  单位时间内处理任务频繁而且任务处理时间短
// (2)  对实时性要求较高。如果接受到任务后在创建线程，可能满足不了实时要求，因此必须采用线程池进行预创建。
// (3)  必须经常面对高突发性事件，比如Web服务器，如果有足球转播，则服务器将产生巨大的冲击。此时如果采取传统方法，
//		则必须不停的大量产生线程，销毁线程。此时采用动态线程池可以避免这种情况的发生。
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

