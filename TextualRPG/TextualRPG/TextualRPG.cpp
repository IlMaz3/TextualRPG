#include <iostream>
#include <string>
#include <map>
#include <array>
#include <vector>
#include <random>

/////////*STRUCTS*/////////

struct Item { //Items of the game
    std::string name;
    std::string description;
    std::string increasedStat;
    int statValue;
};

struct Enemy { //Enemies of the game
    int health;
    int strenght;
    int range;
};

bool playerAlive = true; //Always knows if the player is alive or not
int bossSpawn = 0;

/////////////*FUNCTIONS*/////////////

/////*PRINT STUFF ON SCREEN*/////

//Main Menu of the game
int MainMenu();

//Pause the game untill the player press C
void Pause();

//Print the player statistics
void PrintPlayerStats(const std::string &playerName, const std::map<std::string, std::array<int, 2>> &playerStats);

//Menu while fighting
void FightMenu(const std::string &playerName, std::map<std::string, std::array<int, 2>> &playerStats, std::vector<Item>& playerInventory, Enemy &enemy, bool bossFight);

//Shows various menus to the player
void ShowMenu(const std::string &playerName, std::map<std::string, std::array<int, 2>> &playerStats, std::vector<Item> &playerInventory, Enemy &enemy);

//Item inspection - Shows infos about the item
void PrintItemInspection(const std::string &playerName, const Item &itemToPrint);

//Shows player inventory
int PrintPlayerInventory(const std::string &playerName, const std::vector<Item> &playerInventory);

//Print enemy health
void PrintEnemyStats(const Enemy& enemy);

//Choose the action in player inventory
int PlayerInventoryAction(const std::vector<Item> &playerInventory, int choosedItem);

//Start of the game - Name and stats for the player
std::string BeginPlay(std::map<std::string, std::array<int, 2>> &playerStats);

//End of the game - DEAD
void GameOverLose(const std::string &playerName);

//Title screen
void TitlePrint();

//Small Title
void SmallTitlePrint();

/////*ROOMS FUNCTIONS*/////

//Generates a number, corrisponding a casual level
int LevelGeneration(std::string playerName);

//Loot Generation
void LootGeneration(const std::string& playerName, int playerLuck, std::vector<Item>& playerInventory, std::map<std::string, std::array<int, 2>>& playerStats, bool lootRoom, bool startGame);

//Body of an Empty Room
void EmptyRoom(const std::string &playerName);

//Body of a Loot Room
void LootRoom(const std::string &playerName, int playerLuck, std::vector<Item> &playerInventory, std::map<std::string, std::array<int, 2>>& playerStats);

//Body of a Trap Room
void TrapRoom(const std::string &playerName, int playerLuck, int playerElusion, std::map<std::string, std::array<int, 2>> &playerStats);

//Body of an Enemy Room
void EnemyRoom(const std::string &playerName, std::map<std::string, std::array<int, 2>> &playerStats, std::vector<Item>& playerInventory, int enemyLevel);

//Body of a Boss Room
bool BossRoom(const std::string& playerName, std::map<std::string, std::array<int, 2>>& playerStats, std::vector<Item>& playerInventory);

/////*DAMAGE AND HEALS*/////

//Damage applier system
void ApplyDamage(std::map<std::string, std::array<int, 2>> &playerStats, int damage, bool trap);

//Heal for the player
void Heals(const std::string& playerName, std::map<std::string, std::array<int, 2>>& playerStats, std::vector<Item>& playerInventory, int choosedItem, bool shield, int heal);

//Player and Enemy attack
void FightDamage(const std::string &playerName, std::map<std::string, std::array<int, 2>> &playerStats, std::vector<Item>& playerInventory, Enemy &enemy, bool bossFight);


/////*HOW TO PLAY*/////

//How to play Menu
void HowToPlay();

//How to play - Player
void HowToPlayer();

//How to play - Doors Room
void HowToDoorsRoom();

//How to play - Loot Room
void HowToLootRoom();

//How to play - Trap Room
void HowToTrapRoom();

//How to play - Enemy Room
void HowToEnemyRoom();

//How to play - Boss Room
void HowToBossRoom();

/////*OTHERS*/////

//Empty the player inventory
void EmptyInventory(std::vector<Item>& playerInventory);



/////////*MAIN*/////////

