#include "Vehicle.hpp"
#include "Level.hpp"
#include "Renderer.hpp"
#include "Color.hpp"

#include "World.hpp"

namespace
{
	// Credit: http://www.fantasynamegenerators.com/spaceship-names.php

	/*
	BC - Battlecruiser
	BS - Battleship
	CS - Cargo ship / Cruiseship
	HMS - Her Majesty's (Space)Ship
	HWSS - Heavy Weight Spaceship
	ISS - Imperial Spaceship
	LWSS - Light Weight Spaceship
	SC - Spacecruiser
	SS - Spaceship
	SSE - Spaceship Earth (So you could insert your own planet names)
	STS - Space Transportation System
	USS - United (States) Spaceship (So you could insert your own country/nation name)
	*/

	const std::vector<std::wstring> names1 = {
		L"SS", L"SSE", L"LWSS", L"HWSS", L"CS", L"USS", L"BS", L"ISS", L"HMS", L"BC", L"SC", L"STS"
	};

	const std::vector<std::wstring> names2 = {
		L"Ace of Spades", L"Achilles", L"Actium", L"Adder", L"Adventurer", L"Agememnon", L"Albatross", L"Alexander", L"Alexandria", L"Alice", L"Alto", L"Amanda", L"Amazon", L"Ambition", L"Analyzer", L"Anarchy", L"Anastasia", L"Andromeda", L"Angel", L"Angelica", L"Anna", L"Annihilator", L"Antagonist", L"Antioch", L"Apocalypse", L"Apollo", L"Aquila", L"Aquitaine", L"Arcadia", L"Arcadian", L"Archmage", L"Arden", L"Ares", L"Argo", L"Argonaut", L"Aries", L"Arizona", L"Ark Royal", L"Armada", L"Armageddon", L"Arrow Flight", L"Artemis", L"Arthas", L"Ashaton", L"Assassin", L"Athens", L"Atlas", L"Aurora", L"Avadora", L"Avalanche", L"Avalon", L"Avenger", L"Avius",
		L"Babylon", L"Badger", L"Baldrin", L"Bandit", L"Barbara", L"Basilisk", L"Bastion", L"Battalion", L"Battlestar", L"Bayonet", L"Behemoth", L"Beholder", L"Beluga", L"Berserk", L"Big Boy", L"Big Daddy", L"Big Momma", L"Bishop", L"Black Cloud", L"Black Sparrow", L"Black Viper", L"Blade", L"Blossom", L"Blue Whale", L"Boa", L"Bob", L"Bravery", L"Britain", L"Brotherhood", L"Buccaneer", L"Burn", L"Burninator", L"Buzzard",
		L"Caelestis", L"Cain", L"Calamity", L"Calypso", L"Carbonaria", L"Carnage", L"Carthage", L"Cataclysm", L"Cataphract", L"Celina", L"Centipede", L"Centurion", L"Challenger", L"Chimera", L"Chronos", L"Churchill", L"Civilization", L"Clap", L"Claymore", L"Close Encounter", L"Colossus", L"Comet", L"Commissioner", L"Condor", L"Confidence", L"Conqueror", L"Conquistador", L"Conscience", L"Constantine", L"Constellation", L"Cordoba", L"Corsair", L"Cossack", L"Courage", L"Covenant", L"Coyote", L"Crack", L"Crash", L"Crocodile", L"Cromwell", L"Crusher", L"Cyclone", L"Cyclops", L"Cyclopse", L"Cydonia",
		L"Dagger", L"Dakota", L"Damascus", L"Dark", L"Dark Phoenix", L"Dart", L"Dauntless", L"Death", L"Defiance", L"Defiant", L"Deimos", L"Deinonychus", L"Deonida", L"Desire", L"Despot", L"Destiny", L"Destroyer", L"Destructor", L"Detection", L"Detector", L"Determination", L"Devastator", L"Development", L"Diplomat", L"Discovery", L"Dispatcher", L"Divine Intervention", L"Dragonstar", L"Dragontooth", L"Dreadnought", L"Dream", L"Duke",
		L"Eagle", L"Elba", L"Elena", L"Elizabeth", L"Elysium", L"Emissary", L"Empress", L"Endeavor", L"Enterprise", L"Escorial", L"Eternal", L"Eternity", L"Euphoria", L"Europa", L"Event Horizon", L"Evolution", L"Exarch", L"Excalibur", L"Excursionist", L"Executioner", L"Executor", L"Experience", L"Exploration", L"Explorer", L"Exterminator",
		L"Facade", L"Fade", L"Fafnir", L"Falcon", L"Falling Star", L"Fate", L"Final Frontier", L"Fire Wyvern", L"Firebrand", L"Flavia", L"Fortitude", L"Fortune", L"Francesca", L"Freedom", L"Frenzy", L"Frontier", L"Fudgy",
		L"Galactic Core", L"Galactica", L"Galatea", L"Gallimimus", L"Gauntlet", L"Geisha", L"Genesis", L"Ghunne", L"Gibraltar", L"Gladiator", L"Gladius", L"Globetrotter", L"Glorious", L"Goliath", L"Gremlin", L"Griffin", L"Guard", L"Guardian",
		L"Halo", L"Hammer", L"Hannibal", L"Harbinger", L"Harlegand", L"Harlequin", L"Harmony", L"Harpy", L"Hawk", L"Helios", L"Helldiver", L"Hellhound", L"Herald", L"Hercules", L"Herminia", L"Hope", L"Horizon", L"Hummingbird", L"Hunter", L"Huntress", L"Hurricane",
		L"Icarus", L"Ice Lance", L"Immortal", L"Imperial", L"Independence", L"Inferno", L"Infineon", L"Infinitum", L"Infinity", L"Ingenuity", L"Innuendo", L"Inquisitor", L"Insurgent", L"Intelligence", L"Interceptor", L"Intervention", L"Intrepid", L"Intruder", L"Invader", L"Invictus", L"Invincible", L"Irmanda", L"Isabella",
		L"Janissary", L"Javelin", L"Jellyfish", L"Judgment", L"Juggernaut",
		L"Karma", L"Karnack", L"Katherina", L"Kennedy", L"Khan", L"Kingfisher", L"Kipper", L"Knossos", L"Kraken", L"Kryptoria", L"Kyozist",
		L"Lancaster", L"Last Hope", L"Lavanda", L"Legacy", L"Leo", L"Leviathan", L"Liberator", L"Liberty", L"Lifebringer", L"Lightning", L"Lion", L"Little Rascal", L"Loki", L"Lucidity", L"Lucky", L"Luisa", L"Lullaby", L"Lupus",
		L"Mace", L"Maiden Voyage", L"Majestic", L"Malevolent", L"Malta", L"Manchester", L"Manhattan", L"Manticore", L"Marauder", L"Marchana", L"Marduk", L"Maria", L"Matador", L"Memory", L"Memphis", L"Mercenary", L"Mercenary Star", L"Merkava", L"Messenger", L"Meteor", L"Midway", L"Millenium", L"Minotuar", L"Montgomery", L"Muriela", L"Myrmidon",
		L"Navigator", L"Nebuchadnezzar", L"Nemesis", L"Neptune", L"Nero", L"Neurotoxin", L"Neutron", L"New Beginning", L"New Hope", L"New World", L"Nexus", L"Niagara", L"Night", L"Nightfall", L"Nightingale", L"Nihilus", L"Nineveh", L"Ninja", L"Nirvana", L"Nomad", L"Normandy", L"Nostradamus", L"Nuria",
		L"Oberon", L"Oblivion", L"Observer", L"Ohio", L"Olavia", L"Omen", L"Opal Star", L"Oregon", L"Orion",
		L"Paladin", L"Panama", L"Pandora", L"Paradise", L"Paramount", L"Pathfinder", L"Patience", L"Patriot", L"Peacock", L"Pegasus", L"Pelican", L"Pennsylvania", L"Perilous", L"Phalanx", L"Philadelphia", L"Phobetor", L"Phobos", L"Phoenix", L"Pilgrim", L"Pinnacle", L"Pioneer", L"Piranha", L"Plaiedes", L"Polaris", L"Pontiac", L"Poseidon", L"Praetor", L"Prennia", L"Priestess", L"Prometheus", L"Promise", L"Prophet", L"Providence", L"Proximo", L"Pursuer", L"Pursuit", L"Pyrrhus",
		/* Q */
		L"Rafaela", L"Rampart", L"Ramses", L"Rascal", L"Ravager", L"Ravana", L"Raven", L"Raving", L"Reaver", L"Rebellion", L"Rebellious", L"Relentless", L"Reliant", L"Remorseless", L"Remus", L"Renault", L"Repulse", L"Resolution", L"Retribution", L"Revenant", L"Revolution", L"Rhapsody", L"Rhinoceros", L"Rhodes", L"Ripper", L"Rising", L"Romulus", L"Roosevelt", L"Royal",
		L"Saber", L"Sagittarius", L"Samurai", L"Sandra", L"Sara", L"Saragossa", L"Saratoga", L"Scavenger", L"Scimitar", L"Scorpio", L"Scythe", L"Seleucia", L"Seraphim", L"Serenity", L"Serpent", L"Shade", L"Shear Razor", L"Shirley", L"Shooting Star", L"Siberia", L"Silent", L"Silent Watcher", L"Siren", L"Slayer", L"Sonne", L"Sparrow", L"Sparta", L"Spartacus", L"Spectator", L"Spectrum", L"Spider", L"Spitfire", L"Stalker", L"Stalwart", L"Star Finder", L"Star Fury", L"Star Opal", L"Star Talon", L"Starfall", L"Stargazer", L"Starhammer", L"Starhunter", L"Steel Aurora", L"Stellar Flare", L"Storm", L"Stormfalcon", L"Stormspike", L"Strike", L"Striker", L"Sunder", L"Suzanna", L"Syracuse",
		L"Templar", L"Tenacity", L"Tennessee", L"Teresa", L"Terigon", L"Termite", L"Thanatos",
		///* The */ L"The Albatross", L"The Alliance", L"The Colossus", L"The Commissioner", L"The Condor", L"The Diplomat", L"The Executioner", L"The Exterminator", L"The Falcon", L"The Gladiator", L"The Guardian", L"The Hammerhead", L"The Harbinger", L"The Inquisitor", L"The Javelin", L"The Jellyfish", L"The Kraken", L"The Leviathan", L"The Liberator", L"The Messenger", L"The Paladin", L"The Pelican", L"The Promise", L"The Prophet", L"The Raven", L"The Rhinoceros", L"The Serpent", L"The Siren", L"The Spectator", L"The Titan", L"The Tortoise", L"The Traveler", L"The Trident", L"The Vagabond", L"The Warrior", L"The Watcher",
		L"Thebes", L"Thor", L"Thunder", L"Thunderbird", L"Thunderbolt", L"Thunderstorm", L"Thylacine", L"Titan", L"Titania", L"Tomahawk", L"Torment", L"Tortoise", L"Totale", L"Tourist", L"Trafalgar", L"Trailblazer", L"Tranquility", L"Traveler", L"Trenxal", L"Trident", L"Trinity", L"Triumph", L"Troy", L"Twilight", L"Typhoon", L"Tyrant",
		L"Ulysses", L"Unicorn", L"Unity", L"Unstoppable", L"Untouchable", L"Ural", L"Utopia",
		L"Vagabond", L"Valhala", L"Valhalla", L"Valiant", L"Valkyrie", L"Valor", L"Vampire", L"Vanguard", L"Vanquisher", L"Vengeance", L"Venom", L"Vera", L"Verdant", L"Verminus", L"Vespira", L"Victoria", L"Victory", L"Vigilant", L"Vindicator", L"Viper", L"Virginia", L"Vision", L"Visitor", L"Voyager", L"Vulture",
		L"Wailing Wind", L"Warlock", L"Warrior", L"Washington", L"Watcher", L"Wellington", L"Whirlwind", L"Wildcat", L"Wisdom", L"Wish Upon a Star", L"Wolf", L"Wolverine", L"Woodpecker", L"Wyvern",
		L"Xerxes",
		L"Yucatan",
		L"Zenith", L"Zephyr", L"Zeus", L"Zion"
	};
}

