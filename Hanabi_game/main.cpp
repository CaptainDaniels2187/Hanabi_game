#pragma comment(lib, "HanabiLib.lib")
#include <vld.h>
#include "Hanabi.h"
#include "Player0403.h"
#include <iostream>
#include <fstream>
#include <map>

using namespace Hanabi;

/**
 * @brief "Фабрика" класса игрока. см. Hanabi::PlayerFactory
 *
 * Переименуйте эту функцию так же, как и класс игрока.
 *
 * @param view необходимый аргумент конструктора
 * @return указатель на новый динамический объект класса Player0000
*/
PlayerAbstract* player0403(View view);

const int GAMES_COUNT = 1000;

std::ofstream fout("log.txt");
Log LOG{ Log::Level::Silent, fout }; // см. Hanabi::Log

int main() {
	PlayerFactory factories[PLAYERS_COUNT]{ player0403, player0403 };
	std::map<int, int> results;
	for (int gameI = 0; gameI < GAMES_COUNT; gameI++) {
		results[game(factories, gameI, LOG)]++;
	}
	std::cout << "Results over " << GAMES_COUNT << " games:\n";
	std::cout << "Score\tCount\n";
	for (auto const& res : results) {
		std::cout << res.first << "\t" << res.second << '\n';
	}
	fout.close();
	return 0;
}

PlayerAbstract* player0403(View view) {
	// Замечание: можно передавать в конструктор ссылку/указатель на LOG, чтобы иметь возможность логирования с настройкой уровня.
	return new Player0403(view, LOG);
}
