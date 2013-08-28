#include <csignal>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using namespace std;
using namespace boost;
using namespace boost::asio;

void dummyTimerTimeout(const boost::system::error_code& e, deadline_timer *timer)
{
	if ( e == boost::asio::error::operation_aborted ){
		return;
	}
}

int main (void)
{
	io_service io;
	deadline_timer timer(io);
	timer.expires_from_now(boost::posix_time::seconds(2));
	timer.async_wait(boost::bind( dummyTimerTimeout, boost::asio::placeholders::error, &timer));
	io.run();
	cout << "Extied" << endl;

	return 0;
}

