/**
 * @file MP500TCL3.h
 */

#ifndef MP500TCL3_H_
#define MP500TCL3_H_

#ifdef WINDOWS_DBG
#include "MP500CLStruct.h"
#else
#include <CORE/MP500.h>
#include <CONTACTLESS/MP500CLStruct.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

// TCL3 driver version 1.07
#define TCL3_DRIVER_VERSION     0x00000107


/*******************************
 * TCL3 - CONTACTLESS API CALL *
 *******************************/

/*--------------*
 * ALL TYPES    *
 *--------------*/
// Select card type
#define TYPE_A                  1       // Type A
#define TYPE_B                  2       // Type B

#define TYPE_VICINITY           4       // Vicinity
#define TYPE_FELICA             5       // FeliCa (212 or 424)
//#define TYPE_MIFARE             5       // Mifare
#define TYPE_NFC_TAG_TYPE_1     6       // NFC Tag type 1
#define TYPE_FELICA_424         7       // FeliCa 424 , only used in card emulation mode
#define TYPE_FELICA_212         8       // FeliCa 212 , only used in card emulation mode


WORD CALL MPC_SelectType(BYTE CplNum, BYTE Type);

// select ASK modulation index in per cent
WORD CALL MPC_SelectModulationASK(BYTE CplNum, WORD ASK);
// select ASK modulation index in per thousand
WORD CALL MPC_SelectModulationASKpt(BYTE CplNum, WORD ASKpt);


// Select field strength
WORD CALL MPC_SelectFieldStrength(BYTE CplNum, BYTE Unit, WORD value);
#define UNIT_PER_CENT           1
#define UNIT_PER_THOUSAND       4
#define APPLY_DEFAULT_VALUE     3

// select carrier frequency
WORD CALL MPC_SelectCarrier(BYTE CplNum, DWORD Carrier);
#define MIN_CARRIER             12560000
#define MAX_CARRIER             14560000

// selecting rise and fall time of the modulation
WORD CALL MPC_SelectRiseFallTime(BYTE CplNum, WORD RiseFallTime);       // to program identical rise and fall time
WORD CALL MPC_SelectFallAndRiseTime(BYTE CplNum,
                WORD FallTime, WORD RiseTime);    // to program different fall and rise time
#define MAX_RISE_FALL_TIME  10000       // maximum rise and fall time in ns

// selecting rise time of the RF Field
WORD CALL MPC_SelectFieldRiseTime(BYTE CplNum, DWORD RiseTime);
#define MAX_FIELD_RISE_TIME     25000 // 25 ms

// Picc Reset Slow + cmd
WORD CALL MPC_PiccResetSlow(BYTE CplNum, DWORD FallTime_ms, DWORD LowTime_us, DWORD RiseTime_ms,DWORD delay_ms,BYTE *pFrameTX, DWORD nbbitTX,BYTE *pFrameRX, DWORD *pnbbitRX);
#define MAX_SLOW_FALL_TIME      10000           // in ms
#define MAX_LOW_TIME            100000          // in µs
#define MAX_SLOW_RISE_TIME      10000           // in ms
#define MIN_SLOW_FALL_TIME      100             // in ms
#define MIN_LOW_TIME            1000            // in µs
#define MIN_SLOW_RISE_TIME      100             // in ms

// Setting the timeout PICC->PCD
WORD CALL MPC_SetFWTETU(BYTE CplNum, DWORD NbETU);

// Select data rate
WORD CALL MPC_SelectDataRate(BYTE CplNum, WORD DataRateTx, WORD DataRateRx);
#define TX_106KB        106
#define TX_212KB        212
#define TX_424KB        424
#define TX_848KB        848
#define TX_1695KB       1695
#define TX_3390KB       3390
#define TX_6780KB       6780
#define RX_106KB        106
#define RX_212KB        212
#define RX_424KB        424
#define RX_848KB        848
#define RX_1695KB       1695
#define RX_3390KB       3390
#define RX_6780KB       6780

// Exchanging frame (low level API)
WORD CALL MPC_ExchangeCmd(BYTE CplNum, BYTE *pFrameTX, DWORD nbbitTX,
                BYTE *pFrameRX, DWORD *pnbbitRX);
#define MAX_RECEIVED_FRAME_SIZE 5000

// Computing CRC
WORD CALL MPC_ComputeCrc(BYTE CplNum, int CRCType, BYTE *Data,
                DWORD Length, BYTE *TransmitFirst, BYTE *TransmitSecond);

// Exchanging APDU according to 14443-4 protocol
WORD CALL MPC_SendAPDU(BYTE CplNum, DWORD Header,
                DWORD Lc, BYTE *pLcField,
                DWORD Le, BYTE *pLeField, DWORD *pLeFieldLen,
                WORD *pSW1SW2);
#define NO_LC       0x80000000
#define LC_EXTENDED 0x40000000
#define NO_LE       0x80000000
#define LE_EXTENDED 0x40000000

// Exchange frame (not APDU) according to 14443-4 protocol
WORD CALL MPC_SendFrameProtocol(BYTE CplNum, BYTE *pFrameTX,
                DWORD NbByteTX, BYTE *pFrameRX, DWORD *pNbByteRX);

// send DESELECT Sequence
WORD CALL MPC_DeselectSequence(BYTE CplNum);

// send SBlock(PARAMETERS)
WORD CALL MPC_SendSBlockParameters(BYTE CplNum, BYTE *SParameters,
                WORD LngSParameters, BYTE *RParameters, WORD *LngRParameters);


WORD CALL MPC_SelectRxGain(BYTE CplNum, DWORD Gain);
#define RX_GAIN_MIN     0
#define RX_GAIN_MAX     100

/*------------------*
 * SPECIFIC TYPE A  *
 *------------------*/
// select type A pause width
WORD CALL MPC_SelectPauseWidth(BYTE CplNum, WORD PauseWidth);
#define MAX_PAUSE_WIDTH     4500      // maximum pause width (in ns)
#define MIN_PAUSE_WIDTH     100       // minimum pause width (in ns)
// Type A Request command
WORD CALL MPC_RequestA(BYTE CplNum, WORD *ATQA);
// type A anticollision
WORD CALL MPC_AnticollA(BYTE CplNum, BYTE *snr,WORD *plng, BYTE *pSAK);
// type A select command
WORD CALL MPC_SelectCardA(BYTE CplNum, BYTE *snr,WORD lng, BYTE *pSAK);
// Type A Wake Up command
WORD CALL MPC_WakeUpA(BYTE CplNum, WORD *ATQA);
// Type A HALT command
WORD CALL MPC_HaltA(BYTE CplNum);
// Type A RATS command
WORD CALL MPC_SendRATS(BYTE CplNum, BYTE *ATS, WORD *Lng);
WORD CALL MPC_SendRATSFree(BYTE CplNum, BYTE *RATS, WORD LngRATS,  BYTE *ATS, WORD *Lng);
// Type A PPS command
WORD CALL MPC_SendPPS(BYTE CplNum, BYTE CID, BYTE DRI,  BYTE DSI);
// Exchanging bit in raw mode (Type A 106 Kbits/s only)
WORD CALL MPC_ExchangeCmdRawA(BYTE CplNum,BYTE *pFrameTX, DWORD nbbitTX, BYTE *pFrameRX, DWORD *pnbbitRX);
// get the position of the collision (Type A 106 kbits/s only)
WORD CALL MPC_GetPosColl(BYTE CplNum,DWORD *PosColl);

/*------------------*
 * SPECIFIC TYPE B  *
 *------------------*/
// Type B REQUEST command
WORD CALL MPC_RequestB(BYTE CplNum, BYTE NbSlot, BYTE *ATQB,WORD *Lng );
WORD CALL MPC_RequestBFree(BYTE CplNum, BYTE *Reqb, WORD ReqbLen, BYTE *ATQB,WORD *Lng );

// Type B SLOT MARKER command
WORD CALL MPC_SlotMarkerCmd(BYTE CplNum, BYTE NumSlot, BYTE *ATQB,WORD *Lng );

// Type B ATTRIB command
WORD CALL MPC_SendATTRIB(BYTE CplNum, BYTE *ATTRIB, WORD LngATTRIB, BYTE *Answer, WORD *pLngAnswer);

// Type B WAKE UP command
WORD CALL MPC_WakeUpB(BYTE CplNum, BYTE NbSlot, BYTE *ATQB,WORD *Lng );

// Type B HALT command
WORD CALL MPC_HaltB(BYTE CplNum, BYTE *Identifier);


/*----------------------*
 * SPECIFIC TYPE FELICA *
 *----------------------*/
WORD CALL MPC_FELICA_Polling(BYTE CplNum, WORD SystemCode, BYTE Reserved, BYTE TimeSlot, BYTE *bufrep, WORD *lgrep);
WORD CALL MPC_FELICA_Read_Without_Encryption(BYTE CplNum, BYTE *pIDm,
                 BYTE bNumberOfService, WORD *pwServiceCodeList,
                 BYTE bNumberOfBlocks, DWORD *pdwBlockList,
                 BYTE *pIDm2, BYTE *bStatus1, BYTE *bStatus2,
                 BYTE *bNumberOfBlocksRet, BYTE *ppBlockData);
WORD CALL MPC_FELICA_Write_Without_Encryption(BYTE CplNum, BYTE *pIDm,
                 BYTE bNumberOfService, WORD *pwServiceCodeList,
                 BYTE bNumberOfBlocks, DWORD *pdwBlockList, BYTE *ppBlockData,
                 BYTE *pIDm2, BYTE *bStatus1, BYTE *bStatus2);


/*----------------------*
 * MIFARE ULTRALIGHT    *
 *----------------------*/
WORD CALL MPC_MFULReadPage(BYTE CplNum, DWORD page, BYTE *buf);
WORD CALL MPC_MFULWritePage(BYTE CplNum, DWORD page, BYTE *buf);
WORD CALL MPC_MFULCWriteKey(BYTE CplNum,BYTE *key1,BYTE *key2);
WORD CALL MPC_MFULCAuthenticate(BYTE CplNum, BYTE *key1, BYTE *key2,DWORD RFU);

/*------------------*
 * VICINITY CARDS   *
 *------------------*/
WORD CALL MPC_SelectVCCommunication(BYTE CplNum, BYTE CodingMode,
                BYTE DataRateRx, BYTE NbSubCarrier);    // select coding mode and data rate
#define ONE_OUTOF_4     1       // VCD -> VICC coding mode= 1 out of 4
#define ONE_OUTOF_256   2       // VCD -> VICC coding mode= 1 out of 256
#define LOW_DATA_RATE   0       // VICC->VCD low data rate
#define HIGH_DATA_RATE  1       // VICC->VCD high data rate
#define ONE_SUBCARRIER  1       // VICC->VCD number of subcarrier
#define TWO_SUBCARRIER  2       // VICC->VCD number of subcarrier
WORD CALL MPC_ExchangeCmdVicinity(BYTE CplNum, BYTE *pFrameTX, WORD NbByte,
                BYTE *pFrameRX, WORD *pNbByteRX);       // Exchange command
// select vicinity pause width
WORD CALL MPC_SelectPauseWidthVicinity(BYTE CplNum, WORD PauseWidth);
#define MAX_PAUSE_WIDTH_VICINITY     9440       // maximum pause width (in ns)
#define MIN_PAUSE_WIDTH_VICINITY     100        // minimum pause width (in ns)

/*-----------------*
 * MISCELLANEOUS   *
 *-----------------*/
WORD CALL MPS_ResetHard(BYTE CplNum);
WORD CALL MPC_GetVDCIn(BYTE CplNum,int *pVDCIn, DWORD Duration, DWORD RFU);

// Select the Electro Magnetic Load on the Micropross NFC antenna for MP500 TCL3 (useful in card emulation, target passive mode)
WORD CALL MPC_SelectLoadAntennaNfc(BYTE CplNum, WORD Load);
#define ANTENNA_LOAD_82         82      // 82 Ohms
#define ANTENNA_LOAD_330        330     // 330 Ohms
#define ANTENNA_LOAD_820        820     // 820 Ohms

// Set low and high value of the signal generating the load modulation amplitude (useful in card emulation, target passive mode)
WORD CALL MPC_SetLMAForCardEmulation(BYTE CplNum, int lowValuemV, int highValuemV);
#define LMA_MIN_MV      -2000
#define LMA_MAX_MV      9000


/*------------------*
 * MODULATION SHAPE *
 *------------------*/
WORD CALL MPC_SetModulationShape(BYTE CplNum, DWORD NbPointsFallingEdge, WORD *PointsFallingEdge, DWORD NbPointsRisingEdge, WORD *PointsRisingEdge );
#define MODULATION_MIN_POINTS           2
#define MODULATION_MAX_POINTS           1500
WORD CALL MPC_SelectModulationGeneration(BYTE CplNum, BYTE Mode);
#define LINEAR_MODULATION               1
#define CUSTOMIZED_MODULATION           2       // values applied every 20 ns
#define CUSTOMIZED_MODULATION_CLK       3       // values applied every 1/fc
#define CUSTOMIZED_MODULATION_CLK_10    4       // values applied every 1/10 fc


