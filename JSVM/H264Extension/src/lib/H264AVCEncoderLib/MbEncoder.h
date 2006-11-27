/*
********************************************************************************

NOTE - One of the two copyright statements below may be chosen
       that applies for the software.

********************************************************************************

This software module was originally developed by

Heiko Schwarz    (Fraunhofer HHI),
Tobias Hinz      (Fraunhofer HHI),
Karsten Suehring (Fraunhofer HHI)

in the course of development of the ISO/IEC 14496-10:2005 Amd.1 (Scalable Video
Coding) for reference purposes and its performance may not have been optimized.
This software module is an implementation of one or more tools as specified by
the ISO/IEC 14496-10:2005 Amd.1 (Scalable Video Coding).

Those intending to use this software module in products are advised that its
use may infringe existing patents. ISO/IEC have no liability for use of this
software module or modifications thereof.

Assurance that the originally developed software module can be used
(1) in the ISO/IEC 14496-10:2005 Amd.1 (Scalable Video Coding) once the
ISO/IEC 14496-10:2005 Amd.1 (Scalable Video Coding) has been adopted; and
(2) to develop the ISO/IEC 14496-10:2005 Amd.1 (Scalable Video Coding):

To the extent that Fraunhofer HHI owns patent rights that would be required to
make, use, or sell the originally developed software module or portions thereof
included in the ISO/IEC 14496-10:2005 Amd.1 (Scalable Video Coding) in a
conforming product, Fraunhofer HHI will assure the ISO/IEC that it is willing
to negotiate licenses under reasonable and non-discriminatory terms and
conditions with applicants throughout the world.

Fraunhofer HHI retains full right to modify and use the code for its own
purpose, assign or donate the code to a third party and to inhibit third
parties from using the code for products that do not conform to MPEG-related
ITU Recommendations and/or ISO/IEC International Standards.

This copyright notice must be included in all copies or derivative works.
Copyright (c) ISO/IEC 2005.

********************************************************************************

COPYRIGHT AND WARRANTY INFORMATION

Copyright 2005, International Telecommunications Union, Geneva

The Fraunhofer HHI hereby donate this source code to the ITU, with the following
understanding:
    1. Fraunhofer HHI retain the right to do whatever they wish with the
       contributed source code, without limit.
    2. Fraunhofer HHI retain full patent rights (if any exist) in the technical
       content of techniques and algorithms herein.
    3. The ITU shall make this code available to anyone, free of license or
       royalty fees.

DISCLAIMER OF WARRANTY

These software programs are available to the user without any license fee or
royalty on an "as is" basis. The ITU disclaims any and all warranties, whether
express, implied, or statutory, including any implied warranties of
merchantability or of fitness for a particular purpose. In no event shall the
contributor or the ITU be liable for any incidental, punitive, or consequential
damages of any kind whatsoever arising from the use of these programs.

This disclaimer of warranty extends to the user of these programs and user's
customers, employees, agents, transferees, successors, and assigns.

The ITU does not represent or warrant that the programs furnished hereunder are
free of infringement of any third-party patents. Commercial implementations of
ITU-T Recommendations, including shareware, may be subject to royalty fees to
patent holders. Information regarding the ITU-T patent policy is available from
the ITU Web site at http://www.itu.int.

THIS IS NOT A GRANT OF PATENT RIGHTS - SEE THE ITU-T PATENT POLICY.

********************************************************************************
*/






#if !defined(AFX_MBENCODER_H__F725C8AD_2589_44AD_B904_62FE2A7F7D8D__INCLUDED_)
#define AFX_MBENCODER_H__F725C8AD_2589_44AD_B904_62FE2A7F7D8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "H264AVCCommonLib/FrameMng.h"
#include "DistortionIf.h"
#include "RateDistortionIf.h"
#include "MotionEstimation.h"
#include "MbTempData.h"
#include "MbCoder.h"
#include "BitCounter.h"
#include "UvlcWriter.h"
#include "H264AVCCommonLib/Quantizer.h"


H264AVC_NAMESPACE_BEGIN

class Transform;
class MbFGSCoefMap;
class FrameMng;
class IntraPredictionSearch;
class CodingParameter;

//TMM_WP
#define MAX_REF_FRAMES 64
//TMM_WP


