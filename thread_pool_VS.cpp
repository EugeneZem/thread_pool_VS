#include <iostream>
#include <random>
#include <chrono>
#include "thread_pool.h"

void test_rand_mcs();

int main(int argc, char** argv)
{

	/*
	 * Алгоритм работы:
	 * 1. Объявляется объект класса thread_pool
	 * 2. Описывается несколько тестовых функций, выводящих в консоль свое имя
	 * 3. Раз в секунду в пул одновременно  кладется
	 *    2 функции и проверяется их исполнение
	 */

	thread_pool pool;

	for (int i = 0; i < 10; i++)
	{
		pool.submit(test_rand_mcs);
	}
	pool.work();

}

void test_rand_mcs()
{
	int random_dugation = rand() * 20 / RAND_MAX + 11;
	std::cout << "Test function duration of " << random_dugation <<
		" micro sec. is runing\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(random_dugation * 100));
	std::cout << "Test function duration of " << random_dugation <<
		" finished work\n";
}