int main()
{
    srand(time(0)); // to full randomize the random numbers
    std::map<std::string, std::array<int, 2>> playerStats = { //Stats of the player
    {"Health:", {0, 0}}, //first number: maximum stat value
	{"Shield:", {0, 0}}, //second number: current stat value
    {"Speed:", {0, 0}},
    {"Strenght:", {0, 0}},
    {"Elusion:", {0, 0}},
    {"Luck:", {0, 0}}
    };
    std::vector <Item> playerInventory;
    std::string playerName = "";

    while (playerAlive) { //Repeats untill the player leaves
		playerName = ""; //Reset the player name if the player wants to continue after winning or losing
        while (playerName == "" && playerAlive) {
            system("cls");
            int mainMenuChoice = MainMenu(); //Choice by the menu
            switch (mainMenuChoice) {
            case 0:
                return 0; //Close the game
                break;
            case 1:
                playerName = BeginPlay(playerStats); //The game starts!
                break;
            case 2:
                system("cls");
                HowToPlay(); //How to play menu
                break;
            default:
                "Error, wrong input on Main_Menu_Choice";
                break;
            }
        }

        std::cout << "\n" << playerName << " finds an item in their backpack\n";
        LootGeneration(playerName, playerStats["Luck:"][1], playerInventory, playerStats, false, true);

        //Little pause after continuing, and screen cleaning
        Pause();
        system("cls");
        SmallTitlePrint();
        std::cout << "The adventure of " << playerName << " starts as they enters the mysterious cave near their village\n";

		bool win = false; //Variable to check if the player wins the game

        while (playerAlive) {
            if (win) {
                std::cout << playerName << " managed to defeat the dungeon boss, and returned to his village as the hero they are\n";
                std::cout << "The villagers will remember their name for generations to come as the hero that freed the village\n\n";
                std::cout << playerName << "... \nThe hero of the dungeon!\n\n";
                std::cout << "Congratulations, you won!\n";
                bossSpawn = 0; //Reset the room counter if the player continues to play after winning
                EmptyInventory(playerInventory);
                Pause();
                system("cls");
				break;
            }
            //Generating the rooms for the levels
            int generatedLevel = LevelGeneration(playerName);
            switch (generatedLevel) {
            case 1: {//Empty Room
                EmptyRoom(playerName);
                bossSpawn++;
                Pause();
                system("cls");
                SmallTitlePrint();
                break;
            }
            case 2: {//Loot Room
                LootRoom(playerName, playerStats["Luck:"][1], playerInventory, playerStats);
                bossSpawn++;
                Pause();
                system("cls");
                SmallTitlePrint();
                break;
            }
            case 3: {//Trap Room
                TrapRoom(playerName, playerStats["Luck:"][1], playerStats["Elusion:"][1], playerStats);
                bossSpawn++;
                Pause();
                system("cls");
                SmallTitlePrint();
                break;
            }
            case 11: {//Enemy level 1 Room
                EnemyRoom(playerName, playerStats, playerInventory, 1);
                bossSpawn++;
                break;
            }
            case 12: {//Enemy level 2 Room
                EnemyRoom(playerName, playerStats, playerInventory, 2);
                bossSpawn++;
                break;
            }
            case 13: {//Enemy level 3 Room
                EnemyRoom(playerName, playerStats, playerInventory, 3);
                bossSpawn++;
                break;
            }
			case 4: {//Boss Room
                win = BossRoom(playerName, playerStats, playerInventory);
                break;
            }
            
            default:
                std::cout << "Error, wrong input in Main - generatedLevel switch";
                break;
            }
        }

        if (!playerAlive) {
            GameOverLose(playerName);
			bossSpawn = 0; //Reset the room counter
            EmptyInventory(playerInventory);
            Pause();
            system("cls");
        }
    }
}

/////////*FUNCTIONS*/////////

/////*PRINT STUFF ON SCREEN*/////

//Main Menu of the game
int MainMenu() {
    int choice = -1;
	TitlePrint();
    std::cout << "<<===  Main Menu  ===>>"
        <<"\n  1) Play " 
        <<"\n  2) How to play" 
        <<"\n  3) Exit "
        << "\nPlease choose an option : ";

    //Loop to check the correct input choice - It repeats the choice untill the user write a valid input
    while (choice < 1 || choice > 3 || choice == -1) {
        std::cin >> choice;
        if (choice < 1 || choice > 3) {
            std::cout << "Not a valid input. Try again. \n Please choose an option: ";
        }
    }
    switch (choice) {
    case 1:
        playerAlive = true; //New player
        return 1; //The game starts
        break;
    case 2:
        return 2;
        break;
    case 3:
        return 0; //The game close
        break;
    default:
        std::cout << "How tf did you end up here?! Error in function MainMenu - wrong input on choice"; //Almost impossible, but ok
        break;
    }
}

//Pause the game untill the player press C
void Pause() {
    //The user must enter the letter C to continue the game
    std::cout << "Enter C to continue...";
    char temp = 'x';
    while (temp != 'C') {
        std::cin >> temp;
    }
}

//Print the player statistics
void PrintPlayerStats(const std::string &playerName, std::map<std::string, std::array<int, 2>> &playerStats) {
    std::cout << "\n" << playerName << "'s statistics: \n";
    //Player stat printer
    for (const auto stats : playerStats) {
        std::cout << stats.first << stats.second[1] << " / " << stats.second[0] << "\n";
    }
	std::cout << "\n";
}

//Print the enemy health
void PrintEnemyStats(const Enemy &enemy) {
    std::cout << "Enemy's health: " << enemy.health << "\n";
	std::cout << "Enemy's strenght: " << enemy.strenght << "\n\n";
}

