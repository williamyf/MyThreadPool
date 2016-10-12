#pragma once
#include "Export.h"
#include <list>
#include <TaskBase.h>
#include <MySingleton.h>

namespace xxx
{
enum TASK_EXEC_TYPE {
	TASK_EXECUTE_SEQUENCE, 	// 顺序执行
	TASK_EXECUTE_PARALLEL	// 并发执行
};
class _EXPORT TaskMgr
{
	MY_DECLARE_SINGLETON(TaskMgr,ins)

	// 执行一个任务
	void execute(Task::Ptr task);

	// 执行一组任务
	void execute(const std::vector<Task::Ptr> &tasks, TASK_EXEC_TYPE exectype);	

	// 判断一个任务组是否正在执行
	bool taskGroupIsRunning(const std::string& groupName) const;

	// 判断一个任务是否正在执行
	bool taskIsRunning(const std::string& taskName) const;

	// 停止所有任务
	void stopAll();

	// 等待任务完成
	void waitCompletion();

private:
	// 机制内部使用，不要公开
	void addTask(Task::Ptr t);
	void removeTask(Task::Ptr t);

	std::list<Task::Ptr>		_tasklist;		// 正在执行的任务列表
	mutable OpenThreads::Mutex 	_tasklist_mut;

	friend class TaskGroup;
	friend class WorkerThread;
	friend class TaskMgrThread;
	TaskMgr();
	~TaskMgr();
};

}