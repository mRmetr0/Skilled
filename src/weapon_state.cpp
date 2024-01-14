#include "weapon_state.h"

using namespace godot;

#pragma region base_weapon_state
void WeaponState::start(Node& node, int p_start_clip) {
    if (p_start_clip > 0)
        bullet_amount = p_start_clip;
    else 
        bullet_amount = magazine_size;

    bullet_pool = Object::cast_to<Node>(node.get_node_or_null("/root/BulletPool"));
    input = Input::get_singleton();
}
WeaponState* WeaponState::update(Node2D& node,double delta) {
    return nullptr;
}
void WeaponState::shoot(Vector2 position, double angle){}

Vector2i WeaponState::_get_bullets() {
    if (reloading) return Vector2i(-1, -1);
    return Vector2i(bullet_amount, magazine_size);
}

#pragma endregion base_weapon_state

#pragma region pistol
void PistolState::start(Node& node, int p_start_clip){
    id = 1;

    //reloading
    reload_duration = 1.0;
    reload_time = 0.0;
    reloading = false;

    //firing 
    inaccuracy = 4;
    rate_of_fire;
    fire_time;
    crit_rate = 2;
    in_recoil = false;

    //shotgun
    spread = 0; 
    shot_amount = 1;    

    //bullet
    bullet_damage = 2;
    bullet_pierce = 0;
    bullet_amount = 0;
    magazine_size = 13;

    WeaponState::start(node, p_start_clip);
}
WeaponState* PistolState::update(Node2D& node,double delta){
    if (input->is_action_just_pressed("RClick")){
        shoot(node.get_position(), (node.get_global_mouse_position() - node.get_position()).angle());
    } else if (input->is_action_just_pressed("reload")){
        reload();
    }

    if (!reloading) return nullptr;
    reload_time += delta;
    if (reload_time >= reload_duration){
        bullet_amount = magazine_size;
        reloading = false;
    }   
    return nullptr;
}
void PistolState::shoot(Vector2 position, double angle){
    if (reloading) return;
    
    //setting up everything:
    Node2D* init = cast_to<Node2D>(bullet_pool->call("_get_bullet"));
    init->call("_set_bullet", bullet_damage, bullet_pierce, crit_rate);

    int accuracy = rand() % (inaccuracy*2) - inaccuracy;

    //Marking everything down:
    init->set_position(position + (Vector2(cos(angle), sin(angle)) * 100));
    init->set_rotation(angle + (accuracy * 0.01));
    
    
    //managing bullet amout:
    bullet_amount--;
    if (bullet_amount == 0) 
        reload();
}
void PistolState::reload(){
    if (bullet_amount == magazine_size || reloading) return;
    reloading = true;
    reload_time = 0.0;
}
#pragma endregion pistol

#pragma region auto_rifle
void AutoState::start(Node& node, int p_start_clip){
    id = 2;

    //reloading
    reload_duration;
    reload_time;
    reloading;

    //firing 
    inaccuracy = 3;
    rate_of_fire = 0.08;
    fire_time = 0.0;
    crit_rate = 2;
    in_recoil = false;

    //shotgun
    spread = 0; 
    shot_amount = 1;    

    //bullet
    bullet_damage = 1;
    bullet_pierce = 0;
    bullet_amount = 0;
    magazine_size = 40;

    WeaponState::start(node, p_start_clip);
}
WeaponState* AutoState::update(Node2D& node,double delta){
    if (in_recoil){
        fire_time += delta;
        if (fire_time >= rate_of_fire) {
            in_recoil = false;
            fire_time = 0.0;
        }
    }

    if (input->is_action_pressed("RClick") && !in_recoil){
        shoot(node.get_position(), (node.get_global_mouse_position() - node.get_position()).angle());
        in_recoil = true;
    } 
  
    if (empty) return memnew(PistolState);

    return nullptr;
}
void AutoState::shoot(Vector2 position, double angle){    

    Node2D* init = cast_to<Node2D>(bullet_pool->call("_get_bullet"));
    init->call("_set_bullet", bullet_damage, bullet_pierce, crit_rate);

    int accuracy = rand() % (inaccuracy*2) - inaccuracy;


    //Marking everything down:
    init->set_position(position);
    init->set_rotation(angle + ((double)accuracy * 0.01));

    
    //managing bullet amout:
    bullet_amount--;
    if (bullet_amount == 0) 
        empty = true;
}
#pragma endregion auto_rifle

#pragma region spread_gun

void SpreadState::start(Node& node, int p_start_clip){
    id = 3;

    //reloading
    reload_duration;
    reload_time;
    reloading;

    //firing 
    inaccuracy = 5;
    rate_of_fire = 0.7;
    fire_time = 0.0;
    crit_rate = 2;
    in_recoil = false;

    //shotgun
    spread = 0.12; 
    shot_amount = 5;    

    //bullet
    bullet_damage = 1;
    bullet_pierce = 0;
    bullet_amount = 0;
    magazine_size = 25;

    WeaponState::start(node, p_start_clip);
}
WeaponState* SpreadState::update(Node2D& node,double delta){
    if (in_recoil){
        fire_time += delta;
        if (fire_time >= rate_of_fire) {
            in_recoil = false;
            fire_time = 0.0;
        }
    }

    if (input->is_action_pressed("RClick") && !in_recoil){
        shoot(node.get_position(), (node.get_global_mouse_position() - node.get_position()).angle());
        in_recoil = true;
    } 
  
    if (empty) return memnew(PistolState);

    return nullptr;
}
void SpreadState::shoot(Vector2 position, double angle){    

    for (int i = 0; i < shot_amount; i++){
        float offset = (i-(shot_amount/2))*spread;
        Node2D* init = cast_to<Node2D>(bullet_pool->call("_get_bullet"));
        init->call("_set_bullet", bullet_damage, bullet_pierce, crit_rate);

        int accuracy = rand() % (inaccuracy*2) - inaccuracy;


        //Marking everything down:
        init->set_position(position);
        init->set_rotation(angle + offset + ((double)accuracy * 0.01));
    }
    
    //managing bullet amout:
    bullet_amount--;
    if (bullet_amount == 0) 
        empty = true;
}

#pragma endregion spread_gun