//Menu while fighting
void FightMenu(const std::string& playerName, std::map<std::string, std::array<int, 2>>& playerStats, std::vector<Item>& playerInventory, Enemy& enemy, bool bossFight) {
    while (playerStats["Health:"][1] > 0 && enemy.health > 0) {
        //Menu for the fight
        int choice = -1;
        std::cout << "Time to fight! \n1) Stats \n2) Try to run away \n3) Attack \n4) Inventory \nPlease choose an option: ";
        //Loop to check the correct input choice - It repeats the choice untill the user write a valid input
        while (choice < 1 || choice > 4 || choice == -1) {
            std::cin >> choice;
            if (choice < 1 || choice > 4) {
                std::cout << "Not a valid input. Try again. \n Please choose an option: ";
            }
        }
        switch (choice) {
        case 1:
            PrintPlayerStats(playerName, playerStats);
            PrintEnemyStats(enemy);
            Pause();
            system("cls");
            SmallTitlePrint();
            break;
        case 2:
            if(bossFight) {
                std::cout << "The boss won't let me get away at this point - " << playerName << " thinks\n";
                Pause();
                system("cls");
                SmallTitlePrint();
                break;
			}
            std::cout << playerName << " tries to run away\n";
            if (((rand() % 100) + 1) <= playerStats["Elusion:"][1]) { //Success on running away
                std::cout << "They succeed, and manage to run into the next room\n";
                Pause();
                system("cls");
                SmallTitlePrint();
                return;
            }
            else { //Failing on running away
                int damage = rand() % ((playerStats["Health:"][1] + 1) / 2);
                damage = (damage - 5 + 1) + 5;
                ApplyDamage(playerStats, damage, false); //Apply damage
                std::cout << "They fail, and get hit by the enemy for " << damage << " points of damage\n";
				if (playerStats["Health:"][1] <= 0) playerAlive = false; //If the player dies, the game is over
            }
            Pause();
            system("cls");
            SmallTitlePrint();
            break;
        case 3:
            if (bossFight) {
                if (rand() % 5 + 1 == 5) FightDamage(playerName, playerStats, playerInventory, enemy, true); //Boss special attack
				else FightDamage(playerName, playerStats, playerInventory, enemy, false); //Boss normal attack
            }
            else {
                FightDamage(playerName, playerStats, playerInventory, enemy, false);
            }
            if (playerAlive) {
                Pause();
                system("cls");
                SmallTitlePrint();
            }
            break;
        case 4: {
            ShowMenu(playerName, playerStats, playerInventory, enemy);
            break;
        }
        }
    }
}

//Shows various menus to the player
void ShowMenu(const std::string &playerName, std::map<std::string, std::array<int, 2>> &playerStats, std::vector<Item> &playerInventory, Enemy &enemy) {
        int choosedItem = PrintPlayerInventory(playerName, playerInventory);
        if (choosedItem != 0) {
            choosedItem--; //To match the index of the vector
            if (playerInventory.size() != 0) {
                //When there is a correct input
                int choice = -1;
                while (choice != 3) { //Untill the player choose the option 3
                    choice = PlayerInventoryAction(playerInventory, choosedItem); //The player choose the action - 1)Use 2)Inspect 3)Go back
                    switch (choice) {
                    case 1: //Heals health or shield, if the item allows it
                        if (playerInventory[choosedItem].name == "Health Potion" || playerInventory[choosedItem].name == "Shield Potion") {
                            if (playerInventory[choosedItem].name == "Health Potion") Heals(playerName, playerStats, playerInventory, choosedItem, false, playerInventory[choosedItem].statValue);
                            else Heals(playerName, playerStats, playerInventory, choosedItem, true, playerInventory[choosedItem].statValue);
                        }
                        else {
                            std::cout << "Cannot use this item\n";
                            Pause();
                        }
                        choice = 3;
                        break;
                    case 2:
                        PrintItemInspection(playerName, playerInventory[choosedItem]);
                        break;
                    }
                }
                system("cls");
                SmallTitlePrint();
            }
        }
        else{
            std::cout << "Exiting the inventory\n\n";
            Pause(); //If the player closes the menu
            system("cls");
            SmallTitlePrint();
        }
    }

//Item inspection - Shows infos about the item
void PrintItemInspection(const std::string &playerName, const Item &itemToPrint) {
    std::cout << playerName << " take a closer look to the item\n\n";
    std::cout << itemToPrint.name << "\n" << itemToPrint.description << "\n"
        << itemToPrint.increasedStat << itemToPrint.statValue << "\n";
    Pause();
	system("cls");
    SmallTitlePrint();
}

//Shows player inventory
int PrintPlayerInventory(const std::string& playerName, const std::vector<Item>& playerInventory) {
    std::cout << playerName << "'s inventory: \n"; //Print player's inventory
    for (int i = 0; i < playerInventory.size(); i++) {
        std::cout << i + 1 << "- " << playerInventory[i].name << "\n";
    }
    std::cout << "Choose an items to see options or go back by typing 0: ";
    int choosedItem = -1;
    //Loop to check the correct input choice - It repeats the choice untill the user write a valid input
    if (choosedItem != 0) {
        while (choosedItem < 0 || choosedItem > playerInventory.size() + 1 || choosedItem == -1) {
            std::cin >> choosedItem;
            if (choosedItem < 0 || choosedItem > playerInventory.size() + 1) {
                std::cout << "Not a valid input. Try again. \n Please choose an option: ";
            }
        }
    }
    else choosedItem = 0;
    return choosedItem;
}

//Choose the action in player inventory
int PlayerInventoryAction(const std::vector<Item> &playerInventory, int choosedItem) {
    int choice = -1;
    std::cout << "\nWhat do you want to do with the " << playerInventory[choosedItem].name
        << "? \n1)Use \n2)Inspect \n3)Go back \nChoose an option:"; //Choose what to do with the selected item
    //Loop to check the correct input choice - It repeats the choice untill the user write a valid input
    while (choice < 1 || choice > 3 || choice == -1) {
        std::cin >> choice;
        if (choice < 1 || choice > 3) {
            std::cout << "Not a valid input. Try again. \n Please choose an option: ";
        }
    }
    return choice;
}

