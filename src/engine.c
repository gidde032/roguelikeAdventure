#include <rogue.h>

bool cursesSetup(void) { //sets up ncurses library, displays error msg if computer does not support color
    initscr();
    noecho();
    curs_set(0);
    if (has_colors()) { //entity and tile color initialization
        start_color();
        init_pair(VISIBLE_COLOR, COLOR_WHITE, COLOR_BLACK);
        init_pair(SEEN_COLOR, COLOR_BLUE, COLOR_BLACK);
        init_pair(PLAYER_COLOR, COLOR_YELLOW, COLOR_BLACK);
        init_pair(GOB_COLOR, COLOR_GREEN, COLOR_BLACK);
        init_pair(SPID_COLOR, COLOR_MAGENTA + 8, COLOR_BLACK);
        init_pair(TROLL_COLOR, COLOR_CYAN, COLOR_BLACK);
        init_pair(HEALS_COLOR, COLOR_RED, COLOR_BLACK);
        init_pair(ITEM_COLOR, 57, COLOR_BLACK);
        return true;
    }
    else {
        mvprintw(20, 50, "SYSTEM DOES NOT SUPPORT COLOR");
        getch();
        return false;
    }
}

void gameLoop(void) { //gameplay loop, ends game when user presses specified character
    int ch;
    makeFOV(player);
    drawEverything();

    while(ch = getch()) { //input handling core of game
        if (ch == 'q') {break;}
        if (currentLevel > maxLevels) {break;}
        handleInput(ch);
        drawEverything();
        gobTurn();
        spidTurn();
        trollTurn();
        if (player->hP <= 0) {break;} //kills player if mons jugged them
        drawEverything();
    }
}

void closeGame(void) { //exits game and frees leftover memory
    endwin();
    free(player);
    for (int i = 0; i < monCount; i++) {
        if (mons[i]) {
            free(mons[i]);
        }
    }
    free(mons);
    for (int i = 0; i < healCount; i++) {
        if (heals[i]) {
            free(heals[i]);
        }
    }
    free(heals);
    for (int i = 0; i < itemCount; i++) {
        if (items[i]) {
            free(items[i]);
        }
    free(items);
    }
}

void levelUp(void) {
    currentLevel++;
    if (currentLevel > maxLevels) {
        showVictoryScreen();
    }
    for (int i = 0; i < monCount; i++) {
        mons[i] = NULL;
    }
    for (int i = 0; i < healCount; i++) {
        heals[i] = NULL;
    }
    for (int i = 0; i < itemCount; i++) {
        items[i] = NULL;
    }
    
    monCount = healCount = itemCount = 0;
    clearFOV(player);

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map[y][x].ch = '#';
            map[y][x].walkable = false;
            map[y][x].transparent = false;
            map[y][x].visible = false;
            map[y][x].seen = false;
        }
    }

    Position start_pos = setupMap();
    player->pos = start_pos;
    makeFOV(player);
}

