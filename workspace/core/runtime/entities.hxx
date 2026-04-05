#pragma once

namespace External::Runtime
{
	struct s_entity
	{
		std::string m_username{};
		vector3_t<float> m_location{};
		float m_health = 0.f;
	};
	
	struct s_entities_frame
	{
		std::vector<s_entity> m_entities{};
	};

	class c_entities_cache : public c_cache
	{
	private:
		static inline c_cache_registrar<c_entities_cache> m_registrar{};
		static inline c_engine_cache* m_engine_cache = 0;

	public:
		c_double_buffer<s_entities_frame> m_data{};

		c_entities_cache() : c_cache(CACHE_ID::entities) {}

		void tick() override
		{
			s_entities_frame frame{};

			m_data.update(frame);
			sleep_short(1);
		}
	};
}