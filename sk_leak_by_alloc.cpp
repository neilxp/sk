#include <vector>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <vector>
#include <list>
using namespace std;
using namespace boost::asio; 

boost::asio::io_service &getIoService()
{
	static io_service io;
	return io;
}
boost::asio::deadline_timer saveTimer_(getIoService());
list<char *> gList;

int gAction = 0;

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
	if (gList.size() > 0){
		cout << "free memory .\n";
		char *ptr = gList.front();
		::operator delete(ptr);
		gList.pop_front();
	}
	else{
		cout << "No memory to free now!\n";
	}
}

void allocMemory()
{
	cout << "alloc memory space.\n";
	
	int sz = 100000000;

	char* p = (char*)::operator new(sz);
	if (p == NULL){
		cout << "Cannot all memory.\n";
		return;
	}
	for (int i = 0; i < sz; i += sz/10000){
		p[i] = 'a';
	}
	gList.push_back(p);
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

