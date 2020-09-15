
#include <unistd.h>
#include <iostream>
#include <chrono> // std::chrono::seconds

#include "engine.h"

using namespace wage;

int main(int argc, char* argv[]) {

  // job::Manager manager;
  // manager.start();

  math::Vector3 vec{1, 2, 3};
  std::cout << vec << "\n";

  // auto batch = manager.dispatch({{[]() {
  //                                  std::cout << "Running 1 yo....\n";
  //                                  std::this_thread::sleep_for(std::chrono::seconds(10));
  //                                  std::cout << "Dizone 1 brah....\n";
  //                                }},
  //     {[]() {
  //       std::cout << "Running 2 yo....\n";
  //       std::this_thread::sleep_for(std::chrono::seconds(5));
  //       std::cout << "Dizone 2 brah....\n";
  //     }}});

  // batch->wait();
  // std::cout << "Jobs Done!\n";
}