#pragma once

#ifdef FTALE_PLATFORM_WINDOWS


extern ftale::Application* ftale::createApplication();


int main(int argc, char** argv)
{
	printf("Fairytale Engine\n");
	auto app = ftale::createApplication();
	app->run();
	delete app;
}


#endif