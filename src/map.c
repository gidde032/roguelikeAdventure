#include <rogue.h>

Tile** createMapTiles(void) { //generates base map with only wall tiles
    Tile** tiles = calloc(MAP_HEIGHT, sizeof(Tile*));;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        tiles[y] = calloc(MAP_WIDTH, sizeof(Tile));
        for (int x = 0; x < MAP_WIDTH; x++) {
            tiles[y][x].ch = '#';
            tiles[y][x].color = COLOR_PAIR(VISIBLE_COLOR);
            tiles[y][x].walkable = false;
            tiles[y][x].transparent = false;
            tiles[y][x].visible = false;
            tiles[y][x].seen = false;
        }
    }
    return tiles;
}

Position setupMap(void) {
    int y, x, height, width, n_rooms;
    //chestsPlaced = 0;
    //n_chests = (rand() % 6) + 4;
    n_rooms = (rand() % 14) + 20;
    Room* rooms = calloc(n_rooms, sizeof(Room));
    //Entity** chests = calloc(n_chests, sizeof(Entity));
    Position start_pos;

    for (int i = 0; i < n_rooms; i++) { //auto-gens between 12-32 rooms with a width of 5-17 tiles and a height of 3-9 tiles
        y = (rand() % (MAP_HEIGHT - 9)) + 1;
        x = (rand() % (MAP_WIDTH - 17)) + 1;
        height = (rand() % 6) + 3;
        width = (rand() % 12) + 5;
        rooms[i] = createRoom(y, x, height, width);
        addRoomToMap(rooms[i]);
       /* if (chestsPlaced < n_chests) {
            chests[chestsPlaced] = createChest(rooms[i].center);
            chestsPlaced++;
        } */
        if (i > 0) { //after first room connects separated rooms
            connectRooms(rooms[i-1].center, rooms[i].center);
        }
    }

    for (int z = 1; z < n_rooms; z += 3) { //balances monster and heal spawns
        if ((z % 19) == 0) {
            int potionY = (rand() % (rooms[z].height - 2)) + rooms[z].pos.y + 1;
            int potionX = (rand() % (rooms[z].width - 2)) + rooms[z].pos.x + 1;
            Position potionPos = {potionY, potionX};
            Item* potion = createPotion(potionPos);
            items[itemCount++] = potion;
        }
        else if ((z % 8) == 0) {
            int medkitY = (rand() % (rooms[z].height - 2)) + rooms[z].pos.y + 1;
            int medkitX = (rand() % (rooms[z].width - 2)) + rooms[z].pos.x + 1;
            Position medkitPos = {medkitY, medkitX};
            Heal* medkit = createMedkit(medkitPos);
            heals[healCount++] = medkit;
        }
        else if ((z % 7) == 0) {
            int trollY = (rand() % (rooms[z].height - 2)) + rooms[z].pos.y + 1;
            int trollX = (rand() % (rooms[z].width - 2)) + rooms[z].pos.x + 1;
            Position trollPos = {trollY, trollX};
            Entity* troll = createTroll(trollPos, ((rand() % 6) + 16), ((rand() % 4) + 5)); //creates troll w pos, hP, and atk
            mons[monCount++] = troll;
        }
        else if ((z % 5) == 0) {
            int bandageY = (rand() % (rooms[z].height - 2)) + rooms[z].pos.y + 1;
            int bandageX = (rand() % (rooms[z].width - 2)) + rooms[z].pos.x + 1;
            Position bandagePos = {bandageY, bandageX};
            Heal* bandage = createBandage(bandagePos);
            heals[healCount++] = bandage;
        }
        else if ((z % 2) == 0) {
            int spidY = (rand() % (rooms[z].height - 2)) + rooms[z].pos.y + 1;
            int spidX = (rand() % (rooms[z].width - 2)) + rooms[z].pos.x + 1;
            Position spidPos = {spidY, spidX};
            Entity* spid = createSpid(spidPos, ((rand() % 2) + 5), ((rand() % 3) + 3)); //creates spid w pos, hP, and atk
            mons[monCount++] = spid;
        }
        else {
            int gobY = (rand() % (rooms[z].height - 2)) + rooms[z].pos.y + 1; //random y-pos for gob in room
            int gobX = (rand() % (rooms[z].width - 2)) + rooms[z].pos.x + 1; //random x-pos for gob in room
            Position gobPos = {gobY, gobX};
            Entity* gob = createGoblin(gobPos, ((rand() % 4) + 8), ((rand() % 2) + 2)); //creates gob w pos, hP, and atk
            mons[monCount++] = gob;
        }
    } 
    if (currentLevel < maxLevels) {
        map[rooms[n_rooms-1].center.y][rooms[n_rooms-1].center.x].ch = '>';
        map[rooms[n_rooms-1].center.y][rooms[n_rooms-1].center.x].walkable = true;
    }
    else if (currentLevel == maxLevels) {
        map[rooms[n_rooms-1].center.y][rooms[n_rooms-1].center.x].ch = '^';
        map[rooms[n_rooms-1].center.y][rooms[n_rooms-1].center.x].walkable = true;
    }
    int hpInc = 3;
    int atkInc = 2;
    if (currentLevel > 0) {
        for (int i = 0; i < monCount; i++) {
            mons[i]->hP += hpInc;
            mons[i]->attack += atkInc;
        }
    }
    start_pos.y = rooms[0].center.y;
    start_pos.x = rooms[0].center.x;
    free(rooms);
    return start_pos;
}

void freeMap(void) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        free(map[y]);
    }
    free(map);
}