void showTitleScreen(void) {
    clear();
    attron(A_BOLD);
    
    // Game Title
    attron(COLOR_PAIR(8));
    mvprintw(1, MAP_WIDTH/2 - 40, "  _____ _  _ ___     _   _____   _____ _  _ _____ _   _ ___ ___ ___    ___  ___ ");
    mvprintw(2, MAP_WIDTH/2 - 40, " |_   _| || | __|   /_\\ |   \\ \\ / / __| \\| |_   _| | | | -_| __/ __|  / _ \\| __|");
    mvprintw(3, MAP_WIDTH/2 - 40, "   | | | __ | _|   / _ \\| |) \\ V /| _|| .` | | | | |_| |  \\| _|\\__ \\ | (_) | _| ");
    mvprintw(4, MAP_WIDTH/2 - 40, "   |_| |_||_|___| /_/ \\_|___/ \\_/ |___|_|\\_| |_|  \\___/|_\\_\\___|___/  \\___/|_|  ");

    mvprintw(5, MAP_WIDTH/2 - 47, "         ,---._                                              ____                        ,--. ");
    mvprintw(6, MAP_WIDTH/2 - 47, "       .-- -.' \\               ,----..    ,----..          ,'  , `.  ,---,             ,--.'| ");
    mvprintw(7, MAP_WIDTH/2 - 47, "       |    |   :        ,--, /   /   \\  /   /   \\      ,-+-,.' _ | '  .' \\        ,--,:  : | ");
    mvprintw(8, MAP_WIDTH/2 - 47, "       :    ;   |      ,'_ /||   :     :|   :     :  ,-+-. ;   , ||/  ;    '.   ,`--.'`|  ' : ");
    mvprintw(9, MAP_WIDTH/2 - 47, "       :        | .--. |  | :.   |  ;. /.   |  ;. / ,--.'|'   |  ;:  :       \\  |   :  :  | | ");
    mvprintw(10, MAP_WIDTH/2 - 47, "       |    :   ,'_ /| :  . |.   ; /--` .   ; /--` |   |  ,', |  ':  |   /\\   \\ :   |   \\ | : ");
    mvprintw(11, MAP_WIDTH/2 - 47, "       :        |  ' | |  . .;   | ;  __;   | ;  __|   | /  | |  ||  :  ' ;.   :|   : '  '; | ");
    mvprintw(12, MAP_WIDTH/2 - 47, "       |    ;   |  | ' |  | ||   : |.' .|   : |.' .'   | :  | :  ||  |  ;/  \\   '   ' ;.    ; ");
    mvprintw(13, MAP_WIDTH/2 - 47, "   ___ l        :  | | :  ' ;.   | '_.' .   | '_.' ;   . |  ; |--''  :  | \\  \\ ,|   | | \\   | ");
    mvprintw(14, MAP_WIDTH/2 - 47, " /    /\\    J   |  ; ' |  | ''   ; : \\  '   ; : \\  |   : |  | ,   |  |  '  '--' '   : |  ; .' ");
    mvprintw(15, MAP_WIDTH/2 - 47, "/  ../  `..-    :  | : ;  ; |'   | '/  .'   | '/  .|   : '  |/    |  :  :       |   | '`--'   ");
    mvprintw(16, MAP_WIDTH/2 - 47, "\\    \\         ;'  :  `--'   |   :    / |   :    / ;   | |`-'     |  | ,'       '   : |       ");
    mvprintw(17, MAP_WIDTH/2 - 47, " \\    \\      ,' :  ,      .-./\\   \\ .'   \\   \\ .'  |   ;/         `--''         ;   |.'       ");
    mvprintw(18, MAP_WIDTH/2 - 47, "  \"---....--'    `--`----'     `---`      `---`    '---'                        '---'         ");
    attroff(COLOR_PAIR(8));
    
    // Menu Options
    attron(COLOR_PAIR(4));
    mvprintw(20, MAP_WIDTH/2 - 15, "            __ ___      _ ___ ");
    mvprintw(21, MAP_WIDTH/2 - 15, " /|   __   (_   |  /\\  |_) |  ");
    mvprintw(22, MAP_WIDTH/2 - 15, "  |        __)  | /--\\ | \\ |  ");
    attroff(COLOR_PAIR(4));

    attron(COLOR_PAIR(7));
    mvprintw(24, MAP_WIDTH/2 - 14, " _          _     _   __  _ ");
    mvprintw(25, MAP_WIDTH/2 - 14, "  )   __   /  |  / \\ (_  |_ ");
    mvprintw(26, MAP_WIDTH/2 - 14, " /_        \\_ |_ \\_/ __) |_ ");
    attroff(COLOR_PAIR(7));
    
    // Controls Legend
    attron(COLOR_PAIR(3));
    mvprintw(28, MAP_WIDTH/2 - 33, "             __  _           _    |    _    ___ _     _    ___ ___");
    mvprintw(29, MAP_WIDTH/2 - 33, " \\    / /\\  (_  | \\   _'_   |_)   |   / \\    | / \\   |_ \\/  |   | ");
    mvprintw(30, MAP_WIDTH/2 - 33, "  \\/\\/ /--\\ __) |_/    '    | \\   |   \\_X    | \\_/   |_ /\\ _|_  | ");
    attroff(COLOR_PAIR(3));
    
    refresh();
    
    // Menu Selection
    int choice;
    while ((choice = getch()) != '1' && choice != '2') {}
    
    if (choice == '2') {
        endwin();
        exit(0);
    }
}

