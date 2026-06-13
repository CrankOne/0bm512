#pragma once

#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

namespace edu0bm512 {

// Constructs geometry entities for homogeneous calorimeter
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

// Constructs geometry entities for heterogeneous calorimeter
class HeterogeneousCalo {
    float scintLayerDepth, converterLayerDepth, cellSizeX, cellSizeY;
    int nCellsX, nCellsY, nLayersZ;
protected:
    G4LogicalVolume * _construct_layer();
    G4LogicalVolume * _construct_stack();
    G4LogicalVolume * _construct_layer_x();
public:
    HeterogeneousCalo(float width, float height
            , int nx, int ny, int nLayersZ_
            , float scintLayerDepth_, float converterLayerDepth_
            )
        : scintLayerDepth(scintLayerDepth_), converterLayerDepth(converterLayerDepth_)
        , cellSizeX(width/nx), cellSizeY(height/ny)
        , nCellsX(nx), nCellsY(ny), nLayersZ(nLayersZ_)
        {}

    G4LogicalVolume * construct_calo();
};

// Construct geometry entitites for a hodoscope
class Hodoscope {
    float slabWidth, slabHeight, slabDepth;
    int nSlabsX, nSlabsY;
protected:
    G4LogicalVolume * _construct_slab(char proj);
    G4LogicalVolume * _construct_layer(size_t n, char proj);
public:
    Hodoscope( float slabWidth_, float slabHeight_, float slabDepth_
             , int nSlabsX_, int nSlabsY_ )
        : slabWidth(slabWidth_), slabHeight(slabHeight_), slabDepth(slabDepth_)
        , nSlabsX(nSlabsX_), nSlabsY(nSlabsY_)
        {}

    G4LogicalVolume * construct_hodoscope();
};

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    DetectorConstruction() = default;
    ~DetectorConstruction() override = default;

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;
};

}  // namespace edu0bm512
