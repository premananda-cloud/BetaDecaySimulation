// src/PhysicsList.cc
#include "PhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "G4ProductionCutsTable.hh"
#include "G4EmParameters.hh"

PhysicsList::PhysicsList() {
    // Set verbosity for physics processes
    G4int verbosity = 1;
    
    // Register electromagnetic physics
    RegisterPhysics(new G4EmStandardPhysics(verbosity));
    
    // Register decay physics (important for beta decay!)
    RegisterPhysics(new G4DecayPhysics(verbosity));
    
    // Register radioactive decay (for beta decay sources)
    RegisterPhysics(new G4RadioactiveDecayPhysics(verbosity));
    
    // Optional: optical physics for scintillation detectors
    // RegisterPhysics(new G4OpticalPhysics(verbosity));
    
    // Set physics list verbosity
    SetVerboseLevel(verbosity);
}

PhysicsList::~PhysicsList() {}

void PhysicsList::SetCuts() {
    // Set cut values for particle tracking
    // Default cut value for gamma, e-, e+, protons
    G4double defaultCutValue = 0.7*mm;  // 0.7 mm cut
    SetCutValue(defaultCutValue, "gamma");
    SetCutValue(defaultCutValue, "e-");
    SetCutValue(defaultCutValue, "e+");
    SetCutValue(defaultCutValue, "proton");
    
    // Apply cuts to production thresholds
    DumpCutValuesTable();
}