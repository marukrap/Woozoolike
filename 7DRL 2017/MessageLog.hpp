#pragma once

#include "Console.hpp"
// #include "Entity.hpp"

#include <SFML/System/Time.hpp>

struct Config;

class MessageLog : public Console
{
public:
	struct Message
	{
		std::wstring string;
		sf::Color color;
		int repeat;
	};

public:
	explicit MessageLog(Config& config);

	/*
	template <typename T>
	void print(const T& t);
	template <typename First, typename... Rest>
	void print(const First& first, const Rest&... rest);
	*/

	void print(const std::wstring& string, const sf::Color& color = sf::Color::White);
	void print(const std::wstring& string, const sf::Color& color, const std::vector<std::wstring>& words);

	void update(sf::Time dt);

private:
	int finalConsonant(wchar_t ch);
	void handleKorean(std::wstring& string);

private:
	bool dirty = true;
	std::size_t maxSize;
	std::vector<Message> messages; // deque?

	friend class MessageLogState;
};