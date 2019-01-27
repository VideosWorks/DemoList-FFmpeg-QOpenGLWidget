#include <iostream>

extern "C"
{
	#include "libavformat/avformat.h"
}

using namespace std; 

int main()
{
	cout << avformat_configuration() << endl;
	cout << "hello" << endl;
	getchar();
	return 0;
}