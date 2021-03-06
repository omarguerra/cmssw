import FWCore.ParameterSet.Config as cms

process = cms.Process("BuildPPSGeometry")

# Load CondDB service
process.load("CondCore.CondDB.CondDB_cfi")

# output database (in this case local sqlite file)
process.CondDB.connect = 'sqlite_file:PPSGeometry_2017.db'

# We define the output service.
process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    process.CondDB,
    timetype = cms.untracked.string('runnumber'),
    toPut = cms.VPSet(cms.PSet(
        record = cms.string('VeryForwardIdealGeometryRecord'),
        tag = cms.string('PPSGeometry_test')
    ))
)

# minimum of logs
process.MessageLogger = cms.Service("MessageLogger",
    statistics = cms.untracked.vstring(),
    destinations = cms.untracked.vstring('cout'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    )
)

# geometry
from Geometry.VeryForwardGeometry.geometryIdealPPSFromDD_2017_cfi import allFiles

process.XMLIdealGeometryESSource_CTPPS = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = allFiles,
    rootNodeName = cms.string('cms:CMSE')
)

# no events to process
process.source = cms.Source("EmptySource",
  firstRun = cms.untracked.uint32(123),
  firstLuminosityBlock = cms.untracked.uint32(1),
  firstEvent = cms.untracked.uint32(1),
  numberEventsInLuminosityBlock = cms.untracked.uint32(3),
  numberEventsInRun = cms.untracked.uint32(30)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

process.ppsGeometryBuilder = cms.EDAnalyzer("PPSGeometryBuilder",
    compactViewTag = cms.untracked.string('XMLIdealGeometryESSource_CTPPS')
)

process.p = cms.Path(
    process.ppsGeometryBuilder
)
