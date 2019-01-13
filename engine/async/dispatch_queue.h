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

  class DispatchQueue {
    
    typedef std::function<void(void)> Operation;

  public:
    
    DispatchQueue(std::string name, size_t threadCount = 1);
    
    ~DispatchQueue();

    // dispatch and copy
    void dispatch(const Operation& op);
    
    // dispatch and move
    void dispatch(Operation&& op);

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

    void threadHandler(void);
  };

}