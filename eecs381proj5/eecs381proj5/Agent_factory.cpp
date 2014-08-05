#include "Agent_factory.h"
#include "Peasant.h"
#include "Warriors.h"
#include "Utility.h"

using std::string;
using std::shared_ptr;

// Create and return the pointer to the specified Agent type. If the type
// is unrecognized, throws Error("Trying to create agent of unknown type!")
// The Agent is allocated with new, so some other component is resposible for deleting it.
shared_ptr<Agent> create_agent(const string& name, const string& type, Point location)
{
   if(type == "Soldier")
   {
      return shared_ptr<Agent>(new Soldier(name, location));
   }
   if(type == "Peasant")
   {
      return shared_ptr<Agent>(new Peasant(name, location));
   }
   if(type == "Archer")
   {
      return shared_ptr<Agent>(new Archer(name, location));
   }
   throw Error("Trying to create agent of unknown type!");
}
