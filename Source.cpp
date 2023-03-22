#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <thread>
#include <Windows.h>
#include <mutex>
#include <cstdlib>
#include <ctime>

// defining globals
int x;
int y;
int gScreenH;
int gScreenW;

HICON icons[] = {
    LoadIcon(NULL, IDI_ERROR),
    LoadIcon(NULL, IDI_WARNING),
    LoadIcon(NULL, IDI_INFORMATION),
    LoadIcon(NULL, IDI_QUESTION),
};

size_t s_icons = sizeof(icons);

// mutex for synchronization
std::mutex g_mutex;

// getting screenres so program is going to work on all resolutions
void GetScreenRes()
{
    gScreenW = GetSystemMetrics(SM_CXSCREEN);
    gScreenH = GetSystemMetrics(SM_CYSCREEN);
}

// drawing icons randomly on our screen
void Draw_Icons()
{
    while (true)
    {
        // lock the mutex before accessing shared resources
        g_mutex.lock();

        HWND hWnd = GetDesktopWindow();
        HDC hdc = GetWindowDC(hWnd);

        DrawIcon(hdc, rand() % GetSystemMetrics(SM_CXSCREEN), rand() % rand() % GetSystemMetrics(SM_CYSCREEN),
            icons[rand() % (s_icons / sizeof(HICON))]);

        ReleaseDC(hWnd, hdc);

        // unlock the mutex when done
        g_mutex.unlock();

        // sleep for a short while to avoid hogging the CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void CrazyMouse()
{
    while (true)
    {
        // Generate random X coordinate
        x = rand() % GetSystemMetrics(SM_CXSCREEN);
        // Generate random Y coordinate
        y = rand() % GetSystemMetrics(SM_CYSCREEN);

        // lock the mutex before accessing shared resources
        g_mutex.lock();

        SetCursorPos(x, y);

        // unlock the mutex when done
        g_mutex.unlock();

        // sleep for a short while to avoid hogging the CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


void Screen_Inverter()
{
    HDC desk = GetDC(0);
    int x1 = GetSystemMetrics(0);
    int y1 = GetSystemMetrics(1);

    while (1)
    {
        // lock the mutex before accessing shared resources
        g_mutex.lock();

        desk = GetDC(0);
        PatBlt(desk, 0, 0, x1, y1, PATINVERT);

        ReleaseDC(0, desk);

        // unlock the mutex when done
        g_mutex.unlock();

        // sleep for a short while to avoid hogging the CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main()
{
    // getting screen res and closing console for victim
    GetScreenRes();
    FreeConsole();

    // start threads
    std::thread t1(CrazyMouse);
    std::thread t2(Screen_Inverter);
    std::thread t3(Draw_Icons);

    // wait for threads to finish
    t1.join();
    t2.join();
    t3.join();

    return 0;
}
