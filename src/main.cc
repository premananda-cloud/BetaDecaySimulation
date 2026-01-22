#include "include/BetaDecay.h"
#include <iostream>
#include <iomanip>

void printHeader(const std::string& title) {
    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << title << "\n";
    std::cout << std::string(80, '=') << "\n\n";
}

void demonstrateSingleBetaDecay() {
    printHeader("SINGLE BETA DECAY SIMULATIONS");
    
    BetaDecaySimulator simulator;
    
    // Example 1: Carbon-14 beta minus decay (14C → 14N + e⁻ + ν̄ₑ)
    std::cout << "Example 1: Carbon-14 Beta Minus Decay\n";
    std::cout << std::string(80, '-') << "\n";
    Nucleus carbon14(6, 14, "C");
    double qValue_C14 = 0.156; // MeV
    
    DecayEvent event1 = simulator.simulateBetaMinus(carbon14, qValue_C14);
    event1.display();
    
    // Example 2: Sodium-22 beta plus decay (22Na → 22Ne + e⁺ + νₑ)
    std::cout << "\n\nExample 2: Sodium-22 Beta Plus Decay\n";
    std::cout << std::string(80, '-') << "\n";
    Nucleus sodium22(11, 22, "Na");
    double qValue_Na22 = 2.842; // MeV
    
    DecayEvent event2 = simulator.simulateBetaPlus(sodium22, qValue_Na22);
    event2.display();
    
    // Example 3: Potassium-40 electron capture (40K + e⁻ → 40Ar + νₑ)
    std::cout << "\n\nExample 3: Potassium-40 Electron Capture\n";
    std::cout << std::string(80, '-') << "\n";
    Nucleus potassium40(19, 40, "K");
    double qValue_K40 = 1.505; // MeV
    
    DecayEvent event3 = simulator.simulateElectronCapture(potassium40, qValue_K40);
    event3.display();
}

void demonstrateDoubleBetaDecay() {
    printHeader("DOUBLE BETA DECAY SIMULATIONS");
    
    BetaDecaySimulator simulator;
    
    // Example 1: Germanium-76 double beta minus decay (76Ge → 76Se + 2e⁻ + 2ν̄ₑ)
    std::cout << "Example 1: Germanium-76 Double Beta Minus Decay (2ν mode)\n";
    std::cout << std::string(80, '-') << "\n";
    Nucleus germanium76(32, 76, "Ge");
    double qValue_Ge76 = 2.039; // MeV
    
    DecayEvent event1 = simulator.simulateDoubleBetaMinus(germanium76, qValue_Ge76);
    event1.display();
    
    // Example 2: Selenium-82 double beta minus decay (82Se → 82Kr + 2e⁻ + 2ν̄ₑ)
    std::cout << "\n\nExample 2: Selenium-82 Double Beta Minus Decay (2ν mode)\n";
    std::cout << std::string(80, '-') << "\n";
    Nucleus selenium82(34, 82, "Se");
    double qValue_Se82 = 2.995; // MeV
    
    DecayEvent event2 = simulator.simulateDoubleBetaMinus(selenium82, qValue_Se82);
    event2.display();
    
    // Example 3: Xenon-136 double beta minus decay (136Xe → 136Ba + 2e⁻ + 2ν̄ₑ)
    std::cout << "\n\nExample 3: Xenon-136 Double Beta Minus Decay (2ν mode)\n";
    std::cout << std::string(80, '-') << "\n";
    Nucleus xenon136(54, 136, "Xe");
    double qValue_Xe136 = 2.458; // MeV
    
    DecayEvent event3 = simulator.simulateDoubleBetaMinus(xenon136, qValue_Xe136);
    event3.display();
}

void demonstrateNeutrinolessDoubleBetaDecay() {
    printHeader("NEUTRINOLESS DOUBLE BETA DECAY SIMULATION (0ν mode)");
    
    BetaDecaySimulator simulator;
    
    std::cout << "Neutrinoless Double Beta Decay is a hypothetical process\n";
    std::cout << "that violates lepton number conservation.\n";
    std::cout << "If observed, it would prove neutrinos are Majorana particles.\n\n";
    
    // Example: Germanium-76 neutrinoless double beta decay
    std::cout << "Example: Germanium-76 Neutrinoless Double Beta Decay (0ν mode)\n";
    std::cout << std::string(80, '-') << "\n";
    Nucleus germanium76(32, 76, "Ge");
    double qValue_Ge76 = 2.039; // MeV
    
    DecayEvent event = simulator.simulateDoubleBetaMinus0Nu(germanium76, qValue_Ge76);
    event.display();
    
    std::cout << "\nKey difference from 2ν mode:\n";
    std::cout << "  - No neutrinos emitted\n";
    std::cout << "  - All decay energy shared between two electrons\n";
    std::cout << "  - Sum of electron energies equals Q-value\n";
}

