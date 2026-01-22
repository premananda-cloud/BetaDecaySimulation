#include "../include/BetaDecay.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>

// ============================================================================
// Nucleus Implementation
// ============================================================================

Nucleus::Nucleus(int z, int a, const std::string& sym) 
    : atomicNumber(z), massNumber(a), energy(0.0), symbol(sym) {
    if (symbol.empty()) {
        symbol = NuclearData::getElementSymbol(z);
    }
}

void Nucleus::display() const {
    std::cout << std::setw(3) << massNumber << symbol << std::setw(2) 
              << " (Z=" << atomicNumber << ", A=" << massNumber << ")";
}

double Nucleus::getBindingEnergy() const {
    // Semi-empirical mass formula (Weizsäcker formula)
    int N = massNumber - atomicNumber;
    double a_v = 15.75;  // Volume term
    double a_s = 17.8;   // Surface term
    double a_c = 0.711;  // Coulomb term
    double a_a = 23.7;   // Asymmetry term
    double a_p = 11.18;  // Pairing term
    
    double binding = a_v * massNumber
                   - a_s * std::pow(massNumber, 2.0/3.0)
                   - a_c * atomicNumber * (atomicNumber - 1) / std::pow(massNumber, 1.0/3.0)
                   - a_a * std::pow(N - atomicNumber, 2.0) / massNumber;
    
    // Pairing term
    if (atomicNumber % 2 == 0 && N % 2 == 0) {
        binding += a_p / std::sqrt(massNumber);
    } else if (atomicNumber % 2 == 1 && N % 2 == 1) {
        binding -= a_p / std::sqrt(massNumber);
    }
    
    return binding;
}

// ============================================================================
// DecayProduct Implementation
// ============================================================================

DecayProduct::DecayProduct(const std::string& p, double e) 
    : particle(p), energy(e) {
    momentum[0] = momentum[1] = momentum[2] = 0.0;
}

// ============================================================================
// DecayEvent Implementation
// ============================================================================

DecayEvent::DecayEvent() 
    : decayType(BetaDecayType::BETA_MINUS),
      parentNucleus(0, 0),
      daughterNucleus(0, 0),
      qValue(0.0),
      decayTime(0.0),
      isSuccessful(false) {}

void DecayEvent::display() const {
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "Beta Decay Event\n";
    std::cout << std::string(70, '=') << "\n";
    
    std::cout << "Decay Type: ";
    switch (decayType) {
        case BetaDecayType::BETA_MINUS:
            std::cout << "β⁻ decay (single)\n";
            break;
        case BetaDecayType::BETA_PLUS:
            std::cout << "β⁺ decay (single)\n";
            break;
        case BetaDecayType::ELECTRON_CAPTURE:
            std::cout << "Electron Capture\n";
            break;
        case BetaDecayType::DOUBLE_BETA_MINUS:
            std::cout << "ββ⁻ decay (2ν mode)\n";
            break;
        case BetaDecayType::DOUBLE_BETA_PLUS:
            std::cout << "ββ⁺ decay (2ν mode)\n";
            break;
        case BetaDecayType::DOUBLE_BETA_MINUS_0NU:
            std::cout << "ββ⁻ decay (0ν mode - neutrinoless)\n";
            break;
    }
    
    std::cout << "\nParent Nucleus: ";
    parentNucleus.display();
    std::cout << "\nDaughter Nucleus: ";
    daughterNucleus.display();
    std::cout << "\n\nQ-value: " << std::fixed << std::setprecision(3) 
              << qValue << " MeV\n";
    std::cout << "Decay Time: " << std::scientific << decayTime << " s\n";
    
    std::cout << "\nDecay Products:\n";
    std::cout << std::string(70, '-') << "\n";
    for (size_t i = 0; i < products.size(); ++i) {
        std::cout << std::setw(15) << products[i].particle 
                  << " | Energy: " << std::fixed << std::setprecision(4)
                  << std::setw(10) << products[i].energy << " MeV"
                  << " | Momentum: (" 
                  << std::setw(8) << products[i].momentum[0] << ", "
                  << std::setw(8) << products[i].momentum[1] << ", "
                  << std::setw(8) << products[i].momentum[2] << ") MeV/c\n";
    }
    
    std::cout << "\nTotal Energy: " << std::fixed << std::setprecision(4) 
              << getTotalEnergy() << " MeV\n";
    std::cout << std::string(70, '=') << "\n";
}

