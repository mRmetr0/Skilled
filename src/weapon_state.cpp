// #include "weapon_state.h"

// using namespace godot;

// void WeaponState::start(Node& node, int p_start_clip = 0) {
//     if (p_start_clip > 0)
//         bullet_amount = p_start_clip;
//     else 
//         bullet_amount = magazine_size;

//     bullet_pool = Object::cast_to<Node>(node.get_node_or_null("/root/BulletPool"));
//     input = Input::get_singleton();
// }
// void WeaponState::update(Node2D& node,double delta) {
//     if (input->is_action_just_pressed("RClick")){
//         shoot(node.get_position(), (node.get_global_mouse_position() - node.get_position()).angle());
//     } else if (input->is_action_just_pressed("reload")){
//         reload();
//     }

//     if (!reloading) return;
//     reload_time += delta;
//     if (reload_time >= reload_duration){
//         bullet_amount = magazine_size;
//         reloading = false;
//     }   
    
// }
// void WeaponState::shoot(Vector2 position, double angle){
//     if (reloading) return;

//     for (int i = 0; i < shot_amount; i++){
//         //setting up everything:
//         float offset = (i-(shot_amount/2))*spread;
//         Node2D* init = cast_to<Node2D>(bullet_pool->call("_get_bullet"));
//         init->call("_set_bullet", bullet_damage, bullet_pierce, crit_rate);

//         //Marking everything down:
//         init->set_position(position);
//         init->set_rotation(angle + offset);
//     }
    
//     //managing bullet amout:
//     bullet_amount--;
//     if (bullet_amount == 0) 
//         reload();
// }
// void WeaponState::reload(){
//     if (bullet_amount == magazine_size || reloading) return;
//     reloading = true;
//     reload_time = 0.0;
//     // emit_signal("reload", reload_duration);
// }