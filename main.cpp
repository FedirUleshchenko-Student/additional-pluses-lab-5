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

void action(const vector<promise<bool>>& v) {

}

void thread1(promise<bool>&& Promise) {
	fast("C2");
	Promise.set_value(true);
}

void thread2(future<bool>& fut) {
	fut.get();
	fast("D2");
}

void thread3() {

}

void work() {

	auto t1 = chrono::steady_clock::now();

	promise<bool> pr1;
	future<bool> fut1 = pr1.get_future();

	future<void> a[2];
	a[0] = async(launch::async, thread1, move(pr1));
	a[1] = async(launch::async, thread2, ref(fut1));

	for (auto& x : a) {
		x.wait();
	}

	auto t2 = chrono::steady_clock::now();

	cout << chrono::duration<double>(t2 - t1) << endl;
	cout << "The work is done!" << endl;
}

int main() {
	work();
	return 0;
}