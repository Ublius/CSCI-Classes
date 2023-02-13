#include "wumpus_functions.h"

string enter_wumpus_file()
{
    string fileName = "";
    cout << "Enter File Name (.wgh): "
    cin >> fileName;
    return fileName;
}

string enter_game_log_file()
{
    string fileName = "";
    cout << "Enter log File Name (.log): "
    cin >> fileName;
    return fileName;
}

