#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4OpticalProcessIndex.hh"
#include "G4SystemOfUnits.hh"


class PhysicsList: public G4VModularPhysicsList {
public:
  PhysicsList();
  virtual ~PhysicsList();
  void SetCuts();
};

#endif
