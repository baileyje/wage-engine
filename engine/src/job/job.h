#pragma once

#include "util/types.h"

#include <functional>

namespace wage {
  namespace job {

    /**
     * Represends a chunk of work to be executed as a managed and tracked job.
     */
    class Job {
    public:
      /** Signature for a work function */
      using Work = std::function<void()>;
      /** Signature for a job type */
      using Type = uint16;

      /** Default type for just general work */
      static Type GeneralJob;

      /**
       * Create a new job with provided work and the general type.
       */
      Job(Work work) : Job(GeneralJob, work) {
      }

      /**
       * Create a new job with provided work and the provided type.
       */
      Job(Type type, Work work) : _type(type), _work(work) {
      }

      Job(const Job& job) {
        _work = std::move(job._work);
        _type = job._type;
      }

      /**
       * Get the job's type.
       */
      inline Type type() {
        return _type;
      }

      /**
       * Invoke the work for this job.
       */
      void operator()() const {
        _work();
      }

      Job& operator=(Job&& other) {
        _work = std::move(other._work);
        _type = other._type;
        return *this;
      }

    private:
      Type _type;
      Work _work;
    };
  }
}