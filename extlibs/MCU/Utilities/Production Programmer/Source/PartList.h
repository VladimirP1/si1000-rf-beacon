#ifndef _PART_LIST_H
#define _PART_LIST_H

/////////////////////////////////////////////////////////////////////////////
// Structs
/////////////////////////////////////////////////////////////////////////////

struct MCU_Device
{
	CString		partNumber;
	UINT		wLockAddr;
	UINT		rLockAddr;
	UINT		numBanks;		// Number of banks excluding the common bank
								// Each bank is 32KB
								// I.E. The F120 has 128KB of flash in 1 common bank + 3 banks
								// I.E. The F584 has 96KB of flash in 1 common bank + 2 banks

	MCU_Device(CString partNum, UINT wLock, UINT rLock, UINT banks = 0)
	{
		partNumber	= partNum;
		wLockAddr	= wLock;
		rLockAddr	= rLock;
		numBanks	= banks;
	}
};

/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////

#define PARTLISTSIZE							528

/////////////////////////////////////////////////////////////////////////////
// External Globals
/////////////////////////////////////////////////////////////////////////////

extern const MCU_Device PartList[PARTLISTSIZE];


#endif // _PART_LIST_H