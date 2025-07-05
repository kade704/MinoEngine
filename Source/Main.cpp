#include "Application.h"

#include <string>
#include <iostream>

int main()
{
	std::unique_ptr<Application> app;

	app = std::make_unique<Application>();

	if (app)
		app->Run();

	return EXIT_FAILURE;
}