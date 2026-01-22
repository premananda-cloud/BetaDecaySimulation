// src/BetaDecay.cc - Minimal Geant4 Implementation
#include "BetaDecay.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

BetaDecayPrimaryGenerator::BetaDecayPrimaryGenerator() {
    fParticleGun = new G4ParticleGun(1);
    
    // Default: Carbon-14 beta decay
    fDecayType = BetaDecayType::BETA_MINUS;
    fParentNucleus = BetaDecayIsotopes::C14;
    fQValue = BetaDecayIsotopes::QValues::C14_BETA_MINUS * MeV;
    fSourcePosition = G4ThreeVector(0., 0., 0.);
}

BetaDecayPrimaryGenerator::~BetaDecayPrimaryGenerator() {
    delete fParticleGun;
}

void BetaDecayPrimaryGenerator::GeneratePrimaries(G4Event* event) {
    // Simple: generate one electron
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* electron = particleTable->FindParticle("e-");
    
    fParticleGun->SetParticleDefinition(electron);
    
    // Random energy up to Q-value
    G4double energy = G4UniformRand() * fQValue;
    fParticleGun->SetParticleEnergy(energy);
    
    // Source position
    fParticleGun->SetParticlePosition(fSourcePosition);
    
    // Random direction
    fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
    
    // Generate
    fParticleGun->GeneratePrimaryVertex(event);
    
    // Print info
    G4cout << "Generated beta decay electron: " 
           << energy/MeV << " MeV" << G4endl;
}

void BetaDecayPrimaryGenerator::SetParentNucleus(G4int Z, G4int A, G4double excitation) {
    fParentNucleus = Nucleus(Z, A, excitation);
}

// Stub implementations for other required methods
void BetaDecayPrimaryGenerator::GenerateBetaMinus(G4Event* event) { GeneratePrimaries(event); }
void BetaDecayPrimaryGenerator::GenerateBetaPlus(G4Event* event) { GeneratePrimaries(event); }
void BetaDecayPrimaryGenerator::GenerateElectronCapture(G4Event* event) {}
void BetaDecayPrimaryGenerator::GenerateDoubleBetaMinus(G4Event* event) {}
void BetaDecayPrimaryGenerator::GenerateDoubleBetaPlus(G4Event* event) {}
void BetaDecayPrimaryGenerator::GenerateDoubleBeta0Nu(G4Event* event) {}
G4double BetaDecayPrimaryGenerator::SampleBetaSpectrum(G4double qValue, G4int Z) { return G4UniformRand() * qValue; }
G4double BetaDecayPrimaryGenerator::FermiFunction(G4double energy, G4int Z) { return 1.0; }
void BetaDecayPrimaryGenerator::GenerateDecayParticles(std::vector<DecayParticle>& particles) {}
void BetaDecayPrimaryGenerator::UpdateDaughterNucleus() {}
