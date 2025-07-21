#include "Application.h"

int main()
{
	std::unique_ptr<Application> app;

	app = std::make_unique<Application>();

	if (app)
		app->Run();

	return EXIT_FAILURE;
}