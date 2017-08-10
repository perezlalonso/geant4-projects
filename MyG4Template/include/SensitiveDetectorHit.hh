#ifndef SensitiveDetectorHit_h
#define SensitiveDetectorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"
#include "G4UnitsTable.hh"

class SensitiveDetectorHit : public G4VHit {

public:
    SensitiveDetectorHit();
    SensitiveDetectorHit(const SensitiveDetectorHit&);
    virtual ~SensitiveDetectorHit();

    // Operators
    const SensitiveDetectorHit& operator=(const SensitiveDetectorHit&);
    G4int operator==(const SensitiveDetectorHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // Methods from base class
    virtual void Draw() {}
    virtual void Print();

    void Add(G4double de);

    G4double GetEdep() const;

  private:
    G4double fEdep;
};
typedef G4THitsCollection<SensitiveDetectorHit> SensitiveDetectorHitsCollection;

extern G4ThreadLocal G4Allocator<SensitiveDetectorHit>* SensitiveDetectorHitAllocator;

inline void* SensitiveDetectorHit::operator new(size_t) {
  if(!SensitiveDetectorHitAllocator) SensitiveDetectorHitAllocator = new G4Allocator<SensitiveDetectorHit>;
  void *hit;
  hit = (void *) SensitiveDetectorHitAllocator->MallocSingle();
  return hit;
}

inline void SensitiveDetectorHit::operator delete(void *hit) {
  if(!SensitiveDetectorHitAllocator)
    SensitiveDetectorHitAllocator = new G4Allocator<SensitiveDetectorHit>;
  SensitiveDetectorHitAllocator->FreeSingle((SensitiveDetectorHit*) hit);
}
inline void SensitiveDetectorHit::Add(G4double de) {
  fEdep += de;
}

inline G4double SensitiveDetectorHit::GetEdep() const {
  return fEdep;
}


#endif
