#ifndef APPLICATION_H
#define APPLICATION_H

#include <spdlog/spdlog.h>

class Application
{
public: 
    Application();
    virtual ~Application() = default;

    void Run();
};

#endif

