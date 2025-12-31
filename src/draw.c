#include <rogue.h>

void drawMap(void) { //determines color and draws map
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map[y][x].visible) {
                mvaddch(y, x, map[y][x].ch | map[y][x].color); //visible in white
            }
            else if (map[y][x].seen) {
                mvaddch(y, x, map[y][x].ch | COLOR_PAIR(SEEN_COLOR)); //seen in blue
            }
            else {
                mvaddch(y, x, ' '); //empty char otherwise
            }
        }
    }
}

void drawEntity(Entity* entity) { 
    mvaddch(entity->pos.y, entity->pos.x, entity->ch | entity->color);
}

void drawHeal(Heal* heal) {
    mvaddch(heal->pos.y, heal->pos.x, heal->ch | heal->color);
}

void drawItem(Item* item) {
    mvaddch(item->pos.y, item->pos.x, item->ch | item->color);
}

void drawEverything(void) { //clears window and draws player + map + mons + heals
    clear();
    drawMap();
    for (int i = 0; i < healCount; i++) {
        if (map[heals[i]->pos.y][heals[i]->pos.x].visible) {
            drawHeal(heals[i]);
        }
    }
    for (int i = 0; i < itemCount; i++) {
        if (map[items[i]->pos.y][items[i]->pos.x].visible) {
            drawItem(items[i]);
        }
    }
    for (int i = 0; i < monCount; i++) { //draws all visible monsters
        if (map[mons[i]->pos.y][mons[i]->pos.x].visible) {
            drawEntity(mons[i]);
        }
    } 
    drawEntity(player);
    mvprintw(0,0, "HP: %d/%d. Don't get jugged.", player->hP, player->maxHP); //HP bar
    mvprintw(1, 0, "ATK: %d", player->attack); //attack bar
    mvprintw(2, 0, "LVL: %d", currentLevel); //level tracker
    mvprintw(3, 0, "JUGGS HIT: %d", monsJugged); //jugg tracker
    mvprintw(MAP_HEIGHT+1, 0, "Monsters nearby!");
}