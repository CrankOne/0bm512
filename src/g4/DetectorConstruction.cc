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

// Homogeneous calorimeter
//

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

// Heterogeneous calorimeter
//

G4LogicalVolume *
HeterogeneousCalo::_construct_layer() {
    float cellSize[2] = { cellSizeX, cellSizeY };

    // Get nist material manager
    G4NistManager * nist = G4NistManager::Instance();

    // create logic for cell encompassing converter and scintillator
    auto solidCell = new G4Box("CaloCell"
            , cellSize[0]/2, cellSize[1]/2, (scintLayerDepth + converterLayerDepth)/2 );
    auto logicCell = new G4LogicalVolume(solidCell,
            nist->FindOrBuildMaterial("G4_AIR"),
            "CaloCell");

    // create converter layer
    auto solidConverter = new G4Box("CaloCellConverter"
            , cellSize[0]/2, cellSize[1]/2, converterLayerDepth/2 );
    auto logicConverter = new G4LogicalVolume(solidConverter,
            nist->FindOrBuildMaterial("G4_Pb"),
            "CaloCellConverter");
    auto physConverter = new G4PVPlacement(nullptr,  // no rotation
        G4ThreeVector(0, 0, -scintLayerDepth/2),  // at ...
        logicConverter,                         // its logical volume
        "CaloCellConverter",                    // its name
        logicCell,                              // its mother  volume
        false,                                  // no boolean operation
        0,                                      // copy number
        true );                                 // do check for overlaps?

    // create scintillator layer
    auto solidScintillator = new G4Box("CaloCellScintillator"
            , cellSize[0]/2, cellSize[1]/2, scintLayerDepth/2 );
    auto logicScintillator = new G4LogicalVolume(solidScintillator,
            nist->FindOrBuildMaterial("G4_PLEXIGLASS"),
            "CaloCellScintillator");
    auto physScintillator = new G4PVPlacement(nullptr,  // no rotation
        G4ThreeVector(0, 0, converterLayerDepth/2),  // at ...
        logicScintillator,                      // its logical volume
        "CaloCellScintillator",                 // its name
        logicCell,                              // its mother  volume
        false,                                  // no boolean operation
        0,                                      // copy number
        true );                                 // do check for overlaps?

    return logicCell;
}

G4LogicalVolume *
HeterogeneousCalo::_construct_stack() {
    G4NistManager * nist = G4NistManager::Instance();

    auto layer = _construct_layer();

    auto solidStack = new G4Box("solidStack", cellSizeX/2, cellSizeY/2
            , (scintLayerDepth + converterLayerDepth)*nLayersZ/2);
    auto logicStack = new G4LogicalVolume(solidStack
            , nist->FindOrBuildMaterial("G4_AIR"), "logicStack");

    new G4PVReplica(
                   "homoLayerX",    // its name
                   layer,           // its logical volume
                   logicStack,      // its mother
                   kZAxis,          // axis of replication
                   nLayersZ,        // number of replica
                   (scintLayerDepth + converterLayerDepth));  // witdth of replica

    return logicStack;
}

G4LogicalVolume *
HeterogeneousCalo::_construct_layer_x() {
    G4NistManager * nist = G4NistManager::Instance();

    auto layer = _construct_stack();

    auto solidRow = new G4Box("solidRow", nCellsX*cellSizeX/2, cellSizeY/2
            , (scintLayerDepth + converterLayerDepth)*nLayersZ/2);
    auto logicRow = new G4LogicalVolume(solidRow
            , nist->FindOrBuildMaterial("G4_AIR"), "logicRow");

    new G4PVReplica(
                   "heteroRow",     // its name
                   layer,           // its logical volume
                   logicRow,        // its mother
                   kXAxis,          // axis of replication
                   nCellsX,         // number of replica
                   cellSizeX);      // witdth of replica

    return logicRow;
}


G4LogicalVolume *
HeterogeneousCalo::construct_calo() {
    G4NistManager * nist = G4NistManager::Instance();

    auto layer = _construct_layer_x();

    auto solidVol = new G4Box("solidCalo", nCellsX*cellSizeX/2, nCellsY*cellSizeY/2
            , (scintLayerDepth + converterLayerDepth)*nLayersZ/2);
    auto logicVol = new G4LogicalVolume(solidVol
            , nist->FindOrBuildMaterial("G4_AIR"), "logicCalo");

    new G4PVReplica(
                   "heteroRow",     // its name
                   layer,           // its logical volume
                   logicVol,        // its mother
                   kYAxis,          // axis of replication
                   nCellsY,         // number of replica
                   cellSizeY);      // witdth of replica

    return logicVol;
}

// Hodoscope
//