//Start of the game - Name and stats for the player
std::string BeginPlay(std::map<std::string, std::array<int, 2>> &playerStats) {
    system("cls"); //Let's clear the console screen!

    //Title
	SmallTitlePrint();

    std::string playerName;
    //Loop to choose a name for the player, between 4 and 20 letters
    std::cout << "All the stories of great heroes start with his name. \nChoose yours (must be between 4 and 20 letters): ";
    while (playerName.length() < 4 || playerName.length() > 20 || playerName == "") {
        std::cin >> playerName;
        if (playerName.length() < 4 || playerName.length() > 20 || playerName == "") {
            std::cout << "Not a valid input. Try again. \n Choose yours (must be between 4 and 20 letters): ";
        }
    }

    //Let's assign and print the player stats

    //Player stat generator
    for (const auto stats : playerStats) {
        int randNum = rand() % (15 - 5 + 1) + 5;

        if (stats.first == "Health:") {
            playerStats[stats.first][0] = randNum * 2;
            playerStats[stats.first][1] = randNum * 2;
        }
        else if (stats.first == "Shield:") {
            playerStats[stats.first][0] = (int)round(randNum * 1.5);
            playerStats[stats.first][1] = (int)round(randNum * 1.5);
        }
        else {
            playerStats[stats.first][0] = 100;
            playerStats[stats.first][1] = randNum;
        }
    }

    PrintPlayerStats(playerName, playerStats);

    //Player stat luck calculator
    //Stats total goes from 112 to 37, the middle is 75
    //So weak is from 37 to 59 - normal is from 60 to 88 - strong is from 89 to 112
    int statsLuck = 0;
    std::string luckResult = "";
    for (auto stats : playerStats) {
        statsLuck += stats.second[1];
    }
    if (statsLuck <= 59) luckResult = "weak";
    if (statsLuck > 59 && statsLuck <= 88) luckResult = "normal";
    if (statsLuck > 89) luckResult = "strong";

    std::cout << playerName << " is a pretty " << luckResult << " human, and this is the start of his own adventure\n";

    return playerName;
}

//End of the game - DEAD
void GameOverLose(const std::string &playerName) {
    std::cout << playerName << " could not manage to survive the dungeon, and rested in peace there for the rest of the history\n";
    std::cout << "GAME OVER\n\n";
    playerAlive = true;
}

//Title screen
void TitlePrint() {
    std::cout <<
        "888    888                          d8b            8888888b.\n" <<                                                        
        "888    888                          88P            888  \"Y88b\n" <<                                                       
        "888    888                          8P             888    888\n" <<
        "8888888888  .d88b.  888d888  .d88b. \"  .d8888b     888    888 888  888 88888b.   .d88b.   .d88b.   .d88b.  88888b.\n" << 
        "888    888 d8P  Y8b 888P\"   d88\"\"88b   88K         888    888 888  888 888\"88b  d88P\"88b d8P  Y8b d88\"\"88b 888\"88b\n" <<
        "888    888 88888888 888     888  888   \"Y8888b.    888    888 888  888 888  888 888  888 88888888 888  888 888  888\n" << 
        "888    888 Y8b.     888     Y88..88P        X88    888  .d88P Y88b 888 888  888 Y88b 888 Y8b.     Y88..88P 888  888\n" <<
        "888    888 \"Y8888   888      \"Y88P\"     88888P'    8888888P\"  \"Y88888  888  888  \"Y88888 \"Y8888    \"Y88P\"  888  888\n" << 
        "                                                                                     888\n" <<
        "                                                                                Y8b d88P\n" <<
        "                                                                                 \"Y88P\"\n";
}

//Small title
void SmallTitlePrint() {
    std::cout <<
        " _   _                _      ______                                    \n"
        "| | | |              ( )     |  _  \\\n"
        "| |_| | ___ _ __ ___ |/ ___  | | | |_   _ _ __   __ _  ___  ___  _ __\n"
        "|  _  |/ _ \\ '__/ _ \\  / __| | | | | | | | '_ \\ / _` |/ _ \\/ _ \\| '_ \\ \n"
        "| | | |  __/ | | (_) | \\__ \\ | |/ /| |_| | | | | (_| |  __/ (_) | | | |\n"
        "\\_| |_/\\___|_|  \\___/  |___/ |___/ \\__,__|_| |_|\\__, |\\___|\\___/|_| |_|\n"
        "                                                 __/ |\n"
        "                                                |___/ \n\n";
}


/*ROOMS FUNCTIONS*/

//Generates a number, corrisponding a casual level
int LevelGeneration(std::string playerName) {
    int levelGenerator = 3;
	if (bossSpawn >= 10) levelGenerator = 5; //After 10 rooms, the boss can spawn
    const int randomLevel = rand() % levelGenerator; //Numbers from 0 to 3
    switch (randomLevel) {
    case 0: {//Empty room
        std::string lightCondition = "dark";
        if (rand() % 2 > 0) lightCondition = "lighted";
        std::cout << playerName << " enters a " << lightCondition << " empty room with 3 doors \nA choice is to be done\n";
        return 1;
        break;
    }
    case 1: {//Enemy room
        int difficulty = 2;
		if (bossSpawn >= 5) difficulty = 3; //After 5 rooms, the enemy can be level 3
		int enemyLevel = (rand() % difficulty) + 1; //Generates a room from 1 to 2, if 5 rooms are passed, it reaches 3
        std::cout << "\nAs " << playerName << " enters the room, a hostile figure approaches him\n";
        //return the enemy level +10 to avoid problems with other cases
        return enemyLevel + 10;
        break;
    }
    case 2: //Loot room
        std::cout << playerName << " enters a room that looks empty\n";
        return 2;
        break;
    case 3: //Trap room
        std::cout << playerName << " enters a room that looks empty\n";
        return 3;
        break;
	case 4: //Boss room
        std::cout << playerName << " enters a big dark room...\n\n";
        std::cout << "Suddently a set of torchs light up all around the room\n Revealing a big figure in front of him\n";
        std::cout << "The dungeon boss appears!\n\n";
        return 4;
        break;
    default: //Error case
        std::cout << "Error, wrong result in LevelGeneration switch";
        break;
    }
}

