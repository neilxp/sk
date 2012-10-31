#include <vector>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <vector>
#include <list>
#include <deque>
using namespace std;
using namespace boost::asio; 

boost::asio::io_service &getIoService()
{
	static io_service io;
	return io;
}
boost::asio::deadline_timer saveTimer_(getIoService());
deque<int> gList;

int gAction = 0;
int gBatchCount = 10000000;
int current = 0;

void sigHandler(int sig)
{
	if (sig == SIGUSR1){
		gAction ^= 1;
	}else{
		cerr << "unhandled signal.\n";
	}
}


void timeup(const boost::system::error_code& error);

void startTimer(void)
{
	saveTimer_.expires_from_now(boost::posix_time::seconds(1));
	saveTimer_.async_wait( boost::bind(
			timeup, boost::asio::placeholders::error) );
}

void freeMemory()
{
	if (gList.size() >= gBatchCount){
		current--;
		cout << "free memory .\n";
		for (int i = 0; i < gBatchCount; ++i){
			gList.pop_back();
		}
	}
	else{
		cout << "No memory to free now!\n";
	}
}

void allocMemory()
{
	if (current >= 5){
		cout << "max exceed. no alloction.\n";
		return;
	
	}
	++current;
	cout << "take space.\n";

	for (int i = 0; i < gBatchCount; i ++){
		gList.push_back(i);
	}
}

void timeup(const boost::system::error_code& error)
{
	if ( error == error::operation_aborted ){
		cout << "timer is carcelled \n";
		return;// any action ???
	}

	if (gAction == 0){
		allocMemory();
	}else{
		freeMemory();
	}

	startTimer();
}


int main(int argc, char** argv)
{
	struct sigaction act;
	act.sa_handler = sigHandler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGUSR1, &act, 0);

	startTimer();
	getIoService().run();

	return 0;
}

