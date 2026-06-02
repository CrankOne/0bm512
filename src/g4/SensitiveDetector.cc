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
    // energy deposit caused by ionization (without radiative/synchrotron
    // radiation in magnetic should match total deposit)
    double ioniziationLoss = step->GetTotalEnergyDeposit()
                           - step->GetNonIonizingEnergyDeposit();
    if(!step->GetTrack()->GetParentID()) {  // if this is a primary particle
        // calculate dE/dx within a step
        double dEdx = (ioniziationLoss / CLHEP::MeV) / (step->GetStepLength() / CLHEP::cm);
        // get own energy of the particle (as mean of pre- and post- step points)
        double ownE = ( step->GetPreStepPoint()->GetKineticEnergy()
                      + step->GetPostStepPoint()->GetKineticEnergy()
                      ) / 2;
        double zCen = ( step->GetPreStepPoint()->GetPosition().z()/CLHEP::cm
                      + step->GetPostStepPoint()->GetPosition().z()/CLHEP::cm
                      ) / 2;
        ownE /= CLHEP::MeV;
        // add mean particle edenrgy (MeV) wrt energy deposition in MeV/cm
        //_primaryParticleLosses.fill(ownE, dEdx);
        //_primaryParticleLosses.fill(zCen, dEdx);  // TODO
        printf("XXX primary particle energy deposition: %e (%.3e MeV, non-ionizing %.3e MeV)"
                ", particle energy = %.3e MeV\n"
                , edep, edep/CLHEP::MeV, (edep - ioniziationLoss)/CLHEP::MeV
                , step->GetTrack()->GetDynamicParticle()->GetKineticEnergy()/CLHEP::MeV
                );
        //step->GetTrack()->GetParentID()
    }

    // Get (1st level) replica number
    auto touchable = (step->GetPreStepPoint()->GetTouchable());
    auto yLayerNumber = touchable->GetReplicaNumber(1);
    auto xLayerNumber = touchable->GetReplicaNumber(0);

    //printf("XXX energy deposition: %e at %d, %d\n", edep, xLayerNumber, yLayerNumber);

    #if 0
    // step length
    G4double stepLength = 0.;
    if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
        stepLength = step->GetStepLength();
    }
    if ( edep==0. && stepLength == 0. ) return false;
    auto touchable = (step->GetPreStepPoint()->GetTouchable());
    auto layerNumber = touchable->GetReplicaNumber(1);
    #endif

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
