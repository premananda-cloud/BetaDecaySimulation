#ifndef BETADECAY_H
#define BETADECAY_H

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4RandomDirection.hh"
#include "globals.hh"
#include <vector>
#include <string>

// Forward declarations
class G4Event;
class G4ParticleDefinition;

//==============================================================================
// Enumerations for decay types
//==============================================================================

enum class BetaDecayType {
    BETA_MINUS,           // β⁻: n → p + e⁻ + ν̄ₑ
    BETA_PLUS,            // β⁺: p → n + e⁺ + νₑ
    ELECTRON_CAPTURE,     // EC: p + e⁻ → n + νₑ
    DOUBLE_BETA_MINUS,    // ββ⁻ (2ν): 2n → 2p + 2e⁻ + 2ν̄ₑ
    DOUBLE_BETA_PLUS,     // ββ⁺ (2ν): 2p → 2n + 2e⁺ + 2νₑ
    DOUBLE_BETA_0NU       // ββ⁻ (0ν): 2n → 2p + 2e⁻ (neutrinoless)
};

//==============================================================================
// Structure to hold nucleus information
//==============================================================================

struct Nucleus {
    G4int Z;              // Atomic number (protons)
    G4int A;              // Mass number (nucleons)
    G4double excitation;  // Excitation energy in keV
    G4String name;        // Element name/symbol
    
    Nucleus() : Z(0), A(0), excitation(0.0), name("") {}
    Nucleus(G4int z, G4int a, G4double exc = 0.0, G4String n = "") 
        : Z(z), A(a), excitation(exc), name(n) {}
};

//==============================================================================
// Structure to hold decay particle information
//==============================================================================

struct DecayParticle {
    G4String particleName;
    G4double energy;           // Kinetic energy in MeV
    G4ThreeVector momentum;    // Momentum vector in MeV/c
    G4ThreeVector direction;   // Direction unit vector
    
    DecayParticle() : particleName(""), energy(0.0) {}
    DecayParticle(G4String name, G4double e) 
        : particleName(name), energy(e) {}
};

//==============================================================================
// Structure to hold complete decay event information
//==============================================================================

struct DecayEvent {
    BetaDecayType decayType;
    Nucleus parentNucleus;
    Nucleus daughterNucleus;
    std::vector<DecayParticle> particles;
    G4double qValue;           // Q-value in MeV
    G4double totalEnergy;      // Total kinetic energy
    G4bool isValid;
    
    DecayEvent() : qValue(0.0), totalEnergy(0.0), isValid(false) {}
    
    void Print() const;
    G4double GetTotalEnergy() const;
};

//==============================================================================
// Primary Generator Action for Beta Decay
//==============================================================================

class BetaDecayPrimaryGenerator : public G4VUserPrimaryGeneratorAction {
public:
    BetaDecayPrimaryGenerator();
    virtual ~BetaDecayPrimaryGenerator();
    
    virtual void GeneratePrimaries(G4Event* event) override;
    
    // Setters for decay configuration
    void SetDecayType(BetaDecayType type) { fDecayType = type; }
    void SetParentNucleus(G4int Z, G4int A, G4double excitation = 0.0);
    void SetQValue(G4double qval) { fQValue = qval; }
    void SetSourcePosition(G4ThreeVector pos) { fSourcePosition = pos; }
    
    // Getters
    BetaDecayType GetDecayType() const { return fDecayType; }
    Nucleus GetParentNucleus() const { return fParentNucleus; }
    G4double GetQValue() const { return fQValue; }
    
    // Single beta decay generators
    void GenerateBetaMinus(G4Event* event);
    void GenerateBetaPlus(G4Event* event);
    void GenerateElectronCapture(G4Event* event);
    
    // Double beta decay generators
    void GenerateDoubleBetaMinus(G4Event* event);
    void GenerateDoubleBetaPlus(G4Event* event);
    void GenerateDoubleBeta0Nu(G4Event* event);
    
private:
    G4ParticleGun* fParticleGun;
    BetaDecayType fDecayType;
    Nucleus fParentNucleus;
    Nucleus fDaughterNucleus;
    G4double fQValue;              // Q-value in MeV
    G4ThreeVector fSourcePosition; // Source position
    
