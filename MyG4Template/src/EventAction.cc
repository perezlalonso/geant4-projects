#include "EventAction.hh"
#include "SensitiveDetector.hh"
#include "SensitiveDetectorHit.hh"
#include "Analysis.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"
#include <iomanip>

EventAction::EventAction()
 : G4UserEventAction(),
   fAbsHCID(-1)
{}

EventAction::~EventAction()
{}



SensitiveDetectorHitsCollection*
EventAction::GetHitsCollection(G4int hcID, const G4Event* event) const
{
  SensitiveDetectorHitsCollection* hitsCollection
  = static_cast<SensitiveDetectorHitsCollection*>(
    event->GetHCofThisEvent()->GetHC(hcID));

  if (!hitsCollection) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID;
    G4Exception("EventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);

  }
  return hitsCollection;
}

void EventAction::PrintEventStatistics(
                            G4double absoEdep) const
{
  // Print event statistics
  //
  G4cout
     << "   Absorber: total energy: "
     << std::setw(7) << G4BestUnit(absoEdep, "Energy")

     << G4endl;
}

void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{}

void EventAction::EndOfEventAction(const G4Event* event)
{
   // Get hist collections IDs
  if ( fAbsHCID == -1 ) {
    fAbsHCID
      = G4SDManager::GetSDMpointer()->GetCollectionID("AbsorberHitsCollection");

  }

SensitiveDetectorHitsCollection* absoHC = GetHitsCollection(fAbsHCID, event);
SensitiveDetectorHit* absoHit = (*absoHC)[absoHC->entries()-1];

G4int eventID = event->GetEventID();
G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
  G4cout << "---> End of event: " << eventID << G4endl;
  PrintEventStatistics(
    absoHit->GetEdep());
  // get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // fill histograms
  //
  analysisManager->FillH1(1, absoHit->GetEdep());

  // fill ntuple
  //
  analysisManager->FillNtupleDColumn(0, absoHit->GetEdep());
  analysisManager->AddNtupleRow();

  }
}
