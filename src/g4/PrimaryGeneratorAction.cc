#include "g4/PrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include <cassert>

namespace edu0bm512 {

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="neutron");
  assert(particle);  // XXX
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(10.*MeV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    G4double x0 = 0;  //;size * envSizeXY * (G4UniformRand()-0.5);
    G4double y0 = 0;  //size * envSizeXY * (G4UniformRand()-0.5);
    G4double z0 = .5 * m;

    fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

    fParticleGun->GeneratePrimaryVertex(anEvent);
}

}