class MbEncoder
: protected MbCoder
, public UvlcWriter
, protected BitCounter
{
protected:
  MbEncoder();
  virtual ~MbEncoder();

public:
  static ErrVal create( MbEncoder*& rpcMbEncoder );
  ErrVal destroy();

  ErrVal init(  Transform* pcTransform,
                IntraPredictionSearch* pcIntraPrediction,
                MotionEstimation *pcMotionEstimation,
                CodingParameter* pcCodingParameter,
                RateDistortionIf* pcRateDistortionIf,
                XDistortion* pcXDistortion
                );

  ErrVal uninit();
  ErrVal initSlice( const SliceHeader& rcSH);

  IntMbTempData* getBestIntData() {return m_pcIntMbBestData; }

  ErrVal  encodeIntra         ( MbDataAccess&   rcMbDataAccess,
                                MbDataAccess*   pcMbDataAccessBase,
                                IntFrame*       pcFrame,
                                IntFrame*       pcRecSubband,
                                IntFrame*       pcBaseLayer,
                                IntFrame*       pcPredSignal,
                                Double          dLambda );
  ErrVal  encodeResidual      ( MbDataAccess&   rcMbDataAccess,
                                IntFrame*       pcFrame,
                                IntFrame*       pcResidual,
                                IntFrame*       pcBaseSubband,
                                IntFrame*        pcSRFrame, // JVT-R091
                                Bool&           rbCoded,
                                Double          dLambda,
                                Int             iMaxDeltaQp );
  ErrVal  encodeResidual      ( MbDataAccess&   rcMbDataAccess,
                                MbDataAccess&   rcMbDataAccessBL,
                                MbFGSCoefMap&   rcMbFGSCoefMap,
                                IntFrame*       pcResidual,
                                Double          dLambda,
                                Bool            bLowPass,
                                Int             iMaxQpDelta );

  ErrVal  encodeInterP        ( MbDataAccess&   rcMbDataAccess,
                                MbDataAccess*   pcMbDataAccessBase,
                                Int             iSpatialScalabilityType,
                                IntFrame*       pcFrame,
                                IntFrame*       pcRecSubband,
                                IntFrame*       pcPredSignal,
                                IntFrame*       pcBaseLayerRec,
                                IntFrame*       pcBaseLayerSbb,
                                RefFrameList&   rcRefFrameList0,
                                RefFrameList*   pcRefFrameList0Base,
                                Double          dLambda );
  ErrVal  compensatePrediction( MbDataAccess&   rcMbDataAccess,
                                IntFrame*       pcMCFrame,
                                RefFrameList&   rcRefFrameList0,
                                RefFrameList&   rcRefFrameList1,
                                Bool            bCalcMv,
                                Bool            bFaultTolerant );

  ErrVal  compensateUpdate(      MbDataAccess&   rcMbDataAccess,
                                 IntFrame*       pcMCFrame,
                                 Int             iRefIdx,
                                 ListIdx         eListPrd,
                                 IntFrame*       pcPrdFrame);


  ErrVal  estimatePrediction  ( MbDataAccess&   rcMbDataAccess,
                                MbDataAccess*   pcMbDataAccessBase,
                                Int              iSpatialScalabilityType,
                                RefFrameList&   rcRefFrameList0,
                                RefFrameList&   rcRefFrameList1,
                                const IntFrame* pcBaseLayerFrame,
                                const IntFrame* pcBaseLayerResidual,
                                const IntFrame& rcOrigFrame,
                                IntFrame&       rcIntraRecFrame,
                                Bool            bBiPredOnly,
                                UInt            uiNumMaxIter,
                                UInt            uiIterSearchRange,
                Bool      bBLSkipEnable, // JVT-Q065 EIDR
                                Double          dLambda );
  ErrVal  encodeMacroblock    ( MbDataAccess&   rcMbDataAccess,
                                IntFrame*       pcFrame,
                                RefFrameList&   rcList0,
                                RefFrameList&   rcList1,
                                UInt            uiNumMaxIter,
                                UInt            uiIterSearchRange,
                                Double          dLambda );

  ErrVal  encodeFGS           ( MbDataAccess&   rcMbDataAccess,
                                MbDataAccess*   pcMbDataAccessBase,
                                RefFrameList&   rcRefFrameList0,
                                RefFrameList&   rcRefFrameList1,
                                const IntFrame& rcOrigFrame,
                                IntFrame*       pcPredSignal,
                                IntFrame*       pcBQPredSignal,
                                RefFrameList*   pcRefFrameListDiff,
                                FGSCoder*       pcFGSCoder,
                                IntYuvMbBuffer& rcBaseLayerBuffer,
                                UInt            uiNumMaxIter,
                                UInt            uiIterSearchRange,
                                Double          dLambda,
                                Int             iMaxQpDelta );

//TMM_WP
  ErrVal getPredWeights( SliceHeader& rcSH, ListIdx eLstIdx,
                         Double(*pafWeight)[3], IntFrame* pOrgFrame,
                         RefFrameList& rcRefFrameListX);


  ErrVal getPredOffsets( SliceHeader& rcSH, ListIdx eLstIdx,
                         Double(*pafOffsets)[3], IntFrame* pOrgFrame,
                         RefFrameList& rcRefFrameListX);

//TMM_WP



  //JVT-R057 LA-RDO{
  Void setLARDOEnable( Bool bLARDO)  { m_bLARDOEnable= bLARDO; }

  Void setLayerID (UInt uiLayer)     { m_uiLayerID=uiLayer;}

  Void setPLR( UInt auiPLR[5])       { for(UInt i=0;i<5;i++) m_auiPLR[i] = auiPLR[i];}

  Void setRatio( Double adRatio[5][2])
  {
    for(UInt i=0;i<5;i++)
      for(UInt j=0;j<2;j++)
        m_aadRatio[i][j] = adRatio[i][j];
  }
 Void setMBSSD      ( UInt uiSSD)      { m_uiMBSSD=uiSSD; }

  Bool getLARDOEnable(){ return m_bLARDOEnable;}

 Void setFrameEcEp  ( IntFrame* p1)    { m_pcFrameEcEp=p1; }

  Int  GetEC_REC            ( IntYuvPicBuffer* pPic1,
                              IntYuvPicBuffer* pPic2,
                              Int              blockX,
                              Int              blockY);

  Void  getChannelDistortion( MbDataAccess&    rcMbDataAccess,
    IntFrame&       rcRefFrame,
                              Int              *distortion,
                              Int              iMvX,
                              Int              iMvY,
                              Int              startX,
                              Int              startY,
                              Int              blockX,
                              Int              blockY,
                              Bool             bSpatial=false);

  Int getEpRef() { return m_iEpRef; }

  Void setEpRef(Int iRef)   { m_iEpRef=iRef; }

  Void  getDistortion       (Int              iDList0,
                             Int              iDList1,
                             SampleWeighting* pcSampleWeighting,
                             MbDataAccess&    rcMbDataAccess);
  //JVT-R057 LA-RDO}

  //S051{
  Void    setUseBDir  ( Bool bUse){ m_bUseBDir = bUse;}
  //S051}

  //JVT-U106 Behaviour at slice boundaries{
  void  setIntraBLFlag(Bool b){ m_bIntraBLFlag=b; }
  //JVT-U106 Behaviour at slice boundaries}
protected:

  ErrVal  xScale4x4Block        ( TCoeff*            piCoeff,
                                  const UChar*       pucScale,
                                  UInt               uiStart,
                                  const QpParameter& rcQP );

  ErrVal  xScale8x8Block        ( TCoeff*            piCoeff,
                                  const UChar*       pucScale,
                                  const QpParameter& rcQP );
  ErrVal  xScaleTCoeffs         ( MbDataAccess&      rcMbDataAccess,
                                  MbTransformCoeffs& rcTCoeffs );

  ErrVal  xSetRdCostIntraMb     ( IntMbTempData&    rcMbTempData,
                                  UInt              uiCoeffBits,
                                  Bool              bBSlice,
                                  Bool              bBLSkip );

  ErrVal  xSetRdCostInterMb     ( IntMbTempData&    rcMbTempData,
                                  MbDataAccess*     pcMbDataAccessBase,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1,
                                  Bool              bBLSkip          = false,
                                  UInt              uiAdditionalBits = 0,
                                  Bool              bSkipMCPrediction = false );
  //-- JVT-R012
  ErrVal  xSetRdCostInterMbSR   ( IntMbTempData&    rcMbTempData,
                                  MbDataAccess*     pcMbDataAccessBase,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1,
                                  IntFrame*          pcBaseLayerSbb,
                                  Bool              bBLSkip          = false,
                                  UInt              uiAdditionalBits = 0 );
  //--
  ErrVal  xSetRdCost8x8InterMb  ( IntMbTempData&    rcMbTempData,
                                  MbDataAccess*     pcMbDataAccessBaseMotion,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1,
                                  Bool              bBLSkip          = false,
                                  UInt              uiAdditionalBits = 0,
                                  Bool              bSkipMCPrediction = false );
  //-- JVT-R012
  ErrVal  xSetRdCost8x8InterMbSR( IntMbTempData&    rcMbTempData,
                                  MbDataAccess*     pcMbDataAccessBaseMotion,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1,
                                  IntFrame*          pcBaseLayerSbb,
                                  Bool              bBLSkip          = false,
                                  UInt              uiAdditionalBits = 0 );
  //--
  ErrVal  xSetRdCostInterSubMb  ( IntMbTempData&    rcMbTempData,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1,
                                  B8x8Idx           c8x8Idx,
                                  Bool              bTrafo8x8,
                                  UInt              uiAddBits );

  ErrVal  xEncodeChromaIntra        ( IntMbTempData& rcMbTempData, UInt& ruiExtCbp, UInt& ruiBits );

  ErrVal  xEncode4x4IntraBlock      ( IntMbTempData& rcMbTempData, LumaIdx cIdx,     UInt& ruiBits, UInt& ruiExtCbp );
  ErrVal  xEncode4x4InterBlock      ( IntMbTempData&               rcMbTempData,
                                      LumaIdx                      cIdx,
                                      UInt&                        ruiBits,
                                      UInt&                        ruiExtCbp,
                                      RefCtx*                      pcRefCtx = NULL );
  ErrVal  xEncode8x8InterBlock      ( IntMbTempData&               rcMbTempData,
                                      B8x8Idx                      c8x8Idx,
                                      UInt&                        ruiBits,
                                      UInt&                        ruiExtCbp,
                                      RefCtx*                      pcRefCtx = NULL );
  ErrVal  xEncode8x8IntraBlock      ( IntMbTempData& rcMbTempData, B8x8Idx cIdx,     UInt& ruiBits, UInt& ruiExtCbp );
  ErrVal  xEncodeChromaTexture      ( IntMbTempData& rcMbTempData, UInt& ruiExtCbp, UInt& ruiBits, MbFGSCoefMap* pcMbFGSCoefMap = NULL );

  Void    xReStoreParameter     ( MbDataAccess&     rcMbDataAccess, IntMbTempData& rcMbBestData );
  Void    xUpDateBest           ( IntMbTempData&    rcMbTempData );

  ErrVal  xCheckInterMbMode8x8  ( IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData,
                                  IntMbTempData*    pcMbRefData,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1,
                                  MbDataAccess*     pcMbDataAccessBaseMotion );
  //-- JVT-R012
  ErrVal  xCheckInterMbMode8x8SR( IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData,
                                  IntMbTempData*    pcMbRefData,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1,
                                  IntFrame*          pcBaseLayerSbb,
                                  MbDataAccess*     pcMbDataAccessBaseMotion );
  //--

  ErrVal  xEstimateMbIntraBL    ( IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData,
                                  const IntFrame*   pcBaseLayerRec,
                                  Bool              bBSlice,
                                  MbDataAccess*     pcMbDataAccessBase );
  ErrVal  xEstimateMbIntraBL8x8 ( IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData,
                                  const IntFrame*   pcBaseLayerRec,
                                  Bool              bBSlice,
                                  Bool              bBLSkip );
  ErrVal  xEstimateMbIntra16    ( IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData,
                                  Bool              bBSlice  );
  ErrVal  xEstimateMbIntra8     ( IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData,
                                  Bool              bBSlice  );
  ErrVal  xEstimateMbIntra4     ( IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData,
                                  Bool              bBSlice  );
  ErrVal  xEstimateMbPCM        ( IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData,
                                  Bool              bBSlice  );

  ErrVal  xEstimateMbSkip       ( IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1 );
  ErrVal  xEstimateMbBLSkip     ( IntMbTempData*&   rpcIntMbTempData,
                                  IntMbTempData*&   rpcIntMbBestData,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1,
                                  const IntFrame*   pcBaseLayerRec,
                                  Bool              bBSlice,
                                  Int        iSpatialScalabilityType,
                                  MbDataAccess*     pcMbDataAccessBaseMotion,
                                  Bool              bResidualPred );

  ErrVal  xEstimateMbFGSSkip    ( IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1,
                                  MbDataAccess*     pcMbDataAccessBase,
                                  IntYuvMbBuffer&   rcBaseLayerBuffer,
                                  IntFrame*         pcPredSignal,
                                  Int               iMaxQpDelta );

  //-- JVT-R091
  ErrVal  xEstimateMbSR          ( IntMbTempData*&   rpcIntMbTempData,
                                  IntMbTempData*&   rpcIntMbBestData,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1,
                                  const IntFrame*   pcBaseLayerSbb,
                                  MbDataAccess*     pcMbDataAccessBaseMotion,
                                  Bool              bResidualPred );
  //--

  ErrVal  xEstimateMbDirect     ( IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1,
                                  MbDataAccess*     pcMbDataAccessBaseMotion,
                                  Bool              bResidualPred );
  ErrVal  xEstimateMb16x16      ( IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1,
                                  Bool              bBiPredOnly,
                                  UInt              uiNumMaxIter,
                                  UInt              uiIterSearchRange,
                                  Bool              bQPelRefinementOnly,
                                  MbDataAccess*     pcMbDataAccessBaseMotion,
                                  Bool              bResidualPred );
  ErrVal  xEstimateMb16x8       ( IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1,
                                  Bool              bBiPredOnly,
                                  UInt              uiNumMaxIter,
                                  UInt              uiIterSearchRange,
                                  Bool              bQPelRefinementOnly,
                                  MbDataAccess*     pcMbDataAccessBaseMotion,
                                  Bool              bResidualPred );
  ErrVal  xEstimateMb8x16       ( IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1,
                                  Bool              bBiPredOnly,
                                  UInt              uiNumMaxIter,
                                  UInt              uiIterSearchRange,
                                  Bool              bQPelRefinementOnly,
                                  MbDataAccess*     pcMbDataAccessBaseMotion,
                                  Bool              bResidualPred );
  ErrVal  xEstimateMb8x8        ( IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1,
                                  Bool              bBiPredOnly,
                                  UInt              uiNumMaxIter,
                                  UInt              uiIterSearchRange,
                                  Bool              bQPelRefinementOnly,
                                  MbDataAccess*     pcMbDataAccessBaseMotion,
                                  Bool              bResidualPred );
  ErrVal  xEstimateMb8x8Frext   ( IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1,
                                  Bool              bBiPredOnly,
                                  UInt              uiNumMaxIter,
                                  UInt              uiIterSearchRange,
                                  Bool              bQPelRefinementOnly,
                                  MbDataAccess*     pcMbDataAccessBaseMotion,
                                  Bool              bResidualPred );
  ErrVal  xEstimateSubMbDirect  ( Par8x8            ePar8x8,
                                  IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1,
                                  Bool              bTrafo8x8,
                                  UInt              uiAddBits,
                                  MbDataAccess*     pcMbDataAccessBaseMotion );
  ErrVal  xEstimateSubMb8x8     ( Par8x8            ePar8x8,
                                  IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1,
                                  Bool              bTrafo8x8,
                                  Bool              bBiPredOnly,
                                  UInt              uiNumMaxIter,
                                  UInt              uiIterSearchRange,
                                  UInt              uiAddBits,
                                  Bool              bQPelRefinementOnly,
                                  MbDataAccess*     pcMbDataAccessBaseMotion );
  ErrVal  xEstimateSubMb8x4     ( Par8x8            ePar8x8,
                                  IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1,
                                  Bool              bBiPredOnly,
                                  UInt              uiNumMaxIter,
                                  UInt              uiIterSearchRange,
                                  UInt              uiAddBits,
                                  Bool              bQPelRefinementOnly,
                                  MbDataAccess*     pcMbDataAccessBaseMotion );
  ErrVal  xEstimateSubMb4x8     ( Par8x8            ePar8x8,
                                  IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1,
                                  Bool              bBiPredOnly,
                                  UInt              uiNumMaxIter,
                                  UInt              uiIterSearchRange,
                                  UInt              uiAddBits,
                                  Bool              bQPelRefinementOnly,
                                  MbDataAccess*     pcMbDataAccessBaseMotion );
  ErrVal  xEstimateSubMb4x4     ( Par8x8            ePar8x8,
                                  IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData,
                                  RefFrameList&     rcRefFrameList0,
                                  RefFrameList&     rcRefFrameList1,
                                  Bool              bBiPredOnly,
                                  UInt              uiNumMaxIter,
                                  UInt              uiIterSearchRange,
                                  UInt              uiAddBits,
                                  Bool              bQPelRefinementOnly,
                                  MbDataAccess*     pcMbDataAccessBaseMotion );

  ErrVal  xCheckBestEstimation  ( IntMbTempData*&   rpcMbTempData,
                                  IntMbTempData*&   rpcMbBestData );
  Void    xStoreEstimation      ( MbDataAccess&     rcMbDataAccess,
                                  IntMbTempData&    rcMbBestData,
                                  IntFrame*         pcRecSubband,
                                  IntFrame*         pcPredSignal,
                                  Bool              bMotionFieldEstimation,
                                  IntYuvMbBuffer*   pcBaseLayerBuffer );
  Bool    xCheckUpdate          ( IntYuvMbBuffer&   rcPredBuffer,
                                  IntYuvMbBuffer&   rcOrigBuffer,
                                  LumaIdx           cIdx,
                                  Int               iXSize,
                                  Int               iYSize );


  ErrVal  xEncode16x16ResidualMB( IntMbTempData&    rcMbTempData,
                                  UInt&             ruiBits,
                                  UInt&             ruiExtCbp );


  UInt  xCalcMbCbp    ( UInt uiExtCbp );

private:
  UChar xGetFrameBits ( ListIdx eLstIdx, Int iRefPic );

protected:
  CodingParameter* m_pcCodingParameter;
  Transform*   m_pcTransform;
  IntraPredictionSearch*    m_pcIntraPrediction;
  MotionEstimation *m_pcMotionEstimation;
  RateDistortionIf* m_pcRateDistortionIf;
  XDistortion*  m_pcXDistortion;
  Bool bInitDone;
  Bool m_bISlice;
  Bool m_bBSlice;
  Bool m_bCabac;
  IntMbTempData  m_acIntMbTempData[5];
  IntMbTempData* m_pcIntMbBestData;
  IntMbTempData* m_pcIntMbTempData;
  IntMbTempData* m_pcIntMbBest8x8Data;
  IntMbTempData* m_pcIntMbTemp8x8Data;
  IntMbTempData* m_pcIntMbBestIntraChroma;

  IntYuvMbBuffer  *m_pcIntOrgMbPelData;
  IntYuvPicBuffer *m_pcIntPicBuffer;
  IntYuvPicBuffer *m_pcIntraPredPicBuffer;

  UInt m_uiMaxRefFrames[2];
  UInt m_uiMaxRefPics[2];

  BitWriteBufferIf* m_BitCounter;
  //JVT-R057 LA-RDO{
  Bool m_bLARDOEnable;
  UInt m_uiLayerID;
  UInt m_auiPLR[5];
  Double m_aadRatio[5][2];
  UInt m_uiMBSSD;
  IntFrame* m_pcFrameEcEp;
  Int  m_iEpRef;
  Double m_dWr0;
  Double m_dWr1;
  //JVT-R057 LA-RDO}

  //S051{
  Bool    m_bUseBDir;
  //S051}
  //JVT-U106 Behaviour at slice boundaries{
  Bool  m_bIntraBLFlag;
  //JVT-U106 Behaviour at slice boundaries}
};


H264AVC_NAMESPACE_END

#endif // !defined(AFX_MBENCODER_H__F725C8AD_2589_44AD_B904_62FE2A7F7D8D__INCLUDED_)
