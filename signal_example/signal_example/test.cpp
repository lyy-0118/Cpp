//#include <iostream>
//#include <csignal> // C++ 风格的信号头文件
//
//// --- 平台相关的头文件和定义 ---
//#ifdef _WIN32
//    // 如果是 Windows 平台 (_WIN32 宏是
//    // Visual Studio 和 MinGW 等编译器预定义的)
//#include <windows.h> 
//#else
//    // 否则，假定是类 UNIX 平台 (Linux, macOS)
//#include <unistd.h>
//#endif
//// ---------------------------------
//
//// 使用 volatile sig_atomic_t 保证在信号处理器中对该变量的访问是原子操作
//volatile sig_atomic_t g_signal_caught = 0;
//
//// 信号处理器函数
//void signalHandler(int signalNum) {
//    std::cout << "\n捕获到信号 " << signalNum << " (Ctrl+C)。" << std::endl;
//    std::cout << "正在进行清理工作，请稍候..." << std::endl;
//    g_signal_caught = 1; // 设置标志位，通知主循环退出
//}
//
//int main() {
//    // 注册信号处理器，关联 SIGINT 信号
//    signal(SIGINT, signalHandler);
//
//    std::cout << "程序已启动，正在运行中..." << std::endl;
//    std::cout << "按下 Ctrl+C 来触发信号处理并优雅地退出。" << std::endl;
//
//    int counter = 0;
//    while (!g_signal_caught) {
//        std::cout << "正在处理任务 " << ++counter << "..." << std::endl;
//
//        // --- 使用平台相关的休眠函数 ---
//#ifdef _WIN32
//        Sleep(1000); // Windows 平台的 Sleep，单位是毫秒
//#else
//        sleep(1);    // 类 UNIX 平台的 sleep, 单位是秒
//#endif
//        // ----------------------------------
//    }
//
//    // 循环结束后，执行最后的清理工作
//    std::cout << "清理工作完成，程序即将退出。" << std::endl;
//
//    return 0;
//}

#include <iostream>
#include <csignal>
#include <windows.h> 

using namespace std;

void signalHandler(int signum)
{
    cout << "Interrupt signal (" << signum << ") received.\n";

    // 清理并关闭
    // 终止程序 

    exit(signum);

}

int main()
{
    int i = 0;
    // 注册信号 SIGINT 和信号处理程序
    signal(SIGINT, signalHandler);

    while (++i) {
        cout << "Going to sleep...." << endl;
        if (i == 3) {
            raise(SIGINT);
        }
        Sleep(1);
    }

    return 0;
}