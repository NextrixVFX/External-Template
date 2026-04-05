#pragma once

namespace External::Features::Visual
{
	using namespace External::Runtime;

	class c_esp : public c_feature
	{
	private:
		static inline c_feature_registrar<c_esp> m_registrar{};
		static inline c_entities_cache* m_entity_cache = 0;

	public:
		c_esp() : c_feature(FEATURE_ID::visual_esp) {}

		void on_render() override
		{
			
		}

		void on_update() override
		{
			
		}
	};
}