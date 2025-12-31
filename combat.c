//#include <rogue.h>
/*
void attack(Entity* attacker, Entity* defender) {
    defender->hP -= attacker->attack;
    if (defender->hP <= 0) {
        defender->alive = false;
        defender->ch = 'X';
    }
}

bool checkPColl(Position newPos) {
    for (int i = 0; i < monCount; i++) {
        if (mons[i]->alive && (mons[i]->pos.y == newPos.y) && (mons[i]->pos.x == newPos.x)) {
            attack(player, mons[i]);
            return true;
        }
    }
    return false;
} */