//Loot Generation
void LootGeneration(const std::string& playerName, int playerLuck, std::vector<Item>& playerInventory, std::map<std::string, std::array<int, 2>>& playerStats, bool lootRoom, bool startGame) {
    int i = 1; //Variable for the for loop
    if (lootRoom) {
        const int doubleFind = rand() % 100 + 1; //Numbers from 1 to 100
        if (doubleFind <= playerLuck) { //Manage what the player finds
            i = 2;
            std::cout << playerName << " finds 2 items!\n";
        }
        else std::cout << playerName << " finds 1 item\n";
    }

    for (i; i > 0; i--) {
        int playerLoot = rand() % 8; //Numbers from 0 to 7
		if (startGame) playerLoot = 0; //If it's the start of the game, the player finds only a health potion to help them in the first fight
        switch (playerLoot) {
        case 0: {
            Item healthPotion = {
                "Health Potion",
                "A glass bottle with a shiny red liquide inside. It glows if shaken and has a good smell.",
                "Health: +",
                20
            };
            playerInventory.push_back(healthPotion);
            std::cout << "It's a health potion!\n" << playerName << " health can be restored by "
                << healthPotion.statValue << " points\n";
            break;
        }
        case 1: {
            int shieldValue = rand() % (15 - 1 + 1) + 1; //+ round(playerLuck / 10)
            Item shieldArtifact = {
                "Shield Artifact",
                "It's a little statue of a turtle made of wood, with some runes carved on it that you can not understand",
                "Shield: +",
                shieldValue
            };
            playerInventory.push_back(shieldArtifact);
            playerStats["Shield:"][0] += shieldArtifact.statValue;
            playerStats["Shield:"][1] += shieldArtifact.statValue;
            std::cout << "It's a shield artifact!\n " << playerName << " shield has been incresead by "
                << shieldArtifact.statValue << " points\n";
            break;
        }
        case 2: {
            int speedValue = rand() % (15 - 1 + 1) + 1; //+ playerLuck / 10
            Item speedArtifact = {
                "Speed Artifact",
                "It's a little boot made of stone with some runes carved on it that you can not understand",
                "Speed: +",
                speedValue
            };
            playerInventory.push_back(speedArtifact);
            playerStats["Speed:"][1] += speedArtifact.statValue;
            std::cout << "It's a speed artifact!\n" << playerName << " speed has been incresead by "
                << speedArtifact.statValue << " points\n";
            break;
        }
        case 3: {
            int strenghtValue = rand() % (15 - 1 + 1) + 1; //+ playerLuck / 10;
            Item strenghtArtifact = {
                "Strenght Artifact",
                "It's a pin made of wood with a sword carved on it, there is also a rune that you can not understand",
                "Strenght: +",
                strenghtValue
            };
            playerInventory.push_back(strenghtArtifact);
            playerStats["Strenght:"][1] += strenghtArtifact.statValue;
            std::cout << "It's a strenght artifact!\n" << playerName << " strenght has been incresead by "
                << strenghtArtifact.statValue << " points\n";
            break;
        }
        case 4: {
            int elusionValue = rand() % (15 - 1 + 1) + 1; //+ playerLuck / 10
            Item elusionArtifact = {
                "Elusion Artifact",
                "It's a ring made of silver, with runes all around it that you can not understand",
                "Elusion: +",
                elusionValue
            };
            playerInventory.push_back(elusionArtifact);
            playerStats["Elusion:"][1] += elusionArtifact.statValue;
            std::cout << "It's an elusion artifact!\n" << playerName << " elusion has been incresead by "
                << elusionArtifact.statValue << " points\n";
            break;
        }
        case 5: {
            int luckValue = rand() % (15 - 1 + 1) + 1; //+ playerLuck / 10
            Item luckArtifact = {
                "Luck Artifact",
                "It's a little four leaf clover made of gold, with one rune carved on each leaf, you can not understand them",
                "Luck: +",
                luckValue
            };
            playerInventory.push_back(luckArtifact);
            playerStats["Luck:"][1] += luckArtifact.statValue;
            std::cout << "It's a luck artifact!\n" << playerName << " luck has been incresead by "
                << luckArtifact.statValue << " points\n";
            break;
        }
        case 6: {
            int healthValue = rand() % (15 - 1 + 1) + 1; //+ playerLuck / 10) * 2;
            Item healthArtifact = {
                "Health Artifact",
                "It's a hearth made of wood with a rune carved in it that you can not understand",
                "Health: +",
                healthValue
            };
            playerInventory.push_back(healthArtifact);
            playerStats["Health:"][0] += healthArtifact.statValue;
            playerStats["Health:"][1] += healthArtifact.statValue;
            std::cout << "It's an health artifact!\n" << playerName << " health has been incresead by "
                << healthArtifact.statValue << " points\n";
            break;
        }
        case 7: {
            Item shieldPotion = {
                "Shield Potion",
                "A glass bottle with a oily blue liquide inside. It smells like iron.",
                "Shield: +",
                15
            };
            playerInventory.push_back(shieldPotion);
            std::cout << "It's a shield potion!\n" << playerName << " shield can be restored by "
                << shieldPotion.statValue << " points\n";
            break;
        }
        default:
            std::cout << "Error, wrong result in LootRoom switch";
            break;
        }
    }
}

