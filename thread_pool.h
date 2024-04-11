#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <future>
#include <functional>
#include <condition_variable>
#include <iostream>

class safe_queue
{
public:
	safe_queue(){};
	safe_queue(const safe_queue& other) = delete;
	safe_queue& operator=(const safe_queue&) = delete;
	~safe_queue(){};
/* 
 * Записывает в начало очереди новую задачу. При этом 
 * захватывается мьютекс, а после окончания операции 
 * нотифицируется условная переменная
 */
	void push(std::function<void()> funct);
/* 
 * Метод pop находится в ожидании, пока не прийдет 
 * уведомление на условную переменную. При нотификации 
 * условной переменной данные считываются из очереди
 */
	std::function<void()> pop();
	bool empty();
	mutable std::mutex _mt;							// Реализация блокировки
protected:	
	std::queue<std::function<void()>> _queue;	// Место для хранения задач

	std::condition_variable_any _notification;		// Уведемления
};

class thread_pool
{
public:
	thread_pool();
	~thread_pool();
/** Выбирает из очередную задачу и  исполняет ее. Этот
 *  метод передается конструктору потоков для исполнения	
**/
	void work();					

/** Помещает в очередь задачу
 *  метод передается конструктору потоков для исполнения	
**/
	void submit(std::function<void()> funct);

protected:
	std::vector<std::thread> _pool;					// Вектор потоков
	safe_queue* _squeue = nullptr;					// Очередь задач
	void work_one_thread();							// Работа, передаваемая в поток
};
