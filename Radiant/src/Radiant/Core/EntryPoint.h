#pragma once

#ifdef RD_PLATFORM_WINDOWS

extern Radiant::Application* Radiant::CreateApplication();

int main(int argc, char** argv)
{
	Radiant::Log::Init();

	RD_PROFILE_BEGIN_SESSION("App Startup", "Radiant_app_startup_session.json");
	auto app = Radiant::CreateApplication();
	RD_PROFILE_END_SESSION();

	RD_PROFILE_BEGIN_SESSION("App Run Loop", "Radiant_app_run_session.json");
	app->Run();
	RD_PROFILE_END_SESSION();

	RD_PROFILE_BEGIN_SESSION("App Shutdown", "Radiant_app_shutdown_session.json");
	delete app;
	RD_PROFILE_END_SESSION();
}

#endif // RD_PLATFORM_WINDOWS
