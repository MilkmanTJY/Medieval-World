#ifndef WARRIORS_H
#define WARRIORS_H

#include "Agent.h"
#include "Geometry.h"
#include <memory>

/*
A Soldier is an Agent that has attack and defense behaviors. It can be commanded
to start attacking another Agent and will continue the attack as long as 
it is alive and the target is alive and in range. If attacked, the Soldier will
start attacking its attacker.
*/

class Soldier : public Agent
{
public:
	Soldier(const std::string& in_name, Point in_location);
		
	// update implements Soldier behavior
	void update() override;
	
	// Make this Soldier start attacking the target Agent.
	// Throws an exception if the target is the same as this Agent,
	// is out of range, or is not alive.
	void start_attacking(std::shared_ptr<Agent> target_ptr) override;
	
	// Overrides Agent's take_hit to counterattack when attacked.
	void take_hit(int attack_strength_, std::shared_ptr<Agent> attacker_ptr) override;
	
	// Overrides Agent's stop to print a message
	void stop() override;

	// output information about the current state
	void describe() const override;

   //returns -1.0 since the soldier does not carry an amount
   double get_amount() override
      {return -1.0;}
private:
   bool attacking;
   int attack_strength;
   double attack_range;
   std::weak_ptr<Agent> target;
   static const int initial_attack_strength_c = 2;
};

class Archer : public Agent
{
   public:
      Archer(const std::string& name, Point location);

      //implements Archer behavior
      void update() override;

      // Make this Soldier start attacking the target Agent.
      // Throws an exception if the target is the same as this Agent,
      // is out of range, or is not alive.
      void start_attacking(std::shared_ptr<Agent> target_ptr) override;

      //Overides Agent take hit to run away
      //to the nearest structure
      void take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr) override;

      //Overrides agent stop to print a message
      void stop() override;

      //desribes the current state of the Archer
      void describe() const override;

      //disable get_amount for archer
      double get_amount() override
         {return -1.0;}
      
   private:
      bool attacking;
      int attack_strength;
      double range;
      std::weak_ptr<Agent> target;
      static const int initial_strength = 1;
};

#endif
