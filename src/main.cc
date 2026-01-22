// src/main.cpp (UPDATED VERSION)
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4ScoringManager.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"
#include "BetaDecay.hh"

int main(int argc, char** argv) {
    // Detect interactive mode (if no arguments) and define UI session
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }
    
    // Construct the run manager
    auto* runManager = G4RunManagerFactory::CreateRunManager();
    
    // Enable scoring manager (optional, for advanced scoring)
    G4ScoringManager::GetScoringManager();
    
    // Set mandatory initialization classes
    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new PhysicsList());
    runManager->SetUserInitialization(new ActionInitialization());
    
    // Initialize G4 kernel
    runManager->Initialize();
    
    // Initialize visualization
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();
    
    // Get the pointer to the UI manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    
    // Process macro or start UI session
    if (!ui) {
        // batch mode: execute macro file
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    } else {
        // interactive mode
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }
    
    // Job termination
    delete visManager;
    delete runManager;
    
    return 0;
}