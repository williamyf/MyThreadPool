#include "../inc/Thread.h"
#include <boost/thread.hpp>

struct Impl
{
	boost::thread t;
};

void* CThread::ThreadFunction(void*)
{
	return 0;

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
	return false;

}

bool CThread::Start(void)
{
	return false;

}

void CThread::Exit(void)
{

}

bool CThread::Wakeup(void)
{
	return false;
}

bool CThread::SetPriority(int priority)
{
	return false;

}

int CThread::GetPriority(void)
{
	return 0;
}

int CThread::GetConcurrency(void)
{
	return 0;

}

void CThread::SetConcurrency(int num)
{

}

bool CThread::Detach(void)
{
	return false;

}

bool CThread::Join(void)
{
	return false;

}

int CThread::Self(void)
{
	return false;

}

