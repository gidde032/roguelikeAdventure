#ifndef ROGUE_H
#define ROGUE_H

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//color pairs for entities and tiles
#define VISIBLE_COLOR 1
#define SEEN_COLOR 2
#define PLAYER_COLOR 3
#define GOB_COLOR 4
#define SPID_COLOR 5
#define TROLL_COLOR 6
#define HEALS_COLOR 7
#define ITEM_COLOR 8

/* typedef struct {
    Tile** tiles;
    int numRooms;
    //int numMonsters;
    struct Room** lRooms;
    //struct Monster** lMonsters;
} Level; */

typedef struct { //position for rooms, players, etc.
    int y;
    int x;
} Position;

typedef struct { //map tiles
    char ch;
    int color;
    bool walkable;
    bool transparent;
    bool visible;
    bool seen;
} Tile;

typedef struct { //room, pos points to corner of room and center points to middle
    int height;
    int width;
    Position pos;
    Position center;
} Room;

typedef struct { //entities such as the player
    Position pos;
    char ch;
    int color;
    int hP;
    int maxHP;
    int attack;
    bool alive;
} Entity;

typedef struct { //heals like medkits & bandages
    Position pos;
    char ch;
    int color;
    int healing;
    bool used;
} Heal;

typedef struct { //Items that buff the player
    Position pos;
    char ch;
    int color;
    int atkAdded;
    bool used;
} Item;

//draw funcs
void drawMap(void);
void drawEntity(Entity* entity);
void drawHeal(Heal* heal);
void drawItem(Item* item);
void drawEverything(void);

//engine funcs
bool cursesSetup(void);
void gameLoop(void);
void closeGame(void);
void levelUp(void);
void showTitleScreen(void);
void showDeathScreen(void);
void showVictoryScreen(void);

//map funcs
Tile** createMapTiles(void);
Position setupMap(void);
void freeMap(void);

//player funcs
Entity* createPlayer(Position start_pos);
void handleInput(int input);
void movePlayer(Position newPos);

//chests funcs
// Entity* createChest (Position chest_pos);

//room funcs
Room createRoom(int y, int x, int height, int width);
void addRoomToMap(Room room);
void connectRooms(Position centerOne, Position centerTwo);

//fov funcs
void makeFOV(Entity* player);
void clearFOV(Entity* player);
int getDistance(Position origin, Position target);
bool isInMap(int y, int x);
bool lineOfSight(Position origin, Position target);
int getSign(int a);

//goblin funcs
Entity* createGoblin(Position pos, int hP, int attack);
void gobTurn(void);

//spider funcs
Entity* createSpid(Position pos, int hP, int attack);
void spidTurn(void);

//troll funcs
Entity* createTroll(Position pos, int hP, int attack);
void trollTurn(void);

//combat funcs
void attack(Entity* attacker, Entity* defender);
bool checkPColl(Position newPos);
bool checkMonColl(Position newPos);
int combatDist(Position a, Position b);

//buffs funcs
void useHeal(Heal* heal);
void useItem(Item* item);
Heal* createMedkit(Position pos);
Heal* createBandage(Position pos);
Item* createPotion(Position pos);

//externs
extern Entity* player;
extern const int MAP_HEIGHT;
extern const int MAP_WIDTH;
extern Tile** map;
extern Entity** mons;
extern int monCount;
extern Heal** heals;
extern int healCount;
extern Item** items;
extern int itemCount;
extern int currentLevel;
extern int maxLevels;
extern int monsJugged;

#endif