#include "ghost.h"
#include "pacman_obj.h"
#include "map.h"
/* Shared variables */
#define GO_OUT_TIME 256
extern uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
extern const int cage_grid_x, cage_grid_y;

/* Declare static function prototypes */
static void ghost_red_move_script_FREEDOM(Ghost* ghost, Map* M);
static void ghost_red_move_script_BLOCKED(Ghost* ghost, Map* M);
static void ghost_red_move_script_CHASE(Ghost* ghost, Map* M, const Pacman* const pacman);

static void ghost_red_move_script_FREEDOM(Ghost* ghost, Map* M) {
    // [HACKATHON 2-4]
    // Uncomment the following code and finish pacman picking random direction.
    Directions i;
    static Directions proba[4]; // possible movement
    int cnt = 0;
    for (i = 1; i <= 4; i++)
        if (ghost_movable(ghost, M, i, true)) 	proba[cnt++] = i;
    if (cnt == 1) {
        //game_log("task 1");
        ghost_NextMove(ghost, proba[0]);
    }
    else if (cnt < 3) { //3 ways. First part will check in the position now if ad sesautu or not.
        //game_log("task 2");
        if ((ghost_movable(ghost, M, ghost->objData.facing, true))) {
            ghost_NextMove(ghost, ghost->objData.facing); //if depan pacmannya movable lsg gas
        }
        else {
            switch (ghost->objData.facing) {
            case UP:
                for (int j = 1; j; j) {
                    i = rand() % 4; //random number trs modulus 4 trs sisanya 0 1 2 3 trs tambah 1 krna gmw 0 (Direction 0 = gada)
                //    if(i==UP) continue;
                    if (ghost_movable(ghost, M, i+1, true)) {
                        ghost_NextMove(ghost, i+1);
                        break;
                    }
                }
                break;
            case DOWN:
                for (int j = 1; j; j) {
                    i = rand() % 4;
                    //       if(i==DOWN) continue;
                    if (ghost_movable(ghost, M, i+1, true)) {
                        ghost_NextMove(ghost, i+1);
                        break;
                    }
                }
                break;
            case LEFT:
                for (int j = 1; j; j) {
                    i = rand() % 4;
                    //     if(i==LEFT) continue;
                    if (ghost_movable(ghost, M, i+1, true)) {
                        ghost_NextMove(ghost, i+1);
                        break;
                    }
                }
                break;
            case RIGHT:
                for (int j = 1; j; j) {
                    i = rand() % 4;
                    //      if(i==RIGHT) continue;
                    if (ghost_movable(ghost, M, i+1, true)) {
                        ghost_NextMove(ghost, i+1);
                        break;
                    }
                }
                break;
            default:
                break;
            }
        }
    }
    else {
        //game_log("task 3");
        int rndom = rand() % 30; //if in intersection 4 sisi so just random %30 gatau
        switch (ghost->objData.facing) {
        case UP:
            if (rndom == 1 && ghost_movable(ghost, M, DOWN, true)) {
                ghost_NextMove(ghost, DOWN);
                break;
            }
            else {
                while (1) {
                    Directions in = rand() % 4;
                    if (ghost_movable(ghost, M, in+1, true) && in+1 != DOWN) {
                        ghost_NextMove(ghost, in+1);
                        break;
                    }
                }
            }
            break;
        case DOWN:
            if (rndom == 1 && ghost_movable(ghost, M, UP, true)) {
                ghost_NextMove(ghost, UP);
                break;
            }
            else {
                while (1) {
                    Directions in = rand() % 4;
                    if (ghost_movable(ghost, M, in+1, true) && in+1 != UP) {
                        ghost_NextMove(ghost, in+1);
                        break;
                    }
                }
            }
            break;
        case LEFT:
            if (rndom == 1 && ghost_movable(ghost, M, RIGHT, true)) {
                ghost_NextMove(ghost, RIGHT);
                break;
            }
            else {
                while (1) {
                    Directions in = rand() % 4;
                    if (ghost_movable(ghost, M, in+1, true) && in+1 != RIGHT) {
                        ghost_NextMove(ghost, in+1);
                        break;
                    }
                }
            }
            break;
        case RIGHT:
            if (rndom == 1 && ghost_movable(ghost, M, LEFT, true)) {
                ghost_NextMove(ghost, LEFT);
                break;
            }
            else {
                while (1) {
                    Directions in = rand() % 4;
                    if (ghost_movable(ghost, M, in+1, true) && in+1 != LEFT) {
                        ghost_NextMove(ghost, in+1);
                        break;
                    }
                }
            }
            break;
        default:
            break;
        }
    }
    //	ghost_NextMove(ghost, proba[generateRandomNumber(1, 4)]);


        // [TODO] (Not in Hackathon) 
        // Description:
        // For red(Blinky) ghost, we ask you to implement an random strategy ghost, 
        // which means moving in random direction.
        // But your random strategy have to designed carefully so that ghost won't walk back and forth.
        // (The code here DO perform walking back and forth.)

}

