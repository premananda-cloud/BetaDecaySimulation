#include "DetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

DetectorConstruction::DetectorConstruction() {}
DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    // Get material manager
    G4NistManager* nist = G4NistManager::Instance();
    
    // Create world volume
    G4double worldSize = 1.0*m;
    G4Material* worldMat = nist->FindOrBuildMaterial("G4_AIR");
    
    G4Box* solidWorld = new G4Box("World", worldSize/2, worldSize/2, worldSize/2);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "World");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0);
    
    // Create a simple detector (NaI scintillator)
    G4double detSize = 10.0*cm;
    G4Material* detMat = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");
    
    G4Box* solidDet = new G4Box("Detector", detSize/2, detSize/2, detSize/2);
    G4LogicalVolume* logicDet = new G4LogicalVolume(solidDet, detMat, "Detector");
    new G4PVPlacement(0, G4ThreeVector(0, 0, 20*cm), logicDet, "Detector", logicWorld, false, 0);
    
    // Create source volume (where beta decays happen)
    G4double sourceSize = 1.0*cm;
    G4Material* sourceMat = nist->FindOrBuildMaterial("G4_CARBON");  // Carbon-14 source
    
    G4Box* solidSource = new G4Box("Source", sourceSize/2, sourceSize/2, sourceSize/2);
    G4LogicalVolume* logicSource = new G4LogicalVolume(solidSource, sourceMat, "Source");
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicSource, "Source", logicWorld, false, 0);
    
    return physWorld;
}
