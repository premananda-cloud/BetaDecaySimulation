// src/ActionInitialization.cc
#include "ActionInitialization.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "BetaDecay.h"

ActionInitialization::ActionInitialization() {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::Build() const {
    // Create primary generator
    BetaDecay* primaryGenerator = new BetaDecay();
    SetUserAction(primaryGenerator);
    
    // Create run action
    RunAction* runAction = new RunAction();
    SetUserAction(runAction);
    
    // Create event action (pass run action for data collection)
    EventAction* eventAction = new EventAction(runAction);
    SetUserAction(eventAction);
}

void ActionInitialization::BuildForMaster() const {
    // For multi-threaded runs (master thread)
    RunAction* runAction = new RunAction();
    SetUserAction(runAction);
}