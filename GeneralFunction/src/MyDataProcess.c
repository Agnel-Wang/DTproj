/** 
  * @brief  ��float�������о��ȶ�ʧ�ؽس�2���ֽڣ�����buff��
 
void EncodeFloatDataLost(float* f, unsigned char* buff)
{
	buff[0] = (u8)(((s16)*f)&0x00ff);
	buff[1] = (u8)(((s16)*f)>>8&0x00ff);
}
  */
	
/** 
  * @brief  ����buff��ʼ��2���ֽ�λƴ��ת���ɸ�����
  
void DecodeFloatDataLost(float* f, unsigned char* buff)
{
	*f=(double)((s16)((u16)(buff[1]&0x00ff)<<8)|buff[0]);
}
	*/ 
	
/** 
  * @brief  ��float�������޾��ȶ�ʧ�ؽس�4���ֽڣ�����buff��
   
void EncodeFloatData(float* f, unsigned char* buff)
{
	*(float*)buff = *f;
}
	*/
	
/** 
  * @brief   ����buff��ʼ��4���ֽ�λƴ��ת����float
   
void DecodeFloatData(float* f, unsigned char* buff)
{
	*f=*(float *)buff;
}
	*/
	
/** 
  * @brief   ��16λ�з�������ת����2���ֽڷ���buff��
   
void EncodeS16Data(s16* f, unsigned char* buff)
{
		*(signed short int*)buff = *f;
}
	*/
	
/** 
  * @brief   ��buff�е�2���ֽ�ƴ��Ϊs16
   
void DecodeS16Data(s16* f, unsigned char* buff)
{
	*f = *(signed short int*)buff;
}
	*/

/** 
  * @brief   ��16λ�޷�������ת����2���ֽڷ���buff��
   
void EncodeU16Data(u16* f, unsigned char* buff)
{
		*(u16*)buff = *f;
}
	*/

/** 
  * @brief   ��buff�е�2���ֽ�ƴ��Ϊu16
   
void DecodeU16Data(u16* f, unsigned char* buff)
{
	*f = *(u16*)buff;
}
	*/

/** 
  * @brief        ��32λ�з�������ת����4���ֽڷ���buff��
   
void EncodeS32Data(s32* f, unsigned char* buff)
{
		*(s32*)buff = *f;
}
	*/

/** 
  * @brief        ��buff�е�4���ֽ�ƴ��Ϊs32 
   
void DecodeS32Data(s32* f, unsigned char* buff)
{
		*f = *(s32 *)buff;
}
	*/