/*------------------------------------*
 * ARBITRATY DISTURBANCE GENERATOR    *
 *------------------------------------*/
WORD CALL MPC_GenerateDisturbance(BYTE CplNum, BYTE DisturbanceOperation,
                BYTE DisturbanceType, long Amplitude, short Offset, DWORD Duration,
                DWORD Param1, DWORD Param2, DWORD Param3, DWORD Param4);
WORD CALL MPC_LoadDisturbanceWaveshape(BYTE CplNum, BYTE DisturbanceOperation, DWORD TimeBase,
                DWORD Count, short * pData);
WORD CALL MPC_SetDisturbanceTrigger(BYTE CplNum, BYTE DisturbanceOperation, DWORD Behavior,
        DWORD Delay, WORD Loop);
WORD CALL MPC_SetDisturbanceOffTrigger(BYTE CplNum, BYTE DisturbanceOperation,
                DWORD Behavior, DWORD Delay);
WORD CALL MPC_ResetDisturbance(BYTE CplNum, BYTE DisturbanceOperation);
#define SIGNAL_CARRIER_ADD      0
#define SIGNAL_CARRIER_MUL      1
#define DISTURBANCE_RAMP        0
#define DISTURBANCE_SQUARE      1
#define DISTURBANCE_SINE        2
#define DISTURBANCE_GLITCH      3


/*------------------*
 *    DEAF TIME     *
 *------------------*/
// set the deaf time in µs
WORD CALL MPC_SetDeafTime(BYTE CplNum, DWORD DeafTime);
#define DEAF_TIME_MIN   30              // 30 µs
#define DEAF_TIME_MAX   10000000        // 10 s

// Force or not ASK Modulation (testing purpose)
WORD CALL MPC_ForceModulationASK(BYTE CplNum, BYTE On);

/*----------------------*
 *    ANTI-TEARING      *
 *----------------------*/
WORD CALL MPS_AntiTearing(BYTE CplNum, DWORD ClockCount);
#define ANTI_TEARING_MAX        0x3FFFFFF

WORD CALL MPS_AntiTearing2(BYTE CplNum, DWORD StartEvent, DWORD RFU);
#define TRIGGER_IN_RISING_EDGE  2


/*------------------------------*
 * CHANGING PROTOCOL PARAMETERS *
 *------------------------------*/
WORD CALL MPC_ChangeProtocolParameters(BYTE CplNum, DWORD ParamType,
                void *Param, DWORD ParamSize);
WORD CALL MPC_GetProtocolParameters(BYTE CplNum, DWORD ParamType,
                void *Param, DWORD ParamSize, DWORD *pParamSize);
#define CPP_CID                         1
#define CPP_NAD                         2
        #define NOT_SUPPORTED                   0
        #define SUPPORTED                       1
#define CPP_FRAME_TYPE_B                3
#define CPP_FRAME_TYPE_B_CLK            4
#define CPP_FRAME_FDT                   5
        #define FRAME_FDT_MIN                   0
        #define FRAME_FDT_MAX                   1000
#define CPP_PROTOCOL_ERROR_MANAGEMENT   6
        #define OFF                             0
        #define ON                              1
#define CPP_PROTOCOL_STOP_TIMEOUT       7
#define CPP_TX_PARITY                   10
#define CPP_RX_PARITY                   11
#define CPP_CREATE_PARITY_ERROR         12
#define CPP_TADT_PLUS_NTRFW             15      // used in active mode, time must be between 70 and 1356000 clk (about between 5 us and 100 ms)
#define CPP_DELAY_RF_SWITCHED_OFF       16      // used in active mode, time must be less or equal to 1356000 clk (about 100 ms)
#define CPP_TARFG                       19      // used in active mode, time must be between 1024/fc (75 µs) and 1356000/clk (100 ms)

#define CPP_ANTI_EMD                    20      // to enable or disable the EMD suppression functionality
#define CPP_VERIFY_PICC_14443_RESPONSE_TIME 24  // to get an error if the PICC timings (FDT, TR0,TR1) are out of range
#define CPP_INITIAL_RF_FIELD_DURATION_IN_ACTIVE_TARGET  28      // used in target active mode ( time in ms, default 1000 ms , max 5000 ms)
#define CPP_EGT_BEFORE_EOF_CLK          31      // allow to add an EGT (in 1/fc) after the last byte and before EOF (type B direction PCD->PICC)
#define CPP_FRAME_FELICA_OPTION         32      // for test purpose, allow to enable disable preamble , synchro bytes and data (direction PCD->PICC)
#define CPP_FORCE_PICC_14443_MAX_FRAME_SIZE 34  // to force the maximum frame size of the PICC (overwrite value present in ATQB or ATS)
#define CPP_POWER_ON_TRIGGER_IN         36      // allow to set the RF Field ON/OFF using the trigger in
#define CPP_ACTIVE_TARGET_MUTE_BEHAVIOR 38      // allow to detect a mute target even if the target doesn't switch On/off the RF field
#define CPP_CHANGE_BIT_BOUNDARY         40      // allow to change the bit boundary type B reader frame
#define CPP_FELICA_DUTY_CYCLE           41      // In poller/reader mode, allows to change the duty cycle of the FeliCa modulation
#define CPP_FELICA_BIT_CODING_REVERSE   42      // In poller/reader mode, allows to change the bit coding of the FeliCa modulation
#define CPP_SET_BURST_NB_SAMPLE         103		// Change the number of burst samples
// Set ETU width (in carrier cycles) for logical 0 and logical 1 (type B only)
WORD CALL MPC_SelectETUWidthTX(BYTE CplNum, WORD ETULogic0, WORD ETULogic1);
#define MIN_ETUWidthTX  16
#define MAX_ETUWidthTX  255


/*----------------------*
 * Encryption using DES *
 *----------------------*/
WORD CALL MPS_InitDES(BYTE CplNum, BYTE *key, WORD Flag);
WORD CALL MPS_InitTripleDES(BYTE CplNum, BYTE *key1, BYTE *key2, BYTE *key3, WORD Flag);
WORD CALL MPS_ComputeDES(BYTE CplNum, BYTE *DataIn, BYTE *DataOut);
WORD CALL MPS_ComputeTripleDES(BYTE CplNum, BYTE *DataIn, BYTE *DataOut);


/*--------------------------*
 *  IMPEDANCE MEASUREMENT   *
 *--------------------------*/
WORD CALL MPC_GetImpedance(BYTE CplNum, BYTE WithWhat, DWORD Spare,
                int *pReal, int *pImaginary, int *pResistance, int *pCapa,
                DWORD *pVccmV, DWORD *pIccmA);
// define for parameter WithWhat
#define WITH_CABLE              1       // impedance is placed at the extremity of the coax cable
#define WITH_CABLE_AND_HEAD     2       // impedance is placed at the extremity of the coax cable and the head

// API for impedance calibration
WORD CALL MPC_ImpedanceCalibration(BYTE CplNum, BYTE Step, DWORD Rfu,
                BYTE ImpedanceNumber, int Rvalue, int Xvalue);
#define CABLE_COMPENSATION_INIT         4
#define CABLE_COMPENSATION_CALIB        5
        #define REFERENCE_IMPEDANCE_OPEN        0
        #define REFERENCE_IMPEDANCE_50          1
        #define REFERENCE_IMPEDANCE_SHORT       2
#define CABLE_COMPENSATION_STORE        6
#define FREQ_COMPENSATION_INIT          14
#define FREQ_COMPENSATION_CALIB         15
#define FREQ_COMPENSATION_STORE         16

// Store calibration value for head compensation
WORD CALL MPC_StoreHeadCalibrationValue(BYTE CplNum,
                long R0,long X0, long R50, long X50);

/*------------------------------------*
 * RESONANCE FREQUENCY MEASUREMENT    *
 *------------------------------------*/
WORD CALL MPC_ResonanceFrequency(BYTE CplNum, BYTE Calibre,
                WORD Amplitude, DWORD FreqMin, DWORD FreqMax,
                DWORD *pFrequency, DWORD *pQ);
WORD CALL MPC_ResonanceFrequencyVS(BYTE CplNum, BYTE Calibre,
                int dbm, DWORD step, DWORD RFU, DWORD FreqMin,
                DWORD FreqMax, DWORD *pFrequency, DWORD *pQ);
#define FREQ_DBM_MIN    -17
#define FREQ_DBM_MAX    10
#define FREQ_RESONANCE_MAX      24000000        // 24 MHz
#define FREQ_RESONANCE_MIN      11000000        // 11 MHz
#define STEP_FREQ_RESONANCE     50000           // 50 kHz
WORD CALL MPC_GetMeasureResonanceFrequency(BYTE CplNum, DWORD *pCount, long **pArray);

#define NB_INITDATARF           2 * ((FREQ_RESONANCE_MAX - FREQ_RESONANCE_MIN) / STEP_FREQ_RESONANCE + 1)
WORD CALL MPC_GetInitDataResonanceFrequency(BYTE CplNum, int *pArray);
WORD CALL MPC_SetInitDataResonanceFrequency(BYTE CplNum, int *pArray);

/*------------------------------------*
 * READER FREQUENCY MEASUREMENT       *
 *------------------------------------*/
WORD CALL MPC_GetRFFrequency(BYTE CplNum, DWORD resolution,DWORD RFU, DWORD *pFrequency);
#define RESOLUTION_1000HZ  1
#define RESOLUTION_100HZ   2
#define RESOLUTION_10HZ    3
#define RESOLUTION_1HZ     4

/*--------------------------*
 * RF FIELD MEASUREMENT     *
 *--------------------------*/
WORD CALL MPC_GetRFField(BYTE CplNum, DWORD Assembly, DWORD *pValue);
#define RFFIELD_ASSEMBLY_1      1
#define RFFIELD_ASSEMBLY_2      2

/*--------------------------*
 * RX CHANNEL SELECTION     *
 *--------------------------*/
WORD CALL MPC_SelectRxChannel(BYTE CplNum, WORD RXChannel);
#define RX_CHANNEL_1    1       // normal channel TX/RX
#define RX_CHANNEL_2    2       // separated RX - 1MO hms
#define RX_CHANNEL_3    3       // separated RX - 50 Ohms


// Adjust level on Separated RX signal
WORD CALL MPC_AdjustRX_Channel_2(BYTE CplNum);

// Force level on Separated RX signal
WORD CALL MPC_ForceGainExtRx(BYTE CplNum, DWORD GainRX);


/*--------------------------*
 * VOLTMETER                *
 *--------------------------*/
WORD CALL MPC_SelectVoltmeterRange(BYTE CplNum, WORD Range);
#define VOLTMETER_RANGE_50        50         // 50 mVpp
#define VOLTMETER_RANGE_100       100        // 100 mVpp
#define VOLTMETER_RANGE_200       200        // 200 mVpp
#define VOLTMETER_RANGE_500       500        // 500 mVpp
#define VOLTMETER_RANGE_1000      1000       // 1 Vpp
#define VOLTMETER_RANGE_2000      2000       // 2 Vpp
#define VOLTMETER_RANGE_5000      5000       // 5 Vpp
#define VOLTMETER_RANGE_10000     10000      // 10 Vpp

WORD CALL GetMeasureVoltmeterToFile(BYTE CplNum, DWORD NbValue, const char *pFileName);


// Measurements types defines
#define SCINFC_MEASTYPE_PICC_LMA               0               // Mesure load modulation amplitude for PICCs
#define SCINFC_MEASTYPE_PCD_FIELD_STRENGTH     1               // Mesure field strength of PCD
#define SCINFC_MEASTYPE_PCD_WAVEFORM           2               // Mesure waveforms of PCDs (t1->t5; modulation indexes; ovs, undershoot...)
WORD CALL GetAnalyzedMeasureVoltmeterToFile(BYTE CplNum,
											DWORD MeasureType,
											DWORD CardType,
											DWORD Datarate,
											DWORD Rfu,
											const char *pFileName,
											unsigned int *pMeasurementCount,
											unsigned int **pMeasurementValues);

WORD CALL MPC_GetDatarate(BYTE CplNum, BYTE Type, DWORD *pDatarate);


WORD CALL GetLicenseContextFile(const char *pFileName);

/*--------------------------*
 * COMMAND FOR A RELAY      *
 *--------------------------*/
WORD CALL MPC_SetRelay(BYTE CplNum, BYTE RelayNumber, BYTE OnOff);
#define RELAY1          1
#define RELAY_OFF       0
#define RELAY_ON        1

/*--------*
 * MIFARE *
 *--------*/
int CALL CLP_Request(BYTE CplNum, unsigned char mode, unsigned int *type);
#define IDLE    0x00
#define ALL     0x01
int CALL CLP_Anticoll(BYTE CplNum, unsigned char bc, unsigned long *snr);
int CALL CLP_Anticoll_CL2(BYTE CplNum, unsigned char bc, unsigned long *snr);
int CALL CLP_Anticoll_CL3(BYTE CplNum, unsigned char bc, unsigned long *snr);
int CALL CLP_GetLastErrorNumber(BYTE CplNum, int *ErrorNumber);
int CALL CLP_LoadKey(BYTE CplNum, unsigned char auth_mode, unsigned char key_addr,
                char *n_key);
