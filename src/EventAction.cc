// src/EventAction.cc
#include "EventAction.hh"
#include "RunAction.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>

EventAction::EventAction(RunAction* runAction)
    : G4UserEventAction(), fRunAction(runAction),
      fEventID(0), fTotalEnergy(0.0), 
      fNumElectrons(0), fDecayType(1) {}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event* event) {
    fEventID = event->GetEventID();
    fTotalEnergy = 0.0;
    fNumElectrons = 0;
    fDecayType = 1;  // Default to single beta
    
    // Print progress every 100 events
    if (fEventID % 100 == 0) {
        G4cout << "Processing event " << fEventID << "..." << G4endl;
    }
}

void EventAction::EndOfEventAction(const G4Event* event) {
    // Send data to RunAction for file output
    if (fNumElectrons > 0) {
        // Simple heuristic: >1 electron might be double beta
        if (fNumElectrons >= 2) {
            fDecayType = 2;  // Double beta
        }
        
        fRunAction->AddEventData(fTotalEnergy, "e-", fDecayType);
    }
}

void EventAction::AddTrack(G4double energy, G4String particleName,
                          G4double x, G4double y, G4double z,
                          G4int decayType) {
    fTotalEnergy += energy;
    
    if (particleName == "e-") {
        fNumElectrons++;
    }
    
    // Store decay type if provided
    if (decayType == 2) {
        fDecayType = 2;
    }
}