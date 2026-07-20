#include "GameManagement/GameDef.h"

int main() {
	std::ios_base::sync_with_stdio(false);
	do {
		Game();
	} while (!GameEndChoice());
	return 0;
}