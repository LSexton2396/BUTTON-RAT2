#include <CLHEP/Units/PhysicalConstants.h>
#include <CLHEP/Units/SystemOfUnits.h>

#include "G4NistManager.hh"
#include <G4Box.hh>
#include <G4ExtrudedSolid.hh>
#include <G4Sphere.hh>
#include <G4SubtractionSolid.hh>
#include <G4Tubs.hh>
#include <G4UnionSolid.hh>
#include <GeoLinerHex.hh>
#include <RAT/PMTInfoParser.hh>
#include <RAT/ToroidalPMTConstruction.hh>
#include <vector>

namespace BUTTON {

G4VSolid *GeoLinerHex::ConstructSolid(RAT::DBLinkPtr table) {
  std::string volume_name = table->GetIndex();
  // Find mother
  std::string mother_name = table->GetS("mother");
  G4LogicalVolume *mother = FindMother(mother_name);

  // Optional parameters
  G4double phi_start = 0.0;
  try {
    phi_start = table->GetD("phi_start") * CLHEP::deg;
  } catch (RAT::DBNotFoundError &e) {
  };
  G4double phi_delta = CLHEP::twopi;
  try {
    phi_delta = table->GetD("phi_delta") * CLHEP::deg;
  } catch (RAT::DBNotFoundError &e) {
  };
  int pmt_bool = 1;
  try {
    pmt_bool = table->GetI("pmt_hole");
  } catch (RAT::DBNotFoundError &e) {
  };
  // end optional parms

  /// Set up the liner volume

  const G4int nsect = 8;
  G4double liner_thickness = table->GetD("liner_thickness") * CLHEP::mm;
  G4double rmax = table->GetD("r_max") * CLHEP::mm;
  G4double dr = table->GetD("dr") * CLHEP::mm;
  G4double r_hole = table->GetD("r_hole") *
                    CLHEP::mm; // radius of the sphere that makes the holes.

  std::vector<G4TwoVector> polygon(nsect);
  std::vector<G4TwoVector> polygon2(nsect);
  G4double ang = 360 * CLHEP::deg / nsect;
  for (G4int i = 0; i < nsect; ++i) {
    G4double phi = i * ang - 0.5 * ang;
    G4double cosphi = std::cos(phi);
    G4double sinphi = std::sin(phi);
    polygon[i].set(rmax * cosphi, rmax * sinphi);
    polygon2[i].set((rmax - liner_thickness) * cosphi,
                    (rmax - liner_thickness) * sinphi);
  }

  G4TwoVector offsetA(0, 0), offsetB(0, 0);
  G4double scaleA = 1, scaleB = 1;

  G4VSolid *liner = new G4ExtrudedSolid(volume_name, polygon, dr, offsetA,
                                        scaleA, offsetB, scaleB);
  liner = new G4UnionSolid(volume_name, liner, liner, 0,
                           G4ThreeVector(0., 0., 32. * CLHEP::mm));

  G4VSolid *innerliner =
      new G4ExtrudedSolid(volume_name, polygon2, dr - liner_thickness, offsetA,
                          scaleA, offsetB, scaleB);
  innerliner = new G4UnionSolid(volume_name, innerliner, innerliner, 0,
                                G4ThreeVector(0., 0., 32. * CLHEP::mm));

  liner = new G4SubtractionSolid(volume_name, liner, innerliner);

  /// Add Holes
  if (pmt_bool == 1) {
    std::cout << "Encapsulation turned on" << std::endl;

    // Get PMT postion and direction
    std::string pmt_table_name = table->GetS("pmt_table");
    RAT::DBLinkPtr pmt_table = RAT::DB::Get()->GetLink(pmt_table_name);

    std::vector<double> x = pmt_table->GetDArray("x"); // get pmt postion
    std::vector<double> y = pmt_table->GetDArray("y");
    std::vector<double> z = pmt_table->GetDArray("z");

    int vector_size = x.size();

    G4Sphere *encapsuation_volume =
        new G4Sphere(volume_name, 0. * CLHEP::mm, r_hole, phi_start, phi_delta,
                     phi_start, phi_delta / 2.0);

    for (int i = 0; i < vector_size; i++) {
      liner = new G4SubtractionSolid(volume_name, liner, encapsuation_volume, 0,
                                     G4ThreeVector(x.at(i) * CLHEP::mm,
                                                   y.at(i) * CLHEP::mm,
                                                   z.at(i) * CLHEP::mm));
    }
  }

  return liner;
}

} // namespace BUTTON
