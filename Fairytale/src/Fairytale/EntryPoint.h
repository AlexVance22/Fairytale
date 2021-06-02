#pragma once

#ifdef FTALE_PLATFORM_WINDOWS


extern ftale::Application* ftale::createApplication();


int main(int argc, char** argv)
{
	ftale::Log::Init();
	FTALE_CORE_INFO("Began Fairytale Engine");
	FTALE_CORE_INFO("Initialized Log");
	FTALE_WARN("Project empty");

	auto app = ftale::createApplication();
	app->run();
	delete app;
}


#endif