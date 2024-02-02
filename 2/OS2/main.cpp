#include <Windows.h>
#include <iostream>
#include <string>

void DoWinExec();
void DoShellExecute();
void DoCreateProcess();

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "Wrong number of parameters!" << std::endl;
		return -1;
	}

	std::string param = argv[1];
	if (param == "WinExec") {
		DoWinExec();
	} else if (param == "ShellExecute") {
		DoShellExecute();
	} else if (param == "CreateProcess") {
		DoCreateProcess();
	} else {
		std::cout << "Wrong Argument!" << std::endl;
		return -2;
	}

	return 0;
}

void DoWinExec() {
	WinExec("notepad.exe", SW_SHOW);
}

void DoShellExecute() {
	ShellExecute(NULL, L"open", L"notepad.exe", NULL, NULL, SW_SHOW);
}

void DoCreateProcess() {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	std::wstring CommandLine(L"notepad.exe");
	LPWSTR lpwCmdlLine = &CommandLine[0];

	BOOL b = NULL;
	b = CreateProcessW(NULL, lpwCmdlLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	GetProcessId(pi.hProcess);
	GetThreadId(pi.hThread);

	std::cout << GetProcessId(pi.hProcess) << std::endl;
	std::cout << GetThreadId(pi.hThread) << std::endl;

	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}