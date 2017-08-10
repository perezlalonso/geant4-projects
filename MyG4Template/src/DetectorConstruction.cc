#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4SDManager.hh"
#include "G4SDChargedFilter.hh"
#include "G4VisAttributes.hh"
#include "SensitiveDetector.hh"
#include "G4Colour.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "CADMesh.hh"
#include "G4MaterialPropertiesTable.hh"

G4ThreadLocal
G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = 0;

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction(),
   fCheckOverlaps(true)
{
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
}

void DetectorConstruction::DefineMaterials()
{
  // Define Materials
  G4NistManager* nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_Xe");
  nistManager->FindOrBuildMaterial("G4_FREON-12");
  G4double a;
  G4double z;
  G4double density;
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                  kStateGas, 2.73*kelvin, 3.e-18*pascal);
}

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
  // Geometry parameters

  // Get materials
  G4Material* defaultMaterial = G4Material::GetMaterial("Galactic");

  G4Material* absorberMaterial = G4Material::GetMaterial("G4_Xe");
  G4double pe[8] = {7.*eV,7.1*eV,7.2*eV,7.3*eV,7.4*eV,7.5*eV,7.6*eV,7.7*eV};
  G4double rindex[8] = {2.,2.,2.,2.,2.,2.,2.,2.};
  G4double abslength[8] = {.2*m,.2*m,.2*m,.2*m,.2*m,.2*m,.2*m,.2*m};

  G4MaterialPropertiesTable* XeMPT = new G4MaterialPropertiesTable();
  XeMPT->AddProperty("RINDEX", pe, rindex, 8);

  absorberMaterial->SetMaterialPropertiesTable(XeMPT);


  G4Material* boxmaterial = G4Material::GetMaterial("G4_FREON-12");
  G4MaterialPropertiesTable* boxMPT = new G4MaterialPropertiesTable();
  G4double scnt_fast[8] = { 0.000134, 0.004432, 0.053991, 0.241971, 0.398942, 0.000134, 0.004432, 0.053991};
  G4double scnt_slow[8] = { 0.000010, 0.000020, 0.000030, 0.004000,0.008000, 0.005000, 0.020000, 0.001000};

  boxMPT->AddProperty("FASTCOMPONENT", pe, scnt_fast, 8);
  boxMPT->AddProperty("SLOWCOMPONENT", pe, scnt_slow, 8);
  boxMPT->AddConstProperty("RESOLUTIONSCALE", 2.0);
  boxMPT->AddConstProperty("FASTTIMECONSTANT",  1.*ns);
  boxMPT->AddConstProperty("SLOWTIMECONSTANT", 10.*ns);
  boxMPT->AddConstProperty("YIELDRATIO", 0.8);
  boxMPT -> AddConstProperty("SCINTILLATIONYIELD",1./MeV);

  boxmaterial->SetMaterialPropertiesTable(boxMPT);

  // World
  G4VSolid* worldS
    = new G4Box("World",           // its name
                 4.*m, 4.*m, 4.*m); // its size

  G4LogicalVolume* worldLV
    = new G4LogicalVolume(
                 worldS,           // its solid
                 defaultMaterial,  // its material
                 "World");         // its name

  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(
                 0,                // rotation
                 G4ThreeVector(),  // pos vector
                 worldLV,          // its logical volume
                 "World",          // its name
                 0,                // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps

  // Absorber

    G4VSolid* absorberS = new G4Box("Abso", 0.5*m, 0.2*m, 0.1*m);

  G4LogicalVolume* absorberLV
    = new G4LogicalVolume(
                 absorberS,        // its solid
                 absorberMaterial, // its material
                 "AbsoLV");          // its name

   new G4PVPlacement(
                 0,                // rotation
                 G4ThreeVector(0., 0., 0.), //  pos vector
                 absorberLV,       // its logical volume
                 "Abso",           // its name
                 worldLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps

G4VSolid* box2 = new G4Box("Abso", 0.5*m, 0.2*m, 0.1*m);
G4LogicalVolume* box2LV =
                  new G4LogicalVolume(box2, boxmaterial, "Box2LV");
new G4PVPlacement(0,G4ThreeVector(0.,0.,0.4*m), box2LV, "Box2", worldLV, false,0,fCheckOverlaps);
  //
  // Visualization attributes
  //
  worldLV->SetVisAttributes (G4VisAttributes::Invisible);

  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  absorberLV->SetVisAttributes(simpleBoxVisAtt);

  return worldPV;
}

void DetectorConstruction::ConstructSDandField()
{

   SensitiveDetector* absoSD = new SensitiveDetector("AbsorberSD", "AbsorberHitsCollection");
   SetSensitiveDetector("AbsoLV", absoSD);


  //
  // Magnetic field
  //
  G4ThreeVector fieldValue = G4ThreeVector();
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);

  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
