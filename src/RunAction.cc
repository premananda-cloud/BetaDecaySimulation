// src/RunAction.cc
#include "RunAction.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>

RunAction::RunAction() 
    : totalEvents(0), totalEnergy(0.0),
      singleBetaCount(0), doubleBetaCount(0) {}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run* run) {
    G4cout << "### Run " << run->GetRunID() << " started." << G4endl;
    
    // Open output file
    outputFile.open("beta_decay_output.txt");
    if (!outputFile.is_open()) {
        G4cerr << "ERROR: Could not open output file!" << G4endl;
        return;
    }
    
    // Write header
    outputFile << "# Beta Decay Simulation Output" << std::endl;
    outputFile << "# Event | Particle | Energy (MeV) | DecayType | X | Y | Z" << std::endl;
    outputFile << "# DecayType: 1=SingleBeta, 2=DoubleBeta" << std::endl;
    outputFile << "########################################" << std::endl;
    
    // Reset counters
    totalEvents = 0;
    totalEnergy = 0.0;
    singleBetaCount = 0;
    doubleBetaCount = 0;
}

void RunAction::EndOfRunAction(const G4Run* run) {
    G4cout << "### Run " << run->GetRunID() << " ended." << G4endl;
    G4cout << "  Total events: " << totalEvents << G4endl;
    G4cout << "  Single beta decays: " << singleBetaCount << G4endl;
    G4cout << "  Double beta decays: " << doubleBetaCount << G4endl;
    if (totalEvents > 0) {
        G4cout << "  Average energy per event: " << totalEnergy/totalEvents << " MeV" << G4endl;
    }
    
    // Write summary to file
    outputFile << "########################################" << std::endl;
    outputFile << "# SUMMARY" << std::endl;
    outputFile << "# Total events: " << totalEvents << std::endl;
    outputFile << "# Single beta decays: " << singleBetaCount << std::endl;
    outputFile << "# Double beta decays: " << doubleBetaCount << std::endl;
    if (totalEvents > 0) {
        outputFile << "# Average energy: " << totalEnergy/totalEvents << " MeV" << std::endl;
    }
    
    // Close file
    outputFile.close();
    
    G4cout << "Output saved to: beta_decay_output.txt" << G4endl;
}

void RunAction::AddEventData(G4double energy, G4String particle, G4int decayType) {
    totalEvents++;
    totalEnergy += energy;
    
    if (decayType == 1) singleBetaCount++;
    else if (decayType == 2) doubleBetaCount++;
}