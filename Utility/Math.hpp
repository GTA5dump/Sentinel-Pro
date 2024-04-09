#pragma once 
#include "../Common.hpp"
#include "../GTA/Invoker/Natives.hpp"

namespace Sentinel::Math
{
	inline void RunTimed(int* timer, int ms, std::function<void()> callback) 
	{
		if (*timer < MISC::GET_GAME_TIMER()) {
			*timer = MISC::GET_GAME_TIMER() + ms;
			callback();
		}
	}

	inline int RandomNumberInRange(int min, int max)
	{
		static bool First = true;
		if (First)
		{
			srand(time(NULL));
			First = false;
		}
		return min + rand() % ((max + 1) - min);
	}

	inline bool IsNumberEven(int num)
	{
		if (num % 2 == 0)
			return true;
		else
			return false;
	}

	inline float SmoothGlide(float aimX, float entityX)
	{
		static float Coords;

		if (aimX > 0 && entityX > 0)
			Coords = aimX - (entityX);

		if (aimX < 0 && entityX < 0)
			Coords = aimX - (entityX);

		if (aimX > 0 && entityX < 0)
			Coords = aimX + (entityX);

		if (aimX < 0 && entityX > 0)
			Coords = aimX + (entityX);

		return Coords;
	}

	inline int GetIndex(std::vector<std::int16_t> v, std::int16_t K)
	{
		auto it = find(v.begin(), v.end(), K);

		// If element was found
		if (it != v.end())
		{
			int index = it - v.begin();
			return index;
		}
		else {
			return -1;
		}
	}

	inline NativeVector3 Multiply(NativeVector3 vector, float inc)
	{
		vector.x *= inc;
		vector.y *= inc;
		vector.z *= inc;
		return vector;
	}

	inline NativeVector3 Add(NativeVector3 vector, NativeVector3 vector2)
	{
		vector.x += vector2.x;
		vector.y += vector2.y;
		vector.z += vector2.z;
		return vector;
	}

	inline NativeVector3 Subtract(NativeVector3 vector, NativeVector3 vector2)
	{
		vector.x -= vector2.x;
		vector.y -= vector2.y;
		vector.z -= vector2.z;
		return vector;
	}

	inline float DistanceBetweenVectors(NativeVector3 a, NativeVector3 b)
	{
		return (float)sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z), 2));
	}

	inline float DegreeToRadian(float deg)
	{
		const double Radian = (3.14159265359 / 180) * deg;
		return (float)Radian;
	}

	inline NativeVector3 GetGameplayCameraDirection()
	{
		auto Rotation = CAM::GET_GAMEPLAY_CAM_ROT(NULL);
		FLOAT l_tz = Rotation.z * 0.0174532924f;
		FLOAT l_tx = Rotation.x * 0.0174532924f;
		FLOAT l_num = abs(cos(l_tx));
		Rotation.x = (-sin(l_tz)) * l_num;
		Rotation.y = (cos(l_tz)) * l_num;
		Rotation.z = sin(l_tx);
		return Rotation;
	}

	inline NativeVector3 RotationToDirection(NativeVector3 rotation)
	{
		float x = DegreeToRadian(rotation.x);
		float z = DegreeToRadian(rotation.z);

		float num = abs(cos(x));

		return NativeVector3
		{
			-sin(z) * num,
			cos(z) * num,
			sin(x)
		};
	}

	inline NativeVector3 CoordsInFrontOfCam(std::float_t distance)
	{
		const NativeVector3 pos = CAM::GET_GAMEPLAY_CAM_COORD();
		const NativeVector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(NULL);
		const NativeVector3 dir = RotationToDirection(rot);
		const NativeVector3 dist = Multiply(dir, distance);
		return Add(pos, dist);
	}

	inline bool Empty(const NativeVector3& vector)
	{
		return (vector.x == 0.f && vector.y == 0.f && vector.z == 0.f);
	}
}