#define KEYA    0x00
#define KEYB    0x04
int CALL CLP_Authentication_2(BYTE CplNum, unsigned char mode, unsigned char key_addr,
                unsigned char block);
int CALL CLP_Authentication_3(BYTE CplNum, unsigned char mode, unsigned char key_addr,
                unsigned char block, unsigned long snr);
int CALL CLP_Select(BYTE CplNum, unsigned long snr, unsigned char *sak);
int CALL CLP_Select_CL2(BYTE CplNum, unsigned long snr, unsigned char *sak);
int CALL CLP_Select_CL3(BYTE CplNum, unsigned long snr, unsigned char *sak);
int CALL CLP_Read(BYTE CplNum, unsigned char bn, unsigned char *d);
int CALL CLP_Write(BYTE CplNum, unsigned char bn, unsigned char *d);
int CALL CLP_Increment(BYTE CplNum, unsigned char addr, unsigned long value);
int CALL CLP_Decrement(BYTE CplNum, unsigned char addr, unsigned long value);
int CALL CLP_Transfer(BYTE CplNum, unsigned char addr);
int CALL CLP_Decrement_Transfer(BYTE CplNum, unsigned char addr, unsigned long value);
int CALL CLP_Restore(BYTE CplNum, unsigned char Adr);
int CALL CLP_Halt(BYTE CplNum);
int CALL CLP_PersonalizeUIDUsage(BYTE CplNum,unsigned char PersonalizationOption);
// value for PersonalizationOption
#define UIDF0   0x00
#define UIDF1   0x40
#define UIDF2   0x20
#define UIDF3   0x60


// Error codes for MIFARE
#define MI_OK                   0
#define MI_NOTAGERR             1
#define MI_CRCERR               2
#define MI_AUTHERR              4
#define MI_PARITYERR            5
#define MI_CODEERR              6
#define MI_SERNRERR             8
#define MI_NOTAUTHERR           10
#define MI_BITCOUNTERR          11
#define MI_BYTECOUNTERR         12
#define MI_WRITEERR             15
#define MI_OVFLERR              19
#define MI_FRAMINGERR           21
#define MI_COLLERR              24
#define MI_INTERFACEERR         26
#define MI_ACCESSTIMEOUT        27
#define MI_VALERR               124

/*----------------*
 * SPECIFIC TESTS *
 *----------------*/
WORD CALL MPC_Test(BYTE CplNum, DWORD TestType, ...);
#define TEST_REQA_REQB                          1
#define TEST_REQB_REQA                          2
#define TEST_POWERON_REQA                       3
#define TEST_POWERON_REQB                       4
#define TEST_REQA_REQA                          5
#define TEST_REQB_REQB                          6
#define TEST_FDT_PICCPCD_A                      8
#define TEST_FDT_PICCPCD_B                      9
#define TEST_POWER_OFF_ON_CMD                   15
#define TEST_WUPA_WUPB                          16
#define TEST_WUPB_WUPA                          17
#define TEST_RF_RESET_CMD                       18
#define TEST_RF_RESET_CMD_WITH_TRIGGER_IN       19
#define TEST_FDT_PICCPCD_FELICA                 20
#define TEST_TON_EXCHANGE_AFTER_DELAY_TOFF        22

#define MIN_TEST_TIME   2000
#define MAX_TEST_TIME   1000000


/*----------------------------------------------------------
 * SEQUENCER                                               *                                                *
 * COMMON DEFINITIONS FOR READER AND CARD EMULATION        *
 *---------------------------------------------------------*/
#define TSCN_PARAM_SOF_LOW              2
#define TSCN_PARAM_SOF_HIGH             3
#define TSCN_PARAM_EGT                  4
#define TSCN_PARAM_EOF                  5
#define TSCN_DO_EXCHANGE                22
#define TSCN_DO_PARITY_ERROR            23
#define TSCN_DO_CHANGE_DATA_RATE        24
#define TSCN_DO_USER_EVENT              25
#define TSCN_DO_TRIGGER_OUT             26
#define TSCN_DO_TRIGGER_OUT_RX_ON       41
        #define TRIGGER_OUT1                    1
        #define TRIGGER_OUT2                    2
        #define TRIGGER_OUT3                    3
        #define TRIGGER_OFF                     0
        #define TRIGGER_ON                      1

/*----------------------------------*
 * DEFS FOR SEQUENCER READER        *
 *----------------------------------*/
WORD CALL MPC_OpenScenarioPicc(BYTE CplNum, DWORD *pScnID, DWORD Mode, DWORD Reserved);
WORD CALL MPC_ExecuteScenarioPicc(BYTE CplNum, DWORD ScnID, DWORD Reserved);
WORD CALL MPC_CloseScenarioPicc(BYTE CplNum, DWORD ScnID);
WORD CALL MPC_AddToScenarioPicc(BYTE CplNum, DWORD ScnID, DWORD ScnTagID, ...);

#define TSCN_PARAM_CARD_TYPE            1
#define TSCN_PARAM_START_BIT            6
#define TSCN_PARAM_B1                   7
#define TSCN_PARAM_B2                   8
#define TSCN_PARAM_B3                   9
#define TSCN_PARAM_B4                   10
#define TSCN_PARAM_B5                   11
#define TSCN_PARAM_B6                   12
#define TSCN_PARAM_B7                   13
#define TSCN_PARAM_B8                   14
#define TSCN_PARAM_STOP_BIT             15
#define TSCN_PARAM_PAUSE_WIDTH          16
        #define PAUSE_WIDTH_MIN                 100
        #define PAUSE_WIDTH_MAX                 4500
#define TSCN_PARAM_FWT                  17
#define TSCN_PARAM_FDT_PCD              18
#define TSCN_DO_TEMPO                   19
#define TSCN_DO_RF_FIELD_STRENGTH       20
#define TSCN_DO_RF_RESET                21
        #define EXCHANGE_NOPCDCRC               1       // with TSCN_DO_EXCHANGE
        #define EXCHANGE_PCDCRC                 2
        #define EXCHANGE_WAIT_RX                1
        #define EXCHANGE_NO_WAIT_RX             2
#define TSCN_DO_CHANGE_VC_COMMUNICATION 35
#define TSCN_PARAM_PAUSE_WIDTH_VICINITY 36
        #define PAUSE_WIDTH_VICINITY_MIN        100
        #define PAUSE_WIDTH_VICINITY_MAX        9440
#define TSCN_DO_RF_RESET_CMD            40
#define TSCN_DO_TON_EXCHANGE_AFTER_DELAY_TOFF 54
#define TSCN_DO_TX_PARITY                      55
#define TSCN_PARAM_MODULATION_ASK_PT           56

/*----------------------------------------------*
 * DEFINITIONS FOR CARD EMULATION SEQUENCER     *
 *----------------------------------------------*/
// Open RX & TX channel in simulation mode
WORD CALL MPC_ChannelOpen(BYTE CplNum, BYTE Mode);
#define SIM_MODE        2       // only Simulation mode is allowed

// Get or Set ChannelOpen parameters
WORD CALL MPC_ChannelGetParameter(BYTE CplNum, DWORD Parameter, DWORD *pValue);
WORD CALL MPC_ChannelSetParameter(BYTE CplNum, DWORD Parameter, DWORD Value);
// force old MPC_ChannelOpen (CHOP) behavior for TCL2 compatibility
#define CHANNEL_TCL2                    1
// same as CHANNEL_TCL2, but store configuration into non volatile memory
#define CHANNEL_TCL2_NON_VOLATILE       2

// Close RX & TX channel
WORD CALL MPC_ChannelClose(BYTE CplNum);
// Open scenario
WORD CALL MPC_OpenScenarioPcd(BYTE CplNum, DWORD *pScnID, DWORD Reserved);
// Add action to scenario
WORD CALL MPC_AddToScenarioPcd(BYTE CplNum, DWORD ScnID, DWORD ScnTagID, ...);
// Start scenario execution
WORD CALL MPC_ExecuteScenarioPcd(BYTE CplNum, DWORD ScnID, DWORD Timeout);
// Close scenario generator
WORD CALL MPC_CloseScenarioPcd(BYTE CplNum, DWORD ScnID);

#define PCD_FRAME_DONT_CARE                     1000000
#define PICC_FRAME_SENT_ON_TRIGGER_IN           1000001
#define PICC_FRAME_SENT_ON_TRIGGER_IN_WITH_DELAY 1000002
#define PICC_FRAME_SENT_ON_TRIGGER_IN_WITH_DELAY_NS 1000003

#define TSCN_DO_TEMPO                           19
#define TSCN_DO_EXCHANGE                        22
        #define EXCHANGE_NOPCDCRC                       1
        #define EXCHANGE_PCDCRC                         2
        #define EXCHANGE_NOPICCCRC                      1
        #define EXCHANGE_PICCCRC                        2
        #define EXCHANGE_SYNCHRO_ON                     1
        #define EXCHANGE_SYNCHRO_OFF                    0
#define TSCN_PARAM_TR0                          27
#define TSCN_PARAM_TR1                          28
#define TSCN_PARAM_TRF                          29
#define TSCN_PARAM_FDT1_PICC                    30
#define TSCN_DO_SEQUENCE_ERROR                  32
        #define SEQUENCE_ERROR_C                        1
        #define SEQUENCE_ERROR_D                        2
        #define SEQUENCE_ERROR_E                        7
#define TSCN_DO_EMD                             33
#define TSCN_DO_CHANGE_VC_DATA_RATE             34
#define TSCN_DO_COMPLETE_ANTICOLLISION          35
#define TSCN_DO_WAIT_VC_EOF_ONLY                36
#define TSCN_EMD_SUBCARRIER                     37
#define TSCN_SET_PCD_MASK                       38
#define TSCN_DO_EXCHANGE_RAW_TYPEA              39
#define TSCN_DO_TRIGGER_OUT_EMD_GENERATION      42
#define TSCN_PARAM_EGT_BEFORE_EOF               53

#define NB_ENTRY_EMD_MAX                        512
#define FDT_EMD_MIN                             420


/*------------------------------------------*
 * GETTING THE RESPONSE TIME OF THE PICC    *
 *------------------------------------------*/
WORD CALL MPC_PiccResponseTime(BYTE CplNum, DWORD Param, DWORD Unit, DWORD *pTime);
#define PRT_ENABLE      1
#define PRT_DISABLE     2
#define PRT_GET         0
#define PRT_GET_TR1     3

// improved API to get response time
WORD CALL MPC_PiccResponseTime2(BYTE CplNum, DWORD Param, DWORD Unit,
                DWORD *ResponseTime, DWORD *NbValues);
#define PRT_CLEAR               4
#define PRT_ENABLE2             5
#define PRT_LAST_APDU           6
#define MAX_NB_VALUE_FDT        10

/*------------------------------------------*
 *              NFC SPECIFIC                *
 *------------------------------------------*/

WORD CALL MPC_NfcConfiguration(BYTE CplNum, BYTE Mode, BYTE InitiatorOrTarget,
                WORD DataRate);
// define for parameter Mode
#define NFC_PASSIVE_MODE        1
#define NFC_ACTIVE_MODE         2

// define for parameter Initiator or Target
#define NFC_INITIATOR   1
#define NFC_TARGET      2

// set up of the reference Picc
WORD CALL MPC_SetUpReferencePICC(BYTE CplNum, BYTE OnOff,DWORD SubCarrier);

// For target passive, adjust the RF field level
WORD CALL MPC_AdjustLevelRFField(BYTE CplNum, DWORD Time);

WORD CALL MPC_NfcRFCollisionAvoidance (BYTE CplNum,  BYTE Unit, WORD value);
WORD CALL MPC_NfcWaitAndGetFrameAsTarget( BYTE CplNum, DWORD Timeout_ms,  BYTE *pFrameRX, DWORD RFU, DWORD *pNbByteRX);
WORD CALL MPC_NfcSendFrameAsTarget( BYTE CplNum, DWORD RFU, BYTE *pFrame, DWORD ByteCount);


WORD CALL MPC_SensReq(BYTE CplNum, WORD *SENS_RES);
WORD CALL MPC_AllReq(BYTE CplNum, WORD *SENS_RES);
WORD CALL MPC_Sdd(BYTE CplNum, BYTE *NFCID1, WORD *plng, BYTE *pSEL_RES);
WORD CALL MPC_SelReq(BYTE CplNum, BYTE *NFCID1, WORD lng, BYTE *pSEL_RES);
WORD CALL MPC_SlpReq(BYTE CplNum);
WORD CALL MPC_PollReq(BYTE CplNum, WORD SystemCode, BYTE RC, BYTE TSN,
                BYTE *NFCID2, BYTE *Pad, WORD *RD);
WORD CALL MPC_AtrReq(BYTE CplNum, BYTE *ATR_REQ, WORD LngATR_REQ, BYTE *ATR_RES,
                WORD *pLngATR_RES);
WORD CALL MPC_DeselectReq(BYTE CplNum);
WORD CALL MPC_ReleaseReq(BYTE CplNum);
WORD CALL MPC_PslReq(BYTE CplNum, BYTE BRS, BYTE FSL);
WORD CALL MPC_DepReq(BYTE CplNum, BYTE *pFrameTX, DWORD NbByteTX, BYTE *pFrameRX,
                DWORD *pNbByteRX);