//Body of an Empty Room
void EmptyRoom(const std::string &playerName) {
    int playerChoice = 0;
    //Ask where to go
    std::cout << "Where should I go? " << playerName << " asked himself" <<
        "\n1)Front door \n2)Right Door \n3)Left Door \n" <<
        "Choose where " << playerName << " will go: ";
    while (playerChoice == 0 || playerChoice < 1 || playerChoice > 3) { //Be sure the input is right
        std::cin >> playerChoice;
        if (playerChoice == 0 || playerChoice < 1 || playerChoice > 3) {
            std::cout << "Not a valid input. Try again.\n Choose where " << playerName << " will go: ";
        }
    }
}

//Body of a Loot Room
void LootRoom(const std::string &playerName, int playerLuck, std::vector<Item> &playerInventory, std::map<std::string, std::array<int, 2>>& playerStats) {
    int playerChoice = 0;
    //Ask if a check is needed
    std::cout << "Should I check better? " << playerName << " asked himself\n" <<
        "\n1)Yes \n2)No \nChoose what " << playerName << " should do: ";
    while (playerChoice == 0 || playerChoice < 1 || playerChoice > 2) { //Be sure the input is right
        std::cin >> playerChoice;
        if (playerChoice == 0 || playerChoice < 1 || playerChoice > 2) {
            std::cout << "Not a valid input. Try again. \nChoose what " << playerName << " should do: ";
        }
    }
    //Manage the answer
    if (playerChoice == 1) { //If the player chooses yes
		LootGeneration(playerName, playerLuck, playerInventory, playerStats, true, false);
    }
    else { //If the player chooses no
        std::cout << playerName << " decides it's better to continue his adventure, and move on\n";
    }
}

//Body of a Trap Room
void TrapRoom(const std::string &playerName, int playerLuck, int playerElusion, std::map<std::string, std::array<int, 2>> &playerStats) {
    int playerChoice = 0;
    //Ask if a check is needed
    std::cout << "Should I check better? " << playerName << " asked himself\n" <<
        "\n1)Yes \n2)No \nChoose what " << playerName << " should do: ";
    while (playerChoice == 0 || playerChoice < 1 || playerChoice > 2) { //Be sure the input is right
        std::cin >> playerChoice;
        if (playerChoice == 0 || playerChoice < 1 || playerChoice > 2) {
            std::cout << "Not a valid input. Try again. \nChoose what " << playerName << " should do: ";
        }
    }
    //Manage the answer
    if (playerChoice == 1) { //If the player chooses yes
        int trapDifficulty = rand() % (210 - 1 + 1) + 1; //Numbers from 1 to 210
        std::cout << playerName << " hears a strange sound while walking in the room. \nA trap activates!\n";
        if (trapDifficulty > playerLuck + playerElusion) { //If the trap is activated
            int trapDamage = rand() % playerStats["Health:"][0] / 2 + 1; //Damage from 1 to half of the player's health
            ApplyDamage(playerStats, trapDamage, true);
            std::cout << playerName << " can't manage to escape it. \nThe trap damages " << playerName << " with "
                << trapDamage << " points of damage. Ouch!\n";
			if (playerStats["Health:"][1] <= 0) playerAlive = false; //If the player dies, the game is over
        }
        else { //If the player dodges the trap
            std::cout << "But the instinct of " << playerName
                << " also activates, and they manage to dodge it! Great reflexes!\n";
        }
    }
    else { //If the player chooses no
        std::cout << playerName << " decides it's better to continue his adventure, and move on\n";
    }
}

//Body of an Enemy Room
void EnemyRoom(const std::string &playerName, std::map<std::string, std::array<int, 2>> &playerStats, std::vector<Item>& playerInventory, int enemyLevel) {
    //Set the string for the enemy difficulty
    std::string enemyDifficulty = "A weak";
    if (enemyLevel == 2) enemyDifficulty = "An average";
    else if (enemyLevel == 3) enemyDifficulty = "A strong";

    //Create the enemy
    switch (enemyLevel) {
    case 1: {
        Enemy level1Enemy{

            rand() % (35 - 15 + 1) + 15,        //Health calculation from 15 to 35
            rand() % (8 - 3 + 1) + 3,           //Strenght calculation from 3 to 8
            rand() % (2 - 1 + 1) + 1            //Range calculation from 1 to 2
        };
        std::cout << enemyDifficulty << " enemy appears\n";
        FightMenu(playerName, playerStats, playerInventory, level1Enemy, false);
        break;
    }
    case 2: {
        Enemy level2Enemy{
            rand() % (40 - 20 + 1) + 20,        //Health calculation from 20 to 40
            rand() % (12 - 6 + 1) + 6,          //Strenght calculation from 6 to 12
            rand() % (3 - 1 + 1) + 1            //Range calculation from 1 to 3
        };
        std::cout << enemyDifficulty << " enemy appears\n";
        FightMenu(playerName, playerStats, playerInventory, level2Enemy, false);
        break;
    }
    case 3: {
        Enemy level3Enemy{
            rand() % (50 - 25 + 1) + 25,        //Health calculation from 25 to 50
            rand() % (15 - 9 + 1) + 9,          //Strenght calculation from 9 to 15
            rand() % (3 - 2 + 1) + 2            //Range calculation from 2 to 3
        };
        std::cout << enemyDifficulty << " enemy appears\n";
        FightMenu(playerName, playerStats, playerInventory, level3Enemy, false);
        break;
    }
    }
}

