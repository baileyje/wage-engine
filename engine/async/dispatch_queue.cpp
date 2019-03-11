
#include "async/dispatch_queue.h"

namespace wage {

  DispatchQueue::DispatchQueue(std::string name, size_t threadCount) : _name(name), _threads(threadCount) {
    for (size_t i = 0; i < _threads.size(); i++) {
      _threads[i] = std::thread(&DispatchQueue::threadHandler, this);
    }
  }

  DispatchQueue::~DispatchQueue() {
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

  void DispatchQueue::dispatch(const Operation& op) {
    std::unique_lock<std::mutex> lock(_lock);
    _queue.push(op);

    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lock.unlock();
    _condition.notify_all();
  }

  void DispatchQueue::dispatch(Operation&& op) {
    std::unique_lock<std::mutex> lock(_lock);
    _queue.push(std::move(op));

    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lock.unlock();
    _condition.notify_all();
  }

  void DispatchQueue::threadHandler(void) {
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
}