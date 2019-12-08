#include <iostream>
#include <thread>
#include <mutex>


std::mutex resource_A;
std::mutex resource_B;
std::timed_mutex resource_AT;
std::timed_mutex resource_BT;

void DeadLockA()
{
	std::lock_guard<std::mutex> lockA(resource_A);
	std::cout << "FooA: ResourceA locked" << std::endl;
	std::lock_guard<std::mutex> lockB(resource_B);
	std::cout << "FooA: ResourceB locked" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void DeadLockB()
{
	std::lock_guard<std::mutex> lockB(resource_B);
	std::cout << "FooA: ResourceB locked" << std::endl;
	std::lock_guard<std::mutex> lockA(resource_A);
	std::cout << "FooA: ResourceA locked" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void NotLockA()
{
	resource_AT.lock();
	std::cout << "ResourceAT locked" << std::endl;

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	if (resource_BT.try_lock_for(std::chrono::milliseconds(2000)))
	{
		std::cout << "ResourceBT locked" << std::endl;
		resource_BT.unlock();
	}
	else std::cout << "Deadlock detected!!!!" << std::endl;
	resource_AT.unlock();
}

void NotLockB()
{
	resource_BT.lock();
	std::cout << "ResourceBT locked" << std::endl;

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	if (resource_AT.try_lock_for(std::chrono::milliseconds(2000)))
	{
		std::cout << "ResourceAT locked" << std::endl;
		resource_AT.unlock();
	}
	else std::cout << "Deadlock detected!!!!" << std::endl;
	resource_BT.unlock();
}

int main(int argc, const char* argv[])
{
	std::cout << "Let's start:" << std::endl;

/*
	std::thread t1(DeadLockA);
	std::thread t2(DeadLockB);
	t1.join();
	t2.join();
/*/
	std::thread t1(NotLockA);
	std::thread t2(NotLockB);
	t1.join();
	t2.join();
//*/
	std::cout << "I don't belive in DeadLock" << std::endl;


	//std::cin.get();
}