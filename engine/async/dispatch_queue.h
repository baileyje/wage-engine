#pragma once

#include <thread>
#include <functional>
#include <vector>
#include <cstdint>
#include <cstdio>
#include <queue>
#include <mutex>
#include <string>
#include <condition_variable>

namespace wage {
  namespace async {

    /**
     * Class to manage a pool pf threads to support arbitrary asynchronous operations.
     */
    class DispatchQueue {
    public:
      // Operation type alias for a simple function.
      typedef std::function<void(void)> Operation;

      DispatchQueue(std::string name, size_t threadCount = 1) : _name(name), _threads(threadCount) {
        for (size_t i = 0; i < _threads.size(); i++) {
          _threads[i] = std::thread(&DispatchQueue::threadHandler, this);
        }
      }

      ~DispatchQueue() {
        // Signal to dispatch threads that it's time to wrap up
        std::unique_lock<std::mutex> lock(_lock);
        _quit = true;
        lock.unlock();
        _condition.notify_all();

        // Wait for threads to finish before we exit
        for (size_t i = 0; i < _threads.size(); i++) {
          if (_threads[i].joinable()) {
            _threads[i].join();
          }
        }
      }

      /**
       * Dispatch an operation on one of the work threads.
       */
      void dispatch(const Operation& op) {
        std::unique_lock<std::mutex> lock(_lock);
        _queue.push(op);
        // Manual unlocking is done before notifying, to avoid waking up
        // the waiting thread only to block again (see notify_one for details)
        lock.unlock();
        _condition.notify_all();
      }

      // Deleted operations
      DispatchQueue(const DispatchQueue& rhs) = delete;
      DispatchQueue& operator=(const DispatchQueue& rhs) = delete;
      DispatchQueue(DispatchQueue&& rhs) = delete;
      DispatchQueue& operator=(DispatchQueue&& rhs) = delete;

    private:
      std::string _name;

      std::mutex _lock;

      std::vector<std::thread> _threads;

      std::queue<Operation> _queue;

      std::condition_variable _condition;

      bool _quit = false;

      void threadHandler(void) {
        std::unique_lock<std::mutex> lock(_lock);
        do {
          //Wait until we have data or a quit signal
          _condition.wait(lock, [this] {
            return (_queue.size() || _quit);
          });

          //after wait, we own the lock
          if (!_quit && _queue.size()) {
            auto op = std::move(_queue.front());
            _queue.pop();

            //unlock now that we're done messing with the queue
            lock.unlock();

            op();

            lock.lock();
          }
        } while (!_quit);
      }
    };
  }
}