void demonstrateStatisticalAnalysis() {
    printHeader("STATISTICAL ANALYSIS OF MULTIPLE DECAY EVENTS");
    
    BetaDecaySimulator simulator;
    
    std::cout << "Running 1000 Carbon-14 beta decay simulations...\n\n";
    
    Nucleus carbon14(6, 14, "C");
    double qValue = 0.156; // MeV
    
    auto events = simulator.runMultipleDecays(carbon14, BetaDecayType::BETA_MINUS, qValue, 1000);
    
    simulator.analyzeEnergyDistribution(events);
    
    // Analyze electron energy distribution
    std::cout << "\nElectron Energy Distribution (first 10 events):\n";
    std::cout << std::string(80, '-') << "\n";
    std::cout << std::setw(10) << "Event #" 
              << std::setw(20) << "Electron Energy" 
              << std::setw(20) << "Neutrino Energy" << "\n";
    std::cout << std::string(80, '-') << "\n";
    
    for (size_t i = 0; i < std::min(size_t(10), events.size()); ++i) {
        std::cout << std::setw(10) << i + 1
                  << std::setw(20) << std::fixed << std::setprecision(6) 
                  << events[i].products[0].energy
                  << std::setw(20) << events[i].products[1].energy << "\n";
    }
}

void demonstrateDecayComparison() {
    printHeader("COMPARISON: 2ν vs 0ν DOUBLE BETA DECAY");
    
    BetaDecaySimulator simulator;
    Nucleus germanium76(32, 76, "Ge");
    double qValue = 2.039; // MeV
    
    std::cout << "Running 5 events of each type for comparison:\n\n";
    
    // 2ν mode
    std::cout << "Two-Neutrino Mode (2ν):\n";
    std::cout << std::string(80, '-') << "\n";
    auto events_2nu = simulator.runMultipleDecays(germanium76, BetaDecayType::DOUBLE_BETA_MINUS, qValue, 5);
    
    for (size_t i = 0; i < events_2nu.size(); ++i) {
        std::cout << "Event " << i + 1 << ": ";
        std::cout << "e1=" << std::fixed << std::setprecision(4) << events_2nu[i].products[0].energy << " MeV, ";
        std::cout << "e2=" << events_2nu[i].products[1].energy << " MeV, ";
        std::cout << "ν1=" << events_2nu[i].products[2].energy << " MeV, ";
        std::cout << "ν2=" << events_2nu[i].products[3].energy << " MeV | ";
        std::cout << "Total e⁻: " << (events_2nu[i].products[0].energy + events_2nu[i].products[1].energy) << " MeV\n";
    }
    
    // 0ν mode
    std::cout << "\nZero-Neutrino Mode (0ν - Neutrinoless):\n";
    std::cout << std::string(80, '-') << "\n";
    auto events_0nu = simulator.runMultipleDecays(germanium76, BetaDecayType::DOUBLE_BETA_MINUS_0NU, qValue, 5);
    
    for (size_t i = 0; i < events_0nu.size(); ++i) {
        std::cout << "Event " << i + 1 << ": ";
        std::cout << "e1=" << std::fixed << std::setprecision(4) << events_0nu[i].products[0].energy << " MeV, ";
        std::cout << "e2=" << events_0nu[i].products[1].energy << " MeV | ";
        std::cout << "Total e⁻: " << (events_0nu[i].products[0].energy + events_0nu[i].products[1].energy) << " MeV";
        std::cout << " (Q-value: " << qValue << " MeV)\n";
    }
    
    std::cout << "\nKey Observation:\n";
    std::cout << "  - In 2ν mode: electron energies vary, some energy goes to neutrinos\n";
    std::cout << "  - In 0ν mode: sum of electron energies always equals Q-value\n";
}

int main() {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    BETA DECAY SIMULATION PROGRAM                           ║\n";
    std::cout << "║                Single and Double Beta Decay Simulator                      ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════════════════╝\n";
    
    int choice;
    bool running = true;
    
    while (running) {
        std::cout << "\n\nSelect simulation type:\n";
        std::cout << "  1. Single Beta Decay (β⁻, β⁺, EC)\n";
        std::cout << "  2. Double Beta Decay (ββ 2ν mode)\n";
        std::cout << "  3. Neutrinoless Double Beta Decay (ββ 0ν mode)\n";
        std::cout << "  4. Statistical Analysis (1000 events)\n";
        std::cout << "  5. Comparison: 2ν vs 0ν Double Beta Decay\n";
        std::cout << "  6. Run All Demonstrations\n";
        std::cout << "  0. Exit\n";
        std::cout << "\nEnter choice: ";
        
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                demonstrateSingleBetaDecay();
                break;
            case 2:
                demonstrateDoubleBetaDecay();
                break;
            case 3:
                demonstrateNeutrinolessDoubleBetaDecay();
                break;
            case 4:
                demonstrateStatisticalAnalysis();
                break;
            case 5:
                demonstrateDecayComparison();
                break;
            case 6:
                demonstrateSingleBetaDecay();
                demonstrateDoubleBetaDecay();
                demonstrateNeutrinolessDoubleBetaDecay();
                demonstrateStatisticalAnalysis();
                demonstrateDecayComparison();
                break;
            case 0:
                running = false;
                std::cout << "\nThank you for using the Beta Decay Simulator!\n";
                break;
            default:
                std::cout << "\nInvalid choice. Please try again.\n";
        }
    }
    
    return 0;
}