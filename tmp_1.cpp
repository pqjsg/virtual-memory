#include <iostream>
#include "VM.h"
#include <windows.h>
#include <fstream>
using namespace std;

int main()
{
	VM vm(10000);
	srand(time(0));
	for (int i = 0; i < 10000; i++) 
	{
		cout << i << ": ";
		if (rand() % 2) 
		{
			int random = rand() % 100 - 50;
			cout << random << endl;
			vm.write(i, rand() % 100 - 50);
		}
		else
			cout << "skipped" << endl;
	}
	cout << endl;

	for (int i = 0; i < 10000; i++) 
	{
		int temp = 0;
		cout << i << ": ";
		if (vm.read(i, temp))
			cout << temp << endl;
		else
			cout << "empty" << endl;
	}
}