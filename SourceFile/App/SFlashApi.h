#ifndef		_SFLASHAPI_H_
#define		_SFLASHAPI_H_


void LockFlashBlockErase(u32 a);
void LockFlashErase(u32 a);
//void Flash_WriteInfo(u8 *pDataBuf,int dwAddr, int nDataNum);
//void Flash_ReadInfo(u32 *buff_addr,int ADDR,int FLASH_SECTION_SIZE);

 void FlashEraseRecordSect(u32 lBlock);
 void FlashEraseDataSect(u32 lBlock);

//#define	copyDataFromFlash( pDataBuf,nOffset,nDataNum) 	Flash_ReadInfo(pDataBuf,nOffset,nDataNum)
//#define	copyDataToFlash(pDataBuf,nOffset,nDataNum) 		Flash_WriteInfo(pDataBuf,nOffset,nDataNum)


void Flash_ReadInfo(u8 *pDataBuf,u32 ReadAddr,u16 nDataNum);
void Flash_WriteInfo(u8 *pDataBuf,u32 WriteAddr, u16 nDataNum);


#endif

