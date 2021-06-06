#include "VM.h"
#include <iostream>
using namespace std;

//real_element_index - индекс элемента в исходном массиве
//target_page_number - номер искомой страницы
//real_page_number реальный номер страницы

int* VM::find_add(int real_element_index)
{
	try 
	{
		if (real_element_index >= size)
			throw "Out of range!";

		int element_count = 512 / sizeof(int);
		int target_page_number = real_element_index / element_count;
		int relative_element_index = real_element_index % element_count;

		if (!is_in_buffer(target_page_number)) 
		{
			// находим самую старую страницу
			Page oldest_page = buffer[0];
			int oldest_buffer_page_index = 0;
			for (int i = 1; i < 4; i++)
				if ((int)buffer[i].time < (int)oldest_page.time) 
				{
					oldest_page = buffer[i];
					oldest_buffer_page_index = i;
				}

			int oldest_page_number = oldest_page.real_page_number;
			bool* current_bitmap = new bool[element_count];
			Page current_page;
			if (oldest_page.status)
			{
				// находим страницу в файле
				file.open(name + ".bm", fstream::in | fstream::out | fstream::binary);
				do
				{
					for (int i = 0; i < element_count; i++)
						file.read((char*)&current_bitmap[i], sizeof(bool));
					file.read((char*)&current_page, sizeof(Page));
				} while (current_page.real_page_number != oldest_page_number);

				//перезаписываем битмап и страницу
				long pos = file.tellg();
				file.seekp(pos - sizeof(Page) - element_count * sizeof(bool));
				for (int i = 0; i < element_count; i++)
					file.write((char*)&bitmap_collection[oldest_page_number][i], sizeof(bool));
				file.write((char*)&buffer[oldest_buffer_page_index], sizeof(Page));
				file.close();
			}
			//загружаем в буффер страницу target_page_number вместо oldest_buffer_page_index
			file.open(name + ".bm", fstream::in | fstream::binary);
			do
			{
				for (int i = 0; i < element_count; i++)
					file.read((char*)&current_bitmap[i], sizeof(bool));
				file.read((char*)&current_page, sizeof(Page));
			} while (current_page.real_page_number != target_page_number);
			bitmap_collection[target_page_number] = current_bitmap;
			buffer[oldest_buffer_page_index] = current_page;
			buffer[oldest_buffer_page_index].time = time(0);
			buffer[oldest_buffer_page_index].status = 0;
			file.close();
			return &buffer[oldest_buffer_page_index].array[relative_element_index];
		}
		else 
		{
			for (int i = 0; i < 4; i++)
				if (target_page_number == buffer[i].real_page_number)
					return &buffer[i].array[relative_element_index];
		}
	}
	catch (exception & exception)
	{
		return NULL;
	}
}

bool VM::is_in_buffer(int target_page_number) 
{
	for (Page this_page : buffer)
		if (target_page_number == this_page.real_page_number)
			return true;
	return false;
}

void VM::initialization() 
{
	// bitmap
	int element_count = 512 / sizeof(int);
	int page_count = size / element_count + 1;

	bitmap_collection = new bool* [page_count];
	for (int i = 0; i < page_count; i++)
		bitmap_collection[i] = new bool[element_count];
	for (int j = 0; j < page_count; j++)
		for (int i = 0; i < element_count; i++)
			bitmap_collection[j][i] = false;

	// file
	Page page;
	file.open(name + ".bm", fstream::out | fstream::binary);
	if (file.is_open())
		for (int i = 0; i < page_count; i++)
		{
			page.real_page_number = i;

			for (int j = 0; j < element_count; j++)
				file.write((char*)&bitmap_collection[i][j], sizeof(bool));
			file.write((char*)&page, sizeof(Page));
		}
	file.close();

	//buffer
	file.open(name + ".bm", fstream::in | fstream::binary);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < element_count; j++)
			file.read((char*)&bitmap_collection[i][j], sizeof(bool));
		file.read((char*)&buffer[i], sizeof(Page));
		buffer[i].status = 0;
		buffer[i].time = time(0);
	}
	file.close();
}

bool VM::read(int index, int& variable)
{
	int element_count = 512 / sizeof(int);
	int target_page_number = index / element_count;
	int relative_element_index = index % element_count;
	try
	{
		if (bitmap_collection[target_page_number][relative_element_index])
		{
			int* address = find_add(index);
			variable = *address;
			return true;
		}
		else
			return false;
	}
	catch (exception & exception)
	{
		cout << exception.what() << endl;
	}
}

bool VM::write(int index, int variable)
{
	int element_count = 512 / sizeof(int);
	int target_page_number = index / element_count;
	int relative_element_index = index % element_count;
	try
	{
		int* address = find_add(index);
		*address = variable;
		bitmap_collection[target_page_number][relative_element_index] = true;
		for (int i = 0; i < 4; i++)
			if (buffer[i].real_page_number == target_page_number) {
				buffer[i].status = true;
				break;
			}
		return true;
	}
	catch (exception & exception)
	{
		return false;
	}
}