void showDeathScreen(void) {
    clear();
    attron(A_BOLD | COLOR_PAIR(7));
    
    // Death Message
    mvprintw(2, MAP_WIDTH/2 - 27, "     ___  __   __  _______  _______  _______  ______  ");
    mvprintw(3, MAP_WIDTH/2 - 27, "    |   ||  | |  ||       ||       ||       ||      | ");
    mvprintw(4, MAP_WIDTH/2 - 27, "    |   ||  | |  ||    ___||    ___||    ___||  _    |");
    mvprintw(5, MAP_WIDTH/2 - 27, "    |   ||  |_|  ||   | __ |   | __ |   |___ | | |   |");
    mvprintw(6, MAP_WIDTH/2 - 27, " ___|   ||       ||   ||  ||   ||  ||    ___|| |_|   |");
    mvprintw(7, MAP_WIDTH/2 - 27, "|       ||       ||   |_| ||   |_| ||   |___ |       |");
    mvprintw(8, MAP_WIDTH/2 - 27, "|_______||_______||_______||_______||_______||______| ");
    
    // Stats Display
    attron(COLOR_PAIR(1));    
    mvprintw(11, MAP_WIDTH/2 - 29, "     _       _       _   _       _      _  _       ");
    mvprintw(12, MAP_WIDTH/2 - 29, " |  |_ \\  / |_ |    |_) |_  /\\  /  |_| |_ | \\   o  ");
    mvprintw(13, MAP_WIDTH/2 - 29, " |_ |_  \\/  |_ |_   | \\ |_ /--\\ \\_ | | |_ |_/   o  ");

    if (currentLevel == 0) {
        mvprintw(11, MAP_WIDTH/2 + 22, "    _  ");
        mvprintw(12, MAP_WIDTH/2 + 22, "   / \\ ");
        mvprintw(13, MAP_WIDTH/2 + 22, "   \\_/ ");
    }
    else if (currentLevel == 1) {
        mvprintw(11, MAP_WIDTH/2 + 25, "      ");
        mvprintw(12, MAP_WIDTH/2 + 25, "   /| ");
        mvprintw(13, MAP_WIDTH/2 + 25, "    | ");
        }
    else if (currentLevel == 2) {
        mvprintw(11, MAP_WIDTH/2 + 25, "   _  ");
        mvprintw(12, MAP_WIDTH/2 + 25, "    ) ");
        mvprintw(13, MAP_WIDTH/2 + 25, "   /_ ");
    
    }
    else {
        mvprintw(11, MAP_WIDTH/2 + 25, "   _  ");
        mvprintw(12, MAP_WIDTH/2 + 25, "   _) ");
        mvprintw(13, MAP_WIDTH/2 + 25, "   _) ");
    }

    mvprintw(16, MAP_WIDTH/2 - 24, "  __  _   _   _     _  _  _  _   _ ___     \\   ");
    mvprintw(17, MAP_WIDTH/2 - 24, " /__ / \\ / \\ | \\   |_ |_ |_ / \\ |_) |    o  |  ");
    mvprintw(18, MAP_WIDTH/2 - 24, " \\_| \\_/ \\_/ |_/   |_ |  |  \\_/ | \\ |    o  | ");
    mvprintw(19, MAP_WIDTH/2 - 24, "                                           /  ");
    
    // Exit Prompt
    attron(COLOR_PAIR(3));  
    mvprintw(21, MAP_WIDTH/2 - 26, "  _   _   _  __  __    _    ___ _     _    ___ ___");
    mvprintw(22, MAP_WIDTH/2 - 26, " |_) |_) |_ (_  (_    / \\    | / \\   |_ \\/  |   | ");
    mvprintw(23, MAP_WIDTH/2 - 26, " |   | \\ |_ __) __)   \\_X    | \\_/   |_ /\\ _|_  | ");
    refresh();
    
    // Input Handling
    int input;
    while ((input = getch()) && input != 'q') {}  
    endwin();
    exit(0);

}

void showVictoryScreen(void) {
    clear();
    attron(A_BOLD | COLOR_PAIR(5));
    
    // Victory Message
    mvprintw(2, MAP_WIDTH/2 - 30, " __   __  ___   ______  _______  _______  ______    __   __ ");
    mvprintw(3, MAP_WIDTH/2 - 30, "|  | |  ||   | |     _||       ||       ||    _ |  |  | |  |");
    mvprintw(4, MAP_WIDTH/2 - 30, "|  |_|  ||   | |    |  |_     _||   _   ||   | ||  |  |_|  |");
    mvprintw(5, MAP_WIDTH/2 - 30, "|       ||   | |    |    |   |  |  | |  ||   |_||_ |       |");
    mvprintw(6, MAP_WIDTH/2 - 30, "|       ||   | |    |    |   |  |  |_|  ||    __  ||_     _|");
    mvprintw(7, MAP_WIDTH/2 - 30, " |     | |   | |    |_   |   |  |       ||   |  | |  |   |  ");
    mvprintw(8, MAP_WIDTH/2 - 30, "  |___|  |___| |______|  |___|  |_______||___|  |_|  |___|  ");

    attron(COLOR_PAIR(4));
    mvprintw(13, MAP_WIDTH/2 - 16, "\\ / _       __ __ __ _  _  __ _ ");
    mvprintw(14, MAP_WIDTH/2 - 16, " Y / \\| |  |_ (_ /  |_||_)|_ | \\");
    mvprintw(15, MAP_WIDTH/2 - 16, " | \\_/|_|  |__ _)\\__| ||  |__|_/");
    
    // Exit Prompt
    attron(COLOR_PAIR(3));  
    mvprintw(21, MAP_WIDTH/2 - 26, "  _   _   _  __  __    _    ___ _     _    ___ ___");
    mvprintw(22, MAP_WIDTH/2 - 26, " |_) |_) |_ (_  (_    / \\    | / \\   |_ \\/  |   | ");
    mvprintw(23, MAP_WIDTH/2 - 26, " |   | \\ |_ __) __)   \\_X    | \\_/   |_ /\\ _|_  | ");
    refresh();
    
    // Input Handling
    int input;
    while ((input = getch()) && input != 'q') {}  
    endwin();
    exit(0);
}