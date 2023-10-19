#include "weapon.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/classes/resource_loader.hpp>

using namespace godot;

void Weapon::_bind_methods() {
    // ClassDB::bind_method(D_METHOD("get_rate_of_fire"), &Weapon::get_rate_of_fire);
    // ClassDB::bind_method(D_METHOD("set_rate_of_fire", "p_rof"), &Weapon::set_rate_of_fire);
    // ClassDB::add_property("Weapon", PropertyInfo(Variant::FLOAT, "rate_of_fire"), "set_rate_of_fire", "get_rate_of_fire");

    ClassDB::bind_method(D_METHOD("get_reload_duration"), &Weapon::get_reload_duration);
    ClassDB::bind_method(D_METHOD("set_reload_duration", "p_speed"), &Weapon::set_reload_duration);
    ClassDB::add_property("Weapon", PropertyInfo(Variant::FLOAT, "reload_duration"), "set_reload_duration", "get_reload_duration");
    
    ClassDB::bind_method(D_METHOD("get_magazine"), &Weapon::get_magazine);
    ClassDB::bind_method(D_METHOD("set_magazine", "p_magazine"), &Weapon::set_magazine);
    ClassDB::add_property("Weapon", PropertyInfo(Variant::INT, "magazine_size"), "set_magazine", "get_magazine");

    ClassDB::bind_method(D_METHOD("get_bullet_damage"), &Weapon::get_bullet_damage);
    ClassDB::bind_method(D_METHOD("set_bullet_damage", "p_damage"), &Weapon::set_bullet_damage);
    ClassDB::add_property("Weapon", PropertyInfo(Variant::INT, "bullet_damage"), "set_bullet_damage", "get_bullet_damage");

    ClassDB::bind_method(D_METHOD("get_bullet_pierce"), &Weapon::get_bullet_pierce);
    ClassDB::bind_method(D_METHOD("set_bullet_pierce", "p_pierce"), &Weapon::set_bullet_pierce);
    ClassDB::add_property("Weapon", PropertyInfo(Variant::INT, "bullet_pierce"), "set_bullet_pierce", "get_bullet_pierce");

    ClassDB::bind_method(D_METHOD("get_spread"), &Weapon::get_spread);
    ClassDB::bind_method(D_METHOD("set_spread", "p_spread"), &Weapon::set_spread);
    ClassDB::add_property("Weapon", PropertyInfo(Variant::FLOAT, "spread"), "set_spread", "get_spread");

    ClassDB::bind_method(D_METHOD("get_shot_amount"), &Weapon::get_shot_amount);
    ClassDB::bind_method(D_METHOD("set_shot_amount", "p_amount"), &Weapon::set_shot_amount);
    ClassDB::add_property("Weapon", PropertyInfo(Variant::INT, "shot_amout"), "set_shot_amount", "get_shot_amount");

    ClassDB::bind_method(D_METHOD("get_fire_range"), &Weapon::get_fire_range);
    ClassDB::bind_method(D_METHOD("set_fire_range", "p_range"), &Weapon::set_fire_range);
    ClassDB::add_property("Weapon", PropertyInfo(Variant::FLOAT, "fire_range"), "set_fire_range", "get_fire_range");

    ClassDB::bind_method(D_METHOD("get_crit"), &Weapon::get_crit);
    ClassDB::bind_method(D_METHOD("set_crit", "p_crit"), &Weapon::set_crit);
    ClassDB::add_property("Weapon", PropertyInfo(Variant::INT, "crit_rate"), "set_crit", "get_crit");

    ADD_SIGNAL(MethodInfo("log", PropertyInfo(Variant::OBJECT, "node"), PropertyInfo(Variant::STRING, "message")));
    ADD_SIGNAL(MethodInfo("reload", PropertyInfo(Variant::FLOAT, "time")));
}

Weapon::Weapon() {
    fire_range = -1;

    reload_duration = 5.0;
    reload_time = 0.0;
    reloading = false;


    rate_of_fire = 0.1;
    fire_time = 0.0;
    in_recoil = false;
    crit_rate = 0;

    spread = 0.0;
    shot_amount = 1;

    bullet_damage = 1;
    bullet_pierce = 1;
    bullet_amount = 0;
    magazine_size = 5;


    if (Engine::get_singleton()->is_editor_hint())
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
        
}

Weapon::~Weapon() {
    // Add your cleanup here.
}

void Weapon::_ready(){
    bullet_amount = magazine_size;

    bullet_pool = Object::cast_to<Node>(get_node_or_null("/root/BulletPool"));
}

void Weapon::_process(double delta){
    // if (in_recoil){
    //     fire_time += delta;
    //     if (fire_time >= rate_of_fire)
    //         in_recoil = false;
    // }

    if (!reloading) return;
    reload_time += delta;
    if (reload_time >= reload_duration){
        bullet_amount = magazine_size;
        reloading = false;
    }
}

void Weapon::shoot(Vector2 position, double angle){
    if (reloading) return;
    // if (in_recoil) return;
    // in_recoil = true;

    for (int i = 0; i < shot_amount; i++){
        //setting up everything:
        float offset = (i-(shot_amount/2))*spread;
        Node2D* init = cast_to<Node2D>(bullet_pool->call("_get_bullet"));
        init->call("_set_bullet", bullet_damage, bullet_pierce, crit_rate);

        //Marking everything down:
        // get_parent()->get_parent()->add_child(init);
        init->set_position(position);
        init->set_rotation(angle + offset);
    }
    
    //managing bullet amout:
    bullet_amount--;
    if (bullet_amount == 0) 
        reload();
}

void Weapon::reload(){
    if (bullet_amount == magazine_size || reloading) return;
    reloading = true;
    reload_time = 0.0;
    emit_signal("reload", reload_duration);
}

#pragma region getters_setters

void Weapon::set_rate_of_fire(const double p_rof) {
    rate_of_fire = p_rof;
}
double Weapon::get_rate_of_fire() const{
    return rate_of_fire;
}

void Weapon::set_fire_range(const double p_range){
    fire_range = p_range;
}
double Weapon::get_fire_range() const{
    return fire_range;
}

void Weapon::set_reload_duration(const double p_duration) {
    reload_duration = p_duration;
}
double Weapon::get_reload_duration() const {
    return reload_duration;
}

void Weapon::set_bullet_amount(const int p_amount) {
    bullet_amount = p_amount;
}
int Weapon::get_bullet_amount() const {
    return bullet_amount;
}

void Weapon::set_magazine(const int p_magazine){
    magazine_size = p_magazine;
}
int Weapon::get_magazine() const {
    return magazine_size;
}

void Weapon::set_crit(const int p_crit) {
    crit_rate = p_crit;
}
int Weapon::get_crit() const {
    return crit_rate;
}

//BULLET DATA
void Weapon::set_bullet_damage(const int p_damage){
    bullet_damage = p_damage;
}
int Weapon::get_bullet_damage() const{
    return bullet_damage;
}

void Weapon::set_bullet_pierce(const int p_pierce) {
    bullet_pierce = p_pierce;
}
int Weapon::get_bullet_pierce() const{
    return bullet_pierce;
}
//SHOTGUD DATE
void Weapon::set_spread(const double p_spread){
    spread = p_spread;
}
double Weapon::get_spread() const{
    return spread;
}

void Weapon::set_shot_amount(const int p_amount) {
    shot_amount = p_amount;
}
int Weapon::get_shot_amount() const {
    return shot_amount;
}


#pragma endregion getters_setters