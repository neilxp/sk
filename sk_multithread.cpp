#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <poll.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <boost/unordered_map.hpp>

using namespace boost;
using namespace std;

shared_mutex gMutex;
unordered_map<int, string> gMap;


void threadRead(int thread)
{
	while(true){
		shared_lock<shared_mutex> l(gMutex);
		cout << gMap.size() << endl;
		int key = rand() % 50;
		unordered_map<int, string>::const_iterator itr = gMap.find(key);
		
		if (itr != gMap.end()){
			ostringstream oss;
			oss << "n" << key;
			assert(oss.str() == itr->second);
			cout << "Thread " << thread << " find key: " << key << ":" << itr->second << endl;
		}
		
		l.unlock();
		poll(0, 0, rand() % 30 + 1);
	}
}

void threadWrite()
{
	while(true){
		unique_lock<shared_mutex> l(gMutex);
		if (rand() % 3 == 0) {
			for (int i = 0; i < 10; ++i){
				int key = rand() % 50;
				gMap.erase(key);
			}
		}else{
			int key = rand() % 50;
			ostringstream oss;
			oss << "n" << key;
			gMap[key] = oss.str();

		}
		l.unlock();
		poll(0, 0, rand() % 10 + 1);
		
	}
}

int main(int argc, char** argv)
{
	srand(time(NULL));
	int n = 0;
	new thread(bind(threadRead, n++));
	new thread(bind(threadRead, n++));
	new thread(bind(threadRead, n++));
	new thread(bind(threadRead, n++));
	new thread(bind(threadRead, n++));
	new thread(bind(threadRead, n++));
	new thread(bind(threadRead, n++));
	new thread(bind(threadRead, n++));
	new thread(bind(threadRead, n++));
	new thread(bind(threadRead, n++));
	new thread(bind(threadRead, n++));

	new thread(threadWrite);
	new thread(threadWrite);
	new thread(threadWrite);
	new thread(threadWrite);
	new thread(threadWrite);
	new thread(threadWrite);
	new thread(threadWrite);
	new thread(threadWrite);
	new thread(threadWrite);
	new thread(threadWrite);
	new thread(threadWrite);
	new thread(threadWrite);
	poll(0, 0, 600000);

	
	return 0;
}
