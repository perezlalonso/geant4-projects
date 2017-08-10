#include "SensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"


SensitiveDetector::SensitiveDetector(const G4String& name, const G4String& hitsCollectionName)
: G4VSensitiveDetector(name),
  fHitsCollection(0) {
  collectionName.insert(hitsCollectionName);
}

SensitiveDetector::~SensitiveDetector() {
}

void SensitiveDetector::Initialize(G4HCofThisEvent * hce) {
  // Create hits collection
  fHitsCollection = new SensitiveDetectorHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce
  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);
  fHitsCollection->insert(new SensitiveDetectorHit());
}

G4bool SensitiveDetector::ProcessHits(G4Step* step,
                                     G4TouchableHistory*) {
//This is where the magic occurs. Think of how to get the info you want.
  G4double edep = step->GetTotalEnergyDeposit();
  G4TouchableHistory* touchable
    = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
  SensitiveDetectorHit * hit = (*fHitsCollection)[0];
  if ( ! hit ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hit " << 0;
    G4Exception("B4cCalorimeterSD::ProcessHits()",
      "MyCode0004", FatalException, msg);}
  hit->Add(edep);
  return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent *) {
}
