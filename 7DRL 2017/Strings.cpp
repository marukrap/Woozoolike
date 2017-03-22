#include "Strings.hpp"

#include <unordered_map>

namespace
{
	Language language = Language::Korean;
	std::unordered_map<std::string, std::wstring> stringMap;
}

namespace Strings
{
	Language getLanguage()
	{
		return language;
	}

	void load(Language language)
	{
		::language = language;

		if (language == Language::English)
		{
			// HelpState
			stringMap["Commands"] = L"- Commands -";
			stringMap["Command"] = L"move or melee attack";
			stringMap["Command2"] = L"(numpad keys, vi keys,";
			stringMap["Command3"] = L" or arrow keys with PgUp, PgDn, Home, End)";
			stringMap["Command4"] = L"land on/leave a planet, rest once";
			stringMap["Command5"] = L"enter/exit a vehicle, pick up an item";
			stringMap["Command6"] = L"display old messages";
			stringMap["Command7"] = L"select previous/next weapon";
			stringMap["Command8"] = L"drop equipped weapon";
			stringMap["Command9"] = L"fire (q/w to change target, Esc/Tab to cancel)";
			stringMap["Functions"] = L"- Function Keys -";
			stringMap["Zoom"] = L"zoom in/out";
			stringMap["F1"] = L"help (this screen)";
			stringMap["F2"] = L"toggle graphics mode (ASCII/tile)";
			stringMap["F3"] = L"toggle video filter";

			// GameState
			stringMap["OutOfAmmo"] = L"Out of ammo, no more bullets to shoot...";
		
			// World
			stringMap["Welcome"] = L"Hello, astronaut. Welcome to the Universe of Doom!";
			stringMap["Welcome2"] = L"Find the the Amulet of Yendor, and save the universe with it!";
			stringMap["Welcome3"] = L"Press 'F1' or '?' for help at any time.";
			stringMap["FuelEmpty"] = L"Out of fuel, the engine stops...";
			stringMap["OnWhiteHole"] = L"There is a white hole here.";
			stringMap["OnSingularity"] = L"There is the singularity here.";
			stringMap["LandBlackHole"] = L"Thie place is too dangerous to land.";
			stringMap["LandBlackHole2"] = L"The pieces of the amulet shine and protect you.";
			stringMap["Landed"] = L"You land softly on the planet.";
			stringMap["Left"] = L"You left the planet.";
			stringMap["Invade"] = L"You quietly break into the ship.";
			stringMap["Cockpit"] = L"You get up from the cockpit.";
			stringMap["PilotFail"] = L"There are still enemies in the ship.";

			// Sidebar
			stringMap["Health"] = L"Health";
			stringMap["Fuel"] = L"Fuel";
			stringMap["Oxygen"] = L"Oxygen";
			stringMap["Armor"] = L"Armor";
			stringMap["EnterShip"] = L"enter ship";
			stringMap["ExitShip"] = L"exit ship";
			stringMap["GoInside"] = L"go inside";
			stringMap["GoOutside"] = L"go outside";
			stringMap["PilotShip"] = L"pilot ship";
			stringMap["LandOn"] = L"land on";
			stringMap["TakeOff"] = L"take off";
			stringMap["Fire"] = L"fire";
			stringMap["WeaponSwap"] = L"swap weapon";
			stringMap["WeaponDrop"] = L"drop weapon";
			stringMap["PickUp"] = L"pick up";
			stringMap["Escape"] = L"escape";
			stringMap["Destroy"] = L"destroy";
			stringMap["Wait"] = L"wait";

			// ActorData
			stringMap["You"] = L"you";
			stringMap["Alien"] = L"alien";
			stringMap["Birdian"] = L"birdian";
			stringMap["Clay"] = L"clay";
			// d
			stringMap["EvolvedMutant"] = L"evolved mutant";
			stringMap["Folyp"] = L"folyp";
			stringMap["RiotPolice"] = L"galactic riot police";
			// h
			stringMap["Ilyth"] = L"ilyth";
			// j
			// k
			// l
			stringMap["Mutant"] = L"mutant";
			// n
			stringMap["Ooze"] = L"ooze";
			stringMap["Predator"] = L"predator";
			// q
			stringMap["Raider"] = L"raider";
			stringMap["SpaceMarine"] = L"space marine";
			stringMap["Turret"] = L"turret";
			// u
			stringMap["Viper"] = L"viper";
			// w
			// x
			stringMap["Yeti"] = L"yeti";
			stringMap["Zombie"] = L"zombie";
			stringMap["Something"] = L"something";

			// Weapon
			stringMap["CombatKnife"] = L"combat knife";
			stringMap["LaserCutlass"] = L"laser cutlass";
			stringMap["Pistol"] = L"pistol";
			stringMap["Shotgun"] = L"shotgun";
			stringMap["AssaultRifle"] = L"assault rifle";
			stringMap["SniperRifle"] = L"sniper rifle";
			stringMap["RocketLauncher"] = L"rocket launcher";

			// Actor
			stringMap["OxygenLow"] = L"Oxygen is running out! You need to hurry!";
			stringMap["OxygenEmpty"] = L"Out of oxygen, you can not breathe!";
			stringMap["OxygenDied"] = L"You died of lack of oxygen...";

			// Combat
			stringMap["Destroyed"] = L"{1} has been destroyed.";
			stringMap["Graze"] = L"{1} grazes {2}.";
			stringMap["GlanceOff"] = L"{1} glances off {2}.";
			stringMap["Block"] = L"{1} blocks the attack.";
			stringMap["Miss"] = L"miss";
			stringMap["Misses"] = L"misses";
			stringMap["Hit"] = L"hit";
			stringMap["Hits"] = L"hits";
			stringMap["TheBullet"] = L"the bullet";
			stringMap["TheBullets"] = L"the bullets";
			stringMap["TheMissile"] = L"the missile";
			stringMap["TheExplosion"] = L"boom! the explosion";
			stringMap["Slash"] = L"slash";
			stringMap["Slashes"] = L"slashes";
			stringMap["Slice"] = L"slice";
			stringMap["Slices"] = L"slices";
			stringMap["CutOff"] = L"cut off";
			stringMap["CutsOff"] = L"cuts off";
			stringMap["Attack"] = L"{1} {2} {3}.";
			stringMap["Attack2"] = L"{1} {2} {3} for {4} damage.";
			// HACK: Korean
			stringMap["RangedHit"] = L"hit";
			stringMap["RangedHits"] = L"hits";
			stringMap["RangedAttack"] = L"{1} {2} {3}.";
			stringMap["RangedAttack2"] = L"{1} {2} {3} for {4} damage.";
			//
			stringMap["YouDie"] = L"You die...";
			stringMap["Kill"] = L"{1} dies.";

			// AI
			stringMap["Notice"] = L"{1} notices {2}.";

			// Armor
			stringMap["Shield"] = L"shield";
			stringMap["Helmet"] = L"helmet";
			stringMap["BodyArmor"] = L"body armor";
			stringMap["WearArmor"] = L"You pick up and wear {1}.";
			stringMap["PickUpArmor"] = L"You pick up {1}.";

			// Medikit
			stringMap["Medkit"] = L"medkit";
			stringMap["UseMedkit"] = L"You pick up and use {1}. You feel better.";

			// Amulet
			stringMap["AmuletPiece"] = L"amulet piece";
			stringMap["AmuletAll"] = L"You collected all pieces of the amulet!";
			stringMap["AmuletAll2"] = L"You now have the complete Amulet of Yendor.";
			stringMap["AmuletThree"] = L"The pieces become one, but there are still missing pieces.";
			stringMap["AmuletThree2"] = L"You can now land on the black hole.";
			stringMap["AmuletGet"] = L"You now have a piece of the Amulet of Yendor.";
			stringMap["AmuletGet2"] = L"You now have {1} pieces of the Amulet of Yendor.";

			// Equipment
			stringMap["DropFail"] = L"There is something there already.";
			stringMap["DropWeapon"] = L"{1} dropped {2}.";
			stringMap["DiscardWeapon"] = L"{1} discarded {2}.";
			stringMap["DropArmor"] = L"{1} dropped {2}.";
			stringMap["TakeAmmo"] = L"You get ammo and load your {1}."; // L"You now have {1}.";
			stringMap["AmmoFull"] = L"You have enough ammo already.";
			stringMap["AlreadyHave"] = L"You already have this kind of weapon.";
			stringMap["PickUpWeapon"] = L"You pick up and grap {1}.";

			// PauseState
			stringMap["Return"]		= L"Return ";
			stringMap["Restart"]	= L"Restart"; // New galaxy
			stringMap["Quit"]		= L"Quit   ";

			// VictoryState
			stringMap["Victory"] = L"The Amulet of Yendor twinkles, it lets you pass the white hole.";
			stringMap["Victory2"] = L"You escaped to another galaxy.";
			stringMap["Victory3"] = L"You put the Amulet of Yendor on the singularity.";
			stringMap["Victory4"] = L"The black hole destroys and you save the galaxy.";
			stringMap["YouWon"] = L"You won!";
			stringMap["SpacePirate"] = L"Space Pirate: You invaded all spaceships.";
			stringMap["PlanetExplorer"] = L"Planet Explorer: You visited all planets.";
		}

		else if (language == Language::Korean)
		{
			// HelpState
			stringMap["Commands"] = L"- 조작키 -";
			stringMap["Command"] = L"이동 또는 근접 공격";
			stringMap["Command2"] = L"(숫자패드, vi 키,";
			stringMap["Command3"] = L" 또는 화살표키 + PgUp, PgDn, Home, End)";
			stringMap["Command4"] = L"행성 착륙/이륙, 한턴 쉬기";
			stringMap["Command5"] = L"탈것 타기/내리기, 아이템 줍기";
			stringMap["Command6"] = L"이전 메시지 보기";
			stringMap["Command7"] = L"이전/다음 무기 선택";
			stringMap["Command8"] = L"장착중인 무기 버리기";
			stringMap["Command9"] = L"발사 (q/w 목표 변경, Esc/Tab 취소)";
			stringMap["Functions"] = L"- 기능키 -";
			stringMap["Zoom"] = L"화면 확대/축소";
			stringMap["F1"] = L"도움말 (이 화면)";
			stringMap["F2"] = L"그래픽 토글 (아스키/타일)";
			stringMap["F3"] = L"화면 필터 효과 토글";

			// GameState
			stringMap["OutOfAmmo"] = L"탄약이 다 떨어졌다. 더 이상 쏠 총알이 없다...";

			// World
			stringMap["Welcome"] = L"어서오세요, 우주 여행자님. 운명의 우주에 오신 것을 환영합니다!";
			stringMap["Welcome2"] = L"옌더의 부적을 찾아 이 우주를 구해주세요!";
			stringMap["Welcome3"] = L"'F1' 또는 '?' 키를 누르면 언제든지 도움말을 볼 수 있습니다.";
			stringMap["FuelEmpty"] = L"연료가 없다. 엔진이 정지했다...";
			stringMap["OnWhiteHole"] = L"여기에 화이트홀이 있다.";
			stringMap["OnSingularity"] = L"여기에 특이점이 있다.";
			stringMap["LandBlackHole"] = L"이 곳에 접근하는 것은 너무 위험해보인다."; // 암흑 에너지(Dark energy)
			stringMap["LandBlackHole2"] = L"빛나는 부적 조각들이 우주선을 보호한다.";
			stringMap["Landed"] = L"안전하게 지상에 착륙했다.";
			stringMap["Left"] = L"행성을 떠났다.";
			stringMap["Invade"] = L"조용히 우주선 안으로 침입했다.";
			stringMap["Cockpit"] = L"조종석에서 일어났다.";
			stringMap["PilotFail"] = L"아직 우주선 안에 적이 남아있다.";

			// Sidebar
			stringMap["Health"] = L"체력";
			stringMap["Fuel"] = L"연료";
			stringMap["Oxygen"] = L"산소";
			stringMap["Armor"] = L"방어";
			stringMap["EnterShip"] = L"우주선 탑승";
			stringMap["ExitShip"] = L"우주선 나가기";
			stringMap["GoInside"] = L"우주선 안으로";
			stringMap["GoOutside"] = L"우주선 밖으로";
			stringMap["PilotShip"] = L"우주선 조종";
			stringMap["LandOn"] = L"착륙";
			stringMap["TakeOff"] = L"이륙";
			stringMap["Fire"] = L"발사";
			stringMap["WeaponSwap"] = L"무기 교체";
			stringMap["WeaponDrop"] = L"무기 버리기";
			stringMap["PickUp"] = L"줍기";
			stringMap["Escape"] = L"탈출하기";
			stringMap["Destroy"] = L"파괴하기";
			stringMap["Wait"] = L"대기";

			// ActorData
			stringMap["You"] = L"당신";
			stringMap["Alien"] = L"에일리언";
			stringMap["Birdian"] = L"버디언";
			stringMap["Clay"] = L"흙인간";
			// d
			stringMap["EvolvedMutant"] = L"진화 돌연변이";
			stringMap["Folyp"] = L"우주 해파리";
			stringMap["RiotPolice"] = L"은하 경찰";
			// h
			stringMap["Ilyth"] = L"무너인간";
			// j
			// k
			// l
			stringMap["Mutant"] = L"돌연변이";
			// n
			stringMap["Ooze"] = L"수액 괴물";
			stringMap["Predator"] = L"포식자";
			// q
			stringMap["Raider"] = L"무법자";
			stringMap["SpaceMarine"] = L"우주 해병";
			stringMap["Turret"] = L"무인 포탑";
			// u
			stringMap["Viper"] = L"우주 해적";
			// w
			// x
			stringMap["Yeti"] = L"설인";
			stringMap["Zombie"] = L"좀비";
			stringMap["Something"] = L"무언가";

			// Actor
			stringMap["OxygenLow"] = L"산소가 부족하다! 서둘러야 한다.";
			stringMap["OxygenEmpty"] = L"산소가 없다. 숨을 쉴 수가 없다!";
			stringMap["OxygenDied"] = L"질식해서 죽었다...";

			// Combat
			stringMap["Destroyed"] = L"{1}[이|가] 파괴되었다.";
			stringMap["Graze"] = L"{1}[이|가] {2}[을|를] 스쳤다."; // 총알이 헬맷을 스쳤다.
			stringMap["GlanceOff"] = L"{1}[은|는] {2}에 튕겨나갔다."; // 총알은 방패에 튕겨나갔다.
			stringMap["Block"] = L"{1}[으로|로] 공격을 막았다."; // 방패로 공격을 막았다.
			stringMap["Miss"] = L"빗맞혔다";
			stringMap["Misses"] = L"빗맞혔다";
			stringMap["Hit"] = L"공격했다";
			stringMap["Hits"] = L"공격했다";
			stringMap["TheBullet"] = L"총알";
			stringMap["TheBullets"] = L"총알";
			stringMap["TheMissile"] = L"미사일";
			stringMap["TheExplosion"] = L"쾅! 폭발";
			stringMap["Slash"] = L"베었다";
			stringMap["Slashes"] = L"베었다";
			stringMap["Slice"] = L"베었다";
			stringMap["Slices"] = L"베었다";
			stringMap["CutOff"] = L"찔렀다";
			stringMap["CutsOff"] = L"찔렀다";
			stringMap["Attack"] = L"{1}[은|는] {3}[을|를] {2}.";
			stringMap["Attack2"] = L"{1}[은|는] {3}[을|를] {2}. {4}의 피해를 주었다.";
			// HACK: Korean
			stringMap["RangedHit"] = L"명중했다";
			stringMap["RangedHits"] = L"명중했다";
			stringMap["RangedAttack"] = L"{1}[이|가] {3}에 {2}.";
			stringMap["RangedAttack2"] = L"{1}[이|가] {3}에 {2}. {4}의 피해를 주었다.";
			//
			stringMap["YouDie"] = L"죽었다...";
			stringMap["Kill"] = L"{1}[은|는] 죽었다.";

			// AI
			stringMap["Notice"] = L"{1}[은|는] {2}[을|를] 알아챘다.";

			// Weapon
			stringMap["CombatKnife"] = L"전투 나이프"; // 전투 단검, 컴뱃 나이프
			stringMap["LaserCutlass"] = L"레이저 검";
			stringMap["Pistol"] = L"권총";
			stringMap["Shotgun"] = L"산탄총";
			stringMap["AssaultRifle"] = L"돌격 소총";
			stringMap["SniperRifle"] = L"저격 소총";
			stringMap["RocketLauncher"] = L"로켓 런처"; // 로켓 발사기

			// Armor
			stringMap["Shield"] = L"방패";
			stringMap["Helmet"] = L"헬멧";
			stringMap["BodyArmor"] = L"방탄복";
			stringMap["WearArmor"] = L"{1}[을|를] 착용했다.";
			stringMap["PickUpArmor"] = L"{1}[을|를] 획득했다.";

			// Medikit
			stringMap["Medkit"] = L"구급 상자"; // 메디킷
			stringMap["UseMedkit"] = L"{1}[을|를] 주워 사용했다. 상태가 좋아졌다.";

			// Amulet
			stringMap["AmuletPiece"] = L"부적 조각";
			stringMap["AmuletAll"] = L"모든 부적 조각을 모았다!";
			stringMap["AmuletAll2"] = L"완전한 옌더의 부적를 획득했다.";
			stringMap["AmuletThree"] = L"조각들이 하나가 되었다. 그러나 아직 부족한 조각이 있다.";
			stringMap["AmuletThree2"] = L"이제 블랙홀에 착륙할 수 있다.";
			stringMap["AmuletGet"] = L"옌더의 부적 조각 하나를 획득했다.";
			stringMap["AmuletGet2"] = L"옌더의 부적 조각을 {1}개째 획득했다.";

			// Equipment
			stringMap["DropFail"] = L"이곳에는 이미 다른 아이템이 있다.";
			stringMap["DropWeapon"] = L"{1}[은|는] {2}[을|를] 떨어뜨렸다.";
			stringMap["DiscardWeapon"] = L"{1}[은|는] {2}[을|를] 버렸다.";
			stringMap["TakeAmmo"] = L"{1}의 탄약을 채웠다.";
			stringMap["AmmoFull"] = L"이미 탄약은 충분하다.";
			stringMap["AlreadyHave"] = L"이 종류의 무기는 이미 가지고 있다.";
			stringMap["PickUpWeapon"] = L"{1}[을|를] 집어들었다.";
			stringMap["DropArmor"] = L"{1}[은|는] {2}[을|를] 버렸다.";

			// PauseState
			stringMap["Return"]		= L"돌아가기";
			stringMap["Restart"]	= L"재시작 ";
			stringMap["Quit"]		= L"종료  ";

			// VictoryState
			stringMap["Victory"] = L"옌더의 부적이 반짝이며 화이트홀을 통과했다.";
			stringMap["Victory2"] = L"당신은 다른 우주로 도망쳤다.";
			stringMap["Victory3"] = L"옌더의 부적을 특이점 위에 내려놓았다.";
			stringMap["Victory4"] = L"블랙홀은 사라지고 당신은 이 우주를 지켜냈다.";
			stringMap["YouWon"] = L"승리했습니다!";
			stringMap["SpacePirate"] = L"우주 해적: 모든 우주선을 침략했다.";
			stringMap["PlanetExplorer"] = L"행성 탐험가: 모든 행성을 방문했다.";
		}
	}

	std::wstring get(const std::string& id)
	{
		return stringMap[id];
	}
}