static void ghost_red_move_script_BLOCKED(Ghost* ghost, Map* M) {

    switch (ghost->objData.preMove)
    {
    case UP:
        if (ghost->objData.Coord.y == 10)
            ghost_NextMove(ghost, DOWN);
        else
            ghost_NextMove(ghost, UP);
        break;
    case DOWN:
        if (ghost->objData.Coord.y == 12)
            ghost_NextMove(ghost, UP);
        else
            ghost_NextMove(ghost, DOWN);
        break;
    default:
        ghost_NextMove(ghost, UP);
        break;
    }
}

static void ghost_red_move_script_CHASE(Ghost* ghost, Map* M, const Pacman* const pacman) {
    Directions fastest = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y, pacman->objData.Coord.x, pacman->objData.Coord.y);
    ghost_NextMove(ghost, fastest);
}

void ghost_red_move_script(Ghost* ghost, Map* M, Pacman* pacman) {
    if (!movetime(ghost->speed))
        return;
    switch (ghost->status)
    {
    case BLOCKED:
        ghost_red_move_script_BLOCKED(ghost, M);
        if (al_get_timer_count(game_tick_timer) > GO_OUT_TIME)
            ghost->status = GO_OUT;
        break;
    case FREEDOM:
        ghost_red_move_script_CHASE(ghost, M, pacman);
        break;
    case GO_OUT:
        ghost_move_script_GO_OUT(ghost, M);
        break;
    case GO_IN:
        ghost_move_script_GO_IN(ghost, M);
        if (M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') {
            ghost->status = BLOCKED;
            ghost->speed = 2;
        }
        break;
    case FLEE:
        ghost_move_script_FLEE(ghost, M, pacman);
        break;
    default:
        break;
    }

    if (ghost_movable(ghost, M, ghost->objData.nextTryMove, false)) {
        ghost->objData.preMove = ghost->objData.nextTryMove;
        ghost->objData.nextTryMove = NONE;
    }
    else if (!ghost_movable(ghost, M, ghost->objData.preMove, false))
        return;

    switch (ghost->objData.preMove) {
    case RIGHT:
        ghost->objData.Coord.x++;
        break;
    case LEFT:
        ghost->objData.Coord.x--;
        break;
    case UP:
        ghost->objData.Coord.y--;
        break;
    case DOWN:
        ghost->objData.Coord.y++;
        break;
    default:
        break;
    }
    ghost->objData.facing = ghost->objData.preMove;
    ghost->objData.moveCD = GAME_TICK_CD;
}

void ghost_pink_move_script(Ghost* ghost, Map* M, Pacman* pacman) {
    if (!movetime(ghost->speed))
        return;
    switch (ghost->status)
    {
    case BLOCKED:
        ghost_red_move_script_BLOCKED(ghost, M);
        if (al_get_timer_count(game_tick_timer) > GO_OUT_TIME * 2)
            ghost->status = GO_OUT;
        break;
    case FREEDOM:
        ghost_red_move_script_FREEDOM(ghost, M);
        break;
    case GO_OUT:
        ghost_move_script_GO_OUT(ghost, M);
        break;
    case GO_IN:
        ghost_move_script_GO_IN(ghost, M);
        if (M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') {
            ghost->status = BLOCKED;
            ghost->speed = 2;
        }
        break;
    case FLEE:
        ghost_move_script_FLEE(ghost, M, pacman);
        break;
    default:
        break;
    }

    if (ghost_movable(ghost, M, ghost->objData.nextTryMove, false)) {
        ghost->objData.preMove = ghost->objData.nextTryMove;
        ghost->objData.nextTryMove = NONE;
    }
    else if (!ghost_movable(ghost, M, ghost->objData.preMove, false))
        return;

    switch (ghost->objData.preMove) {
    case RIGHT:
        ghost->objData.Coord.x++;
        break;
    case LEFT:
        ghost->objData.Coord.x--;
        break;
    case UP:
        ghost->objData.Coord.y--;
        break;
    case DOWN:
        ghost->objData.Coord.y++;
        break;
    default:
        break;
    }
    ghost->objData.facing = ghost->objData.preMove;
    ghost->objData.moveCD = GAME_TICK_CD;
}

