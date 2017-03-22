#include "Planet.hpp"
#include "Level.hpp"
#include "Renderer.hpp"
#include "Color.hpp"

#include "World.hpp"

#include <cwctype> // std::towupper

namespace
{
	// Credit: http://www.fantasynamegenerators.com/planet_names.php

	const std::vector<std::wstring> names1 = { L"b", L"c", L"d", L"f", L"g", L"h", L"i", L"j", L"k", L"l", L"m", L"n", L"p", L"q", L"r", L"s", L"t", L"v", L"w", L"x", L"y", L"z", L"", L"", L"", L"", L"" };
	const std::vector<std::wstring> names2 = { L"a", L"e", L"o", L"u" };
	const std::vector<std::wstring> names3 = { L"br", L"cr", L"dr", L"fr", L"gr", L"pr", L"str", L"tr", L"bl", L"cl", L"fl", L"gl", L"pl", L"sl", L"sc", L"sk", L"sm", L"sn", L"sp", L"st", L"sw", L"ch", L"sh", L"th", L"wh" };
	const std::vector<std::wstring> names4 = { L"ae", L"ai", L"ao", L"au", L"a", L"ay", L"ea", L"ei", L"eo", L"eu", L"e", L"ey", L"ua", L"ue", L"ui", L"uo", L"u", L"uy", L"ia", L"ie", L"iu", L"io", L"iy", L"oa", L"oe", L"ou", L"oi", L"o", L"oy" };
	const std::vector<std::wstring> names5 = { L"turn", L"ter", L"nus", L"rus", L"tania", L"hiri", L"hines", L"gawa", L"nides", L"carro", L"rilia", L"stea", L"lia", L"lea", L"ria", L"nov", L"phus", L"mia", L"nerth", L"wei", L"ruta", L"tov", L"zuno", L"vis", L"lara", L"nia", L"liv", L"tera", L"gantu", L"yama", L"tune", L"ter", L"nus", L"cury", L"bos", L"pra", L"thea", L"nope", L"tis", L"clite" };
	const std::vector<std::wstring> names6 = { L"una", L"ion", L"iea", L"iri", L"illes", L"ides", L"agua", L"olla", L"inda", L"eshan", L"oria", L"ilia", L"erth", L"arth", L"orth", L"oth", L"illon", L"ichi", L"ov", L"arvis", L"ara", L"ars", L"yke", L"yria", L"onoe", L"ippe", L"osie", L"one", L"ore", L"ade", L"adus", L"urn", L"ypso", L"ora", L"iuq", L"orix", L"apus", L"ion", L"eon", L"eron", L"ao", L"omia" };
	const std::vector<std::wstring> names7 = { L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", L"J", L"K", L"L", L"M", L"N", L"O", L"P", L"Q", L"R", L"S", L"T", L"U", L"V", L"W", L"X", L"Y", L"Z", L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"", L"", L"", L"", L"", L"", L"", L"", L"", L"", L"", L"", L"", L"" };

	const std::vector<PlanetInfo> infos =
	{
		{ Planet::Grassy, 40, 4, Color::DarkGreen, Color::Green }, // Grassy
		{ Planet::Desert, 40, 8, Color::Pink, Color::Brown }, // Desert
		{ Planet::Burning, 40, 12, Color::Brown, Color::Red }, // Burning
		{ Planet::Snowy, 40, 16, Color::Cyan, Color::White }, // Snowy
		{ Planet::Ocean, 55, 20, Color::Green, Color::Blue }, // Ocean
		{ Planet::BlackHole, 40, 24, Color::DarkViolet, Color::DarkViolet }, // BlackHole
	};
}

Planet::Planet(int tileNumber, Rng& rng)
	: Entity(0x00d2, tileNumber)
{
	generateName(rng);
	assignGlyph(tileNumber);

	// generate theme
	theme = static_cast<Theme>(rng.getInt(ThemeCount - 1)); // - 1: exclude BlackHole
	this->tileNumber += (theme + rng.getInt(2) * ThemeCount) * 4;
	color = getInfo().wallColor;

}

// HACK: used for BlackHole
Planet::Planet(int tileNumber, Theme theme, Rng& rng)
	: Entity(0x00d2, tileNumber)
{
	// TODO: generate black hole name
	generateName(rng);
	assignGlyph(tileNumber);

	// generate theme
	this->theme = theme;
	this->tileNumber += theme * 4;
	color = getInfo().wallColor;
}

EntityType Planet::getEntityType() const
{
	return EntityType::Planet;
}

std::wstring Planet::getName(Article article) const
{
	if (article == Article::The)
		article = Article::None;

	return attachArticle(name, article);
}

const PlanetInfo& Planet::getInfo() const
{
	return infos[theme];
}

void Planet::attachLevel(std::unique_ptr<Level> level)
{
	level->setName(name);
	this->level = std::move(level);
}

Level* Planet::getLevel() const
{
	return level.get();
}

void Planet::draw(Renderer& renderer)
{
	if (renderer.getType() == Renderer::Ascii)
		renderer.setChar(position.x, position.y, glyph, color);

	else // Renderer::Tile
	{
		sf::Vector2f tileSize = renderer.getTileSize() / 2.f;
		sf::Color tileColor = sf::Color::White;

		// HACK: fov
		if (!world->getLevel().at(position).visible)
			tileColor.a = 51;

		renderer.addChar(position.x, position.y, tileNumber + 0, tileColor, sf::Vector2f(-tileSize.x, -tileSize.y));
		renderer.addChar(position.x, position.y, tileNumber + 1, tileColor, sf::Vector2f(+tileSize.x, -tileSize.y));
		renderer.addChar(position.x, position.y, tileNumber + 2, tileColor, sf::Vector2f(-tileSize.x, +tileSize.y));
		renderer.addChar(position.x, position.y, tileNumber + 3, tileColor, sf::Vector2f(+tileSize.x, +tileSize.y));
	}
}

void Planet::assignGlyph(int tileNumber)
{
	switch (tileNumber)
	{
	case 0x80: glyph = 0x00d2; break; // Ò
	case 0x84: glyph = 0x00d3; break; // Ó
	case 0x88: glyph = 0x00d4; break; // Ô
	case 0x8c: glyph = 0x00d5; break; // Õ
	case 0x90: glyph = 0x00d6; break; // Ö
	case 0x94: glyph = 0x00d8; break; // Ø // Black hole

	case 0x98: glyph = 0x00f2; break; // ò
	case 0x9c: glyph = 0x00f3; break; // ó
	case 0xa0: glyph = 0x00f4; break; // ô
	case 0xa4: glyph = 0x00f5; break; // õ
	case 0xa8: glyph = 0x00f6; break; // ö
	case 0xac: glyph = 0x00f8; break; // ø // Moon
	}

	// 0x014c
	// 0x014e
	// 0x0150
}

void Planet::generateName(Rng& rng)
{
	if (rng.getInt(1000) == 0)
	{
		name = L"Earth";
		return;
	}

	switch (rng.getInt(5))
	{
	case 0:
		name += names1[rng.getInt(names1.size())];
		name += names2[rng.getInt(names2.size())];
		name += names3[rng.getInt(names3.size())];
		name += names4[rng.getInt(names4.size())];
		name += names5[rng.getInt(names5.size())];
		break;

	case 1:
		name += names1[rng.getInt(names1.size())];
		name += names2[rng.getInt(names2.size())];
		name += names3[rng.getInt(names3.size())];
		name += names6[rng.getInt(names6.size())];
		break;

	case 2:
		name += names1[rng.getInt(names1.size())];
		name += names4[rng.getInt(names4.size())];
		name += names5[rng.getInt(names5.size())];
		break;

	case 3:
		name += names3[rng.getInt(names3.size())];
		name += names2[rng.getInt(names2.size())];
		name += names1[rng.getInt(names1.size())];
		name += names2[rng.getInt(names2.size())];
		name += names5[rng.getInt(names5.size())];
		break;

	case 4: // default:
		name += names3[rng.getInt(names3.size())];
		name += names6[rng.getInt(names6.size())];
		name += L" ";
		name += names7[rng.getInt(names7.size())];
		name += names7[rng.getInt(names7.size())];
		name += names7[rng.getInt(names7.size())];
		break;
	}

	name[0] = std::towupper(name[0]);
}