double DecayEvent::getTotalEnergy() const {
    double total = 0.0;
    for (const auto& product : products) {
        total += product.energy;
    }
    return total;
}

// ============================================================================
// BetaDecaySimulator Implementation
// ============================================================================

BetaDecaySimulator::BetaDecaySimulator(unsigned int seed) 
    : generator(seed), uniform(0.0, 1.0) {}

double BetaDecaySimulator::fermiFunction(double electronEnergy, int Z) {
    // Simplified Fermi function
    double pe = std::sqrt(electronEnergy * (electronEnergy + 2.0 * ELECTRON_MASS));
    double eta = 2.0 * M_PI * Z / 137.036; // Fine structure constant
    
    if (pe < 0.01) return 1.0;
    
    double fermi = eta * pe / (1.0 - std::exp(-eta * pe));
    return std::max(0.0, fermi);
}

double BetaDecaySimulator::betaSpectrum(double electronEnergy, double qValue, int Z) {
    if (electronEnergy <= 0 || electronEnergy >= qValue) {
        return 0.0;
    }
    
    double pe = std::sqrt(electronEnergy * (electronEnergy + 2.0 * ELECTRON_MASS));
    double neutrino_energy = qValue - electronEnergy;
    
    double spectrum = pe * (electronEnergy + ELECTRON_MASS) * neutrino_energy * neutrino_energy;
    spectrum *= fermiFunction(electronEnergy, Z);
    
    return spectrum;
}

std::vector<double> BetaDecaySimulator::generateMomentum(double energy, double mass) {
    // Generate random momentum direction
    double cosTheta = 2.0 * uniform(generator) - 1.0;
    double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);
    double phi = 2.0 * M_PI * uniform(generator);
    
    // Calculate momentum magnitude
    double p = std::sqrt(energy * (energy + 2.0 * mass));
    
    std::vector<double> momentum(3);
    momentum[0] = p * sinTheta * std::cos(phi);
    momentum[1] = p * sinTheta * std::sin(phi);
    momentum[2] = p * cosTheta;
    
    return momentum;
}

void BetaDecaySimulator::conserveMomentumEnergy(std::vector<DecayProduct>& products, double qValue) {
    // Simple momentum conservation for multiple particles
    double totalPx = 0, totalPy = 0, totalPz = 0;
    
    for (size_t i = 0; i < products.size() - 1; ++i) {
        totalPx += products[i].momentum[0];
        totalPy += products[i].momentum[1];
        totalPz += products[i].momentum[2];
    }
    
    // Last particle balances momentum
    if (!products.empty()) {
        products.back().momentum[0] = -totalPx;
        products.back().momentum[1] = -totalPy;
        products.back().momentum[2] = -totalPz;
    }
}

