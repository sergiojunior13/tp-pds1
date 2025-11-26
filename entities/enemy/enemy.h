#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "utils.h"
#include "images/animation.h"

typedef struct Renderer Renderer;
typedef struct Game Game;

typedef enum EnemyType {
    Enemy_Weak, Enemy_Strong
} EnemyType;

typedef enum ActionType {
    Attack_Action, Defense_Action
} ActionType;

typedef struct EnemyAction {
    ActionType type;
    int effect;
    int level;
} EnemyAction;

typedef struct Enemy {
    Hp hp;
    int shield_pts;
    EnemyType type;
    EnemyAction actions[3];
    int actions_size;

    Animation idle_animation;
    Animation action_animation; // Dead, attack, hurt
} Enemy;

void RemoveEnemyFromArray(Enemy array[], int* size_ptr, int element_index);
void RenderEnemies(const Renderer* renderer, Game* game);
void RenderEnemy(const Renderer* renderer, Enemy* enemy, int begin_x, int begin_y, int is_focused, int index);
void GenerateEnemies(Enemy gameEnemies[2]);

void StartEnemyDeadAnimation(Enemy* enemy, int enemy_index);
void StartEnemyAttackAnimation(Enemy* enemy, int enemy_index);
void StartEnemyHurtAnimation(Enemy* enemy, int enemy_index);

#endif
