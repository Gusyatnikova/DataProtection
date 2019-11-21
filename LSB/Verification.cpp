#include "pch.h"
#include "Verification.h"
using namespace std;

bool check_command(string command) {
	std::vector <string> commands = {
			"Introduction" , "Extraction", "in", "ex" };
	for (auto it = commands.begin(); it != commands.end(); ++it) {
		if (command == *it) return true;
	}
	return false;
}