#include <iostream>

#include <Windows.h>

#include <tlhelp32.h>

#include <string>

#include <wchar.h>

int main() {
    HANDLE th32,p,rmt;
    PVOID rmb;
    PROCESSENTRY32 p32;
    std::string opt;
    int opt2;
    system("color a");
    while (opt != "quit" || opt != "n") {
        opt = "";
        std::cout << "\n1 - listar processos" << std::endl;
        std::cout << "\n2 - fechar processos" << std::endl;
        std::cout << "\n3 - injectar dll" << std::endl;
        std::cout << "quit - sair" << std::endl;
        std::cin >> opt;
        HANDLE th32 = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPALL, 0);
        p32.dwSize = sizeof(PROCESSENTRY32);
        if (opt == "1") {
            if (!Process32First(th32, &p32)) {
                std::cout << "err" << std::endl;
            }
            while (Process32Next(th32, &p32)) {
                std::wcout << p32.szExeFile << " | PID:" << p32.th32ProcessID << std::endl;
            }
            std::cout << "Voltar ao menu ? [s/n]";
            opt = "";
            std::cin >> opt;
        }
        if (opt == "2") {
            std::cout << "digite o nome" << std::endl;
            opt = "";
            std::cin >> opt;
            while (Process32Next(th32, &p32)) {
                if (wcscmp(p32.szExeFile, std::wstring(opt.begin(),opt.end()).c_str()) == 0) {
                    HANDLE Hproc = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)p32.th32ProcessID);
                    if (Hproc != NULL) {
                        TerminateProcess(Hproc,9);
                        CloseHandle(Hproc);
                    }
                }
            }
            std::cout << "Voltar ao menu ? [s/n]";
            opt = "";
            std::cin >> opt;
        }
        if (opt == "3") {
            std::cout << "Digite o PID" << std::endl;
            opt2 = NULL;
            opt = "";
            std::cin >> opt2;
            std::cout << "Digite o caminho da dll:";
            std::cin >> opt;
            HMODULE krnl32 = GetModuleHandle((LPCWSTR)"Kernel32");
            VOID *ll = GetProcAddress(krnl32, "LoadLibraryA");
            p = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD) opt2 );
            rmb = VirtualAllocEx(p, NULL, sizeof(opt)+1, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);
            WriteProcessMemory(p, rmb, opt.c_str(), sizeof(opt) + 1, NULL);
            rmt = CreateRemoteThread(p, NULL, 0, (LPTHREAD_START_ROUTINE)ll, rmb, 0, NULL);
            CloseHandle(p);
        }
    }
}
