#include "g4/SensitiveDetector.hh"
#include "CLHEP/Units/SystemOfUnits.h"

namespace edu0bm512 {

SensitiveDetector::SensitiveDetector(const G4String& name)
        : G4VSensitiveDetector(name)
        //, _primaryParticleLosses(0, 300, 0, 300, 300, 300)  // TODO
        {
}

SensitiveDetector::~SensitiveDetector() {}

void
SensitiveDetector::Initialize(G4HCofThisEvent* hitCollection) {
}

G4bool
SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* history) {
    // total energy deposit
    auto edep = step->GetTotalEnergyDeposit();

    // Get (1st level) replica number
    auto touchable = (step->GetPreStepPoint()->GetTouchable());

    // We can figure out how to obtain the cell indeces from replica number
    auto yLayerNumber = touchable->GetReplicaNumber(1);
    auto xLayerNumber = touchable->GetReplicaNumber(0);

    G4cout << "got hit at (" << xLayerNumber << ", " << yLayerNumber
        << "), energy is " << edep/CLHEP::MeV << "MeV" << G4endl;

    return true;
}

void
SensitiveDetector::EndOfEvent(G4HCofThisEvent* hitCollection) {
    // open file, dump histogram, close file
    FILE * meanLossesFile = fopen("./meanLosses.dat", "w");
    //_primaryParticleLosses.dump(meanLossesFile);  // TODO
    fclose(meanLossesFile);
}

}
