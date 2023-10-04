#include "Application.h"

int main()
{
	Application* app = new Application();
	app->Construct(780, 480, 2, 2);
	app->Start();
	return 0;
}