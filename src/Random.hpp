#pragma once
#include <random>

namespace Random
{
inline std::mt19937 &rng()
{
	static std::mt19937 engine{std::random_device{}()};
	return engine;
}
} // namespace Random
