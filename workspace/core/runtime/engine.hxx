#pragma once

namespace External::Runtime
{
	struct camera_t
	{
		uintptr_t m_address = 0;
		vector3_t<float> m_positon{};
		vector3_t<float> m_rotation{};
		float m_fov = 0;
	};

	struct s_engine_frame
	{
		uintptr_t m_data_model = 0;
		uintptr_t m_workspace = 0;
		uintptr_t m_players_ptr = 0;
		uintptr_t m_local_player = 0;
		uint64_t m_place_id = 0;
		camera_t m_camera{};
	};

	class c_engine_cache : public c_cache
	{
	private:
		static inline c_cache_registrar<c_engine_cache> m_registrar{};

	public:
		c_double_buffer<s_engine_frame> m_data{};

		c_engine_cache() : c_cache(CACHE_ID::engine) {}

		void tick() override
		{
			s_engine_frame frame{};
			uintptr_t base_address = c_memory::m_application.m_base_address;
			
			m_data.update(frame);
			sleep_short(1);
		}
	};
}