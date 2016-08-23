// Test160822Boost.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <boost/timer.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread_pool.hpp>
#include <boost/pool/singleton_pool.hpp>



template <typename F>
boost::thread make_thread(F f)
{
	return boost::thread(f);
}

template <typename T>
class basic_atom: boost::noncopyable
{
public:
	basic_atom(T x = T()) : n(x) {}
	T operator++ () {
		mutex_t::scoped_lock lock(mu);
		return ++n;
	}
	operator T() { return n; }
private:
	T n;
	typedef boost::mutex mutex_t;
	mutex_t mu;
};
typedef basic_atom<int> atom_int;

boost::mutex io_mu;
void printing(atom_int &x, const std::string& str)
{
	for (int i = 0; i < 5; ++i)
	{
		boost::mutex::scoped_lock lock(io_mu); //io流是一个共享资源不是线程安全的，需要锁定
		std::cout << str << ++x << std::endl;
	}
}

void to_interrupt(atom_int &x, const std::string& str)
try
{
	for (int i = 0; i < 5; ++i)
	{
		boost::this_thread::sleep(boost::posix_time::seconds(1));  // 睡眠1秒钟
		boost::mutex::scoped_lock lock(io_mu);	// 锁定io流操作
		std::cout << str << ++x << std::endl;
	}
}
catch (const boost::thread_interrupted&) // 捕获线程中断异常
{
	std::cout << "thread_interrupted" << std::endl;
}

void to_interrupt_with_interruption_point(atom_int& x, const std::string& str)
try
{
	for (int i = 0; i < 5; ++i)
	{
		//boost::this_thread::sleep(boost::posix_time::seconds(1));  // 睡眠1秒钟
		boost::mutex::scoped_lock lock(io_mu);	// 锁定io流操作
		std::cout << str << ++x << std::endl;
		boost::this_thread::interruption_point(); // 这里允许中断
	}
}
catch (const boost::thread_interrupted&)
{
	std::cout << "thread_interrupted with interruption point" << std::endl;
}

int fab(int n)
{
	if (n == 0 || n == 1)
		return 1;
	return fab(n - 1) + fab(n - 2);
}

int main()
{
	boost::timer t;
	std::cout << "max timespan: " << t.elapsed_max() / 3600 << "h" << std::endl;
	std::cout << "min timespan: " << t.elapsed_min() << "s" << std::endl;
	std::cout << "now time elapsed: " << t.elapsed() << "s" << std::endl;

	//boost::this_thread::sleep(boost::posix_time::seconds(2));
	std::cout << "sleep 2 seconds" << std::endl;

	boost::mutex mu;
	boost::mutex::scoped_lock lock(mu);
	std::cout << "some operations..." << std::endl;

	
	atom_int x = 3;
	std::cout << ++x << std::endl;
	//std::cout << x++ << std::endl;
	int y = x;
	std::cout << ++y << std::endl;

	// 使用临时thread对象启动线程，当成功创建了一个thread对象后，线程就立刻开始执行，thread不提供类似start()、begin()那样的方法。
	// 但实际上，thread类有一个私有的成员函数start_thread()，它负责启动线程。
	boost::thread(printing, boost::ref(x), "hello: ");
	boost::thread(printing, boost::ref(x), "boost: ");

	boost::thread t1(printing, boost::ref(x), "hello: ");
	boost::thread t2(printing, boost::ref(x), "boost: ");
	t1.timed_join(boost::posix_time::seconds(1));	// 最多等待1秒然后返回
	t2.join();		// 等待t2线程结束再返回，不管多长时间

	t1.detach();	// 对象与线程执行体分离，失去对线程的控制，但线程继续运行
	t2.detach();	// 对象与线程执行体分离，失去对线程的控制，但线程继续运行

	// 使用bind和function
	boost::thread t3(boost::bind(printing, boost::ref(x), "thread: "));
	t3.join();
	boost::function<void()> f = boost::bind(printing, boost::ref(x), "mutex: ");
	//f(); //执行函数
	boost::thread t4(f); // 注意：此处若写成"boost::thread(f);"，即创建一个thread的匿名临时对象时，会报"f重定义"错误
	t4.sleep(boost::get_system_time() + boost::posix_time::seconds(2)); // 注意：此处仍然还是主线程在睡眠
	t4.join();
	
	{
		boost::mutex::scoped_lock lock(io_mu);
		std::cout << "t3 id: " << t3.get_id() << std::endl;	// 输出t3的线程id
		std::cout << "boost::thread::id() : " << boost::thread::id() << std::endl;	// 输出t3的线程id
	}
	//assert(t3 != t4);	// 比较两个线程对象
	t3.detach();		// 分离t3代表的线程执行体，但线程仍然继续执行
	assert(t3.get_id() == boost::thread::id()); // t3 不再标识任何线程

	boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(1));
	std::cout << "CPU数量: " << boost::thread::hardware_concurrency() << std::endl;

	// 线程中断
	atom_int ai;
	boost::thread t5(to_interrupt, boost::ref(ai), "interrupt test: ");
	boost::this_thread::sleep(boost::posix_time::seconds(2)); // 睡眠2秒钟
	t5.interrupt();	// 要求线程中断执行
	t5.join();		// 因为线程已经中断，所以join()立即返回
	// 线程中断点
	// 线程的中断是有条件的，只有在线程中断点处，线程才能被中断，thread库预定义了9个中断点:
	// thread::join();
	// thread::timed_join();
	// condition_variable::wait();
	// condition_variable::timed_wait();
	// condition_variable_any::wait();
	// condition_variable_any::timed_wait();
	// thread::sleep();
	// this_thread::sleep();
	// this_thread::interruption_point();
	boost::thread t6(to_interrupt_with_interruption_point, boost::ref(ai), "interruption_point test: ");
	t6.interrupt();	// 要求线程中断执行
	t6.join();		// 因为线程已经中断，所以join()立即返回
	// 线程是否可以中断，可以采用RAII的方式启用或禁用
	// ...


	// thread_group
	// 使用thread_group，我们可以为程序建立一个类似于全局线程池的对象，统一管理程序中使用的
	// thread，它可以使用单件库变成一个单件，以提供一个全局的访问点
	// typedef boost::singleton_default<boost::thread_group> thread_pool;

	// feature 的使用
	// 声明packaged_task对象，用模版参数指明返回值类型
	// packaged_task只接受无参函数，因此需要使用bind
	boost::packaged_task<int> pt(boost::bind(fab, 10));
	// 声明unique_future对象，接受packaged_task的future值,
	// 同样要用模版参数指明返回值类型
	boost::unique_future<int> uf = pt.get_future();
	// 启动线程计算，必须使用boost::move()来转移packaged_task对象
	// 因为packaged_task是不可拷贝的
	boost::thread(boost::move(pt));
	uf.wait();	// unique_future等待计算结果
	assert(uf.is_ready() && uf.has_value());
	std::cout << uf.get();



	//system("pause");
    return 0;
}