WORD CALL MPC_ExchangeNFCData(BYTE CplNum, BYTE *pFrameTX, WORD NbByte,
                BYTE *pFrameRX, WORD *pNbByteRX);


/*--------------*
 * TRIGGER OUT  *
 *--------------*/
// trigger out 1
WORD CALL MPC_Trigger_Out(BYTE CplNum, DWORD Behavior, DWORD Value);
// trigger out 2
WORD CALL MPC_Trigger2_Out(BYTE CplNum, DWORD Behavior, DWORD Value);
// trigger out 3
WORD CALL MPC_Trigger3_Out(BYTE CplNum, DWORD Behavior, DWORD Value);
// Behavior definition
#define TRIG_FORCE                      1
#define TRIG_TX_ON                      2
#define TRIG_TX_OUT                     3
#define TRIG_RX_ON                      6
#define TRIG_DELAY_AFTER_TX             7
#define TRIG_CARRIER                    8
#define TRIG_MODULATION_CARD_EMULATION  9
#define TRIG_EMD_GENERATION             17
#define TRIG_IN                         20

// trigger get logic state
WORD CALL MPC_GetTrigger(BYTE CplNum, BYTE Index, DWORD * pValue);
#define TRIGGER_IN4             4


/*------------*
 * SPY MODULE *
 *------------*/
WORD CALL MPS_OpenLog(BYTE CplNum, DWORD MaskEvent, DWORD Rfu2);

WORD CALL MPS_SetUserEvent(BYTE CplNum, BYTE UserEventNum);
#define USER_EVENT1     0
#define USER_EVENT2     1
#define USER_EVENT3     2
#define USER_EVENT4     3
#define USER_EVENT5     4
#define USER_EVENT6     5
#define USER_EVENT7     6
#define USER_EVENT8     7


WORD CALL MPS_LogClockSelect(BYTE CplNum, WORD Mode);
#define INTERNAL_CLOCK_MODE     2

WORD CALL MPS_FlushLog(BYTE CplNum);
WORD CALL MPS_CloseLog(BYTE CplNum);


WORD CALL MPS_EndDownload(BYTE CplNum);

WORD CALL MPC_StartBurstMeasure(BYTE CplNum, DWORD EventMode, long Delay_ns,
                WORD NbCharReader, WORD NbCharCard, BYTE Char);
WORD CALL MPC_StartRFMeasure (BYTE CplNum, DWORD EventMode, long Delay_ns,
                DWORD FrameLength, BYTE *pFrame, BYTE *pMask);
#define TRIG_EVT_TRIG_FORCE     100
#define TRIG_EVT_RF_FIELD_ON    101
#define TRIG_EVT_RF_FIELD_OFF   102
#define TRIG_EVT_TX_ON          103
#define TRIG_EVT_TX_OFF         104
#define TRIG_EVT_RX_ON_NFC      105
#define TRIG_EVT_FS_DETECT_ON   106
#define TRIG_EVT_FS_DETECT_OFF  107
#define TRIG_EVT_SOF_PCD_TYPE_B 415
#define TRIG_EVT_Z				404
#define TRIG_EVT_SOF_PCD_TYPE_F 423

#define TRIG_EVT_CHAR_PCD_TYPE_A        412
#define TRIG_EVT_CHAR_PCD_TYPE_B        418
#define TRIG_EVT_CHAR_PCD_TYPE_F        426

WORD  CALL MPC_SelectInputImpedanceAnalogIn(BYTE CplNum, DWORD Impedance);
#define INPUT_IMPEDANCE_1M         1000000      // 1 Mega Ohms
#define INPUT_IMPEDANCE_50         50           // 50 Ohms




#define TRIGGER_DELAY_MIN_LIMIT       -1900000                // -1,9 ms


/****************************************
 *                OTHER API             *
 ****************************************/

WORD CALL MPC_GetRxGainExternalRx(BYTE CplNum, DWORD *value);

WORD CALL MPC_SwitchResonanceFrequencyConnector (BYTE CplNum, BYTE Connector);
WORD CALL MPC_DefaultResonanceFrequencyConnector (BYTE CplNum, BYTE Connector);
#define CONNECTOR_TX_RX                 1
#define CONNECTOR_RES_FREQ              2


WORD CALL MPS_CouplerEepromSys(BYTE CplNum, DWORD Type, void *pResult);
#define EEP_COUPLER_IDENT               1       // Coupler Identification number
#define EEP_COUPLER_REVISION            2       // Coupler revision
#define EEP_COUPLER_IDENT2              3       // Coupler Identification number - extended
#define EEP_COUPLER_EXTERNAL_IDENT      4       // External Device Identification number
#define EEP_COUPLER_EXTERNAL_REVISION   5       // External Device Coupler revision

WORD CALL MPS_CouplerCheckLicense(BYTE CplNum, DWORD LicenseId, DWORD *pValidLicense);
#define LICENSE_ISO             1
#define LICENSE_SIMULATOR       6
#define LICENSE_VHBR            12
#define LICENSE_125K            18
#define LICENSE_QI              19
#define LICENSE_ADVANCED_MEAS   21
#define LICENSE_QI_CALIBRATOR   22
#define LICENSE_BIT_BOUNDARY    23

// Counter management
WORD CALL MPS_Counter(BYTE CplNum, DWORD CounterID, DWORD CounterCMD, DWORD *pValue);
// Counter ID
#define COUNTER_CL_PROTOCOL_ERROR_RECOVERY      1
// Counter Command
#define COUNTER_RESET   1
#define COUNTER_READ    2

// Get Internal Parameters
WORD CALL MPS_GetInternalParameter(BYTE CplNum, DWORD ParamId, DWORD RFU, DWORD *pParamValue);
#define INTERNAL_FIELD_STRENGTH                 100
#define INTERNAL_MODULATION_ASK                 101
#define CONFIG_FIELD_STRENGTH                   200
#define CONFIG_MODULATION_ASK_TYPE_A            201
#define CONFIG_MODULATION_ASK_TYPE_B            202
#define CONFIG_MODULATION_ASK_TYPE_FELICA       203
#define CONFIG_MODULATION_ASK_TYPE_VICINITY     204
#define CONFIG_MODULATION_FALL_TIME             205
#define CONFIG_MODULATION_RISE_TIME             206
#define CONFIG_FIELD_RISE_TIME                  207


WORD CALL MPS_OffsetCompensation(BYTE CplNum, BYTE Operation);
#define OFFSET_WPC_COMPENSATE_ADC       0 // Measurements offset
#define OFFSET_WPC_COMPENSATE_VR        1 // Secondary Vr offset
#define OFFSET_WPC_RESET_ADC_COMP       0xFF
#define OFFSET_WPC_RESET_VR_COMP        0xFE


/*-----------------*
 * CONCURRENT MODE *
 *-----------------*/
WORD CALL MPS_SpyOpen(BYTE CplNum);
WORD CALL MPS_SpyClose(BYTE CplNum);

/*----------------------------------------------------*
 * Simulators - Initialization phase APIs (14443-3)
 *----------------------------------------------------*/
/* For simulation */
#define UID_SUPPORTED_SIZE              4
#define REQA_SUPPORTED_SIZE             1
#define WUPA_SUPPORTED_SIZE             1
#define HLTA_SUPPORTED_SIZE             4
#define ATQA_SUPPORTED_SIZE             2
#define ANTICOLL_SUPPORTED_SIZE 2
#define SELECT_SUPPORTED_SIZE   7
#define SAK_SUPPORTED_SIZE              1

#define REQB_SUPPORTED_SIZE             5
#define WUPB_SUPPORTED_SIZE             5
#define HLTB_SUPPORTED_SIZE             5
#define ATQB_SUPPORTED_SIZE             12
#define EXT_ATQB_SUPPORTED_SIZE 13
#define ATTRIB_SUPPORTED_SIZE   5

// Set Initialization parameters for type A & B simulation
WORD CALL MPC_Set14443BInitParameters( BYTE CplNum, DWORD ATQBLength, BYTE *pATQB);
WORD CALL MPC_Set14443AInitParameters( BYTE CplNum, BYTE *pATQA, DWORD RFU, BYTE *pUID, BYTE *pSAK, DWORD ATSLength, BYTE *pATS);

// Get RATS sent by reader
WORD CALL MPC_GetRATS(BYTE CplNum, TYPEA_RATS_STRUCT *pBufRATS);

// Get ATTRIB sent by reader
WORD CALL MPC_GetATTRIB( BYTE CplNum, BYTE *pATTRIB, DWORD* pATTRIBLength);

/*----------------------------------------------------*
 * Simulators - Frames APIs (14443-3)
 *----------------------------------------------------*/

// Check CRC validity of frame type A, B
WORD CALL MPC_CheckCRCFrame( BYTE CplNum, int FrameType, BYTE *pFrame, DWORD ByteCount);
// Compute frame CRC
WORD CALL MPC_ComputeCrc( BYTE CplNum, int FrameType, BYTE *pData, DWORD Length, BYTE *pCRCLow, BYTE *pCRCHigh);
// Get last raw frame
WORD CALL MPC_GetRawFrame(BYTE CplNum, int *pFrameType, BYTE *pFrame, DWORD *pByteCount);
// Get raw frames exchanged
WORD CALL MPC_GetBufferedRawFrame(BYTE CplNum, int *pFrameType, BYTE *pFrame, DWORD *pByteCount);
// Send Raw frame
WORD CALL MPC_SendRawFrame( BYTE CplNum, BYTE *pFrame, DWORD ByteCount);
// Send Raw frame with CRC
WORD CALL MPC_SendRawFrameWithCRC( BYTE CplNum, BYTE *pFrame, DWORD ByteCount);

/*----------------------------------------------------*
 * Simulators - APDU level APIs (14443-4)
 *----------------------------------------------------*/

WORD CALL MPS_GetAPDU(BYTE CplNum, APDU_HEADER *pHAPDU, BYTE *pLcData, DWORD *pLcDataLen);

WORD CALL MPS_GetAPDU2(BYTE CplNum, APDU_HEADER *pHAPDU, BYTE *pLcData, DWORD *pLcDataLen, DWORD *pAPDULen);

WORD CALL MPS_SendRAPDU(BYTE CplNum, BYTE *pRAPDU, DWORD RAPDULen, WORD SW1SW2);

/*----------------------------------------------------*
 * Simulators - NFC Initialization phase APIs (18092)
 *----------------------------------------------------*/

// Set Initialization parameters for type NFC simulation
WORD CALL MPC_SetNFCInitParameters(BYTE CplNum,
                                                               BYTE Mode,
                                                               WORD DataRate,
                                                               DWORD MuteEventMask,
                                                               BYTE *pSENS_RES,
                                                               BYTE *pSEL_RES,
                                                               DWORD NFCIDLength,
                                                               BYTE *pNFCID,
                                                               DWORD ATR_RESLength,
                                                               BYTE *pATR_RES);

// NFC Frame definitions
#define NFC_TRANSPORT_MAX_LENGTH 254
#define NFC_TRANSPORT_CMD_LENGTH 2
#define NFC_TRANSPORT_DATA_LENGTH NFC_TRANSPORT_MAX_LENGTH - NFC_TRANSPORT_CMD_LENGTH

// ATR_REQ definitions
#define NFC_ATR_REQ_NFCID3_LENGTH                       10
#define NFC_ATR_REQ_MAX_GENERAL_BYTES           NFC_TRANSPORT_DATA_LENGTH - NFC_ATR_REQ_NFCID3_LENGTH - 4

WORD CALL MPC_GetNFC_ATR_REQ(BYTE CplNum, WORD* pATR_REQLength, NFC_ATR_REQ_STRUCT *pBufATR_REQ);

WORD CALL MPC_GetNFC_WUP_REQ(BYTE CplNum, WORD* pWUP_REQLength, NFC_WUP_REQ_STRUCT *pBufWUP_REQ);

/*----------------------------------------------------*
 * Simulators - NFC protocol level APIs (18092)
 *----------------------------------------------------*/
WORD CALL MPC_GetNFC_PSL_REQ(BYTE CplNum, NFC_PSL_REQ_STRUCT* pBufPSL_REQ);

WORD CALL MPC_SendNFC_PSL_RES(BYTE CplNum, BYTE* pDID, WORD* pCRC);

WORD CALL MPC_SendNFC_DSL_RES(BYTE CplNum, BYTE* pDID, WORD* pCRC);

WORD CALL MPC_SendNFC_RLS_RES(BYTE CplNum, BYTE* pDID, WORD* pCRC);

WORD CALL MPC_SendNFC_WUP_RES(BYTE CplNum, BYTE* pDID, WORD* pCRC);

WORD CALL MPC_SendNFCTimeoutExtensionRequest(BYTE CplNum, BYTE Ext);

#define NFC_USERDATA_MAX_LENGTH         255
#define NFC_DEP_HEADER_LENGTH           5
#define NFC_DEP_LENGTH    NFC_DEP_HEADER_LENGTH + NFC_USERDATA_MAX_LENGTH

