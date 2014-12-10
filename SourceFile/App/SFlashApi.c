#include "system.h"
#include "app.h"







void Flash_WriteInfo(u8 *pDataBuf,u32 WriteAddr, u16 nDataNum) 
{

	//以后添加保护
	System.Device.W25q64.SPI_Flash_Write(pDataBuf,WriteAddr,nDataNum);


/*
	if(dwAddr>=FLASH_CAPICITY)
	   return ;	
    	store_all_int();
    	SPIflash_Write(dwAddr,pDataBuf,nDataNum);
	restore_all_int();	

*/
}

void Flash_ReadInfo(u8 *pDataBuf,u32 ReadAddr,u16 nDataNum)   
{

	System.Device.W25q64.SPI_Flash_Read(pDataBuf,ReadAddr,nDataNum);
	
	
/*
	if(ADDR>=FLASH_CAPICITY)
	{
	   memset(buff_addr,0xff,FLASH_SECTION_SIZE);	   
	   return ;
	} 
    	store_all_int();
    	SPIflash_Read(ADDR,buff_addr,FLASH_SECTION_SIZE);
	restore_all_int();	
*/
}






