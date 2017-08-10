#include "PhysicsList.hh"

PhysicsList::PhysicsList() : G4VModularPhysicsList() {
  G4EmStandardPhysics* emstandardPhysics = new G4EmStandardPhysics();
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  RegisterPhysics(emstandardPhysics);
  RegisterPhysics( opticalPhysics );
  opticalPhysics->Configure(kNoProcess, true);
  opticalPhysics->Configure(kCerenkov, true);
  opticalPhysics->Configure(kScintillation, true);
  opticalPhysics->SetMaxNumPhotonsPerStep(100);
  opticalPhysics->SetTrackSecondariesFirst(kCerenkov,true);
  opticalPhysics->SetScintillationYieldFactor(300./MeV);
  



}

  //Check Evernote notes for Tips

PhysicsList::~PhysicsList(){}


void PhysicsList::SetCuts() {
  G4VUserPhysicsList::SetCuts();
}