Vehicle::Vehicle(wchar_t ch, Rng& rng)
	: Entity(ch, ch)
	, fuel(rng.getInt(50, 150))
	, maxFuel(150)
{
	if (ch == L'Q')
		color = Color::Red;
	else if (ch <= L'F' || ch == L'K')
		color = Color::White;
	else if (ch <= L'M')
		color = Color::Green;
	else if (ch <= L'T')
		color = Color::Blue;
	else
		color = Color::Brown;

	generateName(rng);
}

EntityType Vehicle::getEntityType() const
{
	return EntityType::Vehicle;
}

std::wstring Vehicle::getName(Article article) const
{
	if (article == Article::The)
		article = Article::None;

	return attachArticle(name, article);
}

void Vehicle::changeFuel(int amount)
{
	fuel += amount;

	if (fuel > maxFuel)
		fuel = maxFuel;
	else if (fuel < 0)
		fuel = 0;
}

int Vehicle::getFuel() const
{
	return fuel;
}

int Vehicle::getMaxFuel() const
{
	return maxFuel;
}

void Vehicle::attachLevel(std::unique_ptr<Level> level)
{
	level->setName(name);
	this->level = std::move(level);
}

Level* Vehicle::getLevel() const
{
	return level.get();
}

void Vehicle::draw(Renderer& renderer)
{
	if (renderer.getType() == Renderer::Ascii)
		renderer.setChar(position.x, position.y, glyph, color);
	
	else // Renderer::Tile
	{
		sf::Color tileColor = sf::Color::White;

		// HACK: fov
		if (!world->getLevel().at(position).visible)
			tileColor.a = 51;

		renderer.addChar(position.x, position.y, tileNumber, tileColor, sf::Vector2f(0.f, 0.f));
	}
}

void Vehicle::generateName(Rng& rng)
{
	if (rng.getInt(3) == 0)
		name = names1[rng.getInt(names1.size())] + L" ";

	std::size_t min = 0, max = 0;

	for (std::size_t i = 0; i < names2.size(); ++i)
	{
		if (names2[i][0] == glyph)
		{
			min = max = i;
			break;
		}
	}

	while (max < names2.size())
	{
		if (names2[max][0] != glyph)
			break;
		else
			++max;
	}

	if (glyph == L'K' && rng.getInt(2) == 0) // L'K'
		name = L"The Kestrel";

	else if (min == 0 && max == 0) // L'Q'
		name += L"Guuuuuum";

	else
	{
		if (name.empty() && rng.getInt(2) == 0)
			name += L"The ";

		name += names2[rng.getInt(min, max - 1)];
	}
}