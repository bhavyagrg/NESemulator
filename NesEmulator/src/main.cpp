#include "Application.h"
#include "LibGui.h"

int main()
{
    Application* app = new Application();
    app->Start();

    delete app;

    return 0;
}