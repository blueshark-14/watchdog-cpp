#include<algorithm>
#include<iostream>
#include<string>
#include "ConfigManager.h"
#include "OSApiWrapper.h"
#include "ProcessMonitor.h"
using namespace std;

int main() {
    ConfigManager cfg("config.json");
    cfg.load();

    OSApiWrapper api;
    ProcessMonitor monitor(cfg, api);

    monitor.run();

    std::cout << "Foreground app should be: " << cfg.getForegroundApp() << std::endl;
    std::cout << "Press Enter to exit...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear any leftover input
    std::cin.get(); // wait for Enter
}