G4LogicalVolume *
Hodoscope::_construct_slab(char proj) {
    G4NistManager * nist = G4NistManager::Instance();
    char namebuf[64];
    auto solidSlab = new G4Box("solidSlab", slabWidth/2, slabHeight/2, slabDepth/2);
    snprintf(namebuf, sizeof(namebuf), "logicSlab%c", proj);
    auto logicSlab = new G4LogicalVolume(solidSlab
            , nist->FindOrBuildMaterial("G4_PLEXIGLASS"), namebuf);
    return logicSlab;
}

G4LogicalVolume *
Hodoscope::_construct_layer(size_t n, char proj) {
    G4NistManager * nist = G4NistManager::Instance();
    auto slab = _construct_slab(proj);
    auto solidLayer = new G4Box("solidLayer", slabWidth*n/2, slabHeight/2
            , slabDepth/2);
    auto logicStack = new G4LogicalVolume(solidLayer
            , nist->FindOrBuildMaterial("G4_AIR"), "logicLayer");
    new G4PVReplica("hodoLayer",     // its name
                    slab,            // its logical volume
                    logicStack,      // its mother
                    kXAxis,          // axis of replication
                    n,               // number of replica
                    slabWidth);     // witdth of replica

    return logicStack;
}

G4LogicalVolume *
Hodoscope::construct_hodoscope() {
    G4NistManager *nist = G4NistManager::Instance();
    G4LogicalVolume *layerX = _construct_layer(nSlabsX, 'X')
                  //, *layerY = _construct_layer(nSlabsY, 'Y')
                  ;
    float side = std::max(slabWidth*nSlabsX, slabWidth*nSlabsY);
    side = std::max(side, slabHeight);
    auto solidVol = new G4Box("solidVol",
            side/2, side/2, slabDepth);  // its size
    auto logicVol = new G4LogicalVolume(solidVol,   // its solid
            nist->FindOrBuildMaterial("G4_AIR"),    // its material
            "logicHodoscope");                      // its name

    new G4PVPlacement(nullptr,    // no rotation for X layer
        G4ThreeVector(0, 0, -slabDepth/2),          // at (0,0,0)
        layerX,                                     // its logical volume
        "hodoscopeX",                               // its name
        logicVol,                                   // its mother  volume
        false,                                      // no boolean operation
        0,                                          // copy number
        true );                            // overlaps checking

    // to rotate Y layer by 90 degrees
    //G4RotationMatrix* rotMx = new G4RotationMatrix();
    //rotMx->rotateZ(90.0 * deg); // Rotates 90 degrees around Y

    //new G4PVPlacement(rotMx,    // rotate Y layer by 90 degrees
    //    G4ThreeVector(0, 0, slabDepth/2),           // at (0,0,0)
    //    layerY,                                     // its logical volume
    //    "hodoscopeY",                               // its name
    //    logicVol,                                   // its mother  volume
    //    false,                                      // no boolean operation
    //    0,                                          // copy number
    //    true );                            // overlaps checking
    return logicVol;
}

// DetectorConstruction interface implementation
//

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

    //auto calo = new HomogeneousCalo(25*cm, 15*cm, 15*cm, 5, 3);
    //G4LogicalVolume * logicCalo = calo->construct_calo();

    //auto calo = new HeterogeneousCalo(30*cm, 50*cm, 5, 8, 10, 5*cm, 1.5*cm);
    //G4LogicalVolume * logicCalo = calo->construct_calo();

    auto calo = new Hodoscope(2*cm, 34*cm, 2*cm, 16, 16);
    G4LogicalVolume * logicCalo = calo->construct_hodoscope();

    new G4PVPlacement(nullptr
            , G4ThreeVector(0, 0, 1*m)
            , logicCalo
            , "Calo", logicWorld, false, 0, checkOverlaps);

    //auto solidPhantom = new G4Box("phantom", 1*m/2, 1*m/2, 1*m/2);
    //auto logicPhantom = new G4LogicalVolume(solidPhantom
    //        , nist->FindOrBuildMaterial("G4_WATER"), "phantom");
    //auto physPhantom = new G4PVPlacement(nullptr
    //        , G4ThreeVector(0, 0, 1*m)
    //        , logicPhantom
    //        , "Calo", logicWorld, false, 0, checkOverlaps);

    return physWorld;
}

void
DetectorConstruction::ConstructSDandField() {
    auto absoSD = new SensitiveDetector("CaloSD");
    // not obvious, but necessary (binding newly created sensitive
    // detector instance with Geant4 API)
    G4SDManager::GetSDMpointer()->AddNewDetector(absoSD);

    SetSensitiveDetector("logicSlabX", absoSD);
    // ^^^ CAVEAT: logic volume name:
    //      "phantom" for a water phantom (when it is constructed)
    //      "homoCell" for a cell of homogeneous calorimeter
    //      "CaloCell" for a layer from heterogeneous calorimeter
    //      "logicSlab[XY]" for hodoscope
}

}
