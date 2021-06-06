#pragma once
#include <ctime>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

struct Page
{
	bool status;
	unsigned int real_page_number;
	time_t time;
	int array[512 / sizeof(int)];
};

class VM
{
private:
	string name;
	long int size;
	fstream file;
	Page buffer[4];
	bool** bitmap_collection;
	void initialization(); //генерация буфера, битмапы и запись файла

	int* find_add(int real_element_index);
	bool is_in_buffer(int relative_element_index);

public:
	VM(int user_size, string user_name = "default")
	{
		name = user_name;
		size = user_size;
		initialization();
	}

	bool read(int index, int& variable);
	bool write(int index, int variable);
};
