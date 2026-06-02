#include "g4/DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4ThreeVector.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4PVReplica.hh"

#include "g4/SensitiveDetector.hh"

namespace edu0bm512 {

G4LogicalVolume *
HomogeneousCalo::_construct_cell() {
    G4NistManager * nist = G4NistManager::Instance();

    auto solidCell = new G4Box("homoCell", cellSizeX/2, cellSizeY/2, cellDepth/2);
    auto logicCell = new G4LogicalVolume(solidCell
            , nist->FindOrBuildMaterial("G4_CESIUM_IODIDE"), "homoCell");
    return logicCell;
}

G4LogicalVolume *
HomogeneousCalo::_construct_layer_x() {
    G4NistManager * nist = G4NistManager::Instance();

    auto cell = _construct_cell();

    auto solidLayerX = new G4Box("homoLayerX", nCellsX*cellSizeX/2, cellSizeY/2, cellDepth/2);
    auto logicLayerX = new G4LogicalVolume(solidLayerX
            , nist->FindOrBuildMaterial("G4_AIR"), "homoLayerX");

    new G4PVReplica(
                   "homoLayerX",     // its name
                   cell,             // its logical volume
                   logicLayerX,      // its mother
                   kXAxis,           // axis of replication
                   nCellsX,          // number of replica
                   cellSizeX);       // witdth of replica

    return logicLayerX;
}

G4LogicalVolume *
HomogeneousCalo::construct_calo() {
    G4NistManager * nist = G4NistManager::Instance();

    auto layerX = _construct_layer_x();

    auto solidLayerY = new G4Box("homoLayerY", nCellsX*cellSizeX/2, nCellsY*cellSizeY/2, cellDepth/2);
    auto logicLayerY = new G4LogicalVolume(solidLayerY
            , nist->FindOrBuildMaterial("G4_AIR"), "homoLayerY");

    new G4PVReplica(
                   "homoLayerX",     // its name
                   layerX,             // its logical volume
                   logicLayerY,      // its mother
                   kYAxis,           // axis of replication
                   nCellsY,          // number of replica
                   cellSizeY);       // witdth of replica

    return logicLayerY;
}

#if 0
G4LogicalVolume *
DetectorConstruction::ConstructHeteroCell() {
    float cellSize[2] = { cellSizeX, cellSizeY }
        , converterThickness = cnvThickness
        , scintillatorThickness = scintThickness
        ;

    // Get nist material manager
    G4NistManager * nist = G4NistManager::Instance();

    // create logic for cell encompassing converter and scintillator
    auto solidCell = new G4Box("CaloCell"
            , cellSize[0]/2, cellSize[1]/2, (scintillatorThickness + converterThickness)/2 );
    auto logicCell = new G4LogicalVolume(solidCell,
            nist->FindOrBuildMaterial("G4_AIR"),
            "CaloCell");

    // create converter layer
    auto solidConverter = new G4Box("CaloCellConverter"
            , cellSize[0]/2, cellSize[1]/2, converterThickness/2 );
    auto logicConverter = new G4LogicalVolume(solidConverter,
            nist->FindOrBuildMaterial("G4_Pb"),
            "CaloCellConverter");
    auto physConverter = new G4PVPlacement(nullptr,  // no rotation
        G4ThreeVector(0, 0, -scintillatorThickness/2),  // at ...
        logicConverter,                         // its logical volume
        "CaloCellConverter",                    // its name
        logicCell,                              // its mother  volume
        false,                                  // no boolean operation
        0,                                      // copy number
        true );                            // overlaps checking

    // create scintillator layer
    auto solidScintillator = new G4Box("CaloCellScintillator"
            , cellSize[0]/2, cellSize[1]/2, scintillatorThickness/2 );
    auto logicScintillator = new G4LogicalVolume(solidScintillator,
            nist->FindOrBuildMaterial("G4_PLEXIGLASS"),
            "CaloCellScintillator");
    auto physScintillator = new G4PVPlacement(nullptr,  // no rotation
        G4ThreeVector(0, 0, converterThickness/2),  // at ...
        logicScintillator,                      // its logical volume
        "CaloCellScintillator",                 // its name
        logicCell,                              // its mother  volume
        false,                                  // no boolean operation
        0,                                      // copy number
        true );                                 // overlaps checking

    return logicCell;
}

G4LogicalVolume *
DetectorConstruction::ConstructXLayer() {
    #if 0
    float depth;
    if(isHomogeneous)
        depth = homoCellThickness*cellDepth;

    auto solidXLayer = new G4Box("caloLayerX",
            worldDims[0]/2, worldDims[1]/2, worldDims[2]/2);  // its size
    auto logicXLayer = new G4LogicalVolume(solidWorld,  // its solid
            nist->FindOrBuildMaterial("G4_AIR"),     // its material
            "World");                                // its name
    #endif
}
#endif

G4VPhysicalVolume *
DetectorConstruction::Construct() {
    // Get nist material manager
    G4NistManager * nist = G4NistManager::Instance();
    // Option to switch on/off checking of volumes overlaps
    const G4bool checkOverlaps = true;

    // Create world (box)
    const double worldDims[] = {1*m, 1*m, 3*m};  // full world dimensions
    auto solidWorld = new G4Box("World",
            worldDims[0]/2, worldDims[1]/2, worldDims[2]/2);  // its size
    auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
            nist->FindOrBuildMaterial("G4_AIR"),     // its material
            "World");                                // its name
    auto physWorld = new G4PVPlacement(nullptr,  // no rotation
        G4ThreeVector(),                           // at (0,0,0)
        logicWorld,                                // its logical volume
        "World",                                   // its name
        nullptr,                                   // its mother  volume
        false,                                     // no boolean operation
        0,                                         // copy number
        checkOverlaps );                           // overlaps checking

    //
    // User geometry
    //auto calo = new HomogeneousCalo(25*cm, 15*cm, 15*cm, 5, 3);  // TODO
    //G4LogicalVolume * logicCalo = calo->construct_calo();

    //auto physCalo = new G4PVPlacement(nullptr
    //        , G4ThreeVector(0, 0, 1*m)
    //        , logicCalo
    //        , "Calo", logicWorld, false, 0, checkOverlaps);

    auto solidPhantom = new G4Box("phantom", 1*m/2, 1*m/2, 1*m/2);
    auto logicPhantom = new G4LogicalVolume(solidPhantom
            , nist->FindOrBuildMaterial("G4_WATER"), "phantom");
    auto physPhantom = new G4PVPlacement(nullptr
            , G4ThreeVector(0, 0, 1*m)
            , logicPhantom
            , "Calo", logicWorld, false, 0, checkOverlaps);

    return physWorld;
}

void
DetectorConstruction::ConstructSDandField() {
    auto absoSD = new SensitiveDetector("CaloSD");
    // not obvious, but necessary (binding newly created sensitive
    // detector instance with Geant4 API)
    G4SDManager::GetSDMpointer()->AddNewDetector(absoSD);

    SetSensitiveDetector("phantom", absoSD);  // CAVEAT: logic volume name
}

}