DecayEvent BetaDecaySimulator::simulateBetaMinus(const Nucleus& parent, double qValue) {
    DecayEvent event;
    event.decayType = BetaDecayType::BETA_MINUS;
    event.parentNucleus = parent;
    event.daughterNucleus = Nucleus(parent.atomicNumber + 1, parent.massNumber);
    event.qValue = qValue;
    event.isSuccessful = true;
    
    // Generate electron energy using rejection sampling
    double electronEnergy = 0.0;
    double maxSpectrum = betaSpectrum(qValue / 2.0, qValue, parent.atomicNumber);
    
    do {
        electronEnergy = uniform(generator) * qValue;
    } while (uniform(generator) * maxSpectrum > betaSpectrum(electronEnergy, qValue, parent.atomicNumber));
    
    // Create decay products
    DecayProduct electron("electron (e⁻)", electronEnergy);
    auto electronMomentum = generateMomentum(electronEnergy, ELECTRON_MASS);
    electron.momentum[0] = electronMomentum[0];
    electron.momentum[1] = electronMomentum[1];
    electron.momentum[2] = electronMomentum[2];
    
    double neutrinoEnergy = qValue - electronEnergy;
    DecayProduct antineutrino("antineutrino (ν̄ₑ)", neutrinoEnergy);
    // Neutrino momentum balances electron momentum
    antineutrino.momentum[0] = -electronMomentum[0];
    antineutrino.momentum[1] = -electronMomentum[1];
    antineutrino.momentum[2] = -electronMomentum[2];
    
    event.products.push_back(electron);
    event.products.push_back(antineutrino);
    
    return event;
}

DecayEvent BetaDecaySimulator::simulateBetaPlus(const Nucleus& parent, double qValue) {
    DecayEvent event;
    event.decayType = BetaDecayType::BETA_PLUS;
    event.parentNucleus = parent;
    event.daughterNucleus = Nucleus(parent.atomicNumber - 1, parent.massNumber);
    event.qValue = qValue;
    event.isSuccessful = true;
    
    // Q-value must account for positron mass
    double availableEnergy = qValue - 2.0 * ELECTRON_MASS;
    if (availableEnergy <= 0) {
        event.isSuccessful = false;
        return event;
    }
    
    // Generate positron energy
    double positronEnergy = 0.0;
    double maxSpectrum = betaSpectrum(availableEnergy / 2.0, availableEnergy, -parent.atomicNumber);
    
    do {
        positronEnergy = uniform(generator) * availableEnergy;
    } while (uniform(generator) * maxSpectrum > betaSpectrum(positronEnergy, availableEnergy, -parent.atomicNumber));
    
    DecayProduct positron("positron (e⁺)", positronEnergy);
    auto positronMomentum = generateMomentum(positronEnergy, ELECTRON_MASS);
    positron.momentum[0] = positronMomentum[0];
    positron.momentum[1] = positronMomentum[1];
    positron.momentum[2] = positronMomentum[2];
    
    double neutrinoEnergy = availableEnergy - positronEnergy;
    DecayProduct neutrino("neutrino (νₑ)", neutrinoEnergy);
    neutrino.momentum[0] = -positronMomentum[0];
    neutrino.momentum[1] = -positronMomentum[1];
    neutrino.momentum[2] = -positronMomentum[2];
    
    event.products.push_back(positron);
    event.products.push_back(neutrino);
    
    return event;
}

DecayEvent BetaDecaySimulator::simulateElectronCapture(const Nucleus& parent, double qValue) {
    DecayEvent event;
    event.decayType = BetaDecayType::ELECTRON_CAPTURE;
    event.parentNucleus = parent;
    event.daughterNucleus = Nucleus(parent.atomicNumber - 1, parent.massNumber);
    event.qValue = qValue;
    event.isSuccessful = true;
    
    // All energy goes to neutrino (ignoring atomic binding energy for simplicity)
    DecayProduct neutrino("neutrino (νₑ)", qValue);
    auto neutrinoMomentum = generateMomentum(qValue, 0.0);
    neutrino.momentum[0] = neutrinoMomentum[0];
    neutrino.momentum[1] = neutrinoMomentum[1];
    neutrino.momentum[2] = neutrinoMomentum[2];
    
    event.products.push_back(neutrino);
    
    return event;
}

