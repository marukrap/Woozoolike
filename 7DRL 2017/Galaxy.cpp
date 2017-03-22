#include "Galaxy.hpp"
#include "Rng.hpp"
#include "Color.hpp"

namespace
{
	// Credit: http://www.fantasynamegenerators.com/galaxy-names.php

	const std::vector<std::wstring> names1 = { L"Alpha", L"Apus", L"Aquila", L"Ara", L"Beta", L"Canis", L"Carina", L"Comae", L"Corona", L"Crux", L"Delta", L"Draco", L"Epsilon", L"Gamma", L"Lambda", L"Lyra", L"Nemo", L"Omega", L"Omicron", L"Pavo", L"Proxima", L"Sagitta", L"Serpens", L"Sigma", L"Theta", L"Upsilon", L"Ursa", L"Vela", L"Virgo", L"Zeta", L"", L"", L"", L"", L"", L"" };
	const std::vector<std::wstring> names2 = { L"Acallaris", L"Achelois", L"Adastreia", L"Aegialeus", L"Aegimius", L"Alatheia", L"Alcyoneus", L"Aldebaran", L"Amphiaraus", L"Anadeia", L"Andromeda", L"Aquarii", L"Arcturus", L"Aristaeus", L"Asteria", L"Asteropaios", L"Astraeus", L"Aurigae", L"Boreas", L"Borysthenis", L"Calesius", L"Capella", L"Cassiopeia", L"Centauri", L"Centaurus", L"Chronos", L"Cymopoleia", L"Dioscuri", L"Draconis", L"Eioneus", L"Eridani", L"Eridanus", L"Eubuleus", L"Euphorion", L"Eusebeia", L"Euthenia", L"Hemithea", L"Hyperbius", L"Hyperes", L"Hyperion", L"Icarius", L"Ichnaea", L"Ilioneus", L"Kentaurus", L"Leporis", L"Librae", L"Lyrae", L"Majoris", L"Miriandynus", L"Myrmidon", L"Nebula", L"Nemesis", L"Odysseus", L"Ophiuchi", L"Orion", L"Orionis", L"Orithyia", L"Palioxis", L"Peleus", L"Perileos", L"Perseus", L"Phoroneus", L"Polystratus", L"Porphyrion", L"Proioxis", L"Sagittarius", L"Sirius", L"Solymus", L"Zagreus", L"Zephyrus" };
	const std::vector<std::wstring> names3 = { L"Abyss", L"Acorn", L"Arrowhead", L"Banana", L"Beansprout", L"Beanstalk", L"Bell", L"Blue Ribbon", L"Blueberry", L"Bottleneck", L"Bowl", L"Bull's Eye", L"Bullet", L"Butterfly", L"Cat's Ear", L"Cat's Eye", L"Catterpillar", L"Cherry", L"Chickpea", L"Clover", L"Coconut", L"Comet", L"Crescent", L"Crow's Feet", L"Crown", L"Dandelion", L"Diamond", L"Dragontooth", L"Droplet", L"Eagle Claw", L"Eggshell", L"Exploding", L"Eyebrow", L"Eyelash", L"Falling", L"Feather", L"Fern Leaf", L"Fingerprint", L"Fisheye", L"Fishscale", L"Flame", L"Football", L"Grain", L"Halo", L"Heart", L"Horseshoe", L"Hurricane", L"Icicle", L"Iris", L"Jellyfish", L"Kettle", L"Leaf", L"Lemon", L"Lightbulb", L"Lilypad", L"Lion's Mane", L"Lion's Tail", L"Maelstrom", L"Meridian", L"Mosaic", L"Mouse", L"Octopus", L"Oculus", L"Onion", L"Owl Head", L"Pear", L"Pepper", L"Pig's Tail", L"Pinecone", L"Ponytail", L"Potato", L"Red Ribbon", L"Rippled", L"Rose Petal", L"Sawblade", L"Seashell", L"Serpent", L"Serpent's Eye", L"Sharkfin", L"Sharktooth", L"Shield", L"Shooting Star", L"Snail Shell", L"Snowflake", L"Soap Bubble", L"Sparrow", L"Spearpoint", L"Spiderleg", L"Spiderweb", L"Spiral", L"Starfish", L"Strawberry", L"Teacup", L"Tiara", L"Tiger Paw", L"Tree Root", L"Tree Trunk", L"Turtle Shell", L"Vortex", L"Wave", L"Whale's Tail", L"Zodiac" };
	const std::vector<std::wstring> names4 = { L"Nebula", L"Galaxy", L"Cloud", L"Star System" };
	const std::vector<std::wstring> names5 = { L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", L"J", L"K", L"L", L"M", L"N", L"O", L"P", L"Q", L"R", L"S", L"T", L"U", L"V", L"W", L"X", L"Y", L"Z", L"" };
	const std::vector<std::wstring> names6 = { L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"" };
}

Galaxy::Galaxy(const sf::Vector2i& size, Rng& rng)
	: Level(size, rng)
{
	generateName(rng);
}

LevelType Galaxy::getLevelType() const
{
	return LevelType::Galaxy;
}

void Galaxy::onGenerate()
{
}

void Galaxy::onDecorate()
{
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			Tile& tile = at(x, y);

			tile.glyph = Tile::Floor;
			tile.color = Color::DarkGray;
			tile.passable = true;
			tile.transparent = true;

			// decorate the universe with stars
			int r = rng.getInt(100);

			if (r < 65)
				tile.tileNumber = 0;
			else if (r < 85) // 20
				tile.tileNumber = 1;
			else if (r < 95) // 10
				tile.tileNumber = 2;
			else // 5
				tile.tileNumber = 3;
		}
}

void Galaxy::onInitialize()
{
	addPlanets(rng.getInt(7, 9));
	addSpaceships(rng.getInt(9, 12));

	addBlackHole();
}

void Galaxy::generateName(Rng& rng)
{
	switch (rng.getInt(5))
	{
	case 0:
		name += names1[rng.getInt(names1.size())];
		if (!name.empty()) name += L" ";
		name += names2[rng.getInt(names2.size())];
		break;

	case 1:
		name += names2[rng.getInt(names2.size())];
		name += L" ";
		name += names4[rng.getInt(names4.size())];
		break;

	case 2:
		name += names3[rng.getInt(names3.size())];
		name += L" ";
		name += names4[rng.getInt(names4.size())];
		break;

	case 3:
		name += names5[rng.getInt(names5.size())];
		name += names5[rng.getInt(names5.size())];
		if (!name.empty()) name += L"-";
		name += names6[rng.getInt(names6.size())];
		name += names6[rng.getInt(names6.size())];
		name += names6[rng.getInt(names6.size())];
		break;

	case 4: // default:
		name += names5[rng.getInt(names5.size())];
		name += names5[rng.getInt(names5.size())];
		name += names5[rng.getInt(names5.size())];
		if (!name.empty()) name += L" ";
		name += names6[rng.getInt(names6.size())];
		name += names6[rng.getInt(names6.size())];
		name += names5[rng.getInt(names5.size())];
		break;
	}

	if (name.empty())
	{
		switch (rng.getInt(4))
		{
		case 1: name = L"The Milky Way"; break;
		case 0: name = L"Milky Way Galaxy"; break;
		case 2: name = L"The Galaxy"; break;
		case 3: name = L"Our Galaxy"; break;
		}
	}
}