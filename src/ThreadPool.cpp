#include <condition_variable>
#include <functional>
#include <vector>
#include <thread>
#include <queue>

class ThreadPool
{
public:
    using Task = std::function<void()>;

    explicit ThreadPool(std::size_t numThreads)
    {
        start(numThreads);
    }

    ~ThreadPool()
    {
        stop();
    }

    void enqueue(Task task)
    {
        {
            std::unique_lock<std::mutex> lock{mEventMutex};
            mTasks.emplace(std::move(task));
        }

        mEventVar.notify_one();

    }
private:
    std::vector<std::thread> mThreads;

    std::condition_variable mEventVar;

    std::mutex mEventMutex;
    bool mStopping = false;

    std::queue<Task> mTasks;

    void start(std::size_t numThreads)
    {
        for(auto i = 0u; i< numThreads; ++i)
        {
            mThreads.emplace_back([=] {
                while(true){
                    std::unique_lock<std::mutex> lock{mEventMutex};

                    mEventVar.wait(lock, [=] {return mStopping; });

                    if(mStopping)
                        break;
                }
            });
        }
    }
    void stop() noexcept
    {
        {
            std::unique_lock<std::mutex> lock{mEventMutex};
            mStopping = true;
        }

        mEventVar.notify_all();

        for (auto &thread : mThreads)
            thread.join();
    }
};

int main()
{
    ThreadPool pool(36);
    printf("Threadpool initialized\n");
    return 0;
}