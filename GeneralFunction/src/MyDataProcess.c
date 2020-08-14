/** 
  * @brief  将float型数据有精度丢失地截成2个字节，放在buff中
 
void EncodeFloatDataLost(float* f, unsigned char* buff)
{
	buff[0] = (u8)(((s16)*f)&0x00ff);
	buff[1] = (u8)(((s16)*f)>>8&0x00ff);
}
  */
	
/** 
  * @brief  将从buff开始的2个字节位拼接转换成浮点数
  
void DecodeFloatDataLost(float* f, unsigned char* buff)
{
	*f=(double)((s16)((u16)(buff[1]&0x00ff)<<8)|buff[0]);
}
	*/ 
	
/** 
  * @brief  将float型数据无精度丢失地截成4个字节，放在buff中
   
void EncodeFloatData(float* f, unsigned char* buff)
{
	*(float*)buff = *f;
}
	*/
	
/** 
  * @brief   将从buff开始的4个字节位拼接转换成float
   
void DecodeFloatData(float* f, unsigned char* buff)
{
	*f=*(float *)buff;
}
	*/
	
/** 
  * @brief   将16位有符号数据转换成2个字节放在buff中
   
void EncodeS16Data(s16* f, unsigned char* buff)
{
		*(signed short int*)buff = *f;
}
	*/
	
/** 
  * @brief   将buff中的2个字节拼接为s16
   
void DecodeS16Data(s16* f, unsigned char* buff)
{
	*f = *(signed short int*)buff;
}
	*/

/** 
  * @brief   将16位无符号数据转换成2个字节放在buff中
   
void EncodeU16Data(u16* f, unsigned char* buff)
{
		*(u16*)buff = *f;
}
	*/

/** 
  * @brief   将buff中的2个字节拼接为u16
   
void DecodeU16Data(u16* f, unsigned char* buff)
{
	*f = *(u16*)buff;
}
	*/

/** 
  * @brief        将32位有符号数据转换成4个字节放在buff中
   
void EncodeS32Data(s32* f, unsigned char* buff)
{
		*(s32*)buff = *f;
}
	*/

/** 
  * @brief        将buff中的4个字节拼接为s32 
   
void DecodeS32Data(s32* f, unsigned char* buff)
{
		*f = *(s32 *)buff;
}
	*/
