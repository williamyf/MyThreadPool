//CThreadManage
//CThreadManage�Ĺ��ܷǳ��򵥣����ṩ��򵥵ķ��������ඨ�����£�
class CThreadManage
{
private:
	//CThreadPool*    m_Pool;	// ָ��ʵ�ʵ��̳߳�
	int          m_NumOfThread; // ��ʼ����ʱ���������Ĳ������̸߳���
protected:
public:
	void     SetParallelNum(int num);
	CThreadManage();
	CThreadManage(int num);
	virtual ~CThreadManage();
	void    Run(/*CJob* job,*/ void* jobdata);
	void    TerminateAll(void);
};