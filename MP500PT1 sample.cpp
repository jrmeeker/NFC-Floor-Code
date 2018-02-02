/*************************************
 * INCLUDES
 ************************************/
#include "stdafx.h"
#include <conio.h>
#include <Windows.h> // For time
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOWS_DBG
#include "MP500.h"
#include "MP500TCL3.h"

/*************************************
* ERROR MANAGEMENT
************************************/

#define CHECK_RET(ErrorLog) if(ret) {gNFCTestingData.LastErrorCode=ret;gNFCTestingData.LastErrorLine=__LINE__;strcpy(gNFCTestingData.LastErrorDetails,ErrorLog);   break;}

/*************************************
* DEFINES
************************************/

#define PT1_EMV_ANTENNA	    // enable to use the EMV form-factor PT1 antenna
//#define EMBEDDED_APPLICATION // enable to use the NFCProductionTesting embedded application on the PT1 

#define MAX_RETRIES				 2		  // Maximum number of retries authorized to get the result
#ifndef PT1_EMV_ANTENNA
	#define FIELD_VSOV_TEST		 1800		  // Field strength applied for listener tests
#else
	#define FIELD_VSOV_TEST		 1700		  // Field strength applied for listener tests
	#define LMA_TEST			 1570		  // Nominal LMA
#endif

#define FIELD_ON_DELAY		 10000		  // Delay after field on for tests (µs)
#define FIELD_OFF_DELAY		 10000		  // Delay after field off for tests (µs)
#define MEASUREMENT_DELAY	 2000		  // Delay before getting the measurement (µs)
#define PRE_TRIGGER_DURATION -1000000	  // All PCD measurement must be performed with the trigger in the middle of the 2ms acquisition window

#define DUT_POLLING_PERIOD				2000		  // Defines the DUT polling period and the scenario timeout, in ms
#define FIELD_STRENGTH_INTEGRATION_TIME 1000		  // Defines the field strength integration time

#define LOG_FILE_DATARATE "C:\\NFC Production Test\\Test Results\\DataRate_v2.mplog"
#define WV_FILE			  "C:\\NFC Production Test\\Test Results\\Waveform_Poller_v2.mpat"
#define WV_FILE_LMA       "C:\\NFC Production Test\\Test Results\\LMA.mpat"

// Test indexes for listener
#define LISTENER_TEST_COUNT	2
#define RESPTIME_TEST_IDX   0
#define LMA_TEST_IDX	    1

// Test indexes for poller
#define POLLER_TEST_COUNT		5

#define MODINDEX_TEST_IDX		0
#define MODDEPTH_TEST_IDX		1
#define OVS_TEST_IDX			2
#define T2_TEST_IDX				3
#define DATARATE_TEST_IDX		3
#define FIELDSTRENGTH_TEST_IDX	4
#define T1_TEST_IDX				5
#define T3_TEST_IDX				6
#define T4_TEST_IDX				7
#define T5_COUNT				8

// Waveform measurement results indexes
// Type B / Type F
#define TYPE_B_WF_MEAS_TF_IDX				0
#define TYPE_B_WF_MEAS_TR_IDX				1
#define TYPE_B_WF_MEAS_TFMON_IDX			2
#define TYPE_B_WF_MEAS_TRMON_IDX			3
#define TYPE_B_WF_MEAS_V1_IDX				4
#define TYPE_B_WF_MEAS_V2_IDX				5
#define TYPE_B_WF_MEAS_V3_IDX				6
#define TYPE_B_WF_MEAS_V4_IDX				7
#define TYPE_B_WF_MEAS_MOD_IDX				8
#define TYPE_B_WF_MEAS_V1NF_IDX				9
#define TYPE_B_WF_MEAS_V2NF_IDX				10
#define TYPE_B_WF_MEAS_OVS_RE_IDX			11
#define TYPE_B_WF_MEAS_OVS_FE_IDX			13
#define TYPE_B_WF_MEAS_UND_RE_IDX			14
#define TYPE_B_WF_MEAS_UND_FE_IDX			15
#define TYPE_B_WF_MEAS_MODDEPTH_IDX			16

// Type A
#define TYPE_A_WF_MEAS_T1_IDX				0
#define TYPE_A_WF_MEAS_T2_IDX				1
#define TYPE_A_WF_MEAS_T3_IDX				2
#define TYPE_A_WF_MEAS_T4_IDX				3
#define TYPE_A_WF_MEAS_V1_IDX				4
#define TYPE_A_WF_MEAS_V2_IDX				5
#define TYPE_A_WF_MEAS_V3_IDX				6
#define TYPE_A_WF_MEAS_V4_IDX				7
#define TYPE_A_WF_MEAS_TFMON_IDX			8
#define TYPE_A_WF_MEAS_TRMON_IDX			9
#define TYPE_A_WF_MEAS_OVS_FE_IDX			10
#define TYPE_A_WF_MEAS_OVS_RE_IDX			11
#define TYPE_A_WF_MEAS_OVS_RE_DELAY_IDX		12
#define TYPE_A_WF_MEAS_RINGING_LVL_IDX		13
#define TYPE_A_WF_MEAS_HSNF_IDX				14
#define TYPE_A_WF_MEAS_T5COUNT_IDX			15
//#define TYPE_A_WF_MEAS_MODINDEX_IDX			17
//#define TYPE_A_WF_MEAS_MODDEPTH_IDX			18

// Standard exhanged commands. Adapt to your own needs
#define FELICA_POLL_REQ_BB			"\x06\x00\xFF\xFF\x00\x03"
#define FELICA_POLL_REQ2			"\x06\x00\xFF\xFF\x01\x0F"
#define FELICA_POLL_REQ_INGENICO	"\x06\x00\xFF\xFF\x00\x00"
#define FELICA_POLL_RES				"\x14\x01\x01\x01\x0A\x10\x73\x16\x6B\x05\x10\x0B\x4B\x42\x84\x85\xD0\xFF\x00\x03"
#define REQA			"\x26"
#define REQB			"\x05\x00\x00"
#define WUPB			"\x05\x00\x08"
#define ATQB			"\x50\xD1\x07\x7D\x78\x00\x00\x00\x00\x80\x81\x75"
#define WUPA			"\x52"
#define SDD_REQ			"\x93\x20"
#define ATTRIB			"\x1D\xD1\x07\x7D\x78\x00\x08\x01\x00"

// Defines used for acquisition time and delays
#define ACQ_TIME_MAX		0xFFFFFFFF
#define ACQ_TIME_LMA_A		24400
#define DELAY_TIME_LMA_A	0
#define ACQ_TIME_LMA_B		4000
#define DELAY_TIME_LMA_B	0
#define ACQ_TIME_LMA_F		6780
#define DELAY_TIME_LMA_F	0
#define ACQ_TIME_PCD_A		8400
#define DELAY_TIME_PCD_A	-62000
#define ACQ_TIME_PCD_B		16800
#define DELAY_TIME_PCD_B	-124000
#define ACQ_TIME_PCD_F		3390
#define DELAY_TIME_PCD_F	0

// Battery Off - Delay A & B
#define DELAY_TIME_LMA_A_BATTERY_OFF	90000
#define DELAY_TIME_LMA_B_BATTERY_OFF	280000

// Defines used for datarates
#define EVENT_TYPE_MASK		0x000000000000F000
#define EVENT_VALUE_MASK	0x0000000000000FFF
#define EVENT_TIME_MASK		0xFFFFFFFFFFFF0000
#define BN_CHAR_TYPE 8
#define BM_CHAR_TYPE 0xF00
#define MAX_CHAR_DATARATE		10
#define EVENT_EVENTTYPE_SCHAR	6
#define EVENT_EVENTTYPE_CHAR	7
#define EVENT_EVENT_PCD_B_START	2
#define EVENT_EVENT_PCD_F_START	4
#define EVENT_EVENT_PCD_A_START	0
#define EVENT_EVENT_PCD_B_END	2
#define EVENT_EVENT_PCD_F_END	4
#define EVENT_EVENT_PCD_A_END	0

//Create output file and open it
FILE *outfile;


// MP500 Header
#pragma pack (push,1)
struct HEADER {
	char    id[4];
	WORD    endian;
	WORD    version;
	char    machine[32];
	char    mversion[32];
	WORD    timebase;
	DWORD   divider;
	BYTE    type;
	BYTE    protocol;
	DWORD   nevents;
	WORD    config;
	DWORD   msk;
	WORD    RFAnalogRange;
	DWORD   RFAnalogFrequency;
	DWORD   RFAnalogBurstDelay;
	BYTE	RFU[28];
};
#pragma pack(pop)

#pragma pack(push,1)
struct EVENT
{
	unsigned long long Date : 48;
	unsigned short Field : 16;
};
#pragma pack(pop)


/*************************************
* APPLICATION ERROR CODES
************************************/

#define RET_WRONG_MEASUREMENT_CALLED 2000
#define RET_RESULT_NOT_ALLOCATED	 2001
#define RET_DATARATE_FAILED			 2002

#define RET_FAILED					 3001			// Test Item failed --> Call GetLastError for details
#define RET_NOTEXECUTED				 3000			// Test Item not executed --> Call GetLastError for details

/*************************************
* APPLICATION STATUS STRUCTURE
************************************/
#define MAX_NAME_LENGTH 200
#define MAX_ERROR_LENGTH 200

typedef struct  
{
	BOOL  CardEmulationModeReady;			// True if the card emulation channel is already configured
	DWORD PollerAScnId;						// Scenario Identifier for Poller A
	DWORD PollerBScnId;						// Scenario Identifier for Poller B
	DWORD PollerFScnId;						// Scenario Identifier for Poller F
	int LastMeasuredFieldStrength;			// Last Measured Field strength (mV)
	double LastMeasuredBDataRate;			// Last B datarate measured
	double LastMeasuredFDataRate;			// Last F datarate measured
	DWORD DataRatesStatus;					// DataRates calculation value
	char LastErrorDetails[MAX_ERROR_LENGTH];// Last Error details
	WORD LastErrorCode;						// Last Error return code
	int LastErrorLine;						// Last Error line number
}NFCTESTING_STATUS;

NFCTESTING_STATUS gNFCTestingData;

/*************************************
* RETURN STRUCTURE
************************************/


// Define measurement names and count
#define MEAS_NAME_FIELDSTRENGH      "Poller Field Strength"

#define MEAS_NAME_POLLER_A_MODINDEX "Waveform A Modulation index"
#define MEAS_NAME_POLLER_A_MODDEPTH "Waveform A Modulation depth"
#define MEAS_NAME_POLLER_A_OVS	    "Waveform A Overshoot"
#define MEAS_NAME_POLLER_A_T2	    "Waveform A T2"
#define MEAS_NAME_POLLER_A_T1	    "Waveform A T1"
#define MEAS_NAME_POLLER_A_T3	    "Waveform A T3"
#define MEAS_NAME_POLLER_A_T4	    "Waveform A T4"

