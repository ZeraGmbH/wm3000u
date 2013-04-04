// header datei für en61850 status info

#ifndef EN61850_H
#define EN61850_H

enum ETHErrorType {    savPdu = 0x00000001, // asn tag errors
		        noASDU = 0x00000002,
		        seqASDU = 0x00000004,
		        seqASDUn = 0x00000008,	
		        svID = 0x00000010,
		        smpCnt = 0x00000020,
		        confRev = 0x00000040,
		        smpSync = 0x00000080,
		        seqData = 0x00000100,
		        
		        macSyncLost = 0x00000200,  // decoder error
		        dataSyncLost = 0x00000400,
                                    ASDUnavail = 0x00000800,
		        ETHMacAdressError = 0x00001000,
		        ETHPriorityTaggedError = 0x00002000,
		        ETHHeaderAppIdError = 0x00004000,
			
		        FifoOverflow = 0x00010000, // mac errors
		        CRCError = 0x00020000,
		        AlignError = 0x00040000,
		        PhysicalLayer = 0x00080000,
		        ReceiveWDogTimeout = 0x0100000,
		        LateCollisionSeen = 0x00200000,
		        RuntFrame = 0x00800000	
};


struct cEN61850Info 
{
    quint32 ByteCount[2]; // 2^64 counter
    quint32 ETHErrors; // alle fehler bits
    quint32 SyncLostCount; // 2^32 counter
};

#endif
