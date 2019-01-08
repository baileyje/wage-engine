
#include "async/dispatch_queue.h"

namespace wage {

  DispatchQueue::DispatchQueue(std::string name, size_t threadCount) :
    name_(name), threads_(threadCount) {
    printf("Creating dispatch queue: %s\n", name.c_str());
    printf("Dispatch threads: %zu\n", threadCount);

    for(size_t i = 0; i < threads_.size(); i++) {
      threads_[i] = std::thread(&DispatchQueue::threadHandler, this);
    }
  }

  DispatchQueue::~DispatchQueue() {
    printf("Destructor: Destroying dispatch threads...\n");

    // Signal to dispatch threads that it's time to wrap up
    std::unique_lock<std::mutex> lock(lock_);
    quit_ = true;
    lock.unlock();
    condition_.notify_all();

    // Wait for threads to finish before we exit
    for(size_t i = 0; i < threads_.size(); i++)
    {
      if(threads_[i].joinable())
      {
        printf("Destructor: Joining thread %zu until completion\n", i);
        threads_[i].join();
      }
    }
  }

  void DispatchQueue::dispatch(const Operation& op)
  {
    std::unique_lock<std::mutex> lock(lock_);
    queue_.push(op);

    // Manual unlocking is done before notifying, to avoid waking up
      // the waiting thread only to block again (see notify_one for details)
    lock.unlock();
    condition_.notify_all();
  }

  void DispatchQueue::dispatch(Operation&& op)
  {
    std::unique_lock<std::mutex> lock(lock_);
    queue_.push(std::move(op));

    // Manual unlocking is done before notifying, to avoid waking up
      // the waiting thread only to block again (see notify_one for details)
    lock.unlock();
    condition_.notify_all();
  }

  void DispatchQueue::threadHandler(void) {
    std::unique_lock<std::mutex> lock(lock_);

    do {
      //Wait until we have data or a quit signal
      condition_.wait(lock, [this]{
        return (queue_.size() || quit_);
      });

      //after wait, we own the lock
      if(!quit_ && queue_.size())
      {
        auto op = std::move(queue_.front());
        queue_.pop();

        //unlock now that we're done messing with the queue
        lock.unlock();

        op();

        lock.lock();
      }
    } while (!quit_);
  }

  // int main(void)
  // {
  //   int r = 0;
  //   dispatch_queue q("Phillip's Demo Dispatch Queue", 4);

  //   q.dispatch([]{printf("Dispatch 1!\n");});
  //   q.dispatch([]{printf("Dispatch 2!\n");});
  //   q.dispatch([]{printf("Dispatch 3!\n");});
  //   q.dispatch([]{printf("Dispatch 4!\n");});

  //   return r;
  // }

}