WORD CALL MPC_GetNFC_DEP_REQ(BYTE CplNum, DWORD* pDepReqLength, BYTE* pDepReqData);

// Send custom DEP_RES frame
#define PFB_INFO                 0x00
#define PFB_NACK                 0x48
#define PFB_ACK                  0x40
#define PFB_ATT                  0x80
#define PFB_TOE                  0x88
#define PFB_PNI_MSK              0x03
WORD CALL MPC_SendNFC_DEP_RES(BYTE CplNum, BYTE PFB, DWORD UserDataBytesLength, BYTE* pUserDataBytes, BYTE* pDID, BYTE* pNAD, WORD* pCRC);

// Higher level API (DEP_REQ, DEP_RES with information PDU)

WORD CALL MPC_GetNFC_UserData(BYTE CplNum, DWORD* pDataLength, BYTE* pData);

WORD CALL MPC_SendNFC_RUserData(BYTE CplNum, DWORD DataLength, BYTE* pData);

/*----------------------------------------------------*
 * Simulators management - Start & Stop
 *----------------------------------------------------*/

// General simulators definition
// Contactless simulators starts at bit 16
#define CL_14443_SIMULATOR      (1 << 16)
#define CL_FELICA_SIMULATOR     (1 << 17)
#define CL_VICINITY_SIMULATOR   (1 << 18)
#define CL_NFC_SIMULATOR        (1 << 19)

#define ALL_SIMULATORS (CL_14443_SIMULATOR | CL_FELICA_SIMULATOR | CL_VICINITY_SIMULATOR | CL_NFC_SIMULATOR)

WORD CALL MPS_SimStart(BYTE CplNum, DWORD Protocol, DWORD EventMask);

WORD CALL MPS_SimStop(BYTE CplNum, DWORD Protocol);

// Start simulation
WORD CALL MPS_SimWaitNStart(BYTE CplNum, DWORD Protocol, DWORD EventMask, BOOL StartSpy, DWORD Timeout);

/*----------------------------------------------------------------*
 * Simulators management - Event driven simulator
 *-----------------------------------------------------------------*/

WORD CALL MPS_WaitSimEvent(BYTE Cplnum, DWORD Timeout, DWORD InProtocol, DWORD *pEvent, DWORD* pOutProtocol);

// Event Mask common to all simulators
#define SIM_EVT_CL_FIELD_POWER_ON                       (1 << 9)
#define SIM_EVT_CL_FIELD_POWER_OFF                      (1 << 10)

// Event Mask for Contactless 14443-4 Simulator
#define SIM_EVT_CL_14443_FRAME_RECEIVED                 (1 << 0)
#define SIM_EVT_CL_14443_FRAME_SENT                     (1 << 1)
#define SIM_EVT_CL_14443_APDU_RECEIVED                  (1 << 2)
#define SIM_EVT_CL_14443_RAPDU_SENT                     (1 << 3)
#define SIM_EVT_CL_14443_PPS_REQUEST                    (1 << 4)
#define SIM_EVT_CL_14443_PPS_RESPONSE_SENT              (1 << 5)
#define SIM_EVT_CL_14443_WTX_REQUEST_SENT               (1 << 6)
#define SIM_EVT_CL_14443_WTX_RESPONSE_RECEIVED          (1 << 7)
#define SIM_EVT_CL_14443_DESELECT_RESPONSE_SENT         (1 << 8)
#define SIM_EVT_CL_14443_DESELECT_RECEIVED              (1 << 11)
#define SIM_EVT_CL_14443_RACK_RECEIVED                  (1 << 12)
#define SIM_EVT_CL_14443_RNACK_RECEIVED                 (1 << 13)

// Event Mask for Contactless 14443-3 type A initialization process
#define SIM_EVT_CL_14443_REQA_RECEIVED                  (1 << 15)
#define SIM_EVT_CL_14443_WUPA_RECEIVED                  (1 << 16)
#define SIM_EVT_CL_14443_HLTA_RECEIVED                  (1 << 17)
#define SIM_EVT_CL_14443_ANTICOLLA_RECEIVED             (1 << 18)
#define SIM_EVT_CL_14443_SELECTA_RECEIVED               (1 << 19)
#define SIM_EVT_CL_14443_RATS_RECEIVED                  (1 << 20)
#define SIM_EVT_CL_14443_RATS_RESPONSE_SENT             (1 << 21)
// Event Mask for Contactless 14443-3 type B initialization process
#define SIM_EVT_CL_14443_REQB_RECEIVED                  (1 << 25)
#define SIM_EVT_CL_14443_WUPB_RECEIVED                  (1 << 26)
#define SIM_EVT_CL_14443_HLTB_RECEIVED                  (1 << 27)
#define SIM_EVT_CL_14443_ATTRIB_RECEIVED                (1 << 28)
#define SIM_EVT_CL_14443_ATTRIB_RESPONSE_SENT           (1 << 29)
#define SIM_EVT_CL_14443_HLTB_RESPONSE_SENT             (1 << 30)

// Event Mask for Felica Simulator
#define SIM_EVT_CL_FELICA_FRAME_RECEIVED                (1 << 0)
#define SIM_EVT_CL_FELICA_FRAME_SENT                    (1 << 1)

// Event Mask for Vicinity Simulator
#define SIM_EVT_CL_VICINITY_FRAME_RECEIVED              (1 << 0)
#define SIM_EVT_CL_VICINITY_FRAME_SENT                  (1 << 1)

// Event Mask for NFC Simulator
// Raw events
#define SIM_EVT_CL_NFC_FRAME_RECEIVED                           (1 << 0)
#define SIM_EVT_CL_NFC_FRAME_SENT                               (1 << 1)
// Initialization phase
// Type A - 106
#define SIM_EVT_CL_NFC_SENS_REQ_RECEIVED                        (1 << 25)
#define SIM_EVT_CL_NFC_ALL_REQ_RECEIVED                         (1 << 26)
#define SIM_EVT_CL_NFC_SLP_REQ_RECEIVED                         (1 << 27)
#define SIM_EVT_CL_NFC_SDD_REQ_RECEIVED                         (1 << 28)
#define SIM_EVT_CL_NFC_SEL_REQ_RECEIVED                         (1 << 29)
//Type F 212 - 424
#define SIM_EVT_CL_NFC_POLLING_REQUEST_RECEIVED                 (1 << 30)
#define SIM_EVT_CL_NFC_POLLING_RESPONSE_SENT             (DWORD)(1 << 31)
// Common events of the protocol
#define SIM_EVT_CL_NFC_UDATA_RECEIVED                           (1 << 2)
#define SIM_EVT_CL_NFC_UDATA_SENT                               (1 << 3)
#define SIM_EVT_CL_NFC_ATR_REQ_RECEIVED                         (1 << 4)
#define SIM_EVT_CL_NFC_ATR_RES_SENT                             (1 << 5)

#define SIM_EVT_CL_NFC_ERROR                                    (1 << 6)

#define SIM_EVT_CL_NFC_ACK_RECEIVED                             (1 << 7)
#define SIM_EVT_CL_NFC_ACK_SENT                                 (1 << 8)
#define SIM_EVT_CL_NFC_NACK_RECEIVED                            (1 << 11)
#define SIM_EVT_CL_NFC_NACK_SENT                                (1 << 12)

#define SIM_EVT_CL_NFC_PSL_REQ_RECEIVED                         (1 << 13)
#define SIM_EVT_CL_NFC_PSL_RES_SENT                             (1 << 14)
#define SIM_EVT_CL_NFC_DSL_REQ_RECEIVED                         (1 << 15)
#define SIM_EVT_CL_NFC_DSL_RES_SENT                             (1 << 16)
#define SIM_EVT_CL_NFC_RLS_REQ_RECEIVED                         (1 << 17)
#define SIM_EVT_CL_NFC_RLS_RES_SENT                             (1 << 18)
#define SIM_EVT_CL_NFC_WUP_REQ_RECEIVED                         (1 << 19)
#define SIM_EVT_CL_NFC_WUP_RES_SENT                             (1 << 20)
#define SIM_EVT_CL_NFC_TIMEOUT_EXTENSION_SENT                   (1 << 21)
#define SIM_EVT_CL_NFC_TIMEOUT_EXTENSION_RESPONSE_RECEIVED      (1 << 22)
#define SIM_EVT_CL_NFC_ATTENTION_RECEIVED                       (1 << 23)
#define SIM_EVT_CL_NFC_TARGET_PRESENT_SENT                      (1 << 24)


/*----------------------------------------------------------------------*
 *  Simulator  - WTX requests management (14443-4)
 *-----------------------------------------------------------------------*/

WORD CALL MPS_SendWTXRequest(BYTE CplNum, BYTE Mode, DWORD Value);
#define WTX_BWT_MULT    1
#define WTX_ETU         2
#define WTX_MS          3

/*----------------------------------------------------*
 *  Simulator  - PPS management (Type A only)
 *-----------------------------------------------------*/
WORD CALL MPC_SendPPSResponse(BYTE CplNum, TYPEA_PPS_STRUCT* pBufPPS);

WORD CALL MPC_GetPPSRequest(BYTE CplNum, TYPEA_PPS_STRUCT *pBufPPS);

/*---------------------------------------------------------------------------*
 * Simulators - Set simulation parameters
 *---------------------------------------------------------------------------*/

WORD CALL MPS_SetSimParameter(BYTE CplNum, DWORD TypeParam, void *Param, DWORD ParamSize);
#define SIM_SET_EVENTS_MASK                                             5
#define SIM_SET_AUTO_WTX                                                20
#define SIM_SET_MUTE_INITRULE_ON                                        21
#define SIM_SET_MUTE_INITRULE_OFF                                       22
#define SIM_SET_ATS_CRC_FORCED                                          23
#define SIM_SET_ATS_CRC_AUTO                                            24
#define SIM_SET_AUTO_WTX_VALUE                                          25
#define SIM_SET_STRICT_RFU_CHECKING                                     26
#define SIM_SET_ATR_RES_CRC_FORCED                                      27
#define SIM_SET_ATR_RES_CRC_AUTO                                        28
#define SIM_SET_WUP_RES_CRC_FORCED                                      29
#define SIM_SET_WUP_RES_CRC_AUTO                                        30
#define SIM_SET_POLL_RES_CRC_FORCED                                     31
#define SIM_SET_POLL_RES_CRC_AUTO                                       32
#define SIM_SET_ATR_RES_DID_FORCED                                      33
#define SIM_SET_ATR_RES_DID_AUTO                                        34
#define SIM_SET_NFC_MAX_SUCCESSIVE_NAK                                  35
#define SIM_SET_NFC_MAX_SUCCESSIVE_ATTENTION                            36

/*---------------------------------------------------------------------------*
 * Simulators - Rules management
 *---------------------------------------------------------------------------*/

// Action conditions definition
// These conditions are described in the following structures
#define PATTERN_MAX_SIZE                          256   ///< Pattern maximum size

#define NO_CONDITION                                    0       ///< For an unconditionnal action
#define CONDITION_DATA_PATTERN                  1       ///< The action depends on simulation data

WORD CALL MPS_SimAddRule(BYTE CplNum
                                                ,DWORD Protocol
                                                ,DWORD Event
                                                ,DWORD Timing
                                                ,DWORD TimingUnit
                                                ,DWORD ExecuteCount
                                                ,DWORD SpecialFlag
                                                ,DWORD StateMachineData
                                                ,DWORD RuleDataType
                                                ,DWORD RuleDataSize
                                                ,BYTE* pRuleData
                                                ,DWORD RemoteOrderSize
                                                ,char* pRemoteOrder
                                                ,DWORD* pRuleId);
// Special flag values
#define STATE_MACHINE_EXECUTION          (1 << 0)       ///< The action must be executed by a real-time state machine
#define SENT_FRAME_WITH_CRC                      (1 << 1)       ///< Sent a frame with CRC
// Action execution definition
#define ALWAYS_ACTIVE                                   0xFFFFFFFF      ///< For an always active action

WORD CALL MPS_SimRemoveRule(BYTE CplNum, DWORD Protocol, DWORD RuleId);
// Rule Id values
#define REMOVE_STATE_MACHINE_RULES              0       ///< To remove all state machine rules

/*---------------------------------------------------------------------------*
 * Simulators - Miscellaneous
 *---------------------------------------------------------------------------*/

WORD CALL MPS_DefaultProtocolProc(BYTE CplNum, DWORD Protocol, DWORD Event);

WORD CALL MPS_GetLastError(BYTE CplNum);

/*----------------------------------------------------------------------*
 * Perturbation filters management
 *-----------------------------------------------------------------------*/

#define FILTER_SENSE_TX         (DWORD)(1 << 31)        ///< use higher bit to set TX filter sense
#define NO_INDEX                0                       ///< Specify this value to use the pattern matching
#define NO_PATTERN              0                       ///< Specify this value to use the index matching
#define FILTER_MAX_FRAME_LEN    256

