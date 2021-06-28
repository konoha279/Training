#include "ProcessList.h"
std::vector<std::wstring> ProcessList::getModules(DWORD processID)
{
	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded;
	unsigned int i;
	std::vector<std::wstring> modules;

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
	if (NULL == hProcess)
		return modules;

	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 1; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH];
			if (GetModuleFileNameEx(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
			{
				modules.push_back(szModName);
			}
		}
	}
	CloseHandle(hProcess);
	return modules;
}
std::vector<Process> ProcessList::getRunningProcesses()
{
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	PVOID pebAddress;
	NTSTATUS Status;
	PVOID rtlUserProcParamsAddress;
	UNICODE_STRING commandLine;
	PROCESS_BASIC_INFORMATION pbi;
	unsigned int i;
	bool result = false;
	std::vector<Process> processList;
	PWSTR cmdLine;

	LONG(WINAPI *NtQueryInformationProcess)(HANDLE ProcessHandle,
		ULONG ProcessInformationClass, PVOID ProcessInformation,
		ULONG ProcessInformationLength, PULONG ReturnLength);

	*(FARPROC *)&NtQueryInformationProcess = GetProcAddress(LoadLibrary(L"ntdll"), "NtQueryInformationProcess");


	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		return processList;
	}
	cProcesses = cbNeeded / sizeof(DWORD);
	for (i = 0; i < cProcesses; i++)
	{
		if (aProcesses[i] != 0)
		{
			TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i]);

			NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), 0);

			if (NULL != hProcess)
			{
				HMODULE hMod;
				DWORD cbNeeded;
				if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
				{
					GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
					
					pebAddress = pbi.PebBaseAddress;
					ReadProcessMemory(hProcess, (PCHAR)pebAddress + 0x10, &rtlUserProcParamsAddress, sizeof(PVOID), NULL);
					ReadProcessMemory(hProcess, (PCHAR)rtlUserProcParamsAddress + 0x40,	&commandLine, sizeof(commandLine), NULL);
					cmdLine = (PWSTR) new char[commandLine.MaximumLength];
					ReadProcessMemory(hProcess, commandLine.Buffer, cmdLine, commandLine.MaximumLength, NULL);

					Process  p;
					p.pid = aProcesses[i];
					p.name = std::wstring(&szProcessName[0]);
					std::wstring cmd = std::wstring(cmdLine);
					p.command = cmd;
					p.Modules = getModules(p.pid);
					processList.push_back(p);
				}
			}
		}
	}
	return processList;
}

