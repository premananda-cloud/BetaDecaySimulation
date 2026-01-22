// include/RunAction.hh
#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <fstream>

class G4Run;

class RunAction : public G4UserRunAction {
public:
    RunAction();
    virtual ~RunAction();
    
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
    
    // Methods to collect data from EventAction
    void AddEventData(G4double energy, G4String particle, G4int decayType);
    
private:
    std::ofstream outputFile;
    G4int totalEvents;
    G4double totalEnergy;
    
    // Counters for different decay types
    G4int singleBetaCount;
    G4int doubleBetaCount;
};

#endif // RUNACTION_HH