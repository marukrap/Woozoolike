#pragma once

#include <string>

enum class Language
{
	English,
	Korean,
};

namespace Strings
{
	Language getLanguage();

	void load(Language language);
	std::wstring get(const std::string& id);
}