#define FILTER_TYPE_CL_CRC_TX (DWORD)(2000 | FILTER_SENSE_TX)
#define FILTER_TYPE_CL_SUPP_RX 2001
#define FILTER_TYPE_CL_SUPP_TX (DWORD)(FILTER_TYPE_CL_SUPP_RX | FILTER_SENSE_TX)
// Add a filter
WORD CALL MPS_AddFilter(BYTE CplNum, DWORD Fprotocol, DWORD Ftype, DWORD FilterElemSize, BYTE* pFilterData);



/*--------------------------------*
 * WPC QI MOBILE DEVICE EMULATION *
 *--------------------------------*/

WORD CALL MPC_QiMDOpen(BYTE CplNum);
WORD CALL MPC_QiMDClose(BYTE CplNum);
WORD CALL MPC_QiMDStart(BYTE CplNum, DWORD Rfu);
WORD CALL MPC_QiMDStop(BYTE CplNum);

WORD CALL MPC_QiMDGetVr(BYTE CplNum, long *pVr);
WORD CALL MPC_QiMDGetIr(BYTE CplNum, long *pIr);
WORD CALL MPC_QiMDGetSignalStrength(BYTE CplNum, DWORD Timeout, BYTE *pSignalStrength);

WORD CALL MPC_QiMDChangeProtocolParam(BYTE CplNum, DWORD Type, void *pParam, DWORD ParamSize);

/**
 * @enum MPC_QiProtocolParamIds
 * Qi Mobile Device protocol parameters identifiers
 */
enum MPC_QiMDProtocolParamIds
{
        MPC_QI_MD_PROT_PARAM_TP,                /**<Test point */
        MPC_QI_MD_PROT_PARAM_MODULATOR,         /**< Modulator type */
        MPC_QI_MD_PROT_PARAM_LOAD,              /**< Load in 1/10 Ohms*/
        MPC_QI_MD_PROT_PARAM_TPR,               /**< Test power receiver */
        MPC_QI_MD_PROT_PARAM_MOD_SWITCH,        /**< Retromod switch */
        MPC_QI_MD_PROT_PARAM_VR_TARGET,         /**< Vr target  */
        MPC_QI_MD_PROT_PARAM_DETECT_THRESHOLD,  /**< Field detection threshold */
        MPC_QI_MD_RFU1,
        MPC_QI_MD_PROT_PARAM_ENABLE_CURRENT_GEN,
        MPC_QI_MD_PROT_PARAM_SET_CURRENT_GEN_VALUE,

/*10*/  MPC_QI_MD_PROT_PARAM_MEDIUM_POWER,
        MPC_QI_MD_PROT_PARAM_FREQUENCY_MEAS,
        MPC_QI_MD_PROT_PARAM_PREAMBLE_SIZE,
        MPC_QI_MD_PROT_PARAM_TIMING,    // See MP_QiMDDataTiming structure
        MPC_QI_MD_PROT_PARAM_RP_OFFSET,
        MPC_QI_MD_PROT_PARAM_CTRL_ERROR_FACTOR,

/*16*/  MPC_QI_MD_RFU2,
        MPC_QI_MD_IDENT_MAJOR_VERSION,
        MPC_QI_MD_IDENT_MINOR_VERSION,
        MPC_QI_MD_IDENT_MANUFACTURER_CODE,
        MPC_QI_MD_IDENT_BASIC_DEVICE_IDENTIFIER,
        MPC_QI_MD_IDENT_EXT_DEVICE_IDENTIFIER,
        MPC_QI_MD_IDENT_EXT_FIELD,

/*23*/  MPC_QI_MD_CONFIG_COUNT_FIELD,
        MPC_QI_MD_CONFIG_NEG_FIELD,
        MPC_QI_MD_CONFIG_RESERVED_FIELD,// RFU1 | RFU2
        MPC_QI_MD_CONFIG_PROP_FIELD,
        MPC_QI_MD_CONFIG_FSK,           // Polarity | Depth
        MPC_QI_MD_CONFIG_RP_WINDOW,     // WindowOffset | WindowSize

/*29*/  MPC_QI_MD_NEGO_FORCE_COUNT_FIELD,
        MPC_QI_MD_NEGO_DESIRED_MAX_POWER,       // PowerClass | MaximumPower
        MPC_QI_MD_NEGO_DESIRED_GUARANTEED_POWER, // GuaranteedPowerClass | GuaranteedPower
        MPC_QI_MD_NEGO_DESIRED_FSK,             // Polarity | Depth
        MPC_QI_MD_NEGO_DESIRED_RP_HEADER,

/*34*/  MPC_QI_MD_POWER_TRANSFER_SKIP,          // See MP_QiMDSkipPowerTransfer structure
        MPC_QI_MD_POWER_TRANSFER_END_POWER_PCK, // See MP_QiMDPowerTransferETPpck structure
        MPC_QI_MD_END,                          // See MP_QiMDEndConfig structure

        MPC_QI_MD_SCENARIO,     // See MP_QiMDDataPhase structure

        MPC_QI_MD_ADD_PCK,       // See MP_QiMDAddPck structure
        MPC_QI_MD_PROT_PARAM_FSK_SENSITIVITY,
        MPC_QI_MD_PROT_PARAM_VR_SPY_STEP,
        MPC_QI_MD_PROT_PARAM_IR_SPY_STEP,
        MPC_QI_MD_PROT_PARAM_Q_REPORT,
        MPC_QI_MD_PROT_PARAM_Q_REPORT_OFFSET,
        MPC_QI_MD_PROT_PARAM_Q_REPORT_OFFSET_PERCENT,
        MPC_QI_MD_PROT_PARAM_Q_REPORT_METHOD,
        MPC_QI_MD_PROT_PARAM_PRECEIVED_DELTA_MAX,
        MPC_QI_MD_PROT_PARAM_PRECEIVED_MEAS_DELAY,
        MPC_QI_MD_PROT_PARAM_PRECEIVED_ERR_THRESHOLD,
        MPC_QI_MD_PROT_PARAM_PRECEIVED_ERR_ACTION,
        MPC_QI_MD_PROT_PARAM_Q_REPORT_REAL,
        MPC_QI_MD_PROT_PARAM_CTRL_ERROR_OFFSET,
        MPC_QI_MD_PROT_PARAM_IR_TARGET,         /**< Ir target  */
        MPC_QI_MD_PROT_PARAM_ANALOG_PING_DURATION,
        MPC_QI_MD_PROT_PARAM_VCOIL_SPY_STEP,
};


/**
 * MP_QiMDDataTiming structure to be used with MPC_QI_MD_PROT_PARAM_TIMING
 */
struct MP_QiMDDataTiming
{
        DWORD Timing;    // See MP_QiMDTimingIds enum
        DWORD Data;
};
/**
 * @enum MP_QiMDTimingIds
 * Timing identifiers to be used with MPC_QI_MD_PROT_PARAM_TIMING
 */
enum MP_QiMDTimingIds
{
        MP_QI_TIMING_WAKE,
        MP_QI_TIMING_DELAY,
        MP_QI_TIMING_SILENT,
        MP_QI_TIMING_CONTROL,
        MP_QI_TIMING_NEXT,
        MP_QI_TIMING_INTERVAL,
        MP_QI_TIMING_RECEIVED,
        MP_QI_TIMING_RESPONSE,
        MP_QI_TIMING_NEXTNEGO,
        MP_QI_TIMING_RECEIVEDMP,
        MP_QI_TIMING_RECEIVED_CALIB_LIGHT,
        MP_QI_TIMING_RECEIVED_CALIB_LOAD,
        MP_QI_TIMING_RETRANSMISSION,
        MP_QI_TIMING_INTERVAL_CALIB
};

enum MP_QiMDPowerReceivedFODActionIds
{
        MP_QI_MD_PR_FOD_ACTION_IGNORE,
        MP_QI_MD_PR_FOD_ACTION_EPT,
        MP_QI_MD_PR_FOD_ACTION_RP
};

/**
 * MP_QiMDSkipPowerTransfer structure to be used with MPC_QI_MD_POWER_TRANSFER_SKIP
 */
struct MP_QiMDSkipPowerTransfer
{
        DWORD Delay;
        WORD StatusNew;      // See MP_QiMDTSStatusCode enum
        BYTE SkipPhase;
        BYTE StatusChange;
};
/**
 * @enum MP_QiMDTSStatusCode
 * Test case status identifiers
 */
enum MP_QiMDTSStatusCode
{
        MP_QI_MD_TS_STATUS_SUCCESS = 0x0000,
        MP_QI_MD_TS_STATUS_NOT_EXIST = 0xF000,
        MP_QI_MD_TS_STATUS_TIMEOUT = 0xF001,
        MP_QI_MD_TS_STATUS_FAILED = 0xF002,
        MP_QI_MD_TS_STATUS_TX_ERR = 0xF003,
        MP_QI_MD_TS_STATUS_BAD_ANSWER = 0xF004,
        MP_QI_MD_TS_STATUS_INCONCLUSIVE = 0xF005,
        MP_QI_MD_TS_STATUS_RETRY = 0xF006,
};
/**
 * MP_QiMDPowerTransferETPpck structure to be used with MPC_QI_MD_POWER_TRANSFER_END_POWER_PCK
 */
struct MP_QiMDPowerTransferETPpck
{
        DWORD Delay;
        BYTE Err;
        BYTE Goto;
        BYTE Leave;
        BYTE Rfu;
};
/* Message Values for End Power Transfer Packet see volI part 1 Table 6-6 */
#define MP_QI_MSG_END_POWER_TRANSFER_UNKNOWN            0x00 /**< End power transfer due to unknown reason*/
#define MP_QI_MSG_END_POWER_TRANSFER_COMPLETE           0x01 /**< Power transfer complete*/
#define MP_QI_MSG_END_POWER_TRANSFER_INTERNAL_FAULT     0x02 /**< End power transfer due to internal fault*/
#define MP_QI_MSG_END_POWER_TRANSFER_OVER_TEMP          0x03 /**< End power transfer due to over temperature*/
#define MP_QI_MSG_END_POWER_TRANSFER_OVER_VOLT          0x04 /**< End power transfer due to over voltage*/
#define MP_QI_MSG_END_POWER_TRANSFER_OVER_CURR          0x05 /**< End power transfer due to over current*/
#define MP_QI_MSG_END_POWER_TRANSFER_BATTERY_FAILURE    0x06 /**< End power transfer due to battery failure*/
#define MP_QI_MSG_END_POWER_TRANSFER_RECONFIGURE        0x07 /**< End power transfer due to reconfiguration */
#define MP_QI_MSG_END_POWER_TRANSFER_NO_RESPONSE        0x08 /**< End power transfer due to no response */
#define MP_QI_MSG_END_POWER_TRANSFER_RENEGOTIATE        0x09 /**< End power transfer due to renegotiation */
#define MP_QI_MSG_END_POWER_TRANSFER_INCOMPATIBLE_POWER 0x0A /**< End power transfer due to incompatible power */


/**
 * MP_QiMDEndConfig structure to be used with MPC_QI_MD_END
 */
struct MP_QiMDEndConfig
{
        DWORD EndSuccess_ChargingTime;
        BYTE EndFail_EnterPowerPhaseMax;
        BYTE EndSuccess_SendControlErrorMax;
        BYTE Success_EnterPhase;     // See MP_QiPhases enum
        BYTE EndFail_RxInPowerTransfer;
        BYTE EndFail_MaxTry;
        BYTE Success_EnterPhase_NpckSent;
        BYTE Rfu[2];
};
/**
 * @enum MP_QiPhases
 * Declaration of phases defined in VolI part 1 section 5.1 to be used with MPC_QI_MD_END
 */
enum MP_QiPhases
{
        MP_QI_PHASE_SELECTION,     /**< Selection phase */
        MP_QI_PHASE_PING,          /**< Ping phase */
        MP_QI_PHASE_IDENT_AND_CFG, /**< Identification and configuration phase */
        MP_QI_PHASE_NEGOTIATION,   /**< Negotiation phase */
        MP_QI_PHASE_POWER_TRANSFER,/**< Power transfer phase */
        MP_QI_PHASE_CALIBRATION,   /**< Calibration phase */
};

/**
 * MP_QiMDDataPhase structure to be used with MPC_QI_MD_SCENARIO
 */
struct MP_QiMDDataPhase
{
        DWORD Phase;    // See MP_QiPhases enum
        DWORDLONG Data; // See defines below
};

#define MP_QI_MD_SCENARIO_PING_SEND_SIGNAL_STRENGTH                (1ULL << 0)
#define MP_QI_MD_SCENARIO_PING_SEND_END_POWER_TRANSFER             (1ULL << 1)
#define MP_QI_MD_SCENARIO_PING_END_POWER_TRANSFER_RESTART          (1ULL << 2)
#define MP_QI_MD_SCENARIO_PING_END_POWER_TRANSFER_STATUS_CHANGE    (1ULL << 3)
#define MP_QI_MD_SCENARIO_BN_PING_END_POWER_TRANSFER_ERR           8
#define MP_QI_MD_SCENARIO_BM_PING_END_POWER_TRANSFER_ERR           0xFFULL
#define MP_QI_MD_SCENARIO_BN_PING_END_POWER_TRANSFER_STATUS_NEW    16
#define MP_QI_MD_SCENARIO_BM_PING_END_POWER_TRANSFER_STATUS_NEW    0xFFFFULL
#define MP_QI_MD_SCENARIO_PING_ENABLE_LOAD                         (1ULL << 4)
#define MP_QI_MD_SCENARIO_PING_ENDLESS_SCENARIO                    (1ULL << 5)

