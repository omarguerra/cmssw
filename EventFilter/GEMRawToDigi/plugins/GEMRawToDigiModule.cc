/** \unpacker for gem
 *  \author J. Lee - UoS
 */
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/Transition.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"

#include "EventFilter/GEMRawToDigi/plugins/GEMRawToDigiModule.h"

using namespace gem;

GEMRawToDigiModule::GEMRawToDigiModule(const edm::ParameterSet& pset)
    : fed_token(consumes<FEDRawDataCollection>(pset.getParameter<edm::InputTag>("InputLabel"))),
      useDBEMap_(pset.getParameter<bool>("useDBEMap")),
      unPackStatusDigis_(pset.getParameter<bool>("unPackStatusDigis")),
      gemRawToDigi_(std::make_unique<GEMRawToDigi>()) {
  produces<GEMDigiCollection>();
  if (unPackStatusDigis_) {
    produces<GEMVfatStatusDigiCollection>("vfatStatus");
    produces<GEMGEBdataCollection>("gebStatus");
    produces<GEMAMCdataCollection>("AMCdata");
    produces<GEMAMC13EventCollection>("AMC13Event");
  }
}

void GEMRawToDigiModule::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("InputLabel", edm::InputTag("rawDataCollector"));
  desc.add<bool>("useDBEMap", false);
  desc.add<bool>("unPackStatusDigis", false);
  descriptions.add("muonGEMDigisDefault", desc);
}

std::shared_ptr<GEMROMapping> GEMRawToDigiModule::globalBeginRun(edm::Run const&, edm::EventSetup const& iSetup) const {
  auto gemROmap = std::make_shared<GEMROMapping>();
  if (useDBEMap_) {
    GEMeMap const& eMap = iSetup.getData(gemEMapToken_);
    auto gemEMap = std::make_unique<GEMeMap>(eMap);
    gemEMap->convert(*gemROmap);
    gemEMap.reset();
  } else {
    // no EMap in DB, using dummy
    auto gemEMap = std::make_unique<GEMeMap>();
    gemEMap->convertDummy(*gemROmap);
    gemEMap.reset();
  }
  return gemROmap;
}

