#include <csignal>
#include <iostream>

void bar()
{
	std::cout << "rcoherc";
}

void foo()
{
	for(int i = 0; i < 100000; ++i){
		bar();
		
	}
}

int
main (void)
{
	foo();

	return 0;
}

