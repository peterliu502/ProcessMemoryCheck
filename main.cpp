#include <iostream>
#include <Windows.h>
#include <psapi.h>
#include <ctime>

double getPidMemory(DWORD processID)
{
    HANDLE hProcess;
    PROCESS_MEMORY_COUNTERS pmc;
    double workingSize = 0.0;
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
                           PROCESS_VM_READ,
                           FALSE, processID);
    if (nullptr == hProcess)
        return 0.0;

    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
    {
        workingSize = pmc.WorkingSetSize / 1024.0 / 1024.0;
    }

    CloseHandle(hProcess);
    return workingSize;
}

int main() {
    int pid        = 11736; // PID of target process
    int t_interval = 1; // Sample interval (sec)
    int t_period   = 2000; // Sample time (sec)
    int stop       = -1;
    auto t_start   = clock();
    auto t_current = clock();

    while ((clock() - t_start) / 1000 < t_period) {
        if ((clock() - t_current) / 1000 >= t_interval) {
            t_current = clock();
            double memory = getPidMemory(pid);
            std::cout << memory << std::endl;
            if (memory == 0){
                if (stop != 0) stop = 0;
                else break;
            }
        }
    }

    return 0;
}
