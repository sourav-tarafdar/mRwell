#pragma once
#pragma once
#include <fun4all/Fun4AllServer.h>
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4detectors/PHG4SectorSubsystem.h>
#include <g4main/PHG4Reco.h>
#include "GlobalVariables.C"
#include "Build_G4_RWell_Bare.C"
#include "Build_G4_RWell_Sup01.C"

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)
R__LOAD_LIBRARY(libg4detectors.so)

namespace Enable
{
  static bool RWELL = false;
}

namespace RWELL
{
  //All units specified in cm unless stated otherwise
  const int n_layer = 2; //tracker layers
  //const double nom_radius[RWELL::n_layer] = {79.5,90.0}; //77 to not hit DIRC
  const double nom_radius[RWELL::n_layer] = {78.67,90.0}; //77 to not hit DIRC
  const double nom_driftgap[RWELL::n_layer] = {0.4,0.4};
  const double nom_length[RWELL::n_layer] = {200.0,200.0};
} //namespace RWELL

void RWellInit(int verbosity = 0)
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, RWELL::nom_radius[RWELL::n_layer - 1] / 10. + 0.7);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, RWELL::nom_length[RWELL::n_layer-1]/2.0);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -RWELL::nom_length[RWELL::n_layer-1]/2.0);
}
double RWell(PHG4Reco* g4Reco, double radius,
            const int absorberactive = 0,
            int verbosity = 0.,
            int type = 0) 
{
  //type selects the RWell material
  //   0: bare RWell (no support structure)
  //   1: Implimentation of FIT support rings

  for (int ilyr = 0; ilyr < RWELL::n_layer; ilyr++) //RWELL trackers are registered in Build_RWELL macro
  {
   if(type==0)
     {
       radius =  Build_G4_RWell_Bare(g4Reco,  //returns RWELL radiaus
                                                  RWELL::nom_radius[ilyr], //radius
                                                  RWELL::nom_driftgap[ilyr],//driftgap, 
                                                  RWELL::nom_length[ilyr],//length
                                                  ilyr);//index
     }
   if(type==1)
    {
       radius =  Build_G4_RWell_Sup01(g4Reco,  //returns RWELL radiaus
                                                   RWELL::nom_radius[ilyr], //radius
                                                   RWELL::nom_driftgap[ilyr],//driftgap, 
                                                   RWELL::nom_length[ilyr],//length
                                                   ilyr);//index
    }
    // update now that we know the outer radius
    BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, radius);
  }
  return radius;  //cm
}
// Central detector cell reco is disabled as EIC setup use the fast tracking sim for now
void RWell_Cells(int verbosity = 0)
{
  // runs the cellularization of the energy deposits (g4hits)
  // into detector hits (g4cells)

  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer* se = Fun4AllServer::instance();

  //-----------
  // SVTX cells
  //-----------

  return;
}
// Central detector  reco is disabled as EIC setup use the fast tracking sim for now
void RWell_Reco(int verbosity = 0)
{
  //---------------
  // Load libraries
  //---------------
  gSystem->Load("libfun4all.so");

  //---------------
  // Fun4All server
  //---------------
  
  Fun4AllServer* se = Fun4AllServer::instance();

  return;
}
