#pragma once

#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

namespace edu0bm512 {

class HomogeneousCalo {
private:
    float cellSizeX, cellSizeY, cellDepth;
    int nCellsX, nCellsY;
protected:
    G4LogicalVolume * _construct_cell();
    G4LogicalVolume * _construct_layer_x();
public:
    HomogeneousCalo(float width, float height, float depth, int nx, int ny)
        : cellSizeX(width/nx)
        , cellSizeY(height/ny)
        , cellDepth(depth)
        , nCellsX(nx)
        , nCellsY(ny)
        {}

    G4LogicalVolume * construct_calo();
};

class HeterogeneousCalo {
    // ...
public:
    G4LogicalVolume * construct_calo();
};

class Hodoscope {
    // ...
public:
    G4LogicalVolume * construct_hodoscope();
};

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    DetectorConstruction() = default;
    ~DetectorConstruction() override = default;

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;
};

}
