#pragma once

#include <thread>

#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <initializer_list>

#include "core/service.h"
#include "core/logger.h"

#include "job/job.h"
#include "job/batch.h"

namespace wage {
  namespace job {

    /**
     * Engine service providing the job execution and parallel execution. 
     */
    class Manager : public core::Service {
    public:
      Manager() : Service("Job") {
      }

      /**
       * Start the service and bind the workers.
       */
      void start() {
        _running = true;
        auto numThreads = std::thread::hardware_concurrency() - 1;
        _threads.resize(numThreads);
        core::Logger::info("Starting Job Service with ", numThreads, " threads.");
        // TODO: How to do we pin to cores???
        for (size_t i = 0; i < _threads.size(); i++) {
          _threads[i] = std::thread(&Manager::threadHandler, this);
        }
      }

      /**
       * Stop the workers and cleanup the queue.
       */
      void stop() {
        std::unique_lock<std::mutex> lock(_lock);
        _running = false;
        lock.unlock();
        _condition.notify_all();
        // Wait for threads to finish before we exit
        for (auto& thread : _threads) {
          if (thread.joinable()) {
            thread.join();
          }
        }
        _queue = {};
      }

      /**
       * Dispatch a job on one of the workers.
       */
      void dispatch(const Job& job) {
        std::unique_lock<std::mutex> lock(_lock);
        _queue.push(job);
        lock.unlock();
        _condition.notify_all();
      }

      /**
       * Dispatch a general piece of work on one of the workers.
       */
      void dispatch(const Job::Work& work) {
        std::unique_lock<std::mutex> lock(_lock);
        _queue.push({work});
        lock.unlock();
        _condition.notify_all();
      }

      /**
       * Dispatch a batch of Jobs and receive a pointer to the batch.
       */
      std::unique_ptr<Batch> dispatch(std::initializer_list<Job> jobs) {
        auto batch = std::unique_ptr<Batch>(new Batch(jobs.size()));
        std::unique_lock<std::mutex> lock(_lock);
        for (auto& job : jobs) {
          _queue.push({[&]() {
            job();
            batch->done();
          }});
        }
        lock.unlock();
        _condition.notify_all();
        return batch;
      }

      /**
       * Dispatch a batch of Jobs and receive a pointer to the batch.
       */
      std::unique_ptr<Batch> dispatch(std::vector<Job> jobs) {
        auto batch = new Batch(jobs.size());
        std::unique_lock<std::mutex> lock(_lock);
        for (auto& job : jobs) {
          _queue.push({[job, batch]() {
            job();
            batch->done();
          }});
        }
        lock.unlock();
        _condition.notify_all();
        return std::unique_ptr<Batch>(batch);
      }

    private:
      void threadHandler(void) {
        std::unique_lock<std::mutex> lock(_lock);
        do {
          _condition.wait(lock, [this] {
            return (_queue.size() || !_running);
          });
          if (_running && _queue.size()) {
            auto job = std::move(_queue.front());
            _queue.pop();
            lock.unlock();
            job();
            lock.lock();
          }
        } while (_running);
      }

      std::queue<Job> _queue;

      std::vector<std::thread> _threads;

      std::mutex _lock;
      std::condition_variable _condition;

      volatile bool _running = false;
    };
  }
}