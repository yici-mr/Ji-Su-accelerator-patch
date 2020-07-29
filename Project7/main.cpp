#include "加载.h"
using namespace std;
//取模块句柄
DWORD GetModeBase(const TCHAR* LPSTRModuleName, DWORD __DwordProcessId)
{
	MODULEENTRY32 lpModuleEntry = { 0 };
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, __DwordProcessId);
	if (!hSnapShot)
		return NULL;
	lpModuleEntry.dwSize = sizeof(lpModuleEntry);
	BOOL __RunModule = Module32First(hSnapShot, &lpModuleEntry);
	while (__RunModule)
	{
		if (!_tcscmp(lpModuleEntry.szModule, LPSTRModuleName))
		{
			CloseHandle(hSnapShot);
			return (DWORD)lpModuleEntry.modBaseAddr;
		}
		__RunModule = Module32Next(hSnapShot, &lpModuleEntry);
	}
	CloseHandle(hSnapShot);
	return NULL;
}
//取进程pid
DWORD GetProcessID(const TCHAR* ProcessName)
{
	PROCESSENTRY32 pe32;
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnapShot == INVALID_HANDLE_VALUE)
		return -1;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	BOOL __RunModule = Process32First(hSnapShot, &pe32);
	while (__RunModule)
	{
		if (!_tcscmp(pe32.szExeFile, ProcessName))
		{
			CloseHandle(hSnapShot);
			return pe32.th32ProcessID;
		}
		__RunModule = Process32Next(hSnapShot, &pe32);
	}
	CloseHandle(hSnapShot);
	return -1;
}

int main() {
	DWORD PID = GetProcessID(L"SpeedClient.exe");
	DWORD modid = GetModeBase(L"SpeedClient.exe", PID);
	HANDLE openid = OpenProcess(PROCESS_ALL_ACCESS,TRUE, PID);
	int buff = 2337370091;
	int buffT = 33257;
	int buffnop = 3431682448;
	//ReadProcessMemory(openid,LPCVOID(modid + 0x5255b),&bug, sizeof(bug),NULL);
	WriteProcessMemory(openid, LPVOID(modid + 0x52CBC), &buff, sizeof(buff), NULL);//2337370091,要破解的第一个点,写内存
	WriteProcessMemory(openid, LPVOID(modid + 0x52D43), &buffT, sizeof(buffT), NULL);//33257,要破解的第二个点,写内存
	WriteProcessMemory(openid, LPVOID(modid + 0x52D48), &buffnop, sizeof(buffnop), NULL);//3431682448,要破解的第二个点,写内存
	cout << "破解成功！" <<endl;
	Sleep(5000000);
}

