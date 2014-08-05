#include "Warriors.h"
#include "Model.h"
#include "Structure.h"
#include "Sim_object.h"
#include "Utility.h"
#include <iostream>

using std::string;
using std::shared_ptr;
using std::cout;
using std::endl;

static const double archer_range = 6.;
static const double initial_attack_range_c = 2.0;

Soldier::Soldier(const string &in_name, Point location) : Agent(in_name, location)
{
   attacking = false;
   attack_strength = initial_attack_strength_c;
   attack_range = initial_attack_range_c;
}

// update implements Soldier behavior
void Soldier::update()
{
   Agent::update();
   
   if(!is_alive())
   {
      return;
   }

   if(!attacking)
   {
      return;
   }
   
   if(attacking)
   {
      if(target.expired() || !target.lock()->is_alive())
      {
         cout << get_name() << ": Target is dead" << endl;
         attacking = false;
         target.reset();
         return;
      }

      if(cartesian_distance(get_location(), target.lock()->get_location()) > attack_range)
      {
         cout << get_name() << ": Target is now out of range" << endl;
         attacking = false;
         return;
      }

      cout << get_name() << ": Clang!" << endl;
      target.lock()->take_hit(attack_strength, shared_from_this());

      if(target.expired() || !target.lock()->is_alive())
      {         
         cout << get_name() << ": I triumph!" << endl;
         attacking = false;
         target.reset();
      }
   }
}

// Make this Soldier start attacking the target Agent.
// Throws an exception if the target is the same as this Agent,
// is out of range, or is not alive.
void Soldier::start_attacking(shared_ptr<Agent>target_ptr)
{
   if(target_ptr == shared_from_this())
   {
      throw Error(get_name() + ": I cannot attack myself!");
   }

   if(!target_ptr->is_alive())
   {
      throw Error(get_name() + ": Target is not alive!");
   }

   if(cartesian_distance(get_location(), target_ptr->get_location()) > attack_range)
   {
      throw Error(get_name() + ": Target is out of range!");
   }

   target = target_ptr;
   attacking = true;
   cout << get_name() << ": I'm attacking!" << endl;
}

// Overrides Agent's take_hit to counterattack when attacked.
void Soldier::take_hit(int attack_strength_, shared_ptr<Agent> attacker_ptr)
{
   lose_health(attack_strength_);

   if(attacking && !is_alive())
   {
      attacking = false;
      target.reset();
   }
   else if(!attacking && is_alive() && attacker_ptr->is_alive())
   {
      attacking = true;
      target = attacker_ptr;
      cout << get_name() << ": I'm attacking!" << endl;
   }
}

// Overrides Agent's stop to print a message
void Soldier::stop()
{
   cout << get_name() << ": Don't bother me" << endl;
}

// output information about the current state
void Soldier::describe() const
{
   cout << "Soldier ";
   Agent::describe();   
   if(attacking)
   {
      if(target.expired() || !target.lock()->is_alive())
      {
         cout << "   Attacking dead target" << endl;
      }
      else
      {
         cout << "   Attacking " << target.lock()->get_name() << endl;
      }
   }
   else
   {
      cout << "   Not attacking" << endl;
   }
}

Archer::Archer(const string& name, Point location)
               : Agent(name, location)
{
   attacking = false;
   attack_strength = initial_strength;
   range = archer_range;
}

//implements Archer behavior
void Archer::update()
{
   Agent::update();
   if(!is_alive())
   {
      return;
   }
   if(attacking)
   {
      if(target.expired() || !target.lock()->is_alive())
      {
         cout << get_name() << ": Target is dead" << endl;
         attacking = false;
         target.reset();
         return;
      }

      if(cartesian_distance(get_location(), target.lock()->get_location()) > range)
      {
         cout << get_name() << ": Target is now out of range" << endl;
         attacking = false;
         return;
      }

      cout << get_name() << ": Twang!" << endl;
      target.lock()->take_hit(attack_strength, shared_from_this());

      if(target.expired() || !target.lock()->is_alive())
      {         
         cout << get_name() << ": I triumph!" << endl;
         attacking = false;
         target.reset();
      }
   }
   
   if(!attacking)
   {
      shared_ptr<Agent> closest_Agent = Model::get_instance()->find_closest_Agent(get_name());
      if(!closest_Agent)
      {
         return;
      }
      else
      {
         if(cartesian_distance(closest_Agent->get_location(), get_location()) <= range)
         {
            target = closest_Agent;
            attacking = true;
            cout << get_name() << ": I'm attacking!" << endl;
         }
      }
   }
}

// Make this Soldier start attacking the target Agent.
// Throws an exception if the target is the same as this Agent,
// is out of range, or is not alive.
void Archer::start_attacking(shared_ptr<Agent> target_ptr)
{
   if(target_ptr == shared_from_this())
   {
      throw Error(get_name() + ": I cannot attack myself!");
   }

   if(!target_ptr->is_alive())
   {
      throw Error(get_name() + ": Target is not alive!");
   }

   if(cartesian_distance(get_location(), target_ptr->get_location()) > range)
   {
      throw Error(get_name() + ": Target is out of range!");
   }

   target = target_ptr;
   attacking = true;
   cout << get_name() << ": I'm attacking!" << endl;
}

//Overides Agent take hit to run away
//to the nearest structure
void Archer::take_hit(int attack_strength_, std::shared_ptr<Agent> attacker_ptr)
{
   lose_health(attack_strength_);

   if(attacking && !is_alive())
   {
      attacking = false;
      target.reset();
   }
   else if(attacker_ptr->is_alive())
   {
      shared_ptr<Structure> closest_Structure = Model::get_instance()->find_closest_Structure(get_name());
      cout << get_name() << ": I'm going to run away to " << closest_Structure->get_name() << endl;
      move_to(closest_Structure->get_location());
   }
}

//outputs a message instead of doing Agent's stop
void Archer::stop()
{
   cout << get_name() << ": Don't bother me" << endl;  
}
 
//desribes the current state of the Archer
void Archer::describe() const
{
   cout << "Archer ";
   Agent::describe();   
   if(attacking)
   {
      if(target.expired() || !target.lock()->is_alive())
      {
         cout << "   Attacking dead target" << endl;
      }
      else
      {
         cout << "   Attacking " << target.lock()->get_name() << endl;
      }
   }
   else
   {
      cout << "   Not attacking" << endl;
   }
}

