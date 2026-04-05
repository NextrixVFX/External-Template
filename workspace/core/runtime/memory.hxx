#pragma once

#define is_invalid(value) if (!value) return false

namespace External::Runtime
{
	struct s_application
	{
		bool m_is_valid = false;
		DWORD m_pid = NULL;
		HANDLE m_handle = NULL;
		uintptr_t m_base_address = 0x0;
	};

	class c_memory
	{
	private:
		static DWORD get_pid(const std::string p_proc_name)
		{
#ifdef is_usermode_mem
			return mm::c_usermode::get_process_pid(p_proc_name);
#endif

			// add kernel get_pid calls if doing km
		}
		
		static HANDLE get_handle(const DWORD p_pid)
		{
#ifdef is_usermode_mem
			return mm::c_usermode::get_process_handle(p_pid);
#endif

			// add kernel get_handle calls if doing km
		}

		static uintptr_t get_base_addr(const HANDLE p_proc_handle)
		{
#ifdef is_usermode_mem
			return mm::c_usermode::get_base_address(p_proc_handle);
#endif

			// add kernel get_base_addr calls if doing km
		}
		
	public:
		static inline s_application m_application{};

		static bool initialize(const std::string& p_name)
		{
			if (m_application.m_is_valid)
				return true;

			// get pid
			m_application.m_pid = get_pid(p_name);
			is_invalid(m_application.m_pid);

			// get handle
			m_application.m_handle = get_handle(m_application.m_pid);
			is_invalid(m_application.m_handle);

			// get base
			m_application.m_base_address = get_base_addr(m_application.m_handle);
			is_invalid(m_application.m_base_address);

			Logger::print(encrypt("Attached to:\t%s"), p_name.c_str());
			Logger::print(encrypt("Found pid:\t%d"), m_application.m_pid);
			Logger::print(encrypt("Found handle:\t 0x%llx"), m_application.m_handle);
			Logger::print(encrypt("Found base address:\t0x%llx\n"), m_application.m_base_address);

			m_application.m_is_valid = true;
			return true;
		}

		static bool is_valid_address(uintptr_t addr)
		{
			return (addr > 0x10000 && addr < 0x7FFFFFFFFFFFFFFF && m_application.m_is_valid);
		}

		template<typename T>
		static T read(uintptr_t addr)
		{
			if (!m_application.m_is_valid)
				return T{};

			T buffer{};
			if (ReadProcessMemory(m_application.m_handle, reinterpret_cast<LPCVOID>(addr), &buffer, sizeof(T), NULL))
				return buffer;

			return T{};
		}

		static bool read(uintptr_t addr, void* buffer, size_t size)
		{
			if (!m_application.m_is_valid)
				return false;

			if (ReadProcessMemory(m_application.m_handle, reinterpret_cast<LPCVOID>(addr), buffer, size, NULL))
				return true;

			return false;
		}

		template<typename T>
		static bool write(uintptr_t addr, T value)
		{
			if (!m_application.m_is_valid)
				return false;

			if (!is_valid_address(addr))
				return false;

			if (WriteProcessMemory(m_application.m_handle, reinterpret_cast<LPCVOID>(addr), &value, sizeof(T), NULL))
				return true;

			return false;
		}
	};
}

#undef is_invalid