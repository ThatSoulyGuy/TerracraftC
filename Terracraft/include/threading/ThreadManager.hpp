#ifndef THREAD_MANAGER_HPP
#define THREAD_MANAGER_HPP

#include <iostream>
#include <string>
#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <map>
#include <cstdarg>
#include <cstdio>
#include <condition_variable>

using Task = std::function<void()>;

class TaskQueue
{

public:

    void PushTask(const Task& task)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mTasks.push(task);

        mCondition.notify_one();
    }

    Task PopTask()
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mCondition.wait(lock, [this] { return !mTasks.empty(); });

        Task task = mTasks.front();
        mTasks.pop();

        return task;
    }

private:

    std::queue<Task> mTasks;
    std::mutex mMutex;
    std::condition_variable mCondition;
};

struct WorkerThread
{
    std::thread thread;
    TaskQueue taskQueue;
};

namespace ThreadManager 
{
    bool keepRunning = true;

    extern std::map<std::string, WorkerThread> workerThreads;
    extern std::mutex threadMutex;

    void ThreadFunction(TaskQueue& queue) 
    {
        while (keepRunning) 
        {
            auto task = queue.PopTask();
            task();
        }
    }

    void Initialize(size_t threadCount, const char* format, ...) 
    {
        std::vector<std::string> threadNames;

        std::va_list args;
        va_start(args, format);

        for (std::size_t c = threadCount; c--;)
        {
            const char* name = va_arg(args, const char*);

            threadNames.push_back(std::string(name));
        }

        va_end(args);

        for (size_t t = 0; t < threadCount; ++t) 
        {
            WorkerThread thread;

            thread.thread = std::thread(ThreadFunction, std::ref(thread.taskQueue));

            //workerThreads[threadNames[t]] = std::move(thread);
        }
    }

    void RunFunctionOnThread(const std::string& name, const Task& task) 
    {
        workerThreads[name].taskQueue.PushTask(task);
    }

    void CleanUp()
    {
        keepRunning = false;

        for (auto& [key, value] : workerThreads)
            value.taskQueue.PushTask(nullptr);

        for (auto& [key, value] : workerThreads)
        {
            if (value.thread.joinable())
                value.thread.join();
        }

        workerThreads.clear();
    }
};

std::map<std::string, WorkerThread> ThreadManager::workerThreads;
std::mutex ThreadManager::threadMutex;

#endif // !THREAD_MANAGER_HPP