    // Helper functions for energy distributions
    G4double SampleBetaSpectrum(G4double qValue, G4int Z);
    G4double FermiFunction(G4double energy, G4int Z);
    void GenerateDecayParticles(std::vector<DecayParticle>& particles);
    void UpdateDaughterNucleus();
    
    // Particle definitions (cached for performance)
    G4ParticleDefinition* fElectron;
    G4ParticleDefinition* fPositron;
    G4ParticleDefinition* fNeutrino;
    G4ParticleDefinition* fAntiNeutrino;
};

//==============================================================================
// Beta Decay Physics Helper Class
//==============================================================================

class BetaDecayPhysics {
public:
    // Q-value calculations
    static G4double CalculateQValue(const Nucleus& parent, const Nucleus& daughter, 
                                     BetaDecayType type);
    
    // Check if decay is energetically allowed
    static G4bool IsDecayAllowed(const Nucleus& parent, BetaDecayType type, 
                                  G4double qValue);
    
    // Get nuclear mass (simplified using semi-empirical mass formula)
    static G4double GetNuclearMass(G4int Z, G4int A);
    
    // Beta spectrum shape function
    static G4double BetaSpectrumShape(G4double energy, G4double qValue, G4int Z);
    
    // Fermi function for Coulomb corrections
    static G4double FermiFunction(G4double electronEnergy, G4int Z);
    
    // Get element symbol from Z
    static G4String GetElementSymbol(G4int Z);
    
    // Constants
    static constexpr G4double ELECTRON_MASS = 0.510998928;  // MeV
    static constexpr G4double NEUTRON_MASS = 939.565379;    // MeV
    static constexpr G4double PROTON_MASS = 938.272046;     // MeV
    static constexpr G4double ALPHA = 1.0/137.035999;       // Fine structure constant
};

//==============================================================================
// Common Isotopes for Beta Decay Studies
//==============================================================================

namespace BetaDecayIsotopes {
    // Single beta decay isotopes
    const Nucleus C14(6, 14, 0.0, "C-14");      // Q = 0.156 MeV
    const Nucleus Na22(11, 22, 0.0, "Na-22");   // Q = 2.842 MeV
    const Nucleus K40(19, 40, 0.0, "K-40");     // Q = 1.505 MeV
    
    // Double beta decay isotopes
    const Nucleus Ge76(32, 76, 0.0, "Ge-76");   // Q = 2.039 MeV
    const Nucleus Se82(34, 82, 0.0, "Se-82");   // Q = 2.995 MeV
    const Nucleus Xe136(54, 136, 0.0, "Xe-136"); // Q = 2.458 MeV
    const Nucleus Te130(52, 130, 0.0, "Te-130"); // Q = 2.527 MeV
    const Nucleus Mo100(42, 100, 0.0, "Mo-100"); // Q = 3.034 MeV
    
    // Q-values (in MeV)
    namespace QValues {
        constexpr G4double C14_BETA_MINUS = 0.156;
        constexpr G4double Na22_BETA_PLUS = 2.842;
        constexpr G4double K40_EC = 1.505;
        constexpr G4double Ge76_DBD = 2.039;
        constexpr G4double Se82_DBD = 2.995;
        constexpr G4double Xe136_DBD = 2.458;
        constexpr G4double Te130_DBD = 2.527;
        constexpr G4double Mo100_DBD = 3.034;
    }
}

//==============================================================================
// Utility Functions
//==============================================================================

namespace BetaDecayUtils {
    // Print decay event information
    void PrintDecayInfo(const DecayEvent& event);
    
    // Convert decay type to string
    G4String DecayTypeToString(BetaDecayType type);
    
    // Get daughter nucleus from parent and decay type
    Nucleus GetDaughterNucleus(const Nucleus& parent, BetaDecayType type);
}

#endif // BETADECAY_H
