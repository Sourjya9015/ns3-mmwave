/*
 * mmwave-apectrum-phy.h
 *
 *  Created on: Nov 4, 2014
 *      Author: sourjya
 */

#ifndef SRC_MMWAVE_MODEL_MMWAVE_SPECTRUM_PHY_H_
#define SRC_MMWAVE_MODEL_MMWAVE_SPECTRUM_PHY_H_


#include <ns3/object-factory.h>
#include <ns3/event-id.h>
#include <ns3/spectrum-value.h>
#include <ns3/mobility-model.h>
#include <ns3/packet.h>
#include <ns3/nstime.h>
#include <ns3/net-device.h>
#include <ns3/spectrum-phy.h>
#include <ns3/spectrum-channel.h>
#include <ns3/spectrum-interference.h>
#include <ns3/data-rate.h>
#include <ns3/generic-phy.h>
#include <ns3/packet-burst.h>
#include "mmwave-spectrum-signal-parameters.h"
#include "ns3/random-variable-stream.h"
#include "ns3/mmwave-beamforming.h"
#include "mmwave-interference.h"
#include "mmwave-control-messages.h"

namespace ns3{

struct tbInfo_t
{
  uint16_t size;
  uint8_t mcs;
  std::vector<int> rbBitmap;
  bool downlink;
  bool corrupt;
};

typedef std::map<uint16_t, tbInfo_t> expectedTbs_t;

typedef Callback< void, Ptr<Packet> > mmWavePhyRxDataEndOkCallback;

class mmWaveSpectrumPhy : public SpectrumPhy
{
public:
	mmWaveSpectrumPhy();
	virtual ~mmWaveSpectrumPhy();

	enum State
	  {
	    IDLE = 0,
		TX,
		RX
	  };

	static TypeId GetTypeId(void);
	virtual void DoDispose();

	void SetDevice(Ptr<NetDevice> d);
	Ptr<NetDevice> GetDevice();
	void SetMobility (Ptr<MobilityModel> m);
	Ptr<MobilityModel> GetMobility ();
	void SetChannel (Ptr<SpectrumChannel> c);
	Ptr<const SpectrumModel> GetRxSpectrumModel () const;

	Ptr<AntennaModel> GetRxAntenna ();
	void SetAntenna (Ptr<AntennaModel> a);

	void SetNoisePowerSpectralDensity (Ptr<const SpectrumValue> noisePsd);
	void SetTxPowerSpectralDensity (Ptr<SpectrumValue> TxPsd);
	void StartRx (Ptr<SpectrumSignalParameters> params);
	void StartRxData (Ptr<mmwaveSpectrumSignalParametersDataFrame> params);
	void StartRxControl (Ptr<SpectrumSignalParameters> params);
	Ptr<SpectrumChannel> GetSpectrumChannel();
	void SetCellId (uint16_t cellId);

	bool StartTxDataFrames (Ptr<PacketBurst> pb, std::list<Ptr<mmWaveControlMessages> > ctrlMsgList, Time duration);

	bool StartTxDlControlFrames (std::list<Ptr<mmWaveControlMessages> > ctrlMsgList, Time duration); // control frames from enb to ue
	bool StartTxUlControlFrames (void); // control frames from ue to enb

	void SetmmWavePhyRxDataEndOkCallback (mmWavePhyRxDataEndOkCallback c);

	void AddDataPowerChunkProcessor (Ptr<mmWaveChunkProcessor> p);
	void AddDataSinrChunkProcessor (Ptr<mmWaveChunkProcessor> p);

	void UpdateSinrPerceived (const SpectrumValue& sinr);

	void AddExpectedTb (uint16_t rnti, uint16_t size, uint8_t mcs, std::vector<int> map, bool downlink);

private:
	void EndTx ();
	void EndRxData ();
	void EndRxCtrl ();

	Ptr<mmWaveInterference> m_interferenceData;
	Ptr<MobilityModel> m_mobility;
	Ptr<NetDevice> m_device;
	Ptr<SpectrumChannel> m_channel;
	Ptr<const SpectrumModel> m_rxSpectrumModel;
	Ptr<SpectrumValue> m_txPsd;
	//Ptr<PacketBurst> m_txPacketBurst;
	std::list<Ptr<PacketBurst> > m_rxPacketBurstList;

	Ptr<AntennaModel> m_anetnna;

	uint16_t m_cellId;

	State m_state;
	/*This will keep growing in number*/

	mmWavePhyRxDataEndOkCallback m_mmWavePhyRxDataEndOkCallback;

	TracedCallback<EnbPhyPacketCountParameter> m_reportEnbPacketCount;
	TracedCallback<UePhyPacketCountParameter> m_reportUePacketCount;

	SpectrumValue m_sinrPerceived;

	expectedTbs_t m_expectedTbs;

	Ptr<UniformRandomVariable> m_random;



};

}


#endif /* SRC_MMWAVE_MODEL_MMWAVE_SPECTRUM_PHY_H_ */