void GEMRawToDigiModule::produce(edm::StreamID iID, edm::Event& iEvent, edm::EventSetup const&) const {
  auto outGEMDigis = std::make_unique<GEMDigiCollection>();
  auto outVFATStatus = std::make_unique<GEMVfatStatusDigiCollection>();
  auto outGEBStatus = std::make_unique<GEMGEBdataCollection>();
  auto outAMCdata = std::make_unique<GEMAMCdataCollection>();
  auto outAMC13Event = std::make_unique<GEMAMC13EventCollection>();

  // Take raw from the event
  edm::Handle<FEDRawDataCollection> fed_buffers;
  iEvent.getByToken(fed_token, fed_buffers);

  auto gemROMap = runCache(iEvent.getRun().index());

  for (unsigned int fedId = FEDNumbering::MINGEMFEDID; fedId <= FEDNumbering::MAXGEMFEDID; ++fedId) {
    const FEDRawData& fedData = fed_buffers->FEDData(fedId);

    int nWords = fedData.size() / sizeof(uint64_t);
    LogDebug("GEMRawToDigiModule") << " words " << nWords;

    if (nWords < 5)
      continue;
    const unsigned char* data = fedData.data();

    const uint64_t* word = reinterpret_cast<const uint64_t*>(data);
    auto amc13Event = gemRawToDigi_->convertWordToAMC13Event(word);

    if (amc13Event == nullptr)
      continue;

    // Read AMC data
    for (auto amcData : *(amc13Event->getAMCpayloads())) {
      uint8_t amcNum = amcData.amcNum();

      // Read GEB data
      for (auto gebData : *amcData.gebs()) {
        uint8_t gebId = gebData.inputID();
        GEMROMapping::chamEC geb_ec = {fedId, amcNum, gebId};
        GEMROMapping::chamDC geb_dc = gemROMap->chamberPos(geb_ec);
        GEMDetId gemChId = geb_dc.detId;

        //Read vfat data
        for (auto vfatData : *gebData.vFATs()) {
          vfatData.setVersion(geb_dc.vfatVer);
          uint16_t vfatId = vfatData.vfatId();
          GEMROMapping::vfatEC vfat_ec = {vfatId, gemChId};

          // check if ChipID exists.
          if (!gemROMap->isValidChipID(vfat_ec)) {
            edm::LogWarning("GEMRawToDigiModule")
                << "InValid: amcNum " << int(amcNum) << " gebId " << int(gebId) << " vfatId " << int(vfatId)
                << " vfat Pos " << int(vfatData.position());
            continue;
          }
          // check vfat data
          if (vfatData.quality()) {
            edm::LogWarning("GEMRawToDigiModule")
                << "Quality " << int(vfatData.quality()) << " b1010 " << int(vfatData.b1010()) << " b1100 "
                << int(vfatData.b1100()) << " b1110 " << int(vfatData.b1110());
            if (vfatData.crc() != vfatData.checkCRC()) {
              edm::LogWarning("GEMRawToDigiModule")
                  << "DIFFERENT CRC :" << vfatData.crc() << "   " << vfatData.checkCRC();
            }
          }

          GEMROMapping::vfatDC vfat_dc = gemROMap->vfatPos(vfat_ec);

          vfatData.setPhi(vfat_dc.localPhi);
          GEMDetId gemId = vfat_dc.detId;
          int bx(vfatData.bc());

          for (int chan = 0; chan < VFATdata::nChannels; ++chan) {
            uint8_t chan0xf = 0;
            if (chan < 64)
              chan0xf = ((vfatData.lsData() >> chan) & 0x1);
            else
              chan0xf = ((vfatData.msData() >> (chan - 64)) & 0x1);

            // no hits
            if (chan0xf == 0)
              continue;

            GEMROMapping::channelNum chMap = {vfat_dc.vfatType, chan};
            GEMROMapping::stripNum stMap = gemROMap->hitPos(chMap);

            int stripId = stMap.stNum + vfatData.phi() * GEMeMap::maxChan_;

            GEMDigi digi(stripId, bx);

            LogDebug("GEMRawToDigiModule")
                << " fed: " << fedId << " amc:" << int(amcNum) << " geb:" << int(gebId) << " vfat:" << vfat_dc.localPhi
                << ",type: " << vfat_dc.vfatType << " id:" << gemId << " ch:" << chMap.chNum << " st:" << digi.strip()
                << " bx:" << digi.bx();

            outGEMDigis.get()->insertDigi(gemId, digi);

          }  // end of channel loop

          if (unPackStatusDigis_) {
            outVFATStatus.get()->insertDigi(gemId, GEMVfatStatusDigi(vfatData));
          }

        }  // end of vfat loop

        if (unPackStatusDigis_) {
          gebData.clearVFATs();
          outGEBStatus.get()->insertDigi(gemChId.chamberId(), (gebData));
        }

      }  // end of geb loop

      if (unPackStatusDigis_) {
        amcData.clearGEBs();
        outAMCdata.get()->insertDigi(amcData.boardId(), (amcData));
      }

    }  // end of amc loop

    if (unPackStatusDigis_) {
      amc13Event->clearAMCpayloads();
      outAMC13Event.get()->insertDigi(amc13Event->bxId(), AMC13Event(*amc13Event));
    }

  }  // end of amc13Event

  iEvent.put(std::move(outGEMDigis));

  if (unPackStatusDigis_) {
    iEvent.put(std::move(outVFATStatus), "vfatStatus");
    iEvent.put(std::move(outGEBStatus), "gebStatus");
    iEvent.put(std::move(outAMCdata), "AMCdata");
    iEvent.put(std::move(outAMC13Event), "AMC13Event");
  }
}
