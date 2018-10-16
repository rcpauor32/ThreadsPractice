#pragma once

#include <thread>
#include <mutex>

// Ex1 ---
std::mutex mtx;
std::condition_variable cond;
int idToCont = 0;
// Ex2 ---
std::condition_variable		conds[2];
std::mutex					mtxs[2];

void Ex1ThreadCode01() {
	{
		std::unique_lock<std::mutex> lock(mtx);
		while (idToCont != 1) {
			cond.wait(lock);
		}
		// Code ---
		// ...
		
		printf("t1\n");

		std::this_thread::sleep_for(std::chrono::seconds(1));
		idToCont = 2;
		cond.notify_all();
	}
}
void Ex1ThreadCode02() {
	{
		std::unique_lock<std::mutex> lock(mtx);
		while (idToCont != 2) {
			cond.wait(lock);
		}

		// Code ---
		// ...

		printf("t2\n");

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

// Ex2 Threads ---
void Ex2Thread01() {
	{
		std::unique_lock<std::mutex> lock(mtxs[0]);
		conds[0].wait(lock);
		// Code ---
		// ...
		printf("t1\n");
		std::this_thread::sleep_for(std::chrono::seconds(1));
		conds[1].notify_one();
	}
}
void Ex2Thread02() {
	std::unique_lock<std::mutex> lock(mtxs[1]);
	conds[1].wait(lock);
	// Code ---
	// ...
	printf("t2\n");
	std::this_thread::sleep_for(std::chrono::seconds(1));
}
// !Ex2 Threads ---

int main() {
	
	// Ex1 ---
	std::thread Ex1t1(Ex1ThreadCode01);
	std::thread Ex1t2(Ex1ThreadCode02);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	idToCont = 1;
	cond.notify_all();

	Ex1t1.join();
	Ex1t2.join();
	printf("main\n\nEx1 - End\n");
	getchar();

	//return 0;
	// !Ex1 ---
	
	// Ex2 ---
	std::thread Ex2t1(Ex2Thread01);
	std::thread Ex2t2(Ex2Thread02);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	conds[0].notify_one();

	Ex2t1.join();
	Ex2t2.join();
	printf("main\n\nEx2 - End\n");
	getchar();
	return 0;
	// !Ex2 ---

}