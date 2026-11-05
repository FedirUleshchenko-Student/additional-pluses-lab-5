// Working in Visual Studio 2022, so compiler:
// MSVC (Microsoft Visual C++)
//
// Standard (for version of C++):
// Preview - ISO C++23 Standard (/std:c++23preview)

#include <iostream>
#include <chrono>
#include <format>

#include <syncstream>
#include <vector>

using namespace std;

#include <future>

void slow(const std::string& name) {
	this_thread::sleep_for(chrono::duration<double>(7));
	osyncstream(cout) << name << endl;
}

void fast(const std::string& name) {
	this_thread::sleep_for(chrono::duration<double>(1));
	osyncstream(cout) << name << endl;
}

void thread1(promise<void> isA1Ready) {
	slow("A1");
	isA1Ready.set_value();
}

void thread2() {
	slow("C2");
	fast("D2");
}

void thread3(future<void> isA1Ready) {
	fast("A2");
	fast("C1");
	isA1Ready.get();
	fast("B");
	fast("D1");
}

void work() {
	promise<void> pra;
	future<void> futa = pra.get_future();

	future<void> a[2];

	auto t1 = chrono::steady_clock::now();
	a[0] = async(launch::async, thread1, move(pra));
	a[1] = async(launch::async, thread2);

	// in main, so it is not waiting still
	thread3(move(futa));

	for (auto& x : a) {
		x.wait();
	}

	auto t2 = chrono::steady_clock::now();

	cout << "The work is done!" << endl;
	cout << "Time : " << chrono::duration<double>(t2 - t1) << " seconds" << endl;
}

int main() {
	work();
	return 0;
}