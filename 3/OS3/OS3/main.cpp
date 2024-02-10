#include <Windows.h>
#include <iostream>

struct ARGS {
	int* arr;
	int size;
};

enum Priority {
	EQUAL, FIRST, SECOND
};

void GenArr(int*, int);
void CreateAndMeasureThreads(Priority, const int);
DWORD WINAPI SortA(LPVOID);
DWORD WINAPI SortB(LPVOID);

int main(int* argc, char** argv) {
	const int size = std::pow(10, 5);

	CreateAndMeasureThreads(EQUAL, size);
	CreateAndMeasureThreads(FIRST, size);
	CreateAndMeasureThreads(SECOND, size);

	return 1;
}

void GenArr(int* arr, int size) {
	srand((unsigned)time(NULL));
	for (int i = 0; i < size; i++) {
		arr[i] = rand() % 101; // from 0 to 100
	}
}

void CreateAndMeasureThreads(Priority prio, const int size) {
	int aPrio, bPrio;
	switch (prio) {
	case EQUAL:
		aPrio = THREAD_PRIORITY_NORMAL;
		bPrio = THREAD_PRIORITY_NORMAL;
		break;
	case FIRST:
		aPrio = THREAD_PRIORITY_HIGHEST;
		bPrio = THREAD_PRIORITY_LOWEST;
		break;
	case SECOND:
		aPrio = THREAD_PRIORITY_LOWEST;
		bPrio = THREAD_PRIORITY_HIGHEST;
		break;
	}

	int* arrA = (int*)malloc(sizeof(int) * size), * arrB = (int*)malloc(sizeof(int) * size);
	ARGS argsA = { arrA, size };
	ARGS argsB = { arrB, size };

	GenArr(arrA, size);
	GenArr(arrB, size);

	DWORD threadAId, threadBId;

	FILETIME creationTimeA, exitTimeA, kernelTimeA, userTimeA,
		creationTimeB, exitTimeB, kernelTimeB, userTimeB;

	SetProcessPriorityBoost(GetCurrentProcess(), FALSE);

	HANDLE hThreadA = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SortA, &argsA, 0, &threadAId);
	SetThreadPriority(hThreadA, aPrio);

	HANDLE hThreadB = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SortB, &argsB, 0, &threadBId);
	SetThreadPriority(hThreadB, bPrio);

	WaitForSingleObject(hThreadA, INFINITE);
	WaitForSingleObject(hThreadB, INFINITE);

	GetThreadTimes(hThreadA, &creationTimeA, &exitTimeA, &kernelTimeA, &userTimeA);
	GetThreadTimes(hThreadB, &creationTimeB, &exitTimeB, &kernelTimeB, &userTimeB);

	std::cout << "A." << std::endl;
	std::cout << "Creation time: " << creationTimeA.dwLowDateTime / 100000.0 << "." << std::endl;
	std::cout << "Exit time: " << exitTimeA.dwLowDateTime / 100000.0 << "." << std::endl;
	std::cout << "Kernel time: " << kernelTimeA.dwLowDateTime / 100000.0 << "." << std::endl;
	std::cout << "User time: " << userTimeA.dwLowDateTime / 100000.0 << "." << std::endl;
	std::cout << "\nWork time: " << (exitTimeA.dwLowDateTime - creationTimeA.dwLowDateTime) / 100000.0 << ".\n\n\n";

	std::cout << "B." << std::endl;
	std::cout << "Creation time: " << creationTimeB.dwLowDateTime / 100000.0 << "." << std::endl;
	std::cout << "Exit time: " << exitTimeB.dwLowDateTime / 100000.0 << "." << std::endl;
	std::cout << "Kernel time: " << kernelTimeB.dwLowDateTime / 100000.0 << "." << std::endl;
	std::cout << "User time: " << userTimeB.dwLowDateTime / 100000.0 << "." << std::endl;
	std::cout << "\nWork time: " << (exitTimeB.dwLowDateTime - creationTimeB.dwLowDateTime) / 100000.0 << ".\n\n\n\n";
}

// bubble sort
DWORD WINAPI SortA(LPVOID lpParam) {
	ARGS args = *((ARGS*)lpParam);
	int* arr = args.arr;
	int size = args.size;

	int* tmpA = (int*)(malloc(sizeof(int) * size));

	for (int i = 0; i < size; i++) {
		tmpA[i] = arr[i];
	}

	int tmpV;
	bool flag;

	do {
		flag = true;
		for (int i = 0; i < size - 1; i++) {
			if (tmpA[i] > tmpA[i + 1]) {
				tmpV = tmpA[i];
				tmpA[i] = tmpA[i + 1];
				tmpA[i + 1] = tmpV;
				flag = false;
			}
		}
	} while (!flag);

	return NULL;
}

// insertion sort
DWORD WINAPI SortB(LPVOID lpParam) {
	ARGS args = *((ARGS*)lpParam);
	int* arr = args.arr;
	int size = args.size;

	int* tmpA = (int*)(malloc(sizeof(int) * size));

	for (int i = 0; i < size; i++) {
		tmpA[i] = arr[i];
	}

	int tmpV, j;

	for (int i = 1; i < size; i++) {
		tmpV = tmpA[i];
		j = i - 1;

		while (j >= 0 && tmpA[j] > tmpV) {
			tmpA[j + 1] = tmpA[j];
			j = j - 1;
		}
		tmpA[j + 1] = tmpV;
	}

	return NULL;
}