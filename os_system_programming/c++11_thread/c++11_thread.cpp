#include <cstdio>
#include <iostream>
#include <thread>
#include <vector>
using std::thread;
using std::vector;

 void worker1(vector<int>::iterator start, vector<int>::iterator end,
	int* result)
{
	int sum = 0;

	for (auto itr = start; itr < end; ++itr)
	{
		sum += *itr;
	}
	*result = sum;

	thread::id this_id = std::this_thread::get_id();

	printf("쓰레드 %x 에서 %d 부터 %d 까지 계산한 결과 : %d \n", this_id, *start,
		*(end - 1), sum);
};

 void Example() {
	vector<int> data(10000);
	for (int i = 0; i < 10000; ++i)
	{
		data[i] = i;
	}

	vector<int> partial_sums(4);

	vector<thread> workers;

	for (int i = 0; i < 4; ++i)
	{
		workers.push_back(
			thread(worker1
				, data.begin() + i * 2500,
				data.begin() + (i + 1) * 2500, &partial_sums[i]));

	}

	for (int i = 0; i < 4; ++i)
	{
		workers[i].join();
	}

	int total = 0;

	for (int i = 0; i < 4; ++i)
	{
		total += partial_sums[i];
	}

	std::cout << "전체 합 : " << total << std::endl;
}

 void worker(int& counter)
 {
	 for (int i = 0; i < 10000; ++i)
	 {
		 counter += 1;
	 }
 };

int main()
{
	int counter = 0;

	vector<thread> workers;
	for (int i = 0; i < 4; ++i)
	{
		workers.push_back(thread(worker, std::ref(counter)));
	}

	for (int i = 0; i < 4; ++i)
	{
		workers[i].join();
	}

	std::cout << "COunter 최종값: " << counter << std::endl;
}
