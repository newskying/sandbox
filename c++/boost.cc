//g++ --std=c++11 -Iboost/usr/local/include -Lboost/usr/local/lib -lboost_system -lboost_thread boost.cc
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using namespace std;

void handler1(const boost::system::error_code &ec) {
   cout << "5 s." << endl;
}

void handler2(const boost::system::error_code &ec) {
   cout << "10 s." << endl;
}

boost::asio::io_service io_service;     //multi thread 1 service
//boost::asio::io_service io_service2;  //1 thread 1 io_service //could opti perf by locality execution using CPU affinity
void run() {
   io_service.run();
}

void test_boost_asio() {
   boost::asio::deadline_timer timer1(io_service, boost::posix_time::seconds(5));
   timer1.async_wait(handler1);
   boost::asio::deadline_timer timer2(io_service, boost::posix_time::seconds(10));
   timer2.async_wait(handler2);
   cout << "in main thread" << endl;
   boost::thread thread1(run);
   boost::thread thread2(run);
   thread1.join();
   thread2.join();
}

int main(int argc, char* argv[]) {
    test_boost_asio();
}