DecayEvent BetaDecaySimulator::simulateDoubleBetaMinus(const Nucleus& parent, double qValue) {
    DecayEvent event;
    event.decayType = BetaDecayType::DOUBLE_BETA_MINUS;
    event.parentNucleus = parent;
    event.daughterNucleus = Nucleus(parent.atomicNumber + 2, parent.massNumber);
    event.qValue = qValue;
    event.isSuccessful = true;
    
    // Generate energies for two electrons and two antineutrinos
    // For simplicity, using a simplified phase space distribution
    
    double e1 = uniform(generator) * qValue * 0.5;
    double e2 = uniform(generator) * (qValue - e1) * 0.5;
    double nu1 = uniform(generator) * (qValue - e1 - e2);
    double nu2 = qValue - e1 - e2 - nu1;
    
    // Create products
    DecayProduct electron1("electron₁ (e⁻)", e1);
    auto p1 = generateMomentum(e1, ELECTRON_MASS);
    electron1.momentum[0] = p1[0];
    electron1.momentum[1] = p1[1];
    electron1.momentum[2] = p1[2];
    
    DecayProduct electron2("electron₂ (e⁻)", e2);
    auto p2 = generateMomentum(e2, ELECTRON_MASS);
    electron2.momentum[0] = p2[0];
    electron2.momentum[1] = p2[1];
    electron2.momentum[2] = p2[2];
    
    DecayProduct antineutrino1("antineutrino₁ (ν̄ₑ)", nu1);
    auto p3 = generateMomentum(nu1, 0.0);
    antineutrino1.momentum[0] = p3[0];
    antineutrino1.momentum[1] = p3[1];
    antineutrino1.momentum[2] = p3[2];
    
    DecayProduct antineutrino2("antineutrino₂ (ν̄ₑ)", nu2);
    // Balance momentum
    antineutrino2.momentum[0] = -(p1[0] + p2[0] + p3[0]);
    antineutrino2.momentum[1] = -(p1[1] + p2[1] + p3[1]);
    antineutrino2.momentum[2] = -(p1[2] + p2[2] + p3[2]);
    
    event.products.push_back(electron1);
    event.products.push_back(electron2);
    event.products.push_back(antineutrino1);
    event.products.push_back(antineutrino2);
    
    return event;
}

DecayEvent BetaDecaySimulator::simulateDoubleBetaPlus(const Nucleus& parent, double qValue) {
    DecayEvent event;
    event.decayType = BetaDecayType::DOUBLE_BETA_PLUS;
    event.parentNucleus = parent;
    event.daughterNucleus = Nucleus(parent.atomicNumber - 2, parent.massNumber);
    event.qValue = qValue;
    
    double availableEnergy = qValue - 4.0 * ELECTRON_MASS;
    if (availableEnergy <= 0) {
        event.isSuccessful = false;
        return event;
    }
    event.isSuccessful = true;
    
    // Generate energies
    double e1 = uniform(generator) * availableEnergy * 0.5;
    double e2 = uniform(generator) * (availableEnergy - e1) * 0.5;
    double nu1 = uniform(generator) * (availableEnergy - e1 - e2);
    double nu2 = availableEnergy - e1 - e2 - nu1;
    
    DecayProduct positron1("positron₁ (e⁺)", e1);
    auto p1 = generateMomentum(e1, ELECTRON_MASS);
    positron1.momentum[0] = p1[0];
    positron1.momentum[1] = p1[1];
    positron1.momentum[2] = p1[2];
    
    DecayProduct positron2("positron₂ (e⁺)", e2);
    auto p2 = generateMomentum(e2, ELECTRON_MASS);
    positron2.momentum[0] = p2[0];
    positron2.momentum[1] = p2[1];
    positron2.momentum[2] = p2[2];
    
    DecayProduct neutrino1("neutrino₁ (νₑ)", nu1);
    auto p3 = generateMomentum(nu1, 0.0);
    neutrino1.momentum[0] = p3[0];
    neutrino1.momentum[1] = p3[1];
    neutrino1.momentum[2] = p3[2];
    
    DecayProduct neutrino2("neutrino₂ (νₑ)", nu2);
    neutrino2.momentum[0] = -(p1[0] + p2[0] + p3[0]);
    neutrino2.momentum[1] = -(p1[1] + p2[1] + p3[1]);
    neutrino2.momentum[2] = -(p1[2] + p2[2] + p3[2]);
    
    event.products.push_back(positron1);
    event.products.push_back(positron2);
    event.products.push_back(neutrino1);
    event.products.push_back(neutrino2);
    
    return event;
}

