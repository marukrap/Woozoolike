#include "MessageLog.hpp"
#include "Config.hpp"
#include "Strings.hpp"
#include "Color.hpp"
#include "Utility.hpp" // capitalize

#include <sstream>

MessageLog::MessageLog(Config& config)
	: Console(config.logSize.x, config.logSize.y, *config.font, config.fontSize)
	, maxSize(config.viewSize.y - 2)
{
	setPosition(0.f, (config.screenSize.y - config.logSize.y) * getTileSize().y);
}

void MessageLog::print(const std::wstring& string, const sf::Color& color)
{
	if (!messages.empty() && messages.back().string == string)
		++messages.back().repeat;

	else
	{
		messages.push_back({ string, color, 0 });

		// capitalize the first letter
		if (Strings::getLanguage() == Language::English)
			messages.back().string = capitalize(messages.back().string);

		// 한국어의 조사 처리
		else if (Strings::getLanguage() == Language::Korean)
			handleKorean(messages.back().string);

		if (messages.size() > maxSize)
			messages.erase(messages.begin());
	}

	dirty = true;
}

void MessageLog::print(const std::wstring& string, const sf::Color& color, const std::vector<std::wstring>& words)
{
	std::wistringstream iss(string);
	std::wstring str;
	std::wstring result;

	while (std::getline(iss, str, L'{'))
	{
		if (!str.empty())
			result += str;

		if (std::getline(iss, str, L'}'))
			result += words[std::stoi(str) - 1];
	}

	print(result, color);
}

void MessageLog::update(sf::Time dt)
{
	if (dirty)
	{
		dirty = false;

		clear(Renderer::TextLayer);
		setBox(0, 0, getSize().x, getSize().y, Color::White);
		
		std::size_t y = 0;

		for (std::size_t i = messages.size() > 3 ? messages.size() - 3 : 0; i < messages.size(); ++i)
		{
			if (messages[i].repeat > 0)
				setString(2, 1 + y++, messages[i].string + L" (x" + std::to_wstring(messages[i].repeat + 1) + L")", messages[i].color);
			else
				setString(2, 1 + y++, messages[i].string, messages[i].color);
		}
	}
}

int MessageLog::finalConsonant(wchar_t ch)
{
	if (ch >= 0xac00 && ch <= 0xd7a3)
	{
		if ((ch - 0xac00) % 28 == 0)
			return 0; // 받침 없음 (vowel)
		else
			return 1; // 받침 있음 (consonant)
	}

	// 숫자
	else if (ch >= L'0' && ch <= L'9')
	{
		if (std::wstring(L"013678").find(ch) != std::wstring::npos)
			return 1;
		else // 2459
			return 0;
	}

	return -1; // 한글이 아님
}

void MessageLog::handleKorean(std::wstring& string)
{
	while (true)
	{
		std::size_t leftBracket = string.find(L'[');

		if (leftBracket == std::wstring::npos)
			return;

		std::size_t rightBracket = string.find(L']');
		std::size_t verticalBar = string.find(L'|');

		if (rightBracket == std::wstring::npos || verticalBar == std::wstring::npos)
			return;

		wchar_t ch = string[leftBracket - 1];

		for (int i = leftBracket - 1; i >= 0; --i)
		{
			if (string[i] >= 0xac00 && string[i] <= 0xd7a3)
			{
				ch = string[i];
				break;
			}
		}

		std::wstring postposition; // 은, 는, 이, 가, 을, 를, ...

		if (finalConsonant(ch) == 1)
			postposition = string.substr(leftBracket + 1, verticalBar - (leftBracket + 1));
		else
			postposition = string.substr(verticalBar + 1, rightBracket - (verticalBar + 1));

		string = string.substr(0, leftBracket) + postposition + string.substr(rightBracket + 1);
	}
}