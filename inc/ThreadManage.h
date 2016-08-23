#pragma once
#include "../inc/DllExport.h"
// CThreadManage
// CThreadManage的功能非常简单，其提供最简单的方法，其类定义如下：
// 

class CJob;
class CThreadPool;
class MYTHREADPOOL_DLL CThreadManage
{
public:
	void     SetParallelNum(int num);
	CThreadManage();
	CThreadManage(int num);
	virtual ~CThreadManage();
	void    Run(CJob* job, void* jobdata);
	void    TerminateAll(void);
private:
	CThreadPool*	m_Pool;			// 指向实际的线程池
	int				m_NumOfThread;	// 初始创建时候允许创建的并发的线程个数
};