DecayEvent BetaDecaySimulator::simulateDoubleBetaMinus0Nu(const Nucleus& parent, double qValue) {
    DecayEvent event;
    event.decayType = BetaDecayType::DOUBLE_BETA_MINUS_0NU;
    event.parentNucleus = parent;
    event.daughterNucleus = Nucleus(parent.atomicNumber + 2, parent.massNumber);
    event.qValue = qValue;
    event.isSuccessful = true;
    
    // Neutrinoless double beta decay - all energy shared between two electrons
    double e1 = uniform(generator) * qValue;
    double e2 = qValue - e1;
    
    DecayProduct electron1("electron₁ (e⁻)", e1);
    auto p1 = generateMomentum(e1, ELECTRON_MASS);
    electron1.momentum[0] = p1[0];
    electron1.momentum[1] = p1[1];
    electron1.momentum[2] = p1[2];
    
    DecayProduct electron2("electron₂ (e⁻)", e2);
    // Momentum conservation
    electron2.momentum[0] = -p1[0];
    electron2.momentum[1] = -p1[1];
    electron2.momentum[2] = -p1[2];
    
    event.products.push_back(electron1);
    event.products.push_back(electron2);
    
    return event;
}

DecayEvent BetaDecaySimulator::simulate(const Nucleus& parent, BetaDecayType type, double qValue) {
    switch (type) {
        case BetaDecayType::BETA_MINUS:
            return simulateBetaMinus(parent, qValue);
        case BetaDecayType::BETA_PLUS:
            return simulateBetaPlus(parent, qValue);
        case BetaDecayType::ELECTRON_CAPTURE:
            return simulateElectronCapture(parent, qValue);
        case BetaDecayType::DOUBLE_BETA_MINUS:
            return simulateDoubleBetaMinus(parent, qValue);
        case BetaDecayType::DOUBLE_BETA_PLUS:
            return simulateDoubleBetaPlus(parent, qValue);
        case BetaDecayType::DOUBLE_BETA_MINUS_0NU:
            return simulateDoubleBetaMinus0Nu(parent, qValue);
        default:
            return DecayEvent();
    }
}

double BetaDecaySimulator::calculateHalfLife(double decayConstant) const {
    return std::log(2.0) / decayConstant;
}

double BetaDecaySimulator::generateDecayTime(double halfLife) {
    double decayConstant = std::log(2.0) / halfLife;
    double randomValue = uniform(generator);
    return -std::log(randomValue) / decayConstant;
}

bool BetaDecaySimulator::canDecay(const Nucleus& parent, BetaDecayType type) const {
    switch (type) {
        case BetaDecayType::BETA_MINUS:
            return parent.massNumber > parent.atomicNumber;
        case BetaDecayType::BETA_PLUS:
        case BetaDecayType::ELECTRON_CAPTURE:
            return parent.atomicNumber > 0;
        case BetaDecayType::DOUBLE_BETA_MINUS:
            return parent.massNumber - parent.atomicNumber >= 2;
        case BetaDecayType::DOUBLE_BETA_PLUS:
            return parent.atomicNumber >= 2;
        case BetaDecayType::DOUBLE_BETA_MINUS_0NU:
            return parent.massNumber - parent.atomicNumber >= 2;
        default:
            return false;
    }
}

