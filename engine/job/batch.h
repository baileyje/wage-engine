#pragma once

#include <atomic>
#include <mutex>
#include <condition_variable>

namespace wage {
  namespace job {

    /**
     * A batch represents a number of jobs enqueued or running withing the workers. When jobs complete they will signal to the 
     * batch they have completed and once all jobs from the batch have completed, anyone waiting for the batch to finish will 
     * notified,
     */
    class Batch {
    public:
      /**
       * Create a batch with number of expected jobs to execute.
       */
      Batch(unsigned int size) : count(size) {}

      // TODO: Dynamic/buildable batches
      // TODO: Batch dependencies.

      /**
       * Signal a job within this batch has completed. 
       * 
       * TODO: I don't like this being public, so make it friendly with the manager/job if needed.
       */
      void done() {
        auto current = --count;
        if (current == 0) {
          std::unique_lock<std::mutex> lock(_lock);
          _condition.notify_all();
        }
      }

      /**
       * Wait for this batch to finish. This will block the calling thread until the batch countdown has completed.
       */
      void wait() {
        std::unique_lock<std::mutex> lock(_lock);
        _condition.wait(lock, [this] {
          return count == 0;
        });
      }

    private:
      std::atomic<int> count;
      std::mutex _lock;
      std::condition_variable _condition;
    };
  }

}