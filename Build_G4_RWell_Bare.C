#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4detectors/PHG4SectorSubsystem.h>
#include <g4eval/PHG4DSTReader.h>
#include <g4trackfastsim/PHG4TrackFastSim.h>
#include <g4trackfastsim/PHG4TrackFastSimEval.h>
#include <g4histos/G4HitNtuple.h>
#include <g4main/PHG4ParticleGenerator.h>
#include <g4main/PHG4Reco.h>
#include <g4main/PHG4TruthSubsystem.h>
#include <phool/recoConsts.h>
R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)
R__LOAD_LIBRARY(libg4detectors.so)
R__LOAD_LIBRARY(libg4trackfastsim.so)
R__LOAD_LIBRARY(libg4histos.so)
#endif


double Build_G4_RWell_Bare(PHG4Reco* g4Reco,
                      double rwellrad = 80.0, 
                      double driftgap = 1.5,
                      double length = 200.0, 
                      int index = 0)
{
  gSystem->Load("libfun4all");
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");
  gSystem->Load("libg4trackfastsim.so");

  const double cm = PHG4Sector::Sector_Geometry::Unit_cm();
  const double mm = 0.1 * cm;
  const double um = 1e-3 * mm;

  //double driftgap = 3.0;
  //double rwellrad = radius;
  //double length = length;
  double rsum= 0.0;


  //MPGD parameters (units in cm)
  double kapton_thickness = 0.0175; //cm
  double cu_thickness = 0.002; //cm
  double pcb_thickness = 0.010; //cm
  double prepreg_thickness = 0.005; //cm

  //Suppoort parameters (units in cm)
  //string supMat = "PEEK"; //support material
  //string supMat = "G4_Galactic"; //support material
  // making carbon fiber epoxy
  // G4Material *cfrp_intt = new G4Material("CFRP_INTT", density = 1.69 * g / cm3, ncomponents = 3);
  string supMat = "CFRP_INTT";
  //inner tube
  double support_01_thickness = 0.50;
  double support_01_length = 7.2;
  //inner ring
  double support_02_thickness = 1.6;
  double support_02_length = 1.2;
  //outer ring
  double support_03_thickness = 0.50;
  double support_03_length = 1.2;

  PHG4CylinderSubsystem *rwell_cyl;

  // here is our uRwell:
      //Gass layer
      rwell_cyl = new PHG4CylinderSubsystem(Form("RWELL_%d",index),0);
      rwell_cyl->set_double_param("radius",rwellrad);
      rwell_cyl->set_string_param("material","G4_Ar");
      rwell_cyl->set_double_param("thickness",driftgap);
      rwell_cyl->SetActive(1);
      rwell_cyl->set_int_param("lengthviarapidity",0);
      rwell_cyl->set_double_param("length",length);
      rwell_cyl->SuperDetector(Form("RWELL_%d",index));
      rwell_cyl->SetActive(1);
      g4Reco->registerSubsystem( rwell_cyl );
      rwell_cyl->OverlapCheck(overlapcheck);

      //Kapton
      rwell_cyl = new PHG4CylinderSubsystem(Form("RWELL_Kapton_%d",index), 0);
      rwell_cyl->set_double_param("radius",rwellrad - kapton_thickness);
      rwell_cyl->set_string_param("material","G4_KAPTON");
      rwell_cyl->set_double_param("thickness",kapton_thickness);
      rwell_cyl->set_int_param("lengthviarapidity",0);
      rwell_cyl->set_double_param("length",length);
      rwell_cyl->SuperDetector(Form("RWELL_Kapton_%d",index));
      rwell_cyl->SetActive(0);
      g4Reco->registerSubsystem( rwell_cyl );
      rwell_cyl->OverlapCheck(overlapcheck);
      //Cu
      rsum = rwellrad + driftgap;
      rwell_cyl = new PHG4CylinderSubsystem(Form("RWELL_Cu_%d",index), 0);
      rwell_cyl->set_double_param("radius",rsum);
      rwell_cyl->set_string_param("material","G4_Cu");
      rwell_cyl->set_double_param("thickness",cu_thickness);
      rwell_cyl->set_int_param("lengthviarapidity",0);
      rwell_cyl->set_double_param("length",length);
      rwell_cyl->SuperDetector(Form("RWELL_Cu_%d",index));
      rwell_cyl->SetActive(0);
      g4Reco->registerSubsystem( rwell_cyl );
      rwell_cyl->OverlapCheck(overlapcheck);

      //Prepreg
      rsum+=cu_thickness;
      rwell_cyl = new PHG4CylinderSubsystem(Form("RWELL_PrePreg_%d",index), 0);
      rwell_cyl->set_double_param("radius",rsum);
      rwell_cyl->set_string_param("material","NOMEX");
      rwell_cyl->set_double_param("thickness",prepreg_thickness);
      rwell_cyl->set_int_param("lengthviarapidity",0);
      rwell_cyl->set_double_param("length",length);
      rwell_cyl->SuperDetector(Form("RWELL_PrePreg_%d",index));
      rwell_cyl->SetActive(0);
      g4Reco->registerSubsystem( rwell_cyl );
      rwell_cyl->OverlapCheck(overlapcheck);
       
      //PCB
      rsum += prepreg_thickness;
      rwell_cyl = new PHG4CylinderSubsystem(Form("RWELL_PCB_%d",index), 0);
      rwell_cyl->set_double_param("radius",rsum);
      rwell_cyl->set_string_param("material","FR4");
      rwell_cyl->set_double_param("thickness",pcb_thickness);
      rwell_cyl->set_int_param("lengthviarapidity",0);
      rwell_cyl->set_double_param("length",length);
      rwell_cyl->SuperDetector(Form("RWELL_PCB_%d",index));
      rwell_cyl->SetActive(0);
      g4Reco->registerSubsystem( rwell_cyl );
      rwell_cyl->OverlapCheck(overlapcheck);
     
 /*
      //---Support structure--
      //tube
      rsum[0]+=pcb_thickness;
      rwell_cyl_0 = new PHG4CylinderSubsystem("RWELL_Support01_0", 0);
      rwell_cyl_0->set_double_param("radius",rsum[0]);
      rwell_cyl_0->set_string_param("material",supMat);
      rwell_cyl_0->set_double_param("thickness",support_01_thickness);
      rwell_cyl_0->set_int_param("lengthviarapidity",0);
      rwell_cyl_0->set_double_param("place_z",-length[0]/2);
      rwell_cyl_0->set_double_param("length",support_01_length);
      rwell_cyl_0->SuperDetector("RWELL_Support01_0");
      rwell_cyl_0->SetActive(0);
      g4Reco->registerSubsystem( rwell_cyl_0 );
      rwell_cyl_0->OverlapCheck(1);
      //tube 2
      rwell_cyl_0 = new PHG4CylinderSubsystem("RWELL_Support01_1", 0);
      rwell_cyl_0->set_double_param("radius",rsum[0]);
      rwell_cyl_0->set_string_param("material",supMat);
      rwell_cyl_0->set_double_param("thickness",support_01_thickness);
      rwell_cyl_0->set_int_param("lengthviarapidity",0);
      rwell_cyl_0->set_double_param("place_z",length[0]/2);
      rwell_cyl_0->set_double_param("length",support_01_length);
      rwell_cyl_0->SuperDetector("RWELL_Support01_1");
      rwell_cyl_0->SetActive(0);
      g4Reco->registerSubsystem( rwell_cyl_0 );
      rwell_cyl_0->OverlapCheck(1);
      //inner ring
      rsum[0]+=support_01_thickness;
      rwell_cyl_0 = new PHG4CylinderSubsystem("RWELL_Support02_0", 0);
      rwell_cyl_0->set_double_param("radius",rsum[0]);
      rwell_cyl_0->set_string_param("material",supMat);
      rwell_cyl_0->set_double_param("thickness",support_02_thickness);
      rwell_cyl_0->set_int_param("lengthviarapidity",0);
      rwell_cyl_0->set_double_param("place_z",-length[0]/2 +  support_01_length/2);
      rwell_cyl_0->set_double_param("length",support_02_length);
      rwell_cyl_0->SuperDetector("RWELL_Support02_0");
      rwell_cyl_0->SetActive(0);
      g4Reco->registerSubsystem( rwell_cyl_0 );
      rwell_cyl_0->OverlapCheck(1);
      //inner ring 2
      rwell_cyl_0 = new PHG4CylinderSubsystem("RWELL_Support02_1", 0);
      rwell_cyl_0->set_double_param("radius",rsum[0]);
      rwell_cyl_0->set_string_param("material",supMat);
      rwell_cyl_0->set_double_param("thickness",support_02_thickness);
      rwell_cyl_0->set_int_param("lengthviarapidity",0);
      rwell_cyl_0->set_double_param("place_z",length[0]/2 - support_01_length/2);
      rwell_cyl_0->set_double_param("length",support_02_length);
      rwell_cyl_0->SuperDetector("RWELL_Support02_1");
      rwell_cyl_0->SetActive(0);
      g4Reco->registerSubsystem( rwell_cyl_0 );
      rwell_cyl_0->OverlapCheck(1);
      //outer ring
      rsum[0]+=support_02_thickness;
      rwell_cyl_0 = new PHG4CylinderSubsystem("RWELL_Support03_0", 0);
      rwell_cyl_0->set_double_param("radius",rsum[0]);
      rwell_cyl_0->set_string_param("material",supMat);
      rwell_cyl_0->set_double_param("thickness",support_03_thickness);
      rwell_cyl_0->set_int_param("lengthviarapidity",0);
      rwell_cyl_0->set_double_param("length",support_03_length);
      rwell_cyl_0->SuperDetector("RWELL_Support03_0");
      rwell_cyl_0->set_double_param("place_z",-length[0]/2 +support_01_length/2);
      rwell_cyl_0->SetActive(0);
      g4Reco->registerSubsystem( rwell_cyl_0 );
      rwell_cyl_0->OverlapCheck(1);
      //outer ring 2
      rwell_cyl_0 = new PHG4CylinderSubsystem("RWELL_Support03_1", 0);
      rwell_cyl_0->set_double_param("radius",rsum[0]);
      rwell_cyl_0->set_string_param("material",supMat);
      rwell_cyl_0->set_double_param("thickness",support_03_thickness);
      rwell_cyl_0->set_int_param("lengthviarapidity",0);
      rwell_cyl_0->set_double_param("length",support_03_length);
      rwell_cyl_0->SuperDetector("RWELL_Support03_1");
      rwell_cyl_0->set_double_param("place_z",length[0]/2 -support_01_length/2);
      rwell_cyl_0->SetActive(0);
      g4Reco->registerSubsystem( rwell_cyl_0 );
      rwell_cyl_0->OverlapCheck(1);
*/
  return rwellrad;
}
