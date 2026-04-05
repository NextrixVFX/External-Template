#include <includes.h>

using namespace External;

namespace global
{
	struct window_t
	{
		const std::string m_process_name = encrypt("Notepad.exe");
		const std::string m_window_name = encrypt("Notepad");
	} window;
}

int main()
{
	Runtime::c_memory::initialize(global::window.m_process_name);
	Render::c_render::initialize(global::window.m_window_name);

	Runtime::register_caches();
	Runtime::start_caches();
	Features::register_features();
	
	static bool running = true;
	while (running)
	{
		if (GetAsyncKeyState(VK_F12))
			running = false;

		Features::on_update();
		sleep_short(1);
	}

	Runtime::stop_caches();
	Render::c_render::destroy_device();

	std::cin.get();
	return 0;
}