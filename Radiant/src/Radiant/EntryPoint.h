#pragma once

#ifdef RD_PLATFORM_WINDOWS

extern Radiant::Application* Radiant::CreateApplication();

int main(int argc, char** argv)
{
	Radiant::Log::Init();

	RD_CORE_WARN("Initialized the logger");
	int a = 10;
	RD_INFO("Client online... test var={0}", a);

	auto app = Radiant::CreateApplication();
	app->Run();
	delete app;
}

#endif // RD_PLATFORM_WINDOWS
