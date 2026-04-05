#pragma once

namespace External::Math
{
	using namespace Common;
	using namespace Primitives;

	class c_camera
	{
		template<typename T>
		[[nodiscard]] 
		static matrix4x4_t<T> create_rotation_matrix(const vector3_t<T>& rotation) noexcept
		{
			// Radian Values
			T rad_pitch = static_cast<T>(rotation.x * std::numbers::pi / 180.0);
			T rad_yaw   = static_cast<T>(rotation.y * std::numbers::pi / 180.0);
			T rad_roll  = static_cast<T>(rotation.z * std::numbers::pi / 180.0);

			// SinCos Pitch
			T SP = sinf(rad_pitch), CP = cosf(rad_pitch);

			// SinCos Yaw
			T SY = sinf(rad_yaw), CY = cosf(rad_yaw);

			// SinCos Roll
			T SR = sinf(rad_roll), CR = cosf(rad_roll);

			matrix4x4_t<T> _mat{};

			_mat.v1.x = CP * CY;
			_mat.v1.y = CP * SY;
			_mat.v1.z = SP;
			_mat.v1.w = 0.0;

			_mat.v2.x = SR * SP * CY - CR * SY;
			_mat.v2.y = SR * SP * SY + CR * CY;
			_mat.v2.z = -SR * CP;
			_mat.v2.w = 0.0;

			_mat.v3.x = -(CR * SP * CY + SR * SY);
			_mat.v3.y = CY * SR - CR * SP * SY;
			_mat.v3.z = CR * CP;
			_mat.v3.w = 0.0;

			_mat.v4.x = 0;
			_mat.v4.y = 0;
			_mat.v4.z = 0;
			_mat.v4.w = 1.0;

			return _mat;
		}

		template<typename T>
		[[nodiscard]]
		static vector2_t<T> project_to_screen(
			const vector3_t<T>& world_location,
			const vector3_t<T>& camera_location,
			const matrix4x4_t<T>& rotation_matrix,
			const double field_of_view)
		{
			const double fov_radians = tan(field_of_view * std::numbers::pi / 360.0);
			vector3_t<T> camera_delta = world_location - camera_location;
			
			vector3_t<T> x_axis{ rotation_matrix.v1.x, rotation_matrix.v1.y, rotation_matrix.v1.z };
			vector3_t<T> y_axis{ rotation_matrix.v2.x, rotation_matrix.v2.y, rotation_matrix.v2.z };
			vector3_t<T> z_axis{ rotation_matrix.v3.x, rotation_matrix.v3.y, rotation_matrix.v3.z };

			vector3_t<T> camera_transform {
				camera_delta.Dot(x_axis), camera_delta.Dot(y_axis),
				std::max<T>( camera_delta.Dot(z_axis), 1.0 )
			};

			return vector2_t<T>(
				static_cast<T>(screen_center.x + camera_transform.x * (screen_center.x / fov_radians) / camera_transform.z),
				static_cast<T>(screen_center.y - camera_transform.y * (screen_center.x / fov_radians) / camera_transform.z)
			);
		}
	};
	
}