#include "../inc/Thread.h"
#include <boost/thread.hpp>

struct Impl
{
	boost::thread t;
};

void* CThread::ThreadFunction(void*)
{

}

CThread::CThread()
{

}

CThread::CThread(bool createsuspended, bool detach)
{

}

CThread::~CThread()
{

}

bool CThread::Terminate(void)
{

}

bool CThread::Start(void)
{

}

void CThread::Exit(void)
{

}

bool CThread::Wakeup(void)
{

}

bool CThread::SetPriority(int priority)
{

}

int CThread::GetPriority(void)
{

}

int CThread::GetConcurrency(void)
{

}

void CThread::SetConcurrency(int num)
{

}

bool CThread::Detach(void)
{

}

bool CThread::Join(void)
{

}

int CThread::Self(void)
{

}