void ghost_orange_move_script(Ghost* ghost, Map* M, Pacman* pacman) {
    if (!movetime(ghost->speed))
        return;
    switch (ghost->status)
    {
    case BLOCKED:
        ghost_red_move_script_BLOCKED(ghost, M);
        if (al_get_timer_count(game_tick_timer) > GO_OUT_TIME * 3)
            ghost->status = GO_OUT;
        break;
    case FREEDOM:
        ghost_red_move_script_FREEDOM(ghost, M);
        break;
    case GO_OUT:
        ghost_move_script_GO_OUT(ghost, M);
        break;
    case GO_IN:
        ghost_move_script_GO_IN(ghost, M);
        if (M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') {
            ghost->status = BLOCKED;
            ghost->speed = 2;
        }
        break;
    case FLEE:
        ghost_move_script_FLEE(ghost, M, pacman);
        break;
    default:
        break;
    }

    if (ghost_movable(ghost, M, ghost->objData.nextTryMove, false)) {
        ghost->objData.preMove = ghost->objData.nextTryMove;
        ghost->objData.nextTryMove = NONE;
    }
    else if (!ghost_movable(ghost, M, ghost->objData.preMove, false))
        return;

    switch (ghost->objData.preMove) {
    case RIGHT:
        ghost->objData.Coord.x++;
        break;
    case LEFT:
        ghost->objData.Coord.x--;
        break;
    case UP:
        ghost->objData.Coord.y--;
        break;
    case DOWN:
        ghost->objData.Coord.y++;
        break;
    default:
        break;
    }
    ghost->objData.facing = ghost->objData.preMove;
    ghost->objData.moveCD = GAME_TICK_CD;
}

void ghost_blue_move_script(Ghost* ghost, Map* M, Pacman* pacman) {
    if (!movetime(ghost->speed))
        return;
    switch (ghost->status)
    {
    case BLOCKED:
        ghost_red_move_script_BLOCKED(ghost, M);
        if (al_get_timer_count(game_tick_timer) > GO_OUT_TIME * 4)
            ghost->status = GO_OUT;
        break;
    case FREEDOM:
        ghost_red_move_script_FREEDOM(ghost, M);
        break;
    case GO_OUT:
        ghost_move_script_GO_OUT(ghost, M);
        break;
    case GO_IN:
        ghost_move_script_GO_IN(ghost, M);
        if (M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') {
            ghost->status = BLOCKED;
            ghost->speed = 2;
        }
        break;
    case FLEE:
        ghost_move_script_FLEE(ghost, M, pacman);
        break;
    default:
        break;
    }

    if (ghost_movable(ghost, M, ghost->objData.nextTryMove, false)) {
        ghost->objData.preMove = ghost->objData.nextTryMove;
        ghost->objData.nextTryMove = NONE;
    }
    else if (!ghost_movable(ghost, M, ghost->objData.preMove, false))
        return;

    switch (ghost->objData.preMove) {
    case RIGHT:
        ghost->objData.Coord.x++;
        break;
    case LEFT:
        ghost->objData.Coord.x--;
        break;
    case UP:
        ghost->objData.Coord.y--;
        break;
    case DOWN:
        ghost->objData.Coord.y++;
        break;
    default:
        break;
    }
    ghost->objData.facing = ghost->objData.preMove;
    ghost->objData.moveCD = GAME_TICK_CD;
}