#define MP_QI_MD_SCENARIO_IDCFG_FORCE_SEND_IDENT         (1ULL << 0)
#define MP_QI_MD_SCENARIO_IDCFG_SEND_IDENT               (1ULL << 1)
#define MP_QI_MD_SCENARIO_IDCFG_SEND_EXTIDENT            (1ULL << 2)
#define MP_QI_MD_SCENARIO_IDCFG_SEND_CTRLHOLDOFF         (1ULL << 3)
#define MP_QI_MD_SCENARIO_IDCFG_SEND_CONFIG              (1ULL << 4)

#define MP_QI_MD_SCENARIO_NEGO_RESTART_IF_NO_NEGO        (1ULL << 0)
#define MP_QI_MD_SCENARIO_NEGO_GET_IDENT                 (1ULL << 1)
#define MP_QI_MD_SCENARIO_NEGO_GET_CAPAB                 (1ULL << 2)
#define MP_QI_MD_SCENARIO_NEGO_AUTO_NEGO                 (1ULL << 3)
#define MP_QI_MD_SCENARIO_NEGO_SEND_END_NEGO             (1ULL << 4)
#define MP_QI_MD_SCENARIO_NEGO_ENDNEGO_RESTART_IF_NAK    (1ULL << 5)
#define MP_QI_MD_SCENARIO_NEGO_SEND_END_POWERTRANSFER    (1ULL << 6)
#define MP_QI_MD_SCENARIO_NEGO_ENDPOWERTRANSFER_GOTO     (1ULL << 7)
#define MP_QI_MD_SCENARIO_NEGO_SEND_FORCE_ADDPCK         (1ULL << 8)
#define MP_QI_MD_SCENARIO_NEGO_SEND_FOD                  (1ULL << 9)
#define MP_QI_MD_SCENARIO_NEGO_SEND_FOD_FORCE            (1ULL << 10)
#define MP_QI_MD_SCENARIO_NEGO_RETRY_IF_NO_NEGO          (1ULL << 11)
#define MP_QI_MD_SCENARIO_BN_NEGO_SEND_FOD_EXPECTED_RESP 24
#define MP_QI_MD_SCENARIO_BM_NEGO_SEND_FOD_EXPECTED_RESP 0xFULL
#define MP_QI_MD_SCENARIO_BN_NEGO_RESEND_MAX             16
#define MP_QI_MD_SCENARIO_BM_NEGO_RESEND_MAX             0xFFULL
#define MP_QI_MD_SCENARIO_BN_NEGO_ENDPOWERTRANSFER_ERR   32
#define MP_QI_MD_SCENARIO_BM_NEGO_ENDPOWERTRANSFER_ERR   0xFFULL


#define MP_QI_MD_SCENARIO_POWERTRAN_SEND_CTRLERR                        (1ULL << 0)
#define MP_QI_MD_SCENARIO_POWERTRAN_SEND_RECEIVEDPOW                    (1ULL << 1)
#define MP_QI_MD_SCENARIO_POWERTRAN_SEND_RECEIVEDPOW_MODE_RESPONSE      (1ULL << 2)
#define MP_QI_MD_SCENARIO_POWERTRAN_SEND_EPT_IF_NAK_ON_RP               (1ULL << 3)


/**
 * MP_QiMDAddPck structure to be used with MPC_QI_MD_ADD_PCK
 */
struct MP_QiMDAddPck
{
        DWORD Delay;
        BYTE Phase;  // See MP_QiPhases enum
        BYTE MessageSize;
        char MaxSend;
        BYTE Reply;  // See MP_QiMDReplyType enum
        char string[256];
};
/**
 * @enum MP_QiMDReplyType
 * To be used with MPC_QI_MD_ADD_PCK
 */
enum MP_QiMDReplyType
{
    MP_QI_REPLY_ANY,
    MP_QI_REPLY_NONE,
    MP_QI_REPLY_ACK,
    MP_QI_REPLY_NAK,
    MP_QI_REPLY_ND,
    MP_QI_REPLY_TYPE2,
    MP_QI_REPLY_ACK_NAK,
    MP_QI_REPLY_TYPE2_IDENT,
    MP_QI_REPLY_TYPE2_CAPAB,
    MP_QI_REPLY_TYPE2_NA,
};


/**
 * @enum MP_QiMDModulatorTypes
 * Modulator types
 */
enum MP_QiMDModulatorTypes
{
    MP_QI_MODULATOR_TYPE_RESISTIVE, //!< MP_QI_MODULATOR_TYPE_RESISTIVE
    MP_QI_MODULATOR_TYPE_CAPACITIVE,//!< MP_QI_MODULATOR_TYPE_CAPACITIVE
};

/**
 * @enum MP_QiMDTPRTypes
 * TPR types
 */
enum MP_QiMDTPRTypes
{
        MP_QI_TPR_TYPE_TPR1A, // TPR#1A
        MP_QI_TPR_TYPE_TPR1B, // TPR#1B
        MP_QI_TPR_TYPE_TPR1C, // TPR#1C
        MP_QI_TPR_TYPE_TPR1D, // TPR#1D
        MP_QI_TPR_TYPE_TPR1E, // TPR#1E
        MP_QI_TPR_TYPE_TPR3, // TPR#3
        MP_QI_TPR_TYPE_TPR4, // TPR#4
        MP_QI_TPR_TYPE_TPR5, // TPR#5
        MP_QI_TPR_TYPE_TPR6, // TPR#6
        MP_QI_TPR_TYPE_TPRMP1A, // TPR#MP1A
        MP_QI_TPR_TYPE_TPRMP1B, // TPR#MP1B
        MP_QI_TPR_TYPE_TPRMP1C, // TPR#MP1C
        MP_QI_TPR_TYPE_TPRMP3, // TPR#MP3
        MP_QI_TPR_TYPE_TPR_THERMAL,  // TPR-THERMAL
};

/**
 * @enum MP_QiMDModCircuitSwitchIds
 * Modulation circuit switch identifiers
 */
enum MP_QiMDModCircuitSwitchIds
{
        MP_QI_MOD_CIRCUIT_SW1 = (1 << 0),  //94nF, 330ohms
        MP_QI_MOD_CIRCUIT_SW2 = (1 << 1),  //47nF, 220ohms
        MP_QI_MOD_CIRCUIT_SW3 = (1 << 2),  //22nF, 100ohms
        MP_QI_MOD_CIRCUIT_SW4 = (1 << 3),  //10nF, 68ohms
        MP_QI_MOD_CIRCUIT_SW5 = (1 << 4),  //4.7nF, 47ohms
        MP_QI_MOD_CIRCUIT_SW6 = (1 << 5),  //2.2nF, 33ohms
        MP_QI_MOD_CIRCUIT_SW7 = (1 << 6),  //1nF, 22ohms
        MP_QI_MOD_CIRCUIT_SW8 = (1 << 7),  //0.47nF, 10ohms
        MP_QI_MOD_CIRCUIT_SW9 = (1 << 8),  //!< MP_QI_MOD_CIRCUIT_SW1
        MP_QI_MOD_CIRCUIT_SW10 = (1 << 9), //!< MP_QI_MOD_CIRCUIT_SW2
        MP_QI_MOD_CIRCUIT_SW11 = (1 << 10),//!< MP_QI_MOD_CIRCUIT_SW3
        MP_QI_MOD_CIRCUIT_SW12 = (1 << 11),//!< MP_QI_MOD_CIRCUIT_SW4
        MP_QI_MOD_CIRCUIT_SW13 = (1 << 12),//!< MP_QI_MOD_CIRCUIT_SW5
        MP_QI_MOD_CIRCUIT_SW14 = (1 << 13),//!< MP_QI_MOD_CIRCUIT_SW6
        MP_QI_MOD_CIRCUIT_SW15 = (1 << 14),//!< MP_QI_MOD_CIRCUIT_SW7
        MP_QI_MOD_CIRCUIT_SW16 = (1 << 15),//!< MP_QI_MOD_CIRCUIT_SW8
        MP_QI_MOD_CIRCUIT_SW17 = (1 << 16),//no modulation capacitance, 82.2ohms
        MP_QI_MOD_CIRCUIT_RCM = (1 << 17),//no modulation capacitance, 60ohms
};


enum MP_QiMDTestPointIds {
        MP_QI_MD_NONE,
        MP_QI_MD_TP1,
        MP_QI_MD_TP2,
        MP_QI_MD_TP3,
        MP_QI_MD_TP4,
        MP_QI_MD_TP5,
        MP_QI_MD_TP6,
};

WORD CALL MPC_QiMDStoreTprInductance(BYTE CplNum, enum MP_QiMDTPRTypes Tpr, DWORD Ls, DWORD LsPrime);
WORD CALL MPC_QiMDGetTprInductance(BYTE CplNum, enum MP_QiMDTPRTypes Tpr, DWORD *pLs, DWORD *pLsPrime);
WORD CALL MPC_QiMDGetAmbientTemp(BYTE CplNum, DWORD *pTemperature);

/*-------------------------------*
 * WPC QI BASE STATION EMULATION *
 *-------------------------------*/

WORD CALL MPC_QiBSOpen(BYTE CplNum);
WORD CALL MPC_QiBSClose(BYTE CplNum);
WORD CALL MPC_QiBSStart(BYTE CplNum, DWORD Rfu);
WORD CALL MPC_QiBSStop(BYTE CplNum);

WORD CALL MPC_QiBSGetDeviceSignalStrength(BYTE CplNum, BYTE *pSignalStrength);

WORD CALL MPC_QiBSChangeProtocolParam(BYTE CplNum, DWORD Type, void *pParam, DWORD ParamSize);

/**
 * @enum MP_QiBSTSStatusCode
 * Test case status identifiers
 */
enum MP_QiBSTSStatusCode
{
        MP_QI_BS_TS_STATUS_SUCCESS = 0x0000,
        MP_QI_BS_TS_STATUS_NOT_EXIST = 0xF000,
        MP_QI_BS_TS_STATUS_TIMEOUT = 0xF001,
        MP_QI_BS_TS_STATUS_FAILED = 0xF002,
        MP_QI_BS_TS_STATUS_UNEXPECTED_RX = 0xF003,
        MP_QI_BS_TS_STATUS_INCONCLUSIVE = 0xF005,
};

/**
 * @enum MP_QiBSTimingIds
 * Timing identifiers
 */
enum MP_QiBSTimingIds
{
        MP_QI_BS_TIMING_PING,   //!< MP_QI_BS_TIMING_PING
        MP_QI_BS_TIMING_TERMINATE,   //!< MP_QI_BS_TIMING_TERMINATE
        MP_QI_BS_TIMING_EXPIRE,   //!< MP_QI_BS_TIMING_EXPIRE
        MP_QI_BS_TIMING_NEXT,   //!< MP_QI_BS_TIMING_NEXT
        MP_QI_BS_TIMING_DELAY,   //!< MP_QI_BS_TIMING_DELAY
        MP_QI_BS_TIMING_NEGOTIATE,   //!< MP_QI_BS_TIMING_NEGOTIATE
        MP_QI_BS_TIMING_RESPONSE,   //!< MP_QI_BS_TIMING_RESPONSE
        MP_QI_BS_TIMING_TIMEOUT,   //!< MP_QI_BS_TIMING_TIMEOUT
        MP_QI_BS_TIMING_ACTIVE,   //!< MP_QI_BS_TIMING_ACTIVE
        MP_QI_BS_TIMING_POWER,   //!< MP_QI_BS_TIMING_POWER
        MP_QI_BS_TIMING_POWERMP,   //!< MP_QI_BS_TIMING_POWERMP
        MP_QI_BS_TIMING_POWER_CALIB_LIGHT,   //!< MP_QI_BS_TIMING_POWER_CALIB_LIGHT
        MP_QI_BS_TIMING_POWER_CALIB_LOAD,   //!< MP_QI_BS_TIMING_POWER_CALIB_LOAD
        MP_QI_BS_TIMING_SETTLE,   //!< MP_QI_BS_TIMING_SETTLE
};


/**
 * @enum MP_QiBSTPTTypes
 * TPT types
 */
enum MP_QiBSTPTTypes
{
        MP_QI_BS_TPT1, // TPT#1
        MP_QI_BS_TPT2, // TPT#2
        MP_QI_BS_TPT_MP1 // TPT#MP1
};

enum MP_QiTPOutputIds {
        MP_QI_TP_OUT1,
        MP_QI_TP_OUT2,
};

enum MP_QiBSTestPointIds {
        MP_QI_BS_NONE,
        MP_QI_BS_TP1,
        MP_QI_BS_TP2,
        MP_QI_BS_TP3,
        MP_QI_BS_TP4,
        MP_QI_BS_TP5,
        MP_QI_BS_TP6,
        MP_QI_BS_TP7,
        MP_QI_BS_TP8,
};

enum MP_QiBSIDemodulatorType {
        MP_QI_BS_IDEMOD_SIMPLE,
        MP_QI_BS_IDEMOD_SYNCHRO,
};

