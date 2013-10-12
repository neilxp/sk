#include <boost/asio.hpp>

int main(int argc, char** argv)
{
	boost::asio::io_service ios;
	boost::asio::ip::tcp::socket s1(ios), s2(ios);
	s2 = s1;
	return 0;
}
