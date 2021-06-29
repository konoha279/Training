#include <iostream>
#include <fstream>
#include "ProcessList.h"
int main()
{
	std::wofstream file;
	file.open("ouput.dat", std::ios::out);
	ProcessList list;
	std::vector<Process> processList;
	list.getRunningProcesses();
	processList = list.getRunningProcesses();
	//print
	/*for(Process p:processList)
	{
		std::wcout << L"PID: " << p.pid << std::endl;
		std::wcout << L"Name: " << p.name << std::endl;
		std::wcout << L"Command: " << p.command << std::endl;
		std::wcout << L"DLL: "<< std::endl;
		for (std::wstring dll: p.Modules)
		{
			std::wcout << L"[+]\t" << dll << std::endl;
		}
		std::wcout << L"------------------------------------------------------------------------------------- " << std::endl;
	}*/
	//output to file
	for (Process p : processList)
	{
		file << L"PID: " << p.pid << std::endl;
		file << L"Name: " << p.name << std::endl;
		file << L"Command: " << p.command << std::endl;
		file << L"DLL: " << std::endl;
		for (std::wstring dll : p.Modules)
		{
			file << L"[+]\t" << dll << std::endl;
		}
		file << L"\n-------------------------------------------------------------------------------------" << std::endl;
	}
	file.close();
	return 0;
}