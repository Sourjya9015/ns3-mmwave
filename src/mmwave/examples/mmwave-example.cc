/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store.h"
#include "ns3/mmwave-helper.h"
#include <ns3/buildings-helper.h>
#include "ns3/global-route-manager.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/log.h"

using namespace ns3;

int 
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);
  //LogComponentEnable ("LteUeRrc", LOG_LEVEL_ALL);
  //LogComponentEnable ("LteEnbRrc", LOG_LEVEL_ALL);
  //LogComponentEnable("PropagationLossModel",LOG_LEVEL_ALL);
  //LogComponentEnable("mmWaveInterference",LOG_LEVEL_ALL);
  //LogComponentEnable("mmWaveBeamforming",LOG_LEVEL_ALL);
  //LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  //LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  Ptr<mmWaveHelper> ptr_mmWave = CreateObject<mmWaveHelper> ();

  ptr_mmWave->Initialize();

  /* A configuration example.
   * ptr_mmWave->GetPhyMacConfigurable ()->SetAttribute("SymbolPerSlot", UintegerValue(30)); */

  NodeContainer enbNodes;
  NodeContainer ueNodes;
  enbNodes.Create (1);
  ueNodes.Create (1);

  Ptr<ListPositionAllocator> enbPositionAlloc = CreateObject<ListPositionAllocator> ();
  enbPositionAlloc->Add (Vector (0.0, 0.0, 0.0));
  MobilityHelper enbmobility;
  enbmobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  enbmobility.SetPositionAllocator(enbPositionAlloc);
  enbmobility.Install (enbNodes);
  BuildingsHelper::Install (enbNodes);

  MobilityHelper uemobility;
  Ptr<ListPositionAllocator> uePositionAlloc = CreateObject<ListPositionAllocator> ();
  uePositionAlloc->Add (Vector (80.0, 0.0, 0.0));
  uemobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  uemobility.SetPositionAllocator(uePositionAlloc);
  uemobility.Install (ueNodes);
  BuildingsHelper::Install (ueNodes);

  NetDeviceContainer enbNetDev = ptr_mmWave->InstallEnbDevice (enbNodes);
  NetDeviceContainer ueNetDev = ptr_mmWave->InstallUeDevice (ueNodes);


  ptr_mmWave->RegisterToClosestEnb (ueNetDev, enbNetDev);
  ptr_mmWave->EnableTraces();

  // Activate a data radio bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  ptr_mmWave->ActivateDataRadioBearer (ueNetDev, bearer);



  Simulator::Stop (Seconds (0.3));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}