#define MEAS_NAME_POLLER_V_MODINDEX "Waveform V Modulation index"
#define MEAS_NAME_POLLER_V_MODDEPTH "Waveform V Modulation depth"
#define MEAS_NAME_POLLER_V_OVS	    "Waveform V Overshoot"
#define MEAS_NAME_POLLER_V_T2	    "Waveform V T2"
#define MEAS_NAME_POLLER_V_T1	    "Waveform V T1"
#define MEAS_NAME_POLLER_V_T3	    "Waveform V T3"
#define MEAS_NAME_POLLER_V_T4	    "Waveform V T4"
#define MEAS_NAME_POLLER_V_T4	    "Waveform V T5 Count"


#define MEAS_NAME_POLLER_B_MODINDEX "Waveform B Modulation index"
#define MEAS_NAME_POLLER_B_MODDEPTH "Waveform B Modulation depth"
#define MEAS_NAME_POLLER_B_OVS	    "Waveform B Overshoot"
#define MEAS_NAME_POLLER_B_DATARATE "Waveform B Datarate"

#define MEAS_NAME_POLLER_F_MODINDEX "Waveform F Modulation index"
#define MEAS_NAME_POLLER_F_MODDEPTH "Waveform F Modulation depth"
#define MEAS_NAME_POLLER_F_OVS	    "Waveform F Overshoot"
#define MEAS_NAME_POLLER_F_DATARATE "Waveform F Datarate"

#define MEAS_NAME_LISTENER_A_RESPONSETIME "Card A FDT"
#define MEAS_NAME_LISTENER_A_LMA		  "Card A LMA"

#define MEAS_NAME_LISTENER_B_RESPONSETIME "Card B FDT"
#define MEAS_NAME_LISTENER_B_LMA		  "Card B LMA"

#define MEAS_NAME_LISTENER_F_RESPONSETIME "Card F FDT"
#define MEAS_NAME_LISTENER_F_LMA		  "Card F LMA"

typedef struct 
{
	char MeasurementName[MAX_NAME_LENGTH];		// Measurement name
	double Value;								// Measurement value
	int ErrorCode;								// 0 : OK, or error code
}NFCTESTING_RESULT;


/*************************************
* UTILITY FUNCTIONS
************************************/

// Get the last error code & details returned by the testing library
WORD NFCTesting_GetLastError(char* pErrorDetails, int* pErrorLine)
{
	if (pErrorDetails)
		strcpy(pErrorDetails, gNFCTestingData.LastErrorDetails);
	if (pErrorLine)
		*pErrorLine = gNFCTestingData.LastErrorLine;

	return gNFCTestingData.LastErrorCode;
}


