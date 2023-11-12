#ifndef ENEMYSTATE_H
#define ENEMYSTATE_H

using namespace godot;
class EnemyState {
    public:
        virtual ~EnemyState();
        virtual EnemyState* update(Enemy& enemy, double delta);    
        virtual void fixed_update(Enemy& enemy, double delta);

        // static HuntingState hunting;
        // static StormingState storming;
        // static AttackingState attacking;
        // static WanderingState wandering;
};

class HuntingState : public EnemyState {
    public:
        EnemyState* update (Enemy& enemy, double delta) override;
};

EnemyState* HuntingState::update (Enemy& enemy, double delta) {
    if (!enemy.can_update(delta)) return NULL;
    enemy.check_path = enemy.tile_map->call("_get_path_raw", enemy.get_position(), enemy.player->get_position());
    if (enemy.get_position().distance_to(enemy.player->get_position()) <= enemy.attack_range){
        return new AttackingState();
        //enemy.switch_state(enemy.ATTACKING);
    } else {
        enemy.astar_hunt();
    }
    return NULL;
}

class HuntingState : public EnemyState {
    public:
        HuntingState();

        virtual void fixed_update(Enemy& enemy);
};

class StormingState : public EnemyState {
    public:
        StormingState();

        virtual EnemyState* update(Enemy& enemy, double delta){
            if (!enemy.can_update(delta)) return;
            enemy.check_path = enemy.tile_map->call("_get_path_raw", enemy.get_position(), enemy.player->get_position());
            if (enemy.check_path.size() > 7) {
                enemy.emit_signal("log", this, "STORMING");
                enemy.astar_storm();
            } else {
                return new HuntingState();
                //enemy.switch_state(enemy.HUNTING);
            }
            return NULL;
        }
        virtual void fixed_update(Enemy& enemy);
};

class AttackingState : public EnemyState {
    public:
        AttackingState();

        virtual EnemyState* update(Enemy& enemy, double delta){
            enemy.player->_take_damage(1);
            return new WanderingState();
            //enemy.switch_state(enemy.WANDERING);
        }
        virtual void fixed_update(Enemy& enemy);
};

class WanderingState : public EnemyState {
    public:
        WanderingState();

        virtual EnemyState* update(Enemy& enemy, double delta){
            if (enemy.check_path.size() < 3) {
                return new HuntingState();
                //enemy.switch_state(enemy.HUNTING);
            } else if (enemy.check_path.size() > 10) {
                return new StormingState();
                //enemy.switch_state(enemy.STORMING);
            }
            //TODO: WALK TO RANDOM POSITIONS
            if (!enemy.can_update(delta)) return;
                enemy.check_path = enemy.tile_map->call("_get_path_raw", enemy.get_position(), enemy.player->get_position());
            return NULL;
        }
        virtual void fixed_update(Enemy& enemy);
};

#endif