enum MP_QiBSDemodulatorIds {
        MP_QI_BS_DEMOD_1,
        MP_QI_BS_DEMOD_2,
        MP_QI_BS_DEMOD_3,
};


/**
 * @enum MPC_QiProtocolParamIds
 * Qi Base Station protocol parameters identifiers
 */
enum MPC_QiBSProtocolParamIds
{
        MPC_QI_BS_PROT_PARAM_TPT,
        MPC_QI_BS_PROT_PARAM_TP,
        MPC_QI_BS_PROT_PARAM_I_DEMOD,
        MPC_QI_BS_PROT_PARAM_OPERATING_FREQUENCY,
        MPC_QI_BS_PROT_PARAM_DUTY_CYCLE,
        MPC_QI_BS_PROT_PARAM_FULL_BRIDGE,
        MPC_QI_BS_PROT_PARAM_PHASE_RATIO,
        MPC_QI_BS_PROT_PARAM_TIMING,

/*8*/   MPC_QI_BS_PROT_PARAM_MEDIUM_POWER,
        MPC_QI_BS_PROT_PARAM_TRANSMITTEDPOWER_MAX,
        MPC_QI_BS_PROT_PARAM_OVER_TRANSMITTEDPOWER_MAX,
        MPC_QI_BS_PROT_PARAM_OVER_TRANSMITTEDPOWER_MAXPERCENT,
        MPC_QI_BS_PROT_PARAM_RECEIVEDPOWER_CALIB_LIGHT_PMAX,
        MPC_QI_BS_PROT_PARAM_RECEIVEDPOWER_PLOSS_MAX,
        MPC_QI_BS_PROT_PARAM_RECEIVEDPOWER_PLOSS_MAX_LIGHT,
        MPC_QI_BS_PROT_PARAM_RECEIVEDPOWER_PLOSS_MAX_LOAD,
        MPC_QI_BS_PROT_PARAM_OVERCONTRACT_RECEIVEDPOWER_MAXPERCENT,
        MPC_QI_BS_PROT_PARAM_FORCE_RP_WINDOW,
        MPC_QI_BS_PROT_PARAM_FREQUENCY_MOD_TARGET,
        MPC_QI_BS_PROT_PARAM_DETECT_THRESHOLD,
        MPC_QI_BS_PROT_PARAM_DEMOD_FILTER,

/*21*/  MPC_QI_BS_PROT_PARAM_FOD_CALIB_METHOD,
        MPC_QI_BS_PROT_PARAM_FOD_CALIB_TMO,
        MPC_QI_BS_PROT_PARAM_FOD_CALIB_CE_MAX,
        MPC_QI_BS_PROT_PARAM_FOD_CALIB_NAK_RP_PCK_NB,
        MPC_QI_BS_PROT_PARAM_FOD_Q_THRESHOLD_FACT,
        MPC_QI_BS_PROT_PARAM_FOD_Q_THRESHOLD_ERR,

        MPC_QI_BS_PROT_PARAM_PID_FREQUENCY_MAX,
        MPC_QI_BS_PROT_PARAM_PID_FREQUENCY_MIN,
        MPC_QI_BS_PROT_PARAM_PID_DUTYCYCLE_MAX,
        MPC_QI_BS_PROT_PARAM_PID_DUTYCYCLE_MIN,
        MPC_QI_BS_PROT_PARAM_PID_VOLTAGE_MAX,
        MPC_QI_BS_PROT_PARAM_PID_VOLTAGE_MIN,
        MPC_QI_BS_PROT_PARAM_PID_PHASERATIO_MAX,
        MPC_QI_BS_PROT_PARAM_PID_PHASERATIO_MIN,
        MPC_QI_BS_PROT_PARAM_PID_PHASERATIO_FREQ,


/*36*/  MPC_QI_BS_RX_IDENTCFG_ACCEPT_VERSION_MAX,
        MPC_QI_BS_RX_IDENTCFG_ACCEPT_VERSION_MIN,
        MPC_QI_BS_RX_IDENTCFG_ACCEPT_CONFIG_POWER_MAX,
        MPC_QI_BS_RX_IDENTCFG_ACCEPT_CTRL_HOLDOFF_MAX,
        MPC_QI_BS_RX_IDENTCFG_ACCEPT_CTRL_HOLDOFF_MIN,

        MPC_QI_BS_IDENT_MAJOR_VERSION,
        MPC_QI_BS_IDENT_MINOR_VERSION,
        MPC_QI_BS_IDENT_MANUFACTURER_CODE,
        MPC_QI_BS_IDENT_GUARANTEED_POWER,
        MPC_QI_BS_IDENT_POTENTIAL_POWER,
        MPC_QI_BS_IDENT_CONSTANT_FREQUENCY,

/*47*/  MPC_QI_BS_NEGO_GUARANTEED_POWER_MAX,
        MPC_QI_BS_NEGO_GUARANTEED_POWER_MIN,
        MPC_QI_BS_NEGO_MAXIMUM_POWER_MAX,
        MPC_QI_BS_NEGO_MAXIMUM_POWER_MIN,
        MPC_QI_BS_NEGO_FSK_MAX,
        MPC_QI_BS_NEGO_FSK_MIN,
        MPC_QI_BS_NEGO_RP_HEADER_MAX,
        MPC_QI_BS_NEGO_RP_HEADER_MIN,
        MPC_QI_BS_NEGO_FORCE_EXPECTED_COUNT_FIELD,

/*56*/  MPC_QI_BS_SCENARIO,//see MP_QiBSDataPhase struct + MP_QiBSScenarioType enum + defines
        MPC_QI_BS_SCENARIO_ENDFAIL_FLAGS,
        MPC_QI_BS_EXIT_AFTER_PHASE, //see MP_QiBSExitAfterPhase struct
        MPC_QI_BS_EXIT_SUCCESS_STR,
        MPC_QI_BS_EXIT_FAIL_STR,
        MPC_QI_BS_CUSTOM_TYPE1_RESPONSE,
        MPC_QI_BS_CUSTOM_TYPE2_RESPONSE,

/*63*/  MPC_QI_BS_PROT_PARAM_VR_SPY_STEP,
        MPC_QI_BS_PROT_PARAM_IR_SPY_STEP,
        MPC_QI_BS_PROT_PARAM_DEMOD_THRESHOLDS,

/*66*/  MPC_QI_BS_PROT_PARAM_CYCLES_PER_FSK,
        MPC_QI_BS_PROT_PARAM_VCOIL_SPY_STEP,

};

struct MP_QiBSDataTiming
{
        DWORD Timing;    // See MP_QiBSTimingIds enum
        DWORD Data;
};

struct MP_QiBSAddResponse
{
        BYTE SpecialReply;
        BYTE MessageSize;
        BYTE ReplySize;
        char string[256];
};

struct MP_QiBSExitAfterPhase
{
        DWORD Delay;
        DWORD RcvMax;
        WORD StatusNew;
        BYTE Phase;
        BYTE StatusChange;
};

struct MP_QiBSDataPhase
{
        DWORD ScenarioType;
        DWORDLONG Data;
};
enum MP_QiBSScenarioType {
        MP_QI_BS_SCENARIO_GENERAL,
        MP_QI_BS_SCENARIO_LOG_LEVEL,
        MP_QI_BS_SCENARIO_RX_PCK_MAX,
        MP_QI_BS_SCENARIO_CHARGING_TIME,
};

#define MP_QI_BS_SCENARIO_GENERAL_ACCEPT_ALL_RP_PCK                     (1ULL << 0)
#define MP_QI_BS_SCENARIO_GENERAL_ACCEPT_PCK_AT_ANY_PHASE               (1ULL << 1)
#define MP_QI_BS_SCENARIO_GENERAL_ACCEPT_CONFIGPCK_RESERVEDBITS         (1ULL << 2)
#define MP_QI_BS_SCENARIO_GENERAL_NEGO_SEND_ND_ON_RESERVED_PCK          (1ULL << 3)
#define MP_QI_BS_SCENARIO_GENERAL_ENDLESS                               (1ULL << 4)
#define MP_QI_BS_SCENARIO_GENERAL_MEASURE_Q_FACTOR                      (1ULL << 5)
#define MP_QI_BS_SCENARIO_GENERAL_NOT_OFFICIAL_PID_LOOP                 (1ULL << 6)
#define MP_QI_BS_SCENARIO_GENERAL_BN_SUCCESS_ENTER_PHASE                16
#define MP_QI_BS_SCENARIO_GENERAL_BM_SUCCESS_ENTER_PHASE                0xFFULL
#define MP_QI_BS_SCENARIO_GENERAL_BN_EPT_IGNORE                         24
#define MP_QI_BS_SCENARIO_GENERAL_BM_EPT_IGNORE                         0xFFULL

#define MP_QI_BS_SCENARIO_BM_LOG_LEVEL                  0xFFFFULL
#define MP_QI_BS_SCENARIO_BM_RX_PCK_MAX                 0xFFFFULL

#define MP_QI_BS_SCENARIO_BM_CHARGING_TIME_DURATION     0xFFFFULL
#define MP_QI_BS_SCENARIO_BN_CHARGING_TIME_ITERATION    32
#define MP_QI_BS_SCENARIO_BM_CHARGING_TIME_ITERATION    0xFFULL


enum MP_QiBSEndPowerTransferBehaviours {
        MP_QI_BS_EPT_DEFAULT,
        MP_QI_BS_EPT_IGNORE,
        MP_QI_BS_EPT_ALWAYS_EXIT,
        MP_QI_BS_EPT_ONLY_JUMP_BACKWARDS,
};

enum MP_QiBSLogLevelIds {
        MP_QI_BS_LOG_NONE = 0,
        MP_QI_BS_LOG_STD = (1<<0),
        MP_QI_BS_LOG_CONTROL_ERROR = (1<<1),
        MP_QI_BS_LOG_RECEIVED_POWER = (1<<2),
        MP_QI_BS_LOG_ALL = 0xFFFFFFFF,
};

enum MP_QiBSFODCalibrationMethod {
        MP_QiBSFODCalibrationMethod_NONE = 0,
        MP_QiBSFODCalibrationMethod_PT,
        MP_QiBSFODCalibrationMethod_PR,
};



#define MP_QI_BS_SCENARIO_ENDFAIL_EXIT_NEGO_ON_CTRLERR          (1UL << 0)
#define MP_QI_BS_SCENARIO_ENDFAIL_IF_NO_NEGO                    (1UL << 1)
#define MP_QI_BS_SCENARIO_ENDFAIL_RCV_PROPRIETARY_PCK           (1UL << 2)
#define MP_QI_BS_SCENARIO_ENDFAIL_RCV_RESERVED_PCK              (1UL << 3)
#define MP_QI_BS_SCENARIO_ENDFAIL_IF_NO_FOD_REQUEST             (1UL << 4)
#define MP_QI_BS_SCENARIO_ENDFAIL_FOD_CALIB_PHASE_ERR           (1UL << 5)
#define MP_QI_BS_SCENARIO_ENDFAIL_FOD_CALIB_TMO                 (1UL << 6)
#define MP_QI_BS_SCENARIO_ENDFAIL_FOD_CALIB_NAK                 (1UL << 7)
#define MP_QI_BS_SCENARIO_ENDFAIL_IF_NO_FOD_CALIB               (1UL << 8)
#define MP_QI_BS_SCENARIO_ENDFAIL_EXIT_NEGO_BAD_COUNT           (1UL << 9)
#define MP_QI_BS_SCENARIO_ENDFAIL_FO_DETECT_ON_RP_PCK           (1UL << 10)
#define MP_QI_BS_SCENARIO_ENDFAIL_BN_MAX_TRY                    (16)
#define MP_QI_BS_SCENARIO_ENDFAIL_BM_MAX_TRY                    (0xFFUL)
#define MP_QI_BS_SCENARIO_ENDFAIL_BN_ENTER_POWER_PHASE_MAX      (24)
#define MP_QI_BS_SCENARIO_ENDFAIL_BM_ENTER_POWER_PHASE_MAX      (0xFFUL)


WORD CALL MPC_QiBSStoreTptInductance(BYTE CplNum, enum MP_QiBSTPTTypes Tpt, DWORD Lp);
WORD CALL MPC_QiBSGetTptInductance(BYTE CplNum, enum MP_QiBSTPTTypes Tpt, DWORD *pLp);

/*-------------------*
 * WPC QI CALIBRATOR *
 *-------------------*/

WORD CALL MPC_QiCalOpen(BYTE CplNum);
WORD CALL MPC_QiCalClose(BYTE CplNum);


/*-------------------------------------------------*
 * USELESS API OR DEFINE - FOR COMPATIBILITY ONLY  *
 *-------------------------------------------------*/
#define TEST_CHANGE_RF_FIELD            14 // MPC_Test
#define CPP_LIMIT_GAIN_EXTERNAL_RX      23 // MPC_ChangeProtocolParameters
#define MASK_ALL_CL_EVENTS              0  // MPS_OpenLog (all events are enabled by the system)

#ifdef __cplusplus
}
#endif
#endif /* MP500TCL3_H_ */