//Body of a Boss Room
bool BossRoom(const std::string& playerName, std::map<std::string, std::array<int, 2>>& playerStats, std::vector<Item>& playerInventory) {
    Enemy boss{
        rand() % (60 - 40 + 1) + 40,        //Health calculation from 40 to 60
        rand() % (13 - 7 + 1) + 7,           //Strenght calculation from 7 to 13
        rand() % (2 - 1 + 1) + 1            //Range calculation from 1 to 2
    };
    FightMenu(playerName, playerStats, playerInventory, boss, true);
    if (boss.health <= 0) {
        return true;
    }
}



/*DAMAGE AND HEALS*/

//Damage applier system
void ApplyDamage(std::map<std::string, std::array<int, 2>> &playerStats, int damage, bool trap) {
    if (trap) { //If the damage is from a trap
        damage = rand() % playerStats["Health:"][0] / 2 + 1; //Damage from 1 to half of the player's health
    }

    if (playerStats["Shield:"][1] > 0) { //If the player has shield left
        double percent = damage * 80;    //Get 80% of the damage
        int shieldDamage = percent / 100;
        int healthDamage = damage - shieldDamage;       //Get the remaining damage (20%)
        while (playerStats["Shield:"][1] > 0 && shieldDamage > 0) {
            playerStats["Shield:"][1] -= 1; //Remove 1 point of shield untill the shield reach 0
            shieldDamage -= 1;  //Or the damage reches 0
        }
        //If there is damage left, apply it to the health 
        if (shieldDamage > 0) playerStats["Health:"][1] -= shieldDamage;
        playerStats["Health:"][1] -= healthDamage; //Then apply the health damage
    }
    else { //If the player has no shield
        playerStats["Health:"][1] -= damage;
    }
}

//Heal for the player
void Heals(const std::string &  playerName, std::map<std::string, std::array<int, 2>> &playerStats, std::vector<Item>& playerInventory, int choosedItem, bool shield, int heal) {
    if (shield) { //If I have to heal the shield stat
        while (playerStats["Shield:"][1] < playerStats["Shield:"][0]) { //While the max value is less or equal than the actual shield
            playerStats["Shield:"][1]++;
            heal--;
        }
        std::cout << playerName << " feels that their armor is repaired\n";
        playerInventory.erase(playerInventory.begin() + choosedItem);
    }
    else {
        while (heal > 0 && playerStats["Health:"][1] < playerStats["Health:"][0]) { //While the max value is less or equal than the actual health
            playerStats["Health:"][1]++;
            heal--;
        }
        std::cout << playerName << " feels that their wounds are healed\n";
        playerInventory.erase(playerInventory.begin() + choosedItem);
    }
}

//Player and Enemy attack
void FightDamage(const std::string &playerName, std::map<std::string, std::array<int, 2>> &playerStats, std::vector<Item>& playerInventory, Enemy &enemy, bool bossFight) {
    if (bossFight) {
        std::cout << "The boss dashes away from " << playerName << " and starts charging an attack from far away\n";
        std::cout << playerName << " tries to reach him to stop it\n";

        if (rand() % 100 + 1 > playerStats["Speed:"][1]) { //The boss manage to attack
            ApplyDamage(playerStats, 15, false);
            std::cout << "The boss manages to charge the attack, and hits " << playerName << " hard\n";
			std::cout << "Causing 15 points of damage to him\n";
			if (playerStats["Health:"][1] <= 0) playerAlive = false; //If the player dies
        }
        else { //The player stops the attack and counterattacks
            enemy.health -= playerStats["Strenght:"][1] + 5;
            std::cout << playerName << " manages to reach the boss and hits him with a strong attack\n";
			std::cout << "Causing " << playerStats["Strenght:"][1] + 5 << " points of damage to him\n";
        }
        return;
    }
    
    //The player attacks the enemy
    enemy.health -= playerStats["Strenght:"][1];
    std::cout << playerName << " attacks the enemy, doing " << playerStats["Strenght:"][1] << " points of damage\n";
    if (enemy.health > 0) {
        ApplyDamage(playerStats, enemy.strenght, false);
        std::cout << "The enemy fights back, doing " << enemy.strenght << " points of damage to " << playerName << "\n";
    }
    if (playerStats["Health:"][1] <= 0) playerAlive = false;
    if (enemy.health <= 0) {
        std::cout << "\nThe enemy has been defeated by " << playerName << "'s sword!\n";
        std::cout << "The monster dropped an item!\n";
		LootGeneration(playerName, playerStats["Luck:"][1], playerInventory, playerStats, false, false); //The loot from the enemy
    }
}



/*HOW TO PLAY*/

//How to play Menu
void HowToPlay() {
    int choice = -1;
    while (choice != 0) {
    choice = -1;
	system("cls");
    std::cout << "How to play \n 1) Player \n 2) Doors room \n 3) Loot Room \n 4) Trap Room \n 5) Enemy Room \n 6) Boss Room \nPlease choose an option (return with 0): ";
    //Loop to check the correct input choice - It repeats the choice untill the user write a valid input
    while (choice < 0 || choice > 6 || choice == -1) {
        std::cin >> choice;
        if (choice < 0 || choice > 6) {
            std::cout << "Not a valid input. Try again. \n Please choose an option: ";
        }
    }
            switch (choice) {
            case 1:
                HowToPlayer();
                break;
            case 2:
                HowToDoorsRoom();
                break;
            case 3:
                HowToLootRoom();
                break;
            case 4:
				HowToTrapRoom();
                break;
            case 5:
				HowToEnemyRoom();
                break;
            case 6:
				HowToBossRoom();
                break;
            }
        }
}

