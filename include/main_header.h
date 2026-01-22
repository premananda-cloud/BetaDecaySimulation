#ifndef BETADECAY_H
#define BETADECAY_H

#include <string>
#include <vector>
#include <random>
#include <cmath>

/**
 * Enum for beta decay types
 */
enum class BetaDecayType {
    BETA_MINUS,           // β⁻ decay: n → p + e⁻ + ν̄ₑ
    BETA_PLUS,            // β⁺ decay: p → n + e⁺ + νₑ
    ELECTRON_CAPTURE,     // EC: p + e⁻ → n + νₑ
    DOUBLE_BETA_MINUS,    // ββ⁻: 2n → 2p + 2e⁻ + 2ν̄ₑ (2ν mode)
    DOUBLE_BETA_PLUS,     // ββ⁺: 2p → 2n + 2e⁺ + 2νₑ (2ν mode)
    DOUBLE_BETA_MINUS_0NU // ββ⁻ (0ν): neutrinoless double beta decay
};

/**
 * Structure to represent a nucleus
 */
struct Nucleus {
    int atomicNumber;      // Z (number of protons)
    int massNumber;        // A (total nucleons)
    double energy;         // Energy in MeV
    std::string symbol;    // Element symbol
    
    Nucleus(int z, int a, const std::string& sym = "");
    void display() const;
    double getBindingEnergy() const;
};

/**
 * Structure to represent decay products
 */
struct DecayProduct {
    std::string particle;
    double energy;         // Kinetic energy in MeV
    double momentum[3];    // Momentum components (px, py, pz) in MeV/c
    
    DecayProduct(const std::string& p = "", double e = 0.0);
};

/**
 * Structure to hold decay event results
 */
struct DecayEvent {
    BetaDecayType decayType;
    Nucleus parentNucleus;
    Nucleus daughterNucleus;
    std::vector<DecayProduct> products;
    double qValue;         // Q-value of the decay in MeV
    double decayTime;      // Time of decay in seconds
    bool isSuccessful;
    
    DecayEvent();
    void display() const;
    double getTotalEnergy() const;
};

/**
 * Main Beta Decay Simulator class
 */
class BetaDecaySimulator {
private:
    std::mt19937 generator;
    std::uniform_real_distribution<double> uniform;
    
    // Physics constants
    static constexpr double ELECTRON_MASS = 0.511;  // MeV/c²
    static constexpr double NEUTRON_MASS = 939.565; // MeV/c²
    static constexpr double PROTON_MASS = 938.272;  // MeV/c²
    static constexpr double C = 299792458.0;        // m/s
    static constexpr double HBAR = 6.582119569e-22; // MeV·s
    
    // Helper functions for energy distributions
    double fermiFunction(double electronEnergy, int Z);
    double betaSpectrum(double electronEnergy, double qValue, int Z);
    std::vector<double> generateMomentum(double energy, double mass);
    void conserveMomentumEnergy(std::vector<DecayProduct>& products, double qValue);
    
public:
    BetaDecaySimulator(unsigned int seed = std::random_device{}());
    
    // Single beta decay simulations
    DecayEvent simulateBetaMinus(const Nucleus& parent, double qValue);
    DecayEvent simulateBetaPlus(const Nucleus& parent, double qValue);
    DecayEvent simulateElectronCapture(const Nucleus& parent, double qValue);
    
    // Double beta decay simulations
    DecayEvent simulateDoubleBetaMinus(const Nucleus& parent, double qValue);
    DecayEvent simulateDoubleBetaPlus(const Nucleus& parent, double qValue);
    DecayEvent simulateDoubleBetaMinus0Nu(const Nucleus& parent, double qValue);
    
    // General simulation function
    DecayEvent simulate(const Nucleus& parent, BetaDecayType type, double qValue);
    
    // Utility functions
    double calculateHalfLife(double decayConstant) const;
    double generateDecayTime(double halfLife);
    bool canDecay(const Nucleus& parent, BetaDecayType type) const;
    
    // Statistical analysis
    std::vector<DecayEvent> runMultipleDecays(const Nucleus& parent, 
                                               BetaDecayType type, 
                                               double qValue, 
                                               int numEvents);
    void analyzeEnergyDistribution(const std::vector<DecayEvent>& events) const;
};

/**
 * Helper class for nuclear data
 */
class NuclearData {
public:
    static double getAtomicMass(int Z, int A);
    static std::string getElementSymbol(int Z);
    static double getQValue(const Nucleus& parent, const Nucleus& daughter, BetaDecayType type);
    static bool isStable(int Z, int A);
};

#endif // BETADECAY_H