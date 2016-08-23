#pragma once
#include "../inc/DllExport.h"
#include <vector>
#include <boost/thread.hpp>

// CThreadPool
// 
// CThreadPool���̵߳ĳ���������һ����Խ���ʵ��Ϊ��ջ��������л���˫����С������ǵ�ϵͳ������ʹ��STL Vector���߳̽��б��档
// ��CThreadPool�д�����������һ���ǿ�������һ����æµ����
// Idle�����д�����еĿ��н��̣����߳�ִ������ʱ����״̬��Ϊæµ״̬��ͬʱ�ӿ���������ɾ����������æµ�����С�
// 
// ��CThreadPool�Ĺ��캯���У����ǽ�����m_InitNum���̣߳�����֮�󼴵���AppendToIdleList����Idle�����У�����Ŀǰû������ַ�����Щ�̣߳�����߳�ִ��Start���Լ�����
// ��ʵ�ϣ��̳߳������ɵ��߳���Ŀ������һ�ɲ���ģ�������ִ�и��ؽ����Զ�������Ϊ����CThreadPool���趨�ĸ�������
// 
// m_InitNum��	��ʼ����ʱ�̳߳��е��̵߳ĸ�����
// m_MaxNum :	��ǰ�̳߳��������������ڵ��̵߳������Ŀ��
// m_AvailLow:	��ǰ�̳߳�����������ڵĿ����̵߳���С��Ŀ�����������Ŀ���ڸ�ֵ���������ؿ��ܹ��أ���ʱ�б�Ҫ���ӿ����̳߳ص���Ŀ��ʵ�����������ǽ��̵߳���Ϊm_InitNum����
// m_AvailHigh����ǰ�̳߳���������Ŀ��е��̵߳������Ŀ�����������Ŀ���ڸ�ֵ��������ǰ���ؿ��ܽ��ᣬ��ʱ��ɾ������Ŀ����̣߳�ɾ���������ҲΪm_InitNum����
// m_AvailNum�� Ŀǰ�̳߳���ʵ�ʴ��ڵ��̵߳ĸ�������ֵ����m_AvailHigh��m_AvailLow֮�䡣����̵߳ĸ���ʼ��ά����m_AvailLow��m_AvailHigh֮�䣬
//				���̼߳Ȳ���Ҫ������Ҳ����Ҫɾ��������ƽ��״̬���������趨m_AvailLow��m_AvailHigh��ֵ��ʹ���̳߳������ܵı���ƽ��̬�����̳߳���Ʊ��뿼�ǵ����⡣
// 
// �̳߳��ڽ��ܵ��µ�����֮���̳߳�����Ҫ����Ƿ����㹻�Ŀ��гؿ��á�����Ϊ�������裺
// 
// (1)	��鵱ǰ����æµ״̬���߳��Ƿ�ﵽ���趨�����ֵm_MaxNum������ﵽ�ˣ�����Ŀǰû�п����߳̿��ã�����Ҳ���ܴ����µ��̣߳�
//		��˱���ȴ�ֱ�����߳�ִ����Ϸ��ص����ж����С�
// (2)	�����ǰ�Ŀ����߳���ĿС�������趨����С�Ŀ�����Ŀm_AvailLow�������Ǳ��봴���µ��̣߳�Ĭ������£���������߳���ĿӦ��Ϊ
//		m_InitNum����˴������߳���ĿӦ��Ϊ(��ǰ�����߳�����m_InitNum);
//		������һ������������뿼�ǣ��������е��߳��������ϴ�������߳������ܳ���m_MaxNum��������Ǳ�����̵߳Ĵ�������Դ���
//		�������������������m_MaxNum���򴴽�����߳�Ϊm_InitNum����������ˣ���ֻ����( m_MaxNum-��ǰ�߳�����)����
// 
// (3)	����GetIdleThread�������ҿ����̡߳������ǰû�п����̣߳�����𣻷�������ָ�ɸ����̣߳�ͬʱ��������æµ���С�
//		���߳�ִ����Ϻ�������MoveToIdleList����������������У����л�����m_IdleCond.Signal()����������GetIdleThread()�п����������̡߳�
// 
class CJob;
typedef boost::mutex CThreadMutex;
typedef boost::condition_variable_any CCondition;
class MYTHREADPOOL_DLL CThreadPool
{
	friend class CWorkerThread;
private:
	unsigned int m_MaxNum;   //the max thread num that can create at the same time
	unsigned int m_AvailLow; //The min num of idle thread that shoule kept
	unsigned int m_AvailHigh;    //The max num of idle thread that kept at the same time
	unsigned int m_AvailNum; //the normal thread num of idle num;
	unsigned int m_InitNum;  //Normal thread num;
protected:
	CWorkerThread* GetIdleThread(void);
	void    AppendToIdleList(CWorkerThread* jobthread);
	void    MoveToBusyList(CWorkerThread* idlethread);
	void    MoveToIdleList(CWorkerThread* busythread);
	void    DeleteIdleThread(int num);
	void    CreateIdleThread(int num);
public:
	CThreadMutex m_BusyMutex;    //when visit busy list,use m_BusyMutex to lock and unlock
	CThreadMutex m_IdleMutex;    //when visit idle list,use m_IdleMutex to lock and unlock
	CThreadMutex m_JobMutex; //when visit job list,use m_JobMutex to lock and unlock
	CThreadMutex m_VarMutex;
	CCondition       m_BusyCond; //m_BusyCond is used to sync busy thread list
	CCondition       m_IdleCond; //m_IdleCond is used to sync idle thread list
	CCondition       m_IdleJobCond;  //m_JobCond is used to sync job list
	CCondition       m_MaxNumCond;
	std::vector<CWorkerThread*>   m_ThreadList;
	std::vector<CWorkerThread*>   m_BusyList;     //Thread List
	std::vector<CWorkerThread*>   m_IdleList; //Idle List
	CThreadPool();
	CThreadPool(int initnum);
	virtual ~CThreadPool();
	void    SetMaxNum(int maxnum) { m_MaxNum = maxnum; }
	int     GetMaxNum(void) { return m_MaxNum; }
	void    SetAvailLowNum(int minnum) { m_AvailLow = minnum; }
	int     GetAvailLowNum(void) { return m_AvailLow; }
	void    SetAvailHighNum(int highnum) { m_AvailHigh = highnum; }
	int     GetAvailHighNum(void) { return m_AvailHigh; }
	int     GetActualAvailNum(void) { return m_AvailNum; }
	int     GetAllNum(void) { return m_ThreadList.size(); }
	int     GetBusyNum(void) { return m_BusyList.size(); }
	void    SetInitNum(int initnum) { m_InitNum = initnum; }
	int     GetInitNum(void) { return m_InitNum; }
	void    TerminateAll(void);
	void    Run(CJob* job, void* jobdata);
};