//#define BOOST_POOL_INSTRUMENT true

#include <vector>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/pool/pool_alloc.hpp>
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

struct Foo{
	int n;
	char c;
};

typedef Foo TargetType;
//deque<TargetType, boost::fast_pool_allocator<TargetType> > gCtnr;
deque<TargetType> gCtnr;

int gAction = 0;
int gBatchCount = 10000000;
//int gBatchCount = 1000;
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

template <int n>
struct For{
	static void carryout(){
		cout << "try free size " << n << endl;
		bool freedSome = boost::singleton_pool<boost::fast_pool_allocator_tag, n>::release_memory();
		if (freedSome){
			cout << "free size " << n << endl;
		}
		For<n-1>::carryout();
	}
};

template <>
struct For<1>{
	static void carryout(){
		bool freedSome = boost::singleton_pool<boost::fast_pool_allocator_tag, 1>::release_memory();
		if (freedSome){
			cout << "free size " << 1 << endl;
		}
	}
};

bool freeMemory()
{
	if (gCtnr.size() >= gBatchCount){
		current--;
		cout << "free memory .\n";
		for (int i = 0; i < gBatchCount; ++i){
			gCtnr.pop_back();
		}
	}
	else{
		cout << "No item to pop_back now, release memory! \n" ;
		For<200>::carryout();
	}
	return true;
}

bool  allocMemory()
{
	if (current >= 5){
		cout << "max exceed. no allocation.\n";
		return false;
	}
	++current;
	cout << "take space.\n";

	for (int i = 0; i < gBatchCount; i ++){
		gCtnr.push_back(TargetType());
	}
	return true;
}

void timeup(const boost::system::error_code& error)
{
	if ( error == error::operation_aborted ){
		cout << "timer is carcelled \n";
		return;// any action ???
	}
	bool ret = false;

	if (gAction == 0){
		ret = allocMemory();
	}else{
		ret = freeMemory();
	}

	if (ret){
		startTimer();
	}
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