//How to play - Player
void HowToPlayer() {
	int choice = -1;
    std::cout << "The player is the brave villager of this story, who enters a dungeon with the objective to defeat his boss and free the village from his domain";
	std::cout << "The player has 6 statistics: \n-Health, the life of the player \n-Shield, the protection the player posses \n" <<
        "-Speed, how fast is the movement of the player \n -Strenght, how strong are the player attacks \n" <<
        "-Elusion, the ability for the player to dodge dangers \n -Luck, how lucky the player is to find things\n\n ";
	std::cout << "The player does not have a limit to the health and shield, but it does have a maximum value for the speed, strenght, elusion and luck, that is 100\n";
    std::cout << "The player has casual statistics when they enters the dungeon, and can increase them by finding artifacts\n\n";
	std::cout << "The player posses a backpack, where they can put any item the find, they can inspect the items they found, or use them if possible\n\n";
	std::cout << "Exit by typing 0: ";
    while (choice != 0) {
        std::cin >> choice;
        if (choice != 0) {
            std::cout << "Not a valid input. Try again. \n Exit by thping 0: ";
        }
    }
    if (choice == 0) return;
}

//How to play - Doors Room
void HowToDoorsRoom() {
    int choice = -1;
    std::cout << "This room of the dungeon is not very exciting, it presents three doors that the player has to choose which one to enter\n";
    std::cout << "There is no way to know what's behind a door, and no way to go back after choosing one\n";
	std::cout << "So a wise choice is needed from the player, because it will forge the destiny of the adventure\n\n";
    std::cout << "Exit by typing 0: ";
    while (choice != 0) {
        std::cin >> choice;
        if (choice != 0) {
            std::cout << "Not a valid input. Try again. \n Exit by thping 0: ";
        }
    }
    if (choice == 0) return;
}

//How to play - Loot Room
void HowToLootRoom() {
    int choice = -1;
    std::cout << "This room of the dungeon contains a treasure somewhere in it, but the player can't be sure of that when they enter\n";
	std::cout << "So a choice is to be done, will the player explore the room further? Or will they continue their adventure in the dungeon?\n";
	std::cout << "If the player decides to explore the room, they can find one item, but if they are particularly lucky, they can even found two of them!\n\n";
    std::cout << "The items can be artifacts, who increase one of the player's statistics permanently. "
        << "Or the items can be potions, who heals the player for a certain amount of health or shield\n\n";
	std::cout << "Exit by typing 0: ";
    while (choice != 0) {
        std::cin >> choice;
        if (choice != 0) {
            std::cout << "Not a valid input. Try again. \n Exit by thping 0: ";
        }
    }
    if (choice == 0) return;
}

//How to play - Trap Room
void HowToTrapRoom() {
    int choice = -1;
    std::cout << "This room of the dungeon contains a trap somewhere in it, but the player can't be sure of that when they enter\n";
    std::cout << "So a choice is to be done, will the player explore the room further? Or will they continue their adventure in the dungeon?\n";
    std::cout << "If the player decides to explore the room, they can fall into a trap!\n\n";
    std::cout << "The trap will damage the player for a certain amount, but if they have great reflexes (and maybe a bit of luck) "
        << "they can dodge it and be completly fine\n\n";
	std::cout << "Exit by typing 0: ";
    while (choice != 0) {
        std::cin >> choice;
        if (choice != 0) {
            std::cout << "Not a valid input. Try again. \n Exit by thping 0: ";
        }
    }
    if (choice == 0) return;
}

//How to play - Enemy Room
void HowToEnemyRoom() {
    int choice = -1;
	std::cout << "This room of the dungeon is dangerous, because it contains an enemy that will try everything to stop the player from reaching the boss\n";
    std::cout << "In this room, a fight is to be done, and the player can choose to:\n Attack the enemy directly (with a very likely response)\n "
        << " Try to escape from the fight by running to the next room (but the enemy would probably not like that if the player fails)\n"
        << " Or they can just use an item from their backpack\n\n";
    std::cout << "The enemy can have different difficulties, that will challenge the player strenght and endurance\n";
	std::cout << "On death, an enemy will always drop an item, either a potion or an artifact, that the player will store in their backpack\n\n";
	std::cout << "Exit by typing 0: ";
    while (choice != 0) {
        std::cin >> choice;
        if (choice != 0) {
            std::cout << "Not a valid input. Try again. \n Exit by thping 0: ";
        }
    }
    if (choice == 0) return;
}

//How to play - Boss Room
void HowToBossRoom() {
	int choice = -1;
    std::cout << "This room of the dungeon will be the player's last one. But to reach it they will have to travel a bit the dungeon first\n";
    std::cout << "In this room, the player will encounter the boss of the dungeon, the reason why they decided to be there\n\n";
	std::cout << "When the boss sees the player, the fight starts!\n" << "In this fight there is no option to run away\n";
	std::cout << "The boss is a strong version of an enemy, with a special charged attack that will hit the player with great damage if they are not fast enough to stop him\n\n";
	std::cout << "Will the player manage to defeat the boss? Or will they remain in the dungeon forever? \n Only one way to find out\n\n";
	std::cout << "Exit by typing 0: ";
    while (choice != 0) {
        std::cin >> choice;
        if (choice != 0) {
            std::cout << "Not a valid input. Try again. \n Exit by thping 0: ";
        }
    }
    if (choice == 0) return;
}



/*OTHERS*/

//Empty the player inventory
void EmptyInventory(std::vector<Item> &playerInventory) {
    playerInventory.clear();
}