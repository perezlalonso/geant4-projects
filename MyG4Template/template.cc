#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "PhysicsList.hh"
#include "Randomize.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

int main(int argc, char ** argv) {
  #ifdef G4MULTITHREADED
  G4MTRunManager * runManager = new G4MTRunManager;
  #else
  G4RunManager * runManager = new G4RunManager;
  #endif

  runManager->SetUserInitialization(new DetectorConstruction());

  runManager->SetUserInitialization(new PhysicsList());

  runManager->SetUserInitialization(new ActionInitialization());

  #ifdef G4VIS_USE
  G4VisManager * visManager = new G4VisExecutive;
  visManager->Initialize();
  #endif

  runManager->Initialize();

  // get the pointer to the UI manager and set verbosities
  G4UImanager * UImanager = G4UImanager::GetUIpointer();

  if (argc == 1) {
    #ifdef G4UI_USE
    G4UIExecutive * ui = new G4UIExecutive(argc, argv);
    #ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute vis.mac");
    #endif
    if (ui->IsGUI()) {
    }
    ui->SessionStart();
    delete ui;
    #endif
  } else {
    G4String command = "/control/execute ";
    G4String filename = argv[1];
    UImanager->ApplyCommand(command + filename);
  }

  #ifdef G4VIS_USE
  delete visManager;
  #endif

  delete runManager;

  return 0;
}