/* Remove if already defined */
typedef long long int64; typedef unsigned long long uint64;
/* Returns the amount of milliseconds elapsed since the UNIX epoch. Works on both
* windows and linux. */
uint64 GetTimeMs64()
{
#ifdef _WIN32
	/* Windows */
	FILETIME ft;
	LARGE_INTEGER li;

	/* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
	* to a LARGE_INTEGER structure. */
	GetSystemTimeAsFileTime(&ft);
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;

	uint64 ret = li.QuadPart;
	ret -= 116444736000000000LL; /* Convert from file time to UNIX epoch time. */
	ret /= 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */

	return ret;
#else
	/* Linux */
	struct timeval tv;

	gettimeofday(&tv, NULL);

	uint64 ret = tv.tv_usec;
	/* Convert from micro seconds (10^-6) to milliseconds (10^-3) */
	ret /= 1000;

	/* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
	ret += (tv.tv_sec * 1000);

	return ret;
#endif
}

void DisplayResults(NFCTESTING_RESULT* Result, int ResultCount)
{
	//printf("- Listener test results :\r\n");
	//fputs("- Listener test results :\r\n", outfile);
	for (int i = 0; i < ResultCount; i++)
	{
		printf("-- Test Name %s\r\n", Result[i].MeasurementName);
		fputs("-- Test Name:", outfile);
		fputs(Result[i].MeasurementName, outfile);
		fputs("\r\n", outfile);
//		printf("-- Test Error code %ld\r\n", Result[i].ErrorCode);
//		printf("-- Test Error log %s\r\n", Result[i].ErrorLog);

		//Create string array to write Result[i].value to our output file
		char str[80];
		sprintf(str, "%.3f", Result[i].Value);

		printf("-- Test value %0.4lf\r\n\r\n", Result[i].Value);
		fputs("-- Test value: ", outfile);
		fputs(str, outfile);
		fputs("\r\n\r\n", outfile);
	}
}

void Init_NFCTestingData()
{
	WORD ret;
	// Set the amount of captured points
	DWORD count = ACQ_TIME_MAX;

	gNFCTestingData.CardEmulationModeReady = FALSE;
	gNFCTestingData.PollerAScnId = 0;
	gNFCTestingData.PollerBScnId = 0;
	gNFCTestingData.PollerFScnId = 0;
	gNFCTestingData.LastMeasuredFieldStrength = 0;
	gNFCTestingData.LastMeasuredBDataRate = 0.0;
	gNFCTestingData.LastMeasuredFDataRate = 0.0;
	gNFCTestingData.DataRatesStatus = RET_DATARATE_FAILED;

	ret = MPC_ChangeProtocolParameters(CPL0, CPP_SET_BURST_NB_SAMPLE, &count, sizeof(DWORD));
}

/*************************************
* INTERNAL TEST FUNCTIONS
************************************/
WORD ComputeDataRates(char* LogFile, double* pDataRateB, double* pDataRateF)
{
	FILE* fmplog;
	WORD ret = 0;
	bool DataRateBCalculated = false;
	bool DataRateFCalculated = false;
	fmplog = fopen(LogFile, "rb");

	if (fmplog == NULL)
	{
		*pDataRateB = 0.0;
		*pDataRateF = 0.0;
		ret = RET_DATARATE_FAILED;
	}
	else
	{
		HEADER myHeader;
		WORD endian = 0;
		WORD Timebase = 0;

		BYTE CharsToComputeB = 1;
		WORD CharStartEventB = EVENT_EVENT_PCD_B_START;
		WORD CharEndEventB = EVENT_EVENT_PCD_B_END;
		unsigned long long CharStartDateB = 0;
		unsigned long long CharEndDateB = 0;
		double CharDataRateB[MAX_CHAR_DATARATE];
		int CharIdxB = 0;
		float dividerB = 8.0;

		BYTE CharsToComputeF = 1;
		WORD CharStartEventF = EVENT_EVENT_PCD_F_START;
		WORD CharEndEventF = EVENT_EVENT_PCD_F_END;
		unsigned long long CharStartDateF = 0;
		unsigned long long CharEndDateF = 0;
		double CharDataRateF[MAX_CHAR_DATARATE];
		int CharIdxF = 0;
		float dividerF = 8.0;

		// Read the header
		if (!feof(fmplog))
		{
			fread(&myHeader, sizeof(HEADER), 1, fmplog);
		}
		Timebase = myHeader.timebase;
		// Parse the file to the end, or till the datarate computation is finished
		while (!feof(fmplog) && !(DataRateBCalculated && DataRateFCalculated))
		{
			unsigned long long Tmp;
			unsigned char Type;
			unsigned short Value;

			fread(&Tmp, sizeof(unsigned long long), 1, fmplog);

			Type = (Tmp & EVENT_TYPE_MASK) >> 12;

			if (Type == EVENT_EVENTTYPE_SCHAR)
			{
				Value = (Tmp & EVENT_VALUE_MASK);
				if (!DataRateBCalculated && (Value == CharStartEventB))
				{
					CharStartDateB = ((Tmp & EVENT_TIME_MASK) >> 16) *Timebase;
				}
				else if (!DataRateFCalculated && (Value == CharStartEventF))
				{
					CharStartDateF = ((Tmp & EVENT_TIME_MASK) >> 16) *Timebase;
				}
			}
			else if ((Type == EVENT_EVENTTYPE_CHAR))
			{
				unsigned short CharType;
				Value = (Tmp & EVENT_VALUE_MASK);
				CharType = (Value & BM_CHAR_TYPE) >> BN_CHAR_TYPE;

				if ((CharType == CharEndEventB) && (CharStartDateB != 0) && !DataRateBCalculated)
				{
					CharEndDateB = ((Tmp & EVENT_TIME_MASK) >> 16) * Timebase;
					if (CharIdxB < MAX_CHAR_DATARATE)
						CharDataRateB[CharIdxB++] = ((double)1E9 / ((CharEndDateB - CharStartDateB) / dividerB));
					if (CharIdxB == CharsToComputeB)
						DataRateBCalculated = true;
				}
				else if ((CharType == CharEndEventF) && (CharStartDateF != 0) && !DataRateFCalculated)
				{
					CharEndDateF = ((Tmp & EVENT_TIME_MASK) >> 16) * Timebase;
					if (CharIdxF < MAX_CHAR_DATARATE)
						CharDataRateF[CharIdxF++] = ((double)1E9 / ((CharEndDateF - CharStartDateF) / dividerF));
					if (CharIdxF == CharsToComputeF)
						DataRateFCalculated = true;
				}
			}
		}
		fclose(fmplog);

		if (DataRateBCalculated)
		{
			for (int i = 0; i < CharsToComputeB; i++)
			{
				*pDataRateB += CharDataRateB[i];
			}
			*pDataRateB /= CharsToComputeB;
			*pDataRateB /= 1E3;
		}
		else
		{
			*pDataRateB = 0.0;
			ret = RET_DATARATE_FAILED;
		}

		if (DataRateFCalculated)
		{
			for (int i = 0; i < CharsToComputeF; i++)
			{
				*pDataRateF += CharDataRateF[i];
			}
			*pDataRateF /= CharsToComputeF;
			*pDataRateF /= 1E3;
		}
		else
		{
			*pDataRateF = 0.0;
			ret = RET_DATARATE_FAILED;
		}

		if ((DataRateFCalculated) && (DataRateBCalculated))
		{
			ret = RET_OK;
		}

	}

	return ret;
}


WORD CloseCardEmulationChannel()
{
	WORD ret;

	do
	{
		// Close the scenario if needed
		if (gNFCTestingData.PollerAScnId)
			MPC_CloseScenarioPcd(CPL0, gNFCTestingData.PollerAScnId);
		
		if (gNFCTestingData.PollerBScnId)
			MPC_CloseScenarioPcd(CPL0, gNFCTestingData.PollerBScnId);
		
		if (gNFCTestingData.PollerFScnId)
			MPC_CloseScenarioPcd(CPL0, gNFCTestingData.PollerFScnId);

		// Close card emulation channel
		ret = MPC_ChannelClose(CPL0);
		CHECK_RET("Unable to close the Channel");

#ifdef PT1_EMV_ANTENNA
		// Select the non-linear load
		ret = MPC_SelectLoadAntennaNfc(CPL0, 0);
		CHECK_RET("Unable to select the non linear load");
#endif

		gNFCTestingData.DataRatesStatus = RET_DATARATE_FAILED;
		gNFCTestingData.LastMeasuredBDataRate = 0.0;
		gNFCTestingData.LastMeasuredFDataRate = 0.0;
		gNFCTestingData.PollerFScnId = 0;
		gNFCTestingData.PollerBScnId = 0;
		gNFCTestingData.PollerAScnId = 0;
		gNFCTestingData.CardEmulationModeReady = FALSE;
	} while (0);

	return ret;

}

WORD OpenCardEmulationChannel(int Speed, BYTE Type)
{
	WORD ret;
	DWORD analyzedDatarate;

	do
	{
		// Log for Datarate
		ret = StartDownloadTo(CPL0, LOG_FILE_DATARATE);
		CHECK_RET("Unable to start the log")
		ret = MPS_OpenLog(CPL0, MASK_ALL_CL_EVENTS, 0);
		CHECK_RET("Unable to open the log")
		
		// Configure the PT1 in NFC target passive mode
		ret = MPC_NfcConfiguration(CPL0, NFC_PASSIVE_MODE, NFC_TARGET, 106);
		CHECK_RET("Unable to activate target passive mode");
		// Configure the PT1 antenna load
		ret = MPC_SelectLoadAntennaNfc(CPL0, ANTENNA_LOAD_330);
		CHECK_RET("Unable to configure the PT1 antenna load");
		
#ifdef PT1_EMV_ANTENNA
		// Select the good LMA load with the PT1 EMV Antenna
		ret = MPC_SetLMAForCardEmulation(CPL0, 0, LMA_TEST);
		CHECK_RET("Unable to set the LMA for card emulation");
#endif
		// Select external reception channel
		ret = MPC_SelectRxChannel(CPL0, RX_CHANNEL_2);
		CHECK_RET("Unable to select the external reception channel");

		// EMV Antenna : We must select the non-linear load before getting the field strength
#ifdef PT1_EMV_ANTENNA
		// Select the non-linear load
		ret = MPC_SelectLoadAntennaNfc(CPL0, 0);
		CHECK_RET("Unable to select the non linear load");
#endif

		// open the communication channel in simulation mode
		ret = MPC_ChannelOpen(CPL0, SIM_MODE);
		CHECK_RET("Unable to open the communication channel in simulation mode");

		// Launch the Datarate automatic analysis -- Type B
		ret = MPC_GetDatarate(CPL0, TYPE_B, &analyzedDatarate);
		CHECK_RET("Unable to open the communication channel in simulation mode");
		gNFCTestingData.LastMeasuredBDataRate = analyzedDatarate / 1E3;
		
		// Launch the Datarate automatic analysis -- Type F
		ret = MPC_GetDatarate(CPL0, 5, &analyzedDatarate);
		CHECK_RET("Unable to open the communication channel in simulation mode");
		gNFCTestingData.LastMeasuredFDataRate = analyzedDatarate / 1E3;
		gNFCTestingData.DataRatesStatus = RET_OK;

		
#ifndef PT1_EMV_ANTENNA
		// Select the analog IN impedance
		ret = MPC_SelectInputImpedanceAnalogIn(CPL0, INPUT_IMPEDANCE_50);
		CHECK_RET("Unable to Select Analog IN impedance");
#endif

		// Get the field strength
		ret = MPC_GetVDCIn(CPL0, &gNFCTestingData.LastMeasuredFieldStrength, FIELD_STRENGTH_INTEGRATION_TIME, 0);
		CHECK_RET("Unable to get the field strength");

#ifdef PT1_EMV_ANTENNA
		// Configure the PT1 antenna load
		ret = MPC_SelectLoadAntennaNfc(CPL0, ANTENNA_LOAD_330);
		CHECK_RET("Unable to select the antenna load");
#endif

		// Select the Analog IN range
		ret = MPC_SelectVoltmeterRange(CPL0, VOLTMETER_RANGE_1000);
		CHECK_RET("Unable to select the analog IN range");

		// First, we must stop the data acquisition.
		ret = MPS_CloseLog(CPL0);
		CHECK_RET("Unable to close the log");
		//and finally, close the download connection.
		ret = MPS_EndDownload(CPL0);
		CHECK_RET("Unable to end the download");

		//gNFCTestingData.DataRatesStatus = ComputeDataRates(LOG_FILE_DATARATE, &gNFCTestingData.LastMeasuredBDataRate, &gNFCTestingData.LastMeasuredFDataRate);

		gNFCTestingData.CardEmulationModeReady = TRUE;

	} while (0);

	if (ret)
	{
		// First, we must stop the data acquisition.
		MPS_CloseLog(CPL0);
		//and finally, close the download connection.
		MPS_EndDownload(CPL0);
	}
	

	return ret;
}

double ReadLowStateFromWaveform(char* FileName)
{
	FILE* fmpat;
	bool LowvalueCalculated = false;
	bool cursorsFound = false;
	int MeasurementIndex = 0;
	double meanValue = 0.0;
	double cursorStart = 0.0;
	double cursorEnd = 0.0;
	char line[256];

	// Read Header
	fmpat = fopen(FileName, "rt");

	if (fmpat != NULL)
	{
		while (fgets(line, sizeof(line), fmpat) && !cursorsFound)
		{
			if (strncmp(line, "#X3=", 4) == 0)
			{
				sscanf(line, "#X3=%lf\n", &cursorStart);
			}
			else if (strncmp(line, "#X4=", 4) == 0)
			{
				sscanf(line, "#X4=%lf\n", &cursorEnd);
			}
			else if (strncmp(line, "##B1#", 5) == 0)
			{
				MeasurementIndex = ftell(fmpat);
				cursorsFound = true;
			}
		}
		fclose(fmpat);

		fmpat = fopen(FileName, "rb");

		if (fmpat != NULL)
		{
			fseek(fmpat, MeasurementIndex, SEEK_SET);

			int CountValues = 0;

			while (!feof(fmpat) && !LowvalueCalculated)
			{
				//fscanf(fmpat)
				double x;
				double y;

				fread(&x, sizeof(double), 1, fmpat);
				fread(&y, sizeof(double), 1, fmpat);

				if (x > cursorEnd)
				{
					meanValue /= CountValues;
					LowvalueCalculated = true;
				}
				else if (x >= cursorStart)
				{
					meanValue += y;
					CountValues++;
				}
			}

		}

		fclose(fmpat);
	}

	return (meanValue * 1E6);
}



void RunPollerTests(BYTE Type, NFCTESTING_RESULT* pDetailedResult)
{
	DWORD ScnId = 0;
	DWORD TriggerType = 0;
	DWORD PcdFrameType = 0;
	WORD ret = RET_NOTEXECUTED;
	int Speed = 0;
	int Download;
	unsigned int MeasurementCount = 0;
	unsigned int* MeasurementTable;
	char WaveformFile[250];
	int Delay = 0;
	DWORD CaptureCount = ACQ_TIME_MAX;

	if (Type == TYPE_A)
	{
		TriggerType = TRIG_EVT_CHAR_PCD_TYPE_A;
		PcdFrameType = TYPE_A;
		ScnId = gNFCTestingData.PollerAScnId;
		Speed = 106;
		strcpy(WaveformFile, "C:\\NFC Production Test\\Test Results\\Waveform_Poller_TypeA.mpat");
		Download = 0;
		Delay = DELAY_TIME_PCD_A;
		CaptureCount = ACQ_TIME_PCD_A;
	}
	else if (Type == TYPE_B)
	{
		TriggerType = TRIG_EVT_CHAR_PCD_TYPE_B;
		PcdFrameType = TYPE_B;
		ScnId = gNFCTestingData.PollerBScnId;
		Speed = 106;
		strcpy(WaveformFile, "C:\\NFC Production Test\\Test Results\\Waveform_Poller_TypeB.mpat");
		Download = 0;
		Delay = DELAY_TIME_PCD_B;
		CaptureCount = ACQ_TIME_PCD_B;
	}
	else if (Type == TYPE_FELICA)
	{
		TriggerType = TRIG_EVT_CHAR_PCD_TYPE_F;
		PcdFrameType = TYPE_FELICA;
		ScnId = gNFCTestingData.PollerFScnId;
		Speed = 212;
		strcpy(WaveformFile, "C:\\NFC Production Test\\Test Results\\Waveform_Poller_TypeF.mpat");
		Download = 0;
		Delay = DELAY_TIME_PCD_F;
		CaptureCount = ACQ_TIME_PCD_F;
	}

	do
	{

		// Open the card emulation channel with appropriate RF parameters
		if (!gNFCTestingData.CardEmulationModeReady)
		{
			ret = OpenCardEmulationChannel(Speed, Type);
			CHECK_RET("Unable to initialize the card emulation channel")
		}

		// Set the amount of captured points
		ret = MPC_ChangeProtocolParameters(CPL0, CPP_SET_BURST_NB_SAMPLE, &CaptureCount, sizeof(CaptureCount));
		CHECK_RET("Unable to change the amount of captured points")

			// Prepare RF Acquisition. Trigg
			ret = MPC_StartRFMeasure(CPL0, TriggerType, Delay/*PRE_TRIGGER_DURATION*/, 0, 0, 0);
		CHECK_RET("Unable to start the waveform measurement")

			// Open a scenario if not already created (mandatory if you want to trigg the measurement)
			if (ScnId == 0)
			{
				ret = MPC_OpenScenarioPcd(CPL0, &ScnId, 0);
				CHECK_RET("Unable to configure the card emulation mode : please check if the PCD under test is polling");
				// Wait for a frame type
				if (Type == TYPE_A)
				{
					gNFCTestingData.PollerAScnId = ScnId;
					ret = MPC_AddToScenarioPcd(CPL0, ScnId, TSCN_DO_EXCHANGE, EXCHANGE_NOPCDCRC, EXCHANGE_NOPICCCRC, PcdFrameType, EXCHANGE_SYNCHRO_ON, 1, REQA, 0, "\x00", NULL);
				}
				else if (Type == TYPE_B)
				{
					gNFCTestingData.PollerBScnId = ScnId;
					ret = MPC_AddToScenarioPcd(CPL0, ScnId, TSCN_DO_EXCHANGE, EXCHANGE_PCDCRC, EXCHANGE_NOPICCCRC, PcdFrameType, EXCHANGE_SYNCHRO_ON, 3, REQB, 0, "\x00", NULL);
				}
				else if (Type == TYPE_FELICA)
				{
					gNFCTestingData.PollerFScnId = ScnId;
					ret = MPC_AddToScenarioPcd(CPL0, ScnId, TSCN_DO_EXCHANGE, EXCHANGE_PCDCRC, EXCHANGE_NOPICCCRC, PcdFrameType, EXCHANGE_SYNCHRO_ON, 6, FELICA_POLL_REQ_INGENICO, 0, "\x00", NULL);
				}
				CHECK_RET("Unable to wait for the specific frame type");
			}

		// execute the scenario with a timeout of DUT_POLLING_PERIOD seconds
		ret = MPC_ExecuteScenarioPcd(CPL0, ScnId, DUT_POLLING_PERIOD);
		CHECK_RET("Unable to execute the scenario");

		// Wait for the measurement to be complete AND
		ret = MPS_DoTempo(MEASUREMENT_DELAY);
		// Exit
		CHECK_RET("Unable to set a delay - Measurement")

			// Analyze the  waveform
			ret = GetAnalyzedMeasureVoltmeterToFile(CPL0, SCINFC_MEASTYPE_PCD_WAVEFORM, Type, Speed, Download, WaveformFile, &MeasurementCount, &MeasurementTable);
		CHECK_RET("Unable to retrieve the measurement")

			if (MeasurementCount > 0)
			{
				if ((Type == TYPE_B) || (Type == TYPE_FELICA))
				{
					pDetailedResult[MODINDEX_TEST_IDX].Value = MeasurementTable[TYPE_B_WF_MEAS_MOD_IDX] / 1E6;
					pDetailedResult[MODINDEX_TEST_IDX].ErrorCode = ret;

					// SL : TODO avec nouveau firmware
					//pDetailedResult[MODDEPTH_TEST_IDX].Value = = MeasurementTable[TYPE_B_WF_MEAS_MODDEPTH_IDX] / 1E6;

					pDetailedResult[MODDEPTH_TEST_IDX].Value = 100.0 * (1.0 - ((double)MeasurementTable[TYPE_B_WF_MEAS_V2_IDX] / (double)MeasurementTable[TYPE_B_WF_MEAS_V1_IDX]));
					pDetailedResult[MODDEPTH_TEST_IDX].ErrorCode = ret;


					pDetailedResult[OVS_TEST_IDX].Value = MeasurementTable[TYPE_B_WF_MEAS_OVS_RE_IDX] / 1E6;
					pDetailedResult[OVS_TEST_IDX].ErrorCode = ret;
					if (Type == TYPE_B)
						pDetailedResult[DATARATE_TEST_IDX].Value = gNFCTestingData.LastMeasuredBDataRate;
					if (Type == TYPE_FELICA)
						pDetailedResult[DATARATE_TEST_IDX].Value = gNFCTestingData.LastMeasuredFDataRate;
					pDetailedResult[DATARATE_TEST_IDX].ErrorCode = gNFCTestingData.DataRatesStatus;

					pDetailedResult[FIELDSTRENGTH_TEST_IDX].Value = (double)gNFCTestingData.LastMeasuredFieldStrength;
					pDetailedResult[FIELDSTRENGTH_TEST_IDX].ErrorCode = ret;
				}
				else if (Type == TYPE_A)
				{
					// SL : TODO avec nouveau firmware
					//pDetailedResult[MODDEPTH_TEST_IDX].Value = MeasurementTable[TYPE_A_WF_MEAS_MODDEPTH_IDX] / 1E6;
					//pDetailedResult[MODINDEX_TEST_IDX].Value = MeasurementTable[TYPE_A_WF_MEAS_MODINDEX_IDX] / 1E6;

					double TypeALowState = ReadLowStateFromWaveform(WV_FILE);
					pDetailedResult[MODINDEX_TEST_IDX].Value = 100 * (1.0 - (TypeALowState / (double)MeasurementTable[TYPE_A_WF_MEAS_V1_IDX])) / (1.0 + (TypeALowState / (double)MeasurementTable[TYPE_A_WF_MEAS_V1_IDX]));
					pDetailedResult[MODINDEX_TEST_IDX].ErrorCode = ret;
					pDetailedResult[MODDEPTH_TEST_IDX].Value = 100.0 * (1.0 - (TypeALowState / (double)MeasurementTable[TYPE_A_WF_MEAS_V1_IDX]));
					pDetailedResult[MODDEPTH_TEST_IDX].ErrorCode = ret;
					pDetailedResult[OVS_TEST_IDX].Value = MeasurementTable[TYPE_A_WF_MEAS_OVS_RE_IDX] / 1E6;
					pDetailedResult[OVS_TEST_IDX].ErrorCode = ret;

					pDetailedResult[T2_TEST_IDX].Value = MeasurementTable[TYPE_A_WF_MEAS_T2_IDX];
					pDetailedResult[T2_TEST_IDX].ErrorCode = ret;

					pDetailedResult[T1_TEST_IDX].Value = MeasurementTable[TYPE_A_WF_MEAS_T1_IDX];
					pDetailedResult[T1_TEST_IDX].ErrorCode = ret;

					pDetailedResult[T3_TEST_IDX].Value = MeasurementTable[TYPE_A_WF_MEAS_T3_IDX];
					pDetailedResult[T3_TEST_IDX].ErrorCode = ret;

					pDetailedResult[T4_TEST_IDX].Value = MeasurementTable[TYPE_A_WF_MEAS_T4_IDX];
					pDetailedResult[T4_TEST_IDX].ErrorCode = ret;

					pDetailedResult[FIELDSTRENGTH_TEST_IDX].Value = (double)gNFCTestingData.LastMeasuredFieldStrength;
					pDetailedResult[FIELDSTRENGTH_TEST_IDX].ErrorCode = ret;


					int monoFalling = MeasurementTable[TYPE_A_WF_MEAS_TFMON_IDX];
					int monoRising = MeasurementTable[TYPE_A_WF_MEAS_TRMON_IDX];


					/*double ringing = MeasurementTable[TYPE_A_WF_MEAS_RINGING_LVL_IDX] / 1E6;
					printf("Ringing = %f\r\n", ringing);

					int countT5 = MeasurementTable[TYPE_A_WF_MEAS_T5COUNT_IDX];
					printf("T5count = %ld\r\n", countT5);

					for (int k = 0; k < countT5; k++)
					{
						int T5Value = MeasurementTable[TYPE_A_WF_MEAS_T5COUNT_IDX + k + 1];
						printf("T5Value = %ld ns\r\n", T5Value);
					}*/

				}
			}
	} while (0);

	// Handle errors
	if (ret)
	{
		CloseCardEmulationChannel();
	}

}


void RunListenerTests(BYTE Type, NFCTESTING_RESULT* pDetailedResult)
{
	WORD ATQA;
	BYTE Buff[256];
	BYTE NFCID2[8];
	WORD RD[256];
	BYTE Pad[8];
	WORD length;
	WORD ret = RET_NOTEXECUTED;
	DWORD time = 0;
	DWORD dummy = 0;
	unsigned int MeasurementCount = 0;
	unsigned int* MeasurementTable;
	int Delay = 0;
	DWORD CaptureCount = ACQ_TIME_MAX;

	// No card emulation mode
	if (gNFCTestingData.CardEmulationModeReady)
		CloseCardEmulationChannel();

	if (Type == TYPE_A)
	{
		Delay = DELAY_TIME_LMA_A;
		CaptureCount = ACQ_TIME_LMA_A;
	}
	else if (Type == TYPE_B)
	{
		Delay = DELAY_TIME_LMA_B;
		CaptureCount = ACQ_TIME_LMA_B;
	}
	else if (Type == TYPE_FELICA)
	{
		Delay = DELAY_TIME_LMA_F;
		CaptureCount = ACQ_TIME_LMA_F;
	}

	do
	{
		// Set the RF conditions
#ifdef PT1_EMV_ANTENNA
		// Non linear load
		ret = MPC_SelectLoadAntennaNfc(CPL0, 0);
		CHECK_RET(RESPTIME_TEST_IDX, "Unable to select the non linear load")
#endif
		// Field ON
		ret = MPC_SelectFieldStrength(CPL0, UNIT_PER_THOUSAND, FIELD_VSOV_TEST);
		CHECK_RET("Unable to apply the field strength")
		// Wait for the field to be applied
		ret = MPS_DoTempo(FIELD_ON_DELAY);
		CHECK_RET( "Unable to set a delay - On field")
		// Select type
		ret = MPC_SelectType(CPL0, Type);
		CHECK_RET("Unable to select type")
		// Select external Rx
		ret = MPC_SelectRxChannel(CPL0, RX_CHANNEL_2);
		CHECK_RET("Unable to select the Analog IN acquisition channel")

		// ANcienne position adjjustRX

		// Select Analog IN impedance
		ret = MPC_SelectInputImpedanceAnalogIn(CPL0, INPUT_IMPEDANCE_50);
		CHECK_RET("Unable to select the input impedance")
			
		// Select the Analog IN range
		ret = MPC_SelectVoltmeterRange(CPL0, 1);
		CHECK_RET("Unable to Analog IN range")

		/*(if(Type == TYPE_FELICA)
		{
			ret = MPC_SelectVoltmeterRange(CPL0, 2000);
			CHECK_RET("Unable to Analog IN range")
		}*/

		// Adjust external Rx
		ret = MPC_AdjustRX_Channel_2(CPL0);
		CHECK_RET("Unable to adjust RX Channel")

		// Enable the Response time measurement
		ret = MPC_PiccResponseTime(CPL0, PRT_ENABLE, TOU_NANO, &dummy);
		CHECK_RET("Unable to enable the response time measurement")

		// Set the amount of captured points
		ret = MPC_ChangeProtocolParameters(CPL0, CPP_SET_BURST_NB_SAMPLE, &CaptureCount, sizeof(CaptureCount));
		CHECK_RET("Unable to change the amount of captured points")

		// Prepare RF Acquisition. Acquisition will trigg when a PICC answers
		ret = MPC_StartRFMeasure(CPL0, TRIG_EVT_RX_ON_NFC, Delay, 0, 0, 0);
		CHECK_RET("Unable to start the LMA measurement")


		// Send WUPA, WUPB
		if (Type == TYPE_A)
		{
			ret = MPC_ChangeProtocolParameters(CPL0, CPP_SET_BURST_NB_SAMPLE, &CaptureCount, sizeof(CaptureCount));
			CHECK_RET("Unable to change the amount of captured points")

			ret = MPC_WakeUpA(CPL0, &ATQA);
			CHECK_RET("Unable to detect a type A card")
		}
		else if (Type == TYPE_B)
		{
			ret = MPC_ChangeProtocolParameters(CPL0, CPP_SET_BURST_NB_SAMPLE, &CaptureCount, sizeof(CaptureCount));
			CHECK_RET("Unable to change the amount of captured points")

			ret = MPC_WakeUpB(CPL0, 1, Buff, &length);
			CHECK_RET("Unable to detect a type B card")
		}
		else if (Type == TYPE_FELICA)
		{
			ret = MPC_ChangeProtocolParameters(CPL0, CPP_SET_BURST_NB_SAMPLE, &CaptureCount, sizeof(CaptureCount));
			CHECK_RET("Unable to change the amount of captured points")

			ret = MPC_SelectType(CPL0, TYPE_FELICA);
			CHECK_RET("Unable to select a type F card")
			ret = MPC_NfcConfiguration(CPL0, 1, 1, 212);
			CHECK_RET("Unable to set type F configuration")
				//ret = MPC_FELICA_Polling(CPL0, 0xFF, 0, 0x1, Buff, &length);
			ret = MPC_PollReq(CPL0, 0xFFFF, 0, 0, NFCID2, Pad, RD);
			CHECK_RET("Unable to detect a type F card")
		}
		
		// Get the PICC Response Time
		ret = MPC_PiccResponseTime(CPL0, PRT_GET, TOU_NANO, &time);
		CHECK_RET("Unable to get the FDT measurement")
		pDetailedResult[RESPTIME_TEST_IDX].Value = time / 1E3;
		pDetailedResult[RESPTIME_TEST_IDX].ErrorCode = ret;

		// Disable the measurement
		ret = MPC_PiccResponseTime(CPL0, PRT_DISABLE, TOU_NANO, &dummy);
		CHECK_RET("Unable to disable the measurement")

		// Field OFF
		ret = MPC_SelectFieldStrength(CPL0, UNIT_PER_THOUSAND, 0);
		CHECK_RET("Unable to switch OFF the field")

		// Wait for the measurement to be complete AND
		// Wait for the field to switched off
		ret = MPS_DoTempo(FIELD_OFF_DELAY);
		CHECK_RET("Unable to set a delay - Off field / Measurement")

		// Measure PICC LMA
		if (Type == TYPE_FELICA)
		{
			ret = GetAnalyzedMeasureVoltmeterToFile(CPL0, SCINFC_MEASTYPE_PICC_LMA, Type, 212, 0, "C:\\NFC Production Test\\Test Results\\LMA_Felica.mpat", &MeasurementCount, &MeasurementTable);
			CHECK_RET("Unable to retrieve the load modulation")
		}
		else if(Type == TYPE_A)
		{
			ret = GetAnalyzedMeasureVoltmeterToFile(CPL0, SCINFC_MEASTYPE_PICC_LMA, Type, 106, 0, "C:\\NFC Production Test\\Test Results\\LMA_Type_A.mpat", &MeasurementCount, &MeasurementTable);
			CHECK_RET("Unable to retrieve the load modulation")
		}
		else if (Type == TYPE_B)
		{
			ret = GetAnalyzedMeasureVoltmeterToFile(CPL0, SCINFC_MEASTYPE_PICC_LMA, Type, 106, 0, "C:\\NFC Production Test\\Test Results\\LMA_Type_B.mpat", &MeasurementCount, &MeasurementTable);
			CHECK_RET("Unable to retrieve the load modulation")
		}

		
	
		if (MeasurementCount > 0)
		{
			pDetailedResult[LMA_TEST_IDX].Value = MeasurementTable[0] / 1E3;
			pDetailedResult[LMA_TEST_IDX].ErrorCode = ret;
		}
		else
		{
			CHECK_RET("Unable to retrieve the load modulation (2)")
		}
	} while (0);

	// Handle errors
	if (ret)
	{
		// Disable the measurement
		MPC_PiccResponseTime(CPL0, PRT_CLEAR, TOU_NANO, &dummy);
		// Select standard channel
		ret = MPC_SelectRxChannel(CPL0, RX_CHANNEL_1);
		// Field OFF
		MPC_SelectFieldStrength(CPL0, UNIT_PER_THOUSAND, 0);
		MPS_DoTempo(FIELD_OFF_DELAY);
	}
}



void RunListenerTests_BatteryOff(BYTE Type, NFCTESTING_RESULT* pDetailedResult)
{
	WORD ATQA;
	BYTE Buff[256];
	WORD length;
	WORD ret = RET_NOTEXECUTED;
	DWORD time = 0;
	DWORD dummy = 0;
	unsigned int MeasurementCount = 0;
	unsigned int* MeasurementTable;
	int Delay = 0;
	DWORD CaptureCount = ACQ_TIME_MAX;

	// No card emulation mode
	if (gNFCTestingData.CardEmulationModeReady)
		CloseCardEmulationChannel();

	if (Type == TYPE_A)
	{
		Delay = DELAY_TIME_LMA_A_BATTERY_OFF;
		//CaptureCount = ACQ_TIME_LMA_A;
	}
	else if (Type == TYPE_B)
	{
		Delay = DELAY_TIME_LMA_B_BATTERY_OFF;
		//CaptureCount = ACQ_TIME_LMA_B;
	}

	do
	{
		// Set the RF conditions
#ifdef PT1_EMV_ANTENNA
		// Non linear load
		ret = MPC_SelectLoadAntennaNfc(CPL0, 0);
		CHECK_RET(RESPTIME_TEST_IDX, "Unable to select the non linear load")
#endif
		// Field ON
		ret = MPC_SelectFieldStrength(CPL0, UNIT_PER_THOUSAND, FIELD_VSOV_TEST);
		CHECK_RET("Unable to apply the field strength")
		// Wait for the field to be applied
		ret = MPS_DoTempo(FIELD_ON_DELAY);
		CHECK_RET("Unable to set a delay - On field")
		// Select type
		ret = MPC_SelectType(CPL0, Type);
		CHECK_RET("Unable to select type")
		// Select external Rx
		ret = MPC_SelectRxChannel(CPL0, RX_CHANNEL_2);
		CHECK_RET("Unable to select the Analog IN acquisition channel")

		// Select Analog IN impedance
		ret = MPC_SelectInputImpedanceAnalogIn(CPL0, INPUT_IMPEDANCE_50);
		CHECK_RET("Unable to select the input impedance")

		// Select the Analog IN range
		ret = MPC_SelectVoltmeterRange(CPL0, 1);
		CHECK_RET("Unable to Analog IN range")

		// Adjust external Rx
		ret = MPC_AdjustRX_Channel_2(CPL0);
		CHECK_RET("Unable to adjust RX Channel")

		// Enable the Response time measurement
		ret = MPC_PiccResponseTime(CPL0, PRT_ENABLE, TOU_NANO, &dummy);
		CHECK_RET("Unable to enable the response time measurement")

		// Set the amount of captured points
		/*	ret = MPC_ChangeProtocolParameters(CPL0, CPP_SET_BURST_NB_SAMPLE, &CaptureCount, sizeof(CaptureCount));
		CHECK_RET("Unable to change the amount of captured points")*/

		// Prepare RF Acquisition. Acquisition will trigg when a PICC answers
		ret = MPC_StartRFMeasure(CPL0, TRIG_EVT_TX_OFF, Delay, 0, 0, 0);
		CHECK_RET("Unable to start the LMA measurement")

			// Send WUPA, WUPB
		if (Type == TYPE_A)
		{
			ret = MPC_WakeUpA(CPL0, &ATQA);
			CHECK_RET("Unable to detect a type A card")
		}
		else if (Type == TYPE_B)
		{
			ret = MPC_WakeUpB(CPL0, 1, Buff, &length);
			CHECK_RET("Unable to detect a type B card")
		}

		// Get the PICC Response Time
		ret = MPC_PiccResponseTime(CPL0, PRT_GET, TOU_NANO, &time);
		CHECK_RET("Unable to get the FDT measurement")
		pDetailedResult[RESPTIME_TEST_IDX].Value = time / 1E3;
		pDetailedResult[RESPTIME_TEST_IDX].ErrorCode = ret;

		// Disable the measurement
		ret = MPC_PiccResponseTime(CPL0, PRT_DISABLE, TOU_NANO, &dummy);
		CHECK_RET("Unable to disable the measurement")

		// Field OFF
		ret = MPC_SelectFieldStrength(CPL0, UNIT_PER_THOUSAND, 0);
		CHECK_RET("Unable to switch OFF the field")

		// Wait for the measurement to be complete AND
		// Wait for the field to switched off
		ret = MPS_DoTempo(FIELD_OFF_DELAY);
		CHECK_RET("Unable to set a delay - Off field / Measurement")

		// Measure PICC LMA

		if (Type == TYPE_FELICA)
		{
			ret = GetAnalyzedMeasureVoltmeterToFile(CPL0, SCINFC_MEASTYPE_PICC_LMA, 5, 212, 0, "", &MeasurementCount, &MeasurementTable);
			CHECK_RET("Unable to retrieve the load modulation")
		}
		else
		{
			ret = GetAnalyzedMeasureVoltmeterToFile(CPL0, SCINFC_MEASTYPE_PICC_LMA, Type, 106, 0, "", &MeasurementCount, &MeasurementTable);
			CHECK_RET("Unable to retrieve the load modulation")
		}

		if (MeasurementCount > 0)
		{
			pDetailedResult[LMA_TEST_IDX].Value = MeasurementTable[0] / 1E3;
			pDetailedResult[LMA_TEST_IDX].ErrorCode = ret;
		}
		else
		{
			CHECK_RET("Unable to retrieve the load modulation (2)")
		}
	} while (0);

	// Handle errors
	if (ret)
	{
		// Disable the measurement
		MPC_PiccResponseTime(CPL0, PRT_CLEAR, TOU_NANO, &dummy);
		// Select standard channel
		ret = MPC_SelectRxChannel(CPL0, RX_CHANNEL_1);
		// Field OFF
		MPC_SelectFieldStrength(CPL0, UNIT_PER_THOUSAND, 0);
		MPS_DoTempo(FIELD_OFF_DELAY);
	}
}


/*************************************
* MAIN ENTRY POINTS
************************************/

int TestListener(BYTE ListenerType, NFCTESTING_RESULT* pDetailedResult, bool NoAnswerMode)
{
	int GlobalResult = RET_OK;
	int TestRetries = MAX_RETRIES;

	if (pDetailedResult)
	{
		if (ListenerType == TYPE_A)
		{
			strcpy(pDetailedResult[RESPTIME_TEST_IDX].MeasurementName, MEAS_NAME_LISTENER_A_RESPONSETIME);
			pDetailedResult[RESPTIME_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[RESPTIME_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[LMA_TEST_IDX].MeasurementName, MEAS_NAME_LISTENER_A_LMA);
			pDetailedResult[LMA_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[LMA_TEST_IDX].Value = 0.0;

			do
			{
				if (NoAnswerMode)
				{
					RunListenerTests_BatteryOff(TYPE_A, pDetailedResult);
				}
				else
				{
					RunListenerTests(TYPE_A, pDetailedResult);
				}

				TestRetries--;
			} while ((pDetailedResult[RESPTIME_TEST_IDX].ErrorCode || pDetailedResult[LMA_TEST_IDX].ErrorCode) && (TestRetries >= 0));


			if (pDetailedResult[RESPTIME_TEST_IDX].ErrorCode || pDetailedResult[LMA_TEST_IDX].ErrorCode)
			{
				GlobalResult = RET_FAILED;
			}
		}
		else if (ListenerType == TYPE_B)
		{
			strcpy(pDetailedResult[RESPTIME_TEST_IDX].MeasurementName, MEAS_NAME_LISTENER_B_RESPONSETIME);
			pDetailedResult[RESPTIME_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[RESPTIME_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[LMA_TEST_IDX].MeasurementName, MEAS_NAME_LISTENER_B_LMA);
			pDetailedResult[LMA_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[LMA_TEST_IDX].Value = 0.0;

			do
			{
				if (NoAnswerMode)
				{
					RunListenerTests_BatteryOff(TYPE_B, pDetailedResult);
				}
				else
				{
					RunListenerTests(TYPE_B, pDetailedResult);
				}
				TestRetries--;
			} while ((pDetailedResult[RESPTIME_TEST_IDX].ErrorCode || pDetailedResult[LMA_TEST_IDX].ErrorCode) && (TestRetries >= 0));

			if (pDetailedResult[RESPTIME_TEST_IDX].ErrorCode || pDetailedResult[LMA_TEST_IDX].ErrorCode)
			{
				GlobalResult = RET_FAILED;
			}
		}
		else if (ListenerType == TYPE_FELICA)
		{
			strcpy(pDetailedResult[RESPTIME_TEST_IDX].MeasurementName, MEAS_NAME_LISTENER_F_RESPONSETIME);
			pDetailedResult[RESPTIME_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[RESPTIME_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[LMA_TEST_IDX].MeasurementName, MEAS_NAME_LISTENER_F_LMA);
			pDetailedResult[LMA_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[LMA_TEST_IDX].Value = 0.0;

			do
			{
				if (NoAnswerMode)
				{
					RunListenerTests_BatteryOff(TYPE_FELICA, pDetailedResult);
				}
				else
				{
					RunListenerTests(TYPE_FELICA, pDetailedResult);
				}
				TestRetries--;
			} while ((pDetailedResult[RESPTIME_TEST_IDX].ErrorCode || pDetailedResult[LMA_TEST_IDX].ErrorCode) && (TestRetries >= 0));

			if (pDetailedResult[RESPTIME_TEST_IDX].ErrorCode || pDetailedResult[LMA_TEST_IDX].ErrorCode)
			{
				GlobalResult = RET_FAILED;
			}
		}

		else
		{
			strcpy(pDetailedResult[RESPTIME_TEST_IDX].MeasurementName, "");
			pDetailedResult[RESPTIME_TEST_IDX].ErrorCode = RET_WRONG_MEASUREMENT_CALLED;
			pDetailedResult[RESPTIME_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[LMA_TEST_IDX].MeasurementName, "");
			pDetailedResult[LMA_TEST_IDX].ErrorCode = RET_WRONG_MEASUREMENT_CALLED;
			pDetailedResult[LMA_TEST_IDX].Value = 0.0;

			GlobalResult = RET_WRONG_MEASUREMENT_CALLED;
		}
	}
	else
	{
		GlobalResult = RET_RESULT_NOT_ALLOCATED;
	}

	return GlobalResult;

}


int TestPoller(BYTE PollerType, NFCTESTING_RESULT* pDetailedResult)
{
	int GlobalResult = RET_OK;
	int TestRetries = MAX_RETRIES;

	if (pDetailedResult)
	{
		if (PollerType == TYPE_A)
		{
			strcpy(pDetailedResult[MODINDEX_TEST_IDX].MeasurementName, MEAS_NAME_POLLER_A_MODINDEX);
			pDetailedResult[MODINDEX_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[MODINDEX_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[MODDEPTH_TEST_IDX].MeasurementName, MEAS_NAME_POLLER_A_MODDEPTH);
			pDetailedResult[MODDEPTH_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[MODDEPTH_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[OVS_TEST_IDX].MeasurementName, MEAS_NAME_POLLER_A_OVS);
			pDetailedResult[OVS_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[OVS_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[T2_TEST_IDX].MeasurementName, MEAS_NAME_POLLER_A_T2);
			pDetailedResult[T2_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[T2_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[T3_TEST_IDX].MeasurementName, MEAS_NAME_POLLER_A_T3);
			pDetailedResult[T3_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[T3_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[T4_TEST_IDX].MeasurementName, MEAS_NAME_POLLER_A_T4);
			pDetailedResult[T4_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[T4_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[T1_TEST_IDX].MeasurementName, MEAS_NAME_POLLER_A_T1);
			pDetailedResult[T1_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[T1_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[FIELDSTRENGTH_TEST_IDX].MeasurementName, MEAS_NAME_FIELDSTRENGH);
			pDetailedResult[FIELDSTRENGTH_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[FIELDSTRENGTH_TEST_IDX].Value = 0.0;

			do
			{
				RunPollerTests(TYPE_A, pDetailedResult);
				TestRetries--;
			} while ((pDetailedResult[MODINDEX_TEST_IDX].ErrorCode ||
				pDetailedResult[MODDEPTH_TEST_IDX].ErrorCode ||
				pDetailedResult[OVS_TEST_IDX].ErrorCode ||
				pDetailedResult[FIELDSTRENGTH_TEST_IDX].ErrorCode ||
				pDetailedResult[T1_TEST_IDX].ErrorCode ||
				pDetailedResult[T3_TEST_IDX].ErrorCode ||
				pDetailedResult[T4_TEST_IDX].ErrorCode ||
				pDetailedResult[T2_TEST_IDX].ErrorCode) &&
				(TestRetries >= 0));


			if (pDetailedResult[MODINDEX_TEST_IDX].ErrorCode ||
				pDetailedResult[MODDEPTH_TEST_IDX].ErrorCode ||
				pDetailedResult[OVS_TEST_IDX].ErrorCode ||
				pDetailedResult[FIELDSTRENGTH_TEST_IDX].ErrorCode ||
				pDetailedResult[T2_TEST_IDX].ErrorCode ||
				pDetailedResult[T1_TEST_IDX].ErrorCode ||
				pDetailedResult[T3_TEST_IDX].ErrorCode ||
				pDetailedResult[T4_TEST_IDX].ErrorCode
				)
			{
				GlobalResult = RET_FAILED;
			}
		}
		else if (PollerType == TYPE_B)
		{
			strcpy(pDetailedResult[MODINDEX_TEST_IDX].MeasurementName, MEAS_NAME_POLLER_B_MODINDEX);
			pDetailedResult[MODINDEX_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[MODINDEX_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[MODDEPTH_TEST_IDX].MeasurementName, MEAS_NAME_POLLER_B_MODDEPTH);
			pDetailedResult[MODDEPTH_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[MODDEPTH_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[OVS_TEST_IDX].MeasurementName, MEAS_NAME_POLLER_B_OVS);
			pDetailedResult[OVS_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[OVS_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[DATARATE_TEST_IDX].MeasurementName, MEAS_NAME_POLLER_B_DATARATE);
			pDetailedResult[DATARATE_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[DATARATE_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[FIELDSTRENGTH_TEST_IDX].MeasurementName, MEAS_NAME_FIELDSTRENGH);
			pDetailedResult[FIELDSTRENGTH_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[FIELDSTRENGTH_TEST_IDX].Value = 0.0;

			do
			{
				RunPollerTests(TYPE_B, pDetailedResult);
				TestRetries--;
			} while ((pDetailedResult[MODINDEX_TEST_IDX].ErrorCode ||
				pDetailedResult[MODDEPTH_TEST_IDX].ErrorCode ||
				pDetailedResult[OVS_TEST_IDX].ErrorCode ||
				pDetailedResult[FIELDSTRENGTH_TEST_IDX].ErrorCode ||
				pDetailedResult[DATARATE_TEST_IDX].ErrorCode) &&
				(TestRetries >= 0));

			if (pDetailedResult[MODINDEX_TEST_IDX].ErrorCode ||
				pDetailedResult[MODDEPTH_TEST_IDX].ErrorCode ||
				pDetailedResult[OVS_TEST_IDX].ErrorCode ||
				pDetailedResult[FIELDSTRENGTH_TEST_IDX].ErrorCode ||
				pDetailedResult[DATARATE_TEST_IDX].ErrorCode
				)
			{
				GlobalResult = RET_FAILED;
			}
		}
		else if (PollerType == TYPE_FELICA)
		{
			strcpy(pDetailedResult[MODINDEX_TEST_IDX].MeasurementName, MEAS_NAME_POLLER_F_MODINDEX);
			pDetailedResult[MODINDEX_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[MODINDEX_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[MODDEPTH_TEST_IDX].MeasurementName, MEAS_NAME_POLLER_F_MODDEPTH);
			pDetailedResult[MODDEPTH_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[MODDEPTH_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[OVS_TEST_IDX].MeasurementName, MEAS_NAME_POLLER_F_OVS);
			pDetailedResult[OVS_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[OVS_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[DATARATE_TEST_IDX].MeasurementName, MEAS_NAME_POLLER_F_DATARATE);
			pDetailedResult[DATARATE_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[DATARATE_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[FIELDSTRENGTH_TEST_IDX].MeasurementName, MEAS_NAME_FIELDSTRENGH);
			pDetailedResult[FIELDSTRENGTH_TEST_IDX].ErrorCode = RET_NOTEXECUTED;
			pDetailedResult[FIELDSTRENGTH_TEST_IDX].Value = 0.0;

			do
			{
				RunPollerTests(TYPE_FELICA, pDetailedResult);
				TestRetries--;
			} while ((pDetailedResult[MODINDEX_TEST_IDX].ErrorCode ||
				pDetailedResult[MODDEPTH_TEST_IDX].ErrorCode ||
				pDetailedResult[OVS_TEST_IDX].ErrorCode ||
				pDetailedResult[FIELDSTRENGTH_TEST_IDX].ErrorCode ||
				pDetailedResult[DATARATE_TEST_IDX].ErrorCode) &&
				(TestRetries >= 0));

			if (pDetailedResult[MODINDEX_TEST_IDX].ErrorCode ||
				pDetailedResult[MODDEPTH_TEST_IDX].ErrorCode ||
				pDetailedResult[OVS_TEST_IDX].ErrorCode ||
				pDetailedResult[FIELDSTRENGTH_TEST_IDX].ErrorCode ||
				pDetailedResult[DATARATE_TEST_IDX].ErrorCode
				)
			{
				GlobalResult = RET_FAILED;
			}
		}
		else
		{
			strcpy(pDetailedResult[MODINDEX_TEST_IDX].MeasurementName, "");
			pDetailedResult[MODINDEX_TEST_IDX].ErrorCode = RET_WRONG_MEASUREMENT_CALLED;
			pDetailedResult[MODINDEX_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[MODDEPTH_TEST_IDX].MeasurementName, "");
			pDetailedResult[MODDEPTH_TEST_IDX].ErrorCode = RET_WRONG_MEASUREMENT_CALLED;
			pDetailedResult[MODDEPTH_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[OVS_TEST_IDX].MeasurementName, "");
			pDetailedResult[OVS_TEST_IDX].ErrorCode = RET_WRONG_MEASUREMENT_CALLED;
			pDetailedResult[OVS_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[DATARATE_TEST_IDX].MeasurementName, "");
			pDetailedResult[DATARATE_TEST_IDX].ErrorCode = RET_WRONG_MEASUREMENT_CALLED;
			pDetailedResult[DATARATE_TEST_IDX].Value = 0.0;

			strcpy(pDetailedResult[FIELDSTRENGTH_TEST_IDX].MeasurementName, "");
			pDetailedResult[FIELDSTRENGTH_TEST_IDX].ErrorCode = RET_WRONG_MEASUREMENT_CALLED;
			pDetailedResult[FIELDSTRENGTH_TEST_IDX].Value = 0.0;
			GlobalResult = RET_WRONG_MEASUREMENT_CALLED;
		}
	}
	else
	{
		GlobalResult = RET_RESULT_NOT_ALLOCATED;
	}


	return GlobalResult;
}

/*************************************
* MAIN
************************************/

void main(int argc, char **argv)
{
	printf("Started.. \n\n");
	outfile = fopen("C:\\NFC Production Test\\outfile.txt", "w");

	WORD ret = 0;
	DWORD ResID;
	HWND console = GetConsoleWindow();
	RECT r;
	uint64 TimeStart;
	uint64 TimeEnd;

	NFCTESTING_RESULT ResultAL[LISTENER_TEST_COUNT];
	NFCTESTING_RESULT ResultBL[LISTENER_TEST_COUNT];
	NFCTESTING_RESULT ResultAP[POLLER_TEST_COUNT+10];
	NFCTESTING_RESULT ResultVP[POLLER_TEST_COUNT+10];
	NFCTESTING_RESULT ResultBP[POLLER_TEST_COUNT];
	NFCTESTING_RESULT ResultFP[POLLER_TEST_COUNT];

	// For resonance frequency measurement
	DWORD dwResf, dwQFactor;

	char buffrep[128];
	char serialNo[128];

	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 800, 600, TRUE);

	SetDLLTimeOutValue(20);

	// Let's try to connect to the MP500 by TCP/IP:
	//ret = OpenCommunication("10779:192.168.107.62"); // set here the correct IP addr
	// or use USB:
	//ret = OpenCommunication("10779:10.84.8.180");
	ret = OpenCommunication("USB");

	if (ret == 0)
	{
		printf("Connected on the MP500\r\n");

		// The connection attempt has been successful.
		// First, we open the hardware resource
		ret = MPOS_GetResourceID(CPL0, &ResID);
		ret = MPOS_OpenResource(ResID, CPL0, OVERRIDE);
		ret = MPC_DefaultResonanceFrequencyConnector(CPL0, 1);

		printf("- Connect the NFC antenna (Tx/Rx and Analog IN) on the PT1\r\n");
		printf("- These test application will store the waveform files in the 'C:\\NFC Production Test\\' directory. You must create it if it does not exist on your system.\r\n");
		printf("-- Press [Enter] to continue\r\n");
		gets_s(buffrep);

		printf("- Remove the DUT from the test antenna to calibrate the resonant frequency measurement channel\r\n");
		printf("-- Press [Enter] to continue\r\n");
		gets_s(buffrep);

		printf("\nThis will take a few seconds...\r\n\n");

		ret = MPC_ResonanceFrequencyVS(CPL0, ON, 0, 0, 0, 0, 0, &dwResf, &dwQFactor);
		if (ret)
		{
			printf("ERROR: Unable to initiate the resonance frequency measurement \r\n");
		}

		printf("- Now place the DUT on the test antenna\r\n");
		printf("-- Press [Enter] to continue\r\n");
		gets_s(buffrep);

		

		// Init application global variables
		Init_NFCTestingData();

		if (ret == 0)
		{

			int TestNum = 0;
			int TestLoop = 1;

			do
			{
				printf("Enter unit serial number and press enter: ");
				gets_s(serialNo);

				printf("\r\n--- MP500 PT1 tests v3 ----------\r\n");
				printf("---- Listener test\r\n");
				printf("------ 1 - Listener type A\r\n");
				printf("------ 2 - Listener type B\r\n");
				printf("------ 3 - Listener type F\r\n");
				printf("------ 4 - Resonant Frequency\r\n");
				/*printf("------ 31 - Listener type A - Battery off\r\n");
				printf("------ 32 - Listener type B - Battery off\r\n");*/

				printf("\r\n---- Poller test\r\n");
				printf("------ 5 - Poller type Felica\r\n");
				printf("------ 6 - Poller type A\r\n");
				printf("------ 7 - Poller type B\r\n");		
				//printf("------ 6 - Poller type Vicinity\r\n");
				
				printf("\r\n---- Batch testing\r\n");
				printf("------ 20 Run all Card Mode tests\r\n");
				printf("------ 21 Run all Reader Mode tests\r\n");
				printf("------ 22 Run all tests\r\n\r\n");

				printf("Choose (test number + Enter) or (L<Loop Number>|<Test Number> + Enter) OR\r\n(0 + Enter) to exit : ");

				// Dummy values
				TestLoop = 1;
				TestNum = 99;

				gets_s(buffrep);

				if (sscanf(buffrep, "L%ld|%ld", &TestLoop, &TestNum) != 2)
				{
					sscanf(buffrep, "%ld", &TestNum);
				}

				if (TestNum != 0)
				{
					for (int idx = 0; idx < TestLoop; idx++)
					{
						if (TestNum != 4)
						{
							printf("Loop %ld...\r\n", idx + 1);
						}

						switch (TestNum)
						{
							case 1:
							{
									  memset(ResultAL, 0, sizeof(NFCTESTING_RESULT)*LISTENER_TEST_COUNT);
									  TimeStart = GetTimeMs64();
									  if (!TestListener(TYPE_A, ResultAL, false))
									  {
										  TimeEnd = GetTimeMs64();
										  printf("* Success\r\nListener A tests details : \r\n");
										  DisplayResults(ResultAL, LISTENER_TEST_COUNT);
										  printf("Time elapsed = %ldms\r\n", TimeEnd - TimeStart);
									  }
									  else
									  {
										  printf("*** Fail !\r\nListener A tests error details : \r\n");
										  DisplayResults(ResultAL, LISTENER_TEST_COUNT);
									  }

									  break;
							}


							case 2:
							{
									  memset(ResultBL, 0, sizeof(NFCTESTING_RESULT)*LISTENER_TEST_COUNT);
									  TimeStart = GetTimeMs64();
									  if (!TestListener(TYPE_B, ResultBL, false))
									  {
										  TimeEnd = GetTimeMs64();
										  printf("* Success\r\nListener B tests details : \r\n");
										  DisplayResults(ResultBL, LISTENER_TEST_COUNT);
										  printf("Time elapsed = %ldms\r\n", TimeEnd - TimeStart);
									  }
									  else
									  {
										  printf("*** Fail !\r\nListener B tests error details : \r\n");
										  DisplayResults(ResultBL, LISTENER_TEST_COUNT);
									  }

									  break;
									  break;
							}

							case 3:
							{
								
								memset(ResultBL, 0, sizeof(NFCTESTING_RESULT)*LISTENER_TEST_COUNT);
								TimeStart = GetTimeMs64();
								if (!TestListener(TYPE_FELICA, ResultBL, false))
								{
									TimeEnd = GetTimeMs64();
									printf("* Success\r\nListener F tests details : \r\n");
									DisplayResults(ResultBL, LISTENER_TEST_COUNT);
									printf("Time elapsed = %ldms\r\n", TimeEnd - TimeStart);
								}
								else
								{
									printf("*** Fail !\r\nListener F tests error details : \r\n");
									DisplayResults(ResultBL, LISTENER_TEST_COUNT);
								}

								break;
								break;
							}

							case 4:
							{

								printf("\n Measuring resonant frequency...\n\n");
								
									ret = MPC_ResonanceFrequencyVS(CPL0, OFF, 0, 5000, 0, 11000000, 19000000, &dwResf, &dwQFactor);
									if (ret)
									{
										printf("Unable to capture resonant frequency. Please try again.\n");
									}
									else
									{
										printf("Resonance frequency measured : %ld     Qfactor : %ld\r\n", dwResf, dwQFactor / 10);
									}
								

								break;
							}
								/*

							case 32:
							{
									  memset(ResultBL, 0, sizeof(NFCTESTING_RESULT)*LISTENER_TEST_COUNT);
									  TimeStart = GetTimeMs64();
									  if (!TestListener(TYPE_B, ResultBL, true))
									  {
										  TimeEnd = GetTimeMs64();
										  printf("* Success\r\nListener B tests details : \r\n");
										  DisplayResults(ResultBL, LISTENER_TEST_COUNT);
										  printf("Time elapsed = %ldms\r\n", TimeEnd - TimeStart);
									  }
									  else
									  {
										  printf("*** Fail !\r\nListener B tests error details : \r\n");
										  DisplayResults(ResultBL, LISTENER_TEST_COUNT);
									  }

									  break;
									  break;
							}*/

							case 5:
							{
                                #define TYPE_FELICA 5
								memset(ResultFP, 0, sizeof(NFCTESTING_RESULT)*POLLER_TEST_COUNT);
								TimeStart = GetTimeMs64();
								if (!TestPoller(TYPE_FELICA, ResultFP))
								{
									TimeEnd = GetTimeMs64();
									printf("* Success\r\nPoller F tests details : \r\n");
									DisplayResults(ResultFP, POLLER_TEST_COUNT);
									printf("Time elapsed = %ldms\r\n", TimeEnd - TimeStart);
								}
								else
								{
									printf("*** Fail !\r\nPoller F tests error details : \r\n");
									DisplayResults(ResultFP, POLLER_TEST_COUNT);
								}

								break;
							}

							case 6:
							{
									  
									  memset(ResultAP, 0, sizeof(NFCTESTING_RESULT)*POLLER_TEST_COUNT + 3);
									  TimeStart = GetTimeMs64();
									  if (!TestPoller(TYPE_A, ResultAP))
									  {
										  TimeEnd = GetTimeMs64();
										  printf("* Success\r\nPoller A tests details : \r\n");

										  DisplayResults(ResultAP, POLLER_TEST_COUNT+3);
										  printf("Time elapsed = %ldms\r\n", TimeEnd - TimeStart);
									  }
									  else
									  {
										  printf("*** Fail !\r\nPoller A tests error details : \r\n");
										  DisplayResults(ResultAP, POLLER_TEST_COUNT+3);
									  }

									  break;
							}

							case 7:
							{
									  memset(ResultBP, 0, sizeof(NFCTESTING_RESULT)*POLLER_TEST_COUNT);
									  TimeStart = GetTimeMs64();
									  if (!TestPoller(TYPE_B, ResultBP))
									  {
										  TimeEnd = GetTimeMs64();
										  printf("* Success\r\nPoller B tests details : \r\n");
										  DisplayResults(ResultBP, POLLER_TEST_COUNT);
										  printf("Time elapsed = %ldms\r\n", TimeEnd - TimeStart);
									  }
									  else
									  {
										  printf("*** Fail !\r\nPoller B tests error details : \r\n");
										  DisplayResults(ResultBP, POLLER_TEST_COUNT);
									  }

									  break;
							}

						/*	case 7:
							{
									  memset(ResultFP, 0, sizeof(NFCTESTING_RESULT)*POLLER_TEST_COUNT);
									  TimeStart = GetTimeMs64();
									  if (!TestPoller(TYPE_FELICA_212, ResultFP))
									  {
										  TimeEnd = GetTimeMs64();
										  printf("* Success\r\nPoller F tests details : \r\n");
										  DisplayResults(ResultFP, POLLER_TEST_COUNT);
										  printf("Time elapsed = %ldms\r\n", TimeEnd - TimeStart);
									  }
									  else
									  {
										  printf("*** Fail !\r\nPoller F tests error details : \r\n");
										  DisplayResults(ResultFP, POLLER_TEST_COUNT);
									  }

									  break;
							}*/

							/*case 6:
							{
									  memset(ResultVP, 0, sizeof(NFCTESTING_RESULT)*POLLER_TEST_COUNT+3);
									  TimeStart = GetTimeMs64();
									  if (!TestPoller(TYPE_VICINITY, ResultVP))
									  {
										  TimeEnd = GetTimeMs64();
										  printf("* Success\r\nPoller V tests details : \r\n");
										  DisplayResults(ResultVP, POLLER_TEST_COUNT+3);
										  printf("Time elapsed = %ldms\r\n", TimeEnd - TimeStart);
									  }
									  else
									  {
										  printf("*** Fail !\r\nPoller V tests error details : \r\n");
										  DisplayResults(ResultFP, POLLER_TEST_COUNT+3);
									  }

									  break;
							}*/

							case 20:
							{
								       printf("- Listener test results :\r\n\n");
								       fputs("- Listener test results :\r\n\n", outfile);

									   TimeStart = GetTimeMs64();

									   memset(ResultAL, 0, sizeof(NFCTESTING_RESULT)*LISTENER_TEST_COUNT);
									   if (!TestListener(TYPE_A, ResultAL, false))
									   {
										   printf("* Success\r\nListener A tests details : \r\n");
										   DisplayResults(ResultAL, LISTENER_TEST_COUNT);
									   }
									   else
									   {
										   printf("*** Fail !\r\nListener A tests error details : \r\n");
										   DisplayResults(ResultAL, LISTENER_TEST_COUNT);
									   }

									   memset(ResultBL, 0, sizeof(NFCTESTING_RESULT)*LISTENER_TEST_COUNT);
									   if (!TestListener(TYPE_B, ResultBL, false))
									   {
										   printf("* Success\r\nListener B tests details : \r\n");
										   DisplayResults(ResultBL, LISTENER_TEST_COUNT);
									   }

									   else
									   {
										   printf("*** Fail !\r\nListener B tests error details : \r\n");
										   DisplayResults(ResultBL, LISTENER_TEST_COUNT);
									   }

									   memset(ResultBL, 0, sizeof(NFCTESTING_RESULT)*LISTENER_TEST_COUNT);
									   if (!TestListener(TYPE_FELICA, ResultBL, false))
									   {
										   printf("* Success\r\nListener Felica tests details : \r\n");
										   DisplayResults(ResultBL, LISTENER_TEST_COUNT);
									   }

									   else
									   {
										   printf("*** Fail !\r\nListener Felica tests error details : \r\n");
										   DisplayResults(ResultBL, LISTENER_TEST_COUNT);
									   }

									   printf("\nPress Enter to run resonant frequency test...\n\n");
									   getchar();

									   printf("\nMeasuring resonant frequency...\n\n");

									   ret = MPC_ResonanceFrequencyVS(CPL0, OFF, 0, 5000, 0, 11000000, 19000000, &dwResf, &dwQFactor);
									   if (ret)
									   {
										   printf("Unable to capture resonant frequency. Please try again.\n");
									   }
									   else
									   {
										   printf("Resonance frequency measured : %ld     Qfactor : %ld\r\n\n", dwResf, dwQFactor / 10);

										   char res[80];
										   sprintf(res, "%.3f", dwResf);

										   fputs("-- Test Name: Resonant Frequency", outfile);
										   fputs(res, outfile);
										   fputs("\r\n", outfile);

									   }

									   TimeEnd = GetTimeMs64();
									  // printf("Time elapsed in batch = %ldms\r\n", TimeEnd - TimeStart);

									   break;
							}

							case 21:
							{
										printf("- Poller test results :\r\n\n");
										fputs("- Poller test results :\r\n\n", outfile);

									   TimeStart = GetTimeMs64();
									  
									   // Type A
									   memset(ResultAP, 0, sizeof(NFCTESTING_RESULT)*POLLER_TEST_COUNT + 3);
									  
									   if (!TestPoller(TYPE_A, ResultAP))
									   {
										   printf("* Success\r\nPoller A tests details : \r\n");
										   DisplayResults(ResultAP, POLLER_TEST_COUNT + 3);
									   }
									   else
									   {
										   printf("*** Fail !\r\nPoller A tests error details : \r\n");
										   DisplayResults(ResultAP, POLLER_TEST_COUNT);
									   }

									   // Type B
									   memset(ResultBP, 0, sizeof(NFCTESTING_RESULT)*POLLER_TEST_COUNT);
									   if (!TestPoller(TYPE_B, ResultBP))
									   {
										   printf("* Success\r\nPoller B tests details : \r\n");
										   DisplayResults(ResultBP, POLLER_TEST_COUNT);
									   }
									   else
									   {
										   printf("*** Fail !\r\nPoller B tests error details : \r\n");
										   DisplayResults(ResultBP, POLLER_TEST_COUNT);
									   }

									  // Type F
									  memset(ResultFP, 0, sizeof(NFCTESTING_RESULT)*POLLER_TEST_COUNT);
									  if (!TestPoller(TYPE_FELICA, ResultFP))
									  {
										  printf("* Success\r\nPoller F tests details : \r\n");
										  DisplayResults(ResultFP, POLLER_TEST_COUNT);
									  }
									  else
									  {
										  printf("*** Fail !\r\nPoller F tests error details : \r\n");
										  DisplayResults(ResultFP, POLLER_TEST_COUNT);
									  }

									  TimeEnd = GetTimeMs64();
									  printf("Time elapsed in batch = %ldms\r\n", TimeEnd - TimeStart);

									  break;
							}

							case 22:
							{
								fputs("S/N: ", outfile);
								fputs(serialNo, outfile);
								fputs("\r\n\n", outfile);

								printf("- Poller test results :\r\n\n");
								fputs("- Poller test results :\r\n\n", outfile);

								TimeStart = GetTimeMs64();

								// Type A
								memset(ResultAP, 0, sizeof(NFCTESTING_RESULT)*POLLER_TEST_COUNT + 3);

								if (!TestPoller(TYPE_A, ResultAP))
								{
									printf("* Success\r\nPoller A tests details : \r\n");
									DisplayResults(ResultAP, POLLER_TEST_COUNT + 3);
								}
								else
								{
									printf("*** Fail !\r\nPoller A tests error details : \r\n");
									DisplayResults(ResultAP, POLLER_TEST_COUNT);
								}

								// Type B
								memset(ResultBP, 0, sizeof(NFCTESTING_RESULT)*POLLER_TEST_COUNT);
								if (!TestPoller(TYPE_B, ResultBP))
								{
									printf("* Success\r\nPoller B tests details : \r\n");
									DisplayResults(ResultBP, POLLER_TEST_COUNT);
								}
								else
								{
									printf("*** Fail !\r\nPoller B tests error details : \r\n");
									DisplayResults(ResultBP, POLLER_TEST_COUNT);
								}

								// Type F
								memset(ResultFP, 0, sizeof(NFCTESTING_RESULT)*POLLER_TEST_COUNT);
								if (!TestPoller(TYPE_FELICA, ResultFP))
								{
									printf("* Success\r\nPoller F tests details : \r\n");
									DisplayResults(ResultFP, POLLER_TEST_COUNT);
								}
								else
								{
									printf("*** Fail !\r\nPoller F tests error details : \r\n");
									DisplayResults(ResultFP, POLLER_TEST_COUNT);
								}

								printf("- Listener test results :\r\n\n");
								fputs("- Listener test results :\r\n\n", outfile);

								for (int i = 0; i < 1; i++)
								{
									memset(ResultAL, 0, sizeof(NFCTESTING_RESULT)*LISTENER_TEST_COUNT);
									if (!TestListener(TYPE_A, ResultAL, false))
									{
										printf("* Success\r\nListener A tests details : \r\n");
										DisplayResults(ResultAL, LISTENER_TEST_COUNT);
									}
									else
									{
										printf("*** Fail !\r\nListener A tests error details : \r\n");
										DisplayResults(ResultAL, LISTENER_TEST_COUNT);
									}

									memset(ResultBL, 0, sizeof(NFCTESTING_RESULT)*LISTENER_TEST_COUNT);
									if (!TestListener(TYPE_B, ResultBL, false))
									{
										printf("* Success\r\nListener B tests details : \r\n");
										DisplayResults(ResultBL, LISTENER_TEST_COUNT);
									}

									else
									{
										printf("*** Fail !\r\nListener B tests error details : \r\n");
										DisplayResults(ResultBL, LISTENER_TEST_COUNT);
									}

									memset(ResultBL, 0, sizeof(NFCTESTING_RESULT)*LISTENER_TEST_COUNT);
									if (!TestListener(TYPE_FELICA, ResultBL, false))
									{
										printf("* Success\r\nListener Felica tests details : \r\n");
										DisplayResults(ResultBL, LISTENER_TEST_COUNT);
									}

									else
									{
										printf("*** Fail !\r\nListener Felica tests error details : \r\n");
										DisplayResults(ResultBL, LISTENER_TEST_COUNT);
									}

								}

								printf("\nPress Enter to run resonant frequency test...\n\n");
								getchar();

								printf("\nMeasuring resonant frequency...\n\n");

								ret = MPC_ResonanceFrequencyVS(CPL0, OFF, 0, 5000, 0, 11000000, 19000000, &dwResf, &dwQFactor);
								if (ret)
								{
									printf("Unable to capture resonant frequency. Please try again.\n");
								}
								else
								{
									printf("Resonance frequency measured : %ld     Qfactor : %ld\r\n\n", dwResf, dwQFactor / 10);

									char res[80];
									sprintf(res, "%d", dwResf);

									fputs("-- Test Name: Resonant Frequency", outfile);
									
									fputs("\r\n", outfile);

									
									fputs("-- Test value: ", outfile);
									fputs(res, outfile);
									fputs("\r\n\r\n", outfile);
								}

								TimeEnd = GetTimeMs64();
								//printf("Time elapsed in batch = %ldms\r\n", (TimeEnd - TimeStart));

								break;
							}

						}

					}

					// hit a key before leaving the application.
					printf("\nPress a Enter to continue...\n");

					fgets(buffrep, sizeof(buffrep), stdin);


				}
			} while (TestNum != 0);

			printf("End of tests\n");

			// Before leaving the application, application allocated resources must be
			// deallocated.
			ret = MPOS_CloseResource(ResID, CPL0);
			if (ret)
				printf("Unable to close the PT1 resource\n");


		}
		else
			printf("Unable to open the PT1 resource\n");

		// The communication must be closed on application end
		CloseCommunication();
	}
	else
		printf("Unable to connect to a MP500 (%d)\n", ret);

		fclose(outfile);
}

