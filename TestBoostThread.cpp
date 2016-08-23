// Test160822Boost.cpp : �������̨Ӧ�ó������ڵ㡣
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
		boost::mutex::scoped_lock lock(io_mu); //io����һ��������Դ�����̰߳�ȫ�ģ���Ҫ����
		std::cout << str << ++x << std::endl;
	}
}

void to_interrupt(atom_int &x, const std::string& str)
try
{
	for (int i = 0; i < 5; ++i)
	{
		boost::this_thread::sleep(boost::posix_time::seconds(1));  // ˯��1����
		boost::mutex::scoped_lock lock(io_mu);	// ����io������
		std::cout << str << ++x << std::endl;
	}
}
catch (const boost::thread_interrupted&) // �����߳��ж��쳣
{
	std::cout << "thread_interrupted" << std::endl;
}

void to_interrupt_with_interruption_point(atom_int& x, const std::string& str)
try
{
	for (int i = 0; i < 5; ++i)
	{
		//boost::this_thread::sleep(boost::posix_time::seconds(1));  // ˯��1����
		boost::mutex::scoped_lock lock(io_mu);	// ����io������
		std::cout << str << ++x << std::endl;
		boost::this_thread::interruption_point(); // ���������ж�
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

	// ʹ����ʱthread���������̣߳����ɹ�������һ��thread������߳̾����̿�ʼִ�У�thread���ṩ����start()��begin()�����ķ�����
	// ��ʵ���ϣ�thread����һ��˽�еĳ�Ա����start_thread()�������������̡߳�
	boost::thread(printing, boost::ref(x), "hello: ");
	boost::thread(printing, boost::ref(x), "boost: ");

	boost::thread t1(printing, boost::ref(x), "hello: ");
	boost::thread t2(printing, boost::ref(x), "boost: ");
	t1.timed_join(boost::posix_time::seconds(1));	// ���ȴ�1��Ȼ�󷵻�
	t2.join();		// �ȴ�t2�߳̽����ٷ��أ����ܶ೤ʱ��

	t1.detach();	// �������߳�ִ������룬ʧȥ���̵߳Ŀ��ƣ����̼߳�������
	t2.detach();	// �������߳�ִ������룬ʧȥ���̵߳Ŀ��ƣ����̼߳�������

	// ʹ��bind��function
	boost::thread t3(boost::bind(printing, boost::ref(x), "thread: "));
	t3.join();
	boost::function<void()> f = boost::bind(printing, boost::ref(x), "mutex: ");
	//f(); //ִ�к���
	boost::thread t4(f); // ע�⣺�˴���д��"boost::thread(f);"��������һ��thread��������ʱ����ʱ���ᱨ"f�ض���"����
	t4.sleep(boost::get_system_time() + boost::posix_time::seconds(2)); // ע�⣺�˴���Ȼ�������߳���˯��
	t4.join();
	
	{
		boost::mutex::scoped_lock lock(io_mu);
		std::cout << "t3 id: " << t3.get_id() << std::endl;	// ���t3���߳�id
		std::cout << "boost::thread::id() : " << boost::thread::id() << std::endl;	// ���t3���߳�id
	}
	//assert(t3 != t4);	// �Ƚ������̶߳���
	t3.detach();		// ����t3������߳�ִ���壬���߳���Ȼ����ִ��
	assert(t3.get_id() == boost::thread::id()); // t3 ���ٱ�ʶ�κ��߳�

	boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(1));
	std::cout << "CPU����: " << boost::thread::hardware_concurrency() << std::endl;

	// �߳��ж�
	atom_int ai;
	boost::thread t5(to_interrupt, boost::ref(ai), "interrupt test: ");
	boost::this_thread::sleep(boost::posix_time::seconds(2)); // ˯��2����
	t5.interrupt();	// Ҫ���߳��ж�ִ��
	t5.join();		// ��Ϊ�߳��Ѿ��жϣ�����join()��������
	// �߳��жϵ�
	// �̵߳��ж����������ģ�ֻ�����߳��жϵ㴦���̲߳��ܱ��жϣ�thread��Ԥ������9���жϵ�:
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
	t6.interrupt();	// Ҫ���߳��ж�ִ��
	t6.join();		// ��Ϊ�߳��Ѿ��жϣ�����join()��������
	// �߳��Ƿ�����жϣ����Բ���RAII�ķ�ʽ���û����
	// ...


	// thread_group
	// ʹ��thread_group�����ǿ���Ϊ������һ��������ȫ���̳߳صĶ���ͳһ���������ʹ�õ�
	// thread��������ʹ�õ�������һ�����������ṩһ��ȫ�ֵķ��ʵ�
	// typedef boost::singleton_default<boost::thread_group> thread_pool;

	// feature ��ʹ��
	// ����packaged_task������ģ�����ָ������ֵ����
	// packaged_taskֻ�����޲κ����������Ҫʹ��bind
	boost::packaged_task<int> pt(boost::bind(fab, 10));
	// ����unique_future���󣬽���packaged_task��futureֵ,
	// ͬ��Ҫ��ģ�����ָ������ֵ����
	boost::unique_future<int> uf = pt.get_future();
	// �����̼߳��㣬����ʹ��boost::move()��ת��packaged_task����
	// ��Ϊpackaged_task�ǲ��ɿ�����
	boost::thread(boost::move(pt));
	uf.wait();	// unique_future�ȴ�������
	assert(uf.is_ready() && uf.has_value());
	std::cout << uf.get();



	//system("pause");
    return 0;
}

