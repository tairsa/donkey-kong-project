#include "FileInputGame.h"
#include "HumanInputGame.h"

int main(int argc, char** argv)
{
	bool isSaveMode = argc > 1 && std::string(argv[1]) == "-save";
	bool isLoadMode = argc > 1 && std::string(argv[1]) == "-load";
	bool isSilent = isLoadMode && argc > 2 && std::string(argv[2]) == "-silent";

	BaseGame* donkeyKong;
	if (isLoadMode)
		donkeyKong = new FileInputGame(isSilent);
	else
		donkeyKong = new HumanInputGame(isSaveMode); 
	donkeyKong->run(); 
	delete(donkeyKong);

}