std::vector<DecayEvent> BetaDecaySimulator::runMultipleDecays(
    const Nucleus& parent, BetaDecayType type, double qValue, int numEvents) {
    
    std::vector<DecayEvent> events;
    events.reserve(numEvents);
    
    for (int i = 0; i < numEvents; ++i) {
        DecayEvent event = simulate(parent, type, qValue);
        events.push_back(event);
    }
    
    return events;
}

void BetaDecaySimulator::analyzeEnergyDistribution(const std::vector<DecayEvent>& events) const {
    if (events.empty()) {
        std::cout << "No events to analyze.\n";
        return;
    }
    
    std::cout << "\n" << std::string(70, '=') << "\n";
    std::cout << "Energy Distribution Analysis\n";
    std::cout << std::string(70, '=') << "\n";
    std::cout << "Total events: " << events.size() << "\n\n";
    
    // Calculate statistics
    double totalEnergy = 0.0;
    double minEnergy = 1e9;
    double maxEnergy = 0.0;
    
    for (const auto& event : events) {
        double energy = event.getTotalEnergy();
        totalEnergy += energy;
        minEnergy = std::min(minEnergy, energy);
        maxEnergy = std::max(maxEnergy, energy);
    }
    
    double avgEnergy = totalEnergy / events.size();
    
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Average total energy: " << avgEnergy << " MeV\n";
    std::cout << "Minimum total energy: " << minEnergy << " MeV\n";
    std::cout << "Maximum total energy: " << maxEnergy << " MeV\n";
    std::cout << "Q-value: " << events[0].qValue << " MeV\n";
    std::cout << std::string(70, '=') << "\n";
}

// ============================================================================
// NuclearData Implementation
// ============================================================================

double NuclearData::getAtomicMass(int Z, int A) {
    // Simplified mass calculation using semi-empirical mass formula
    double mass = Z * PROTON_MASS + (A - Z) * NEUTRON_MASS;
    
    Nucleus nucleus(Z, A);
    double bindingEnergy = nucleus.getBindingEnergy();
    
    return mass - bindingEnergy;
}

std::string NuclearData::getElementSymbol(int Z) {
    static const std::string elements[] = {
        "n", "H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne",
        "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca",
        "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn",
        "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr",
        "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn",
        "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd",
        "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb",
        "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg",
        "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th",
        "Pa", "U"
    };
    
    if (Z >= 0 && Z < 92) {
        return elements[Z];
    }
    return "X";
}

double NuclearData::getQValue(const Nucleus& parent, const Nucleus& daughter, BetaDecayType type) {
    double parentMass = getAtomicMass(parent.atomicNumber, parent.massNumber);
    double daughterMass = getAtomicMass(daughter.atomicNumber, daughter.massNumber);
    
    double qValue = (parentMass - daughterMass);
    
    switch (type) {
        case BetaDecayType::BETA_MINUS:
            qValue -= ELECTRON_MASS;
            break;
        case BetaDecayType::BETA_PLUS:
            qValue -= 2.0 * ELECTRON_MASS;
            break;
        case BetaDecayType::DOUBLE_BETA_MINUS:
            qValue -= 2.0 * ELECTRON_MASS;
            break;
        case BetaDecayType::DOUBLE_BETA_PLUS:
            qValue -= 4.0 * ELECTRON_MASS;
            break;
        default:
            break;
    }
    
    return qValue;
}

bool NuclearData::isStable(int Z, int A) {
    // Very simplified stability check
    int N = A - Z;
    
    // Magic numbers
    static const int magicNumbers[] = {2, 8, 20, 28, 50, 82, 126};
    
    for (int magic : magicNumbers) {
        if (Z == magic || N == magic) {
            return true;
        }
    }
    
    // N/Z ratio check
    double ratio = static_cast<double>(N) / Z;
    if (Z < 20) {
        return ratio >= 0.95 && ratio <= 1.05;
    } else if (Z < 40) {
        return ratio >= 1.0 && ratio <= 1.3;
    } else {
        return ratio >= 1.2 && ratio <= 1.55;
    }
}