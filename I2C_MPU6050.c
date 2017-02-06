/******************** (C) COPYRIGHT 2012  **************************

 * ????:-----------------
 *          |                 |
 *          |  PB6-I2C1_SCL		|
 *          |  PB7-I2C1_SDA   |
 *          |                 |
 *           -----------------
 * ???  :ST3.5.0
 * ??    : Orange 
**********************************************************************************/
#include "I2C_MPU6050.h"

/*
 * ???:I2C_GPIO_Config
 * ??  :I2C1 I/O??
 * ??  :?
 * ??  :?
 * ??  :????
 */
static void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}


/*
 * ???:I2C_Mode_Config
 * ??  :I2C ??????
 * ??  :?
 * ??  :?
 * ??  :????
 */
static void I2C_Mode_Config(void)
{
	I2C_InitTypeDef I2C_InitStructure; 
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C ; 
	//I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; 
	//I2C_InitStructure.I2C_OwnAddress1 = SlaveAddress; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; 
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; 
	I2C_InitStructure.I2C_ClockSpeed = 50000; 
	I2C_Cmd(I2C1,ENABLE); 
	I2C_Init(I2C1, &I2C_InitStructure);	   
	I2C_AcknowledgeConfig(I2C1, ENABLE);   
}
/***************************************************************************************/
/*
 * ???:I2C_MMA_Init
 * ??  :I2C ??(MMA7455)???
 * ??  :?
 * ??  :?
 * ??  :????
 */
void I2C_MPU6050_Init(void)
{	   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE ); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);  

	//GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
	
 	I2C_GPIO_Config();

	I2C_Mode_Config();
}  

/*
 * ???:I2C_ByteWrite
 * ??  :??????I2C??????
 * ??  :REG_Address ?????IIC???????? 
 *         REG_data ??????
 * ??  :?
 * ??  :?
 * ??  :????
 */	
void I2C_ByteWrite(uint8_t REG_Address,uint8_t REG_data)
{

I2C_GenerateSTART(I2C1,ENABLE);

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

I2C_Send7bitAddress(I2C1,SlaveAddress,I2C_Direction_Transmitter);

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

I2C_SendData(I2C1,REG_Address);

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

I2C_SendData(I2C1,REG_data);

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

I2C_GenerateSTOP(I2C1,ENABLE);

}


/*
 * ???:I2C_ByteRead
 * ??  :?IIC????????????
 * ??  :REG_Address ??????????? 
 * ??  :?
 * ??  :?
 * ??  :???? 
*/
uint8_t I2C_ByteRead(uint8_t REG_Address)
{
uint8_t REG_data;

while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));

I2C_GenerateSTART(I2C1,ENABLE);//????

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

I2C_Send7bitAddress(I2C1,SlaveAddress,I2C_Direction_Transmitter);//??????+???

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//

I2C_Cmd(I2C1,ENABLE);

I2C_SendData(I2C1,REG_Address);//????????,?0??

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

I2C_GenerateSTART(I2C1,ENABLE);//????

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

I2C_Send7bitAddress(I2C1,SlaveAddress,I2C_Direction_Receiver);//??????+???

while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

I2C_AcknowledgeConfig(I2C1,DISABLE);

I2C_GenerateSTOP(I2C1,ENABLE);

while(!(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)));

REG_data=I2C_ReceiveData(I2C1);//???????

return REG_data;

}

/*
 * ???:void InitMPU6050(void)
 * ??  :???Mpu6050
 * ??  :?
 * ??  :?
 * ??  :????
 */
void InitMPU6050(void)
{
	I2C_ByteWrite(PWR_MGMT_1,0x00);//??????
	I2C_ByteWrite(SMPLRT_DIV,0x07);
	I2C_ByteWrite(CONFIG,0x06);
	I2C_ByteWrite(GYRO_CONFIG,0x18);
	I2C_ByteWrite(ACCEL_CONFIG,0x01);

}


/*
 * ???:GetData_a
 * ??  :???????16???,???????????,????????g
 * ??  :REG_Address ?????
 * ??  :???????
 * ??  :????
 */

float GetData_a(unsigned char REG_Address)
{
	char H,L;
	int m;
	float res;
	H=I2C_ByteRead(REG_Address);
	L=I2C_ByteRead(REG_Address+1);
	m=(H<<8)+L;   //????
	if( m>32767 ){//?????
		res=(float)(m-65535)/16384;
	}
	else{//?????
		res=(float)m/16384;
	}
	return res;
}

// /*
//  * ???:directionJudge
//  * ??  :????????,?????????
//  * 0:??,1:??,2:??,3:??,4:??
//  * ??  :3?????
//  * ??  :???????
//  * ??  :????
//  */

int directionJudge(void)
{
		float ax=GetData_a(ACCEL_XOUT_H);
		float ay=GetData_a(ACCEL_YOUT_H);

		float anglex=asin(ax)*180/3.14;
		float angley=asin(ay)*180/3.14;

		int x=0,y=0;
		if(anglex>threshold_x) x=1;
		else if(anglex<-threshold_x) x=-1;
		if(angley>threshold_y) y=1;
		else if(angley<-threshold_y) y=-1;	

		if(x==0 && y==1) return 1;
		else if(x==0 && y==-1) return 2;
		else if(y==0 && x==1) return 3;
		else if(y==0 && x==-1) return 4;
		else return 0;
}
/******************* (C) COPYRIGHT 2012  *****END OF FILE************/









