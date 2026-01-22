// include/EventAction.hh
#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include "G4UserEventAction.hh"
#include "globals.hh"

class RunAction;

class EventAction : public G4UserEventAction {
public:
    EventAction(RunAction* runAction);
    virtual ~EventAction();
    
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
    
    // Methods to collect data during event
    void AddTrack(G4double energy, G4String particleName, 
                  G4double x, G4double y, G4double z,
                  G4int decayType = 1);
    
private:
    RunAction* fRunAction;
    G4int fEventID;
    
    // Event data
    G4double fTotalEnergy;
    G4int fNumElectrons;
    G4int fDecayType;
};

#endif // EVENTACTION_HH