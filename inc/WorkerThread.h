#include "../inc/Thread.h"

// CWorkerThread
// 
// CWorkerThread��CThread�������࣬����ʵ�ϵĹ����̡߳���CThreadPool�Ĺ��캯���У����Ǵ�����һ��������CWorkerThread��
// һ����Щ�̴߳�����ϣ����ǽ�����Start()�������̡߳�Start�������ջ����Run������Run�����Ǹ�����ѭ���Ĺ��̡�
// ��û�н��ܵ�ʵ�ʵ������ʱ��m_JobΪNULL����ʱ�߳̽�����Wait�������еȴ����Ӷ����ڹ���״̬��
// һ���̳߳ؽ����������ַ������̣߳��佫�����ѣ��Ӷ�֪ͨ�̴߳ӹ���ĵط�����ִ�С�CWorkerThread��������������:
// 
// ���߳�ִ������֮ǰ���ȱ����жϿ����̵߳���Ŀ�Ƿ����m_AvailLow��������ڣ�����봴���㹻�Ŀ����̣߳�ʹ����Ŀ�ﵽm_InitNum����
// Ȼ�󽫵���MoveToBusyList()�Ƴ����ж��У�����æµ���С�������ִ����Ϻ����ֵ���MoveToIdleList()�Ƴ�æµ���У�������ж��У��ȴ��µ�����
// 
// ����Run����֮�⣬CWorkerThread������һ����Ҫ�ķ�������SetJob���÷�����ʵ�ʵ�����ֵ���̡߳���û���κ�ִ������m_JobΪNULL��ʱ��
// �߳̽�����m_JobCond.Wait���еȴ���һ��Job����ֵ���̣߳��佫����m_JobCond.Signal�������Ѹ��̡߳�����m_JobCond�����߳��ڲ��ı�����
// ÿ���̶߳�ά��һ��m_JobCond��ֻ�еõ�������̲߳ű����ѣ�û�еõ�����Ľ������ȴ�������һ���̺߳�ʱ�����ѣ�
// �䶼���ӵȴ��ĵط�����ִ��m_Job->Run(m_JobData)�������߳�ִ��ʵ������ĵط���
// 
// ���߳�ִ�и���Job�ڼ䣬���Ǳ����ֹ����һ��Job�ָ������̣߳�����ڸ�ֵ֮ǰ��ͨ��m_VarMutex���������� Jobִ���ڼ䣬���ڵ�Job�����ܹ��������̣߳�
// ����ִ����ϣ����ǵ���m_VarMutex.Unlock()���н�������ʱ���߳��ֿ��Խ����µ�ִ������
// 
// ���߳�ִ����������󷵻ؿ��ж���ǰ�����ǻ���Ҫ�жϵ�ǰ���ж����е��߳��Ƿ����m_AvailHigh�����������m_AvailHigh��
// ����������ɾ��(m_ThreadPool->m_IdleList.size() - m_ThreadPool->GetInitNum())���̣߳�ʹ�߳���Ŀ������m_InitNum����
// 
class CJob;
class CThreadPool;
class CThreadMutex;
class CCondition;
class CWorkerThread :public CThread
{
private:
	CThreadPool*  m_ThreadPool;
	CJob*    m_Job;
	void*    m_JobData;
	CThreadMutex m_VarMutex;
	bool      m_IsEnd;
protected:
public:
	CCondition   m_JobCond;
	CThreadMutex m_WorkMutex;
	CWorkerThread();
	virtual ~CWorkerThread();
	void Run();
	void    SetJob(CJob* job, void* jobdata);
	CJob*   GetJob(void) { return m_Job; }
	void    SetThreadPool(CThreadPool* thrpool);
	CThreadPool* GetThreadPool(void) { return m_ThreadPool; }
};