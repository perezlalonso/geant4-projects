#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"

#include "SensitiveDetectorHit.hh"
#include "globals.hh"

class EventAction : public G4UserEventAction
{
public:
  EventAction();
  virtual ~EventAction();

  virtual void  BeginOfEventAction(const G4Event* event);
  virtual void    EndOfEventAction(const G4Event* event);

private:
  // methods
  SensitiveDetectorHitsCollection* GetHitsCollection(G4int hcID,
                                            const G4Event* event) const;
  void PrintEventStatistics(G4double absoEdep) const;

  // data members
  G4int  fAbsHCID;
};
#endif
