#include "SimG4Core/Physics/interface/PhysicsListFactory.h"

#include "DummyPhysics.hh"
#include "FTFCMS_BIC.hh"
#include "FTFPCMS_BERT.hh"
#include "FTFPCMS_BERT_ATL_EMM.hh"
#include "FTFPCMS_BERT_EML.hh"
#include "FTFPCMS_BERT_EMM.hh"
#include "FTFPCMS_BERT_HP_EML.hh"
#include "FTFPCMS_BERT_XS_EML.hh"
#include "FTFPCMS_BERT_EMV.hh"
#include "FTFPCMS_BERT_EMY.hh"
#include "FTFPCMS_BERT_EMZ.hh"
#include "QBBCCMS.hh"
#include "QGSPCMS_BERT.hh"
#include "QGSPCMS_BERT_EML.hh"
#include "QGSPCMS_BERT_EMV.hh"
#include "QGSPCMS_BERT_HP_EML.hh"
#include "QGSPCMS_FTFP_BERT.hh"
#include "QGSPCMS_FTFP_BERT_EML.hh"
#include "QGSPCMS_FTFP_BERT_EMM.hh"
#include "QGSPCMS_FTFP_BERT_EMY.hh"
#include "QGSPCMS_FTFP_BERT_EMZ.hh"

DEFINE_PHYSICSLIST(DummyPhysics);
typedef FTFCMS_BIC FTF_BIC;
DEFINE_PHYSICSLIST(FTF_BIC);
typedef FTFPCMS_BERT FTFP_BERT;
DEFINE_PHYSICSLIST(FTFP_BERT);
typedef FTFPCMS_BERT_ATL_EMM FTFP_BERT_ATL_EMM;
DEFINE_PHYSICSLIST(FTFP_BERT_ATL_EMM);
typedef FTFPCMS_BERT_EML FTFP_BERT_EML;
DEFINE_PHYSICSLIST(FTFP_BERT_EML);
typedef FTFPCMS_BERT_EMM FTFP_BERT_EMM;
DEFINE_PHYSICSLIST(FTFP_BERT_EMM);
typedef FTFPCMS_BERT_HP_EML FTFP_BERT_HP_EML;
DEFINE_PHYSICSLIST(FTFP_BERT_HP_EML);
typedef FTFPCMS_BERT_XS_EML FTFP_BERT_XS_EML;
DEFINE_PHYSICSLIST(FTFP_BERT_XS_EML);
typedef FTFPCMS_BERT_EMV FTFP_BERT_EMV;
DEFINE_PHYSICSLIST(FTFP_BERT_EMV);
typedef FTFPCMS_BERT_EMY FTFP_BERT_EMY;
DEFINE_PHYSICSLIST(FTFP_BERT_EMY);
typedef FTFPCMS_BERT_EMZ FTFP_BERT_EMZ;
DEFINE_PHYSICSLIST(FTFP_BERT_EMZ);
typedef QBBCCMS QBBC;
DEFINE_PHYSICSLIST(QBBC);
typedef QGSPCMS_BERT QGSP_BERT;
DEFINE_PHYSICSLIST(QGSP_BERT);
typedef QGSPCMS_BERT_EML QGSP_BERT_EML;
DEFINE_PHYSICSLIST(QGSP_BERT_EML);
typedef QGSPCMS_BERT_EMV QGSP_BERT_EMV;
DEFINE_PHYSICSLIST(QGSP_BERT_EMV);
typedef QGSPCMS_BERT_HP_EML QGSP_BERT_HP_EML;
DEFINE_PHYSICSLIST(QGSP_BERT_HP_EML);
typedef QGSPCMS_FTFP_BERT QGSP_FTFP_BERT;
DEFINE_PHYSICSLIST(QGSP_FTFP_BERT);
typedef QGSPCMS_FTFP_BERT_EML QGSP_FTFP_BERT_EML;
DEFINE_PHYSICSLIST(QGSP_FTFP_BERT_EML);
typedef QGSPCMS_FTFP_BERT_EMM QGSP_FTFP_BERT_EMM;
DEFINE_PHYSICSLIST(QGSP_FTFP_BERT_EMM);
typedef QGSPCMS_FTFP_BERT_EMY QGSP_FTFP_BERT_EMY;
DEFINE_PHYSICSLIST(QGSP_FTFP_BERT_EMY);
typedef QGSPCMS_FTFP_BERT_EMZ QGSP_FTFP_BERT_EMZ;
DEFINE_PHYSICSLIST(QGSP_FTFP_BERT_EMZ);

//
