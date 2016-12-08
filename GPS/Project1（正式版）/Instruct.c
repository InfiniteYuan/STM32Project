#include "GPS.h"
unsigned char DH_id_sep[32];//ȫ�ֱ������飬��ദ��32����������
unsigned char gps_sum = 0;
unsigned char Real_Process_Enabled = 0x01;

struct _GPS_Real_buf GPS_Real_buf;
struct _GPS_Information GPS_Information;

void USART1_IRQHandler(void){
		return;
}

void USART2_IRQHandler(void)
{
  unsigned char Recv;
  if ( USART_GetITStatus(USART2,USART_IT_RXNE ) != RESET ){
    Recv = USART_ReceiveData(USART2);  
    if ( Recv == '$' ){
      GPS_Real_buf.rx_pc = 0;
    }
    else{
      if (GPS_Real_buf.rx_pc < sizeof(GPS_Real_buf.data) - 1 ){
        GPS_Real_buf.rx_pc++;
      }
    }
    GPS_Real_buf.data[GPS_Real_buf.rx_pc] = Recv;
    if ( Recv == '\r' ) {  //��ʼ����GPS����
      if ( Real_Process_Enabled == Valid ){
        if ( Calc_GPS_Sum( GPS_Real_buf.data ) == Valid ){
          Creat_DH_Index( GPS_Real_buf.data );
          Real_GPS_Command_Process();
        }
      }
    }
  }

  if ( USART_GetFlagStatus( USART2, USART_FLAG_ORE ) == SET ) {//����ж�
    USART_ClearFlag( USART2, USART_FLAG_ORE );   //�����λ
    USART_ReceiveData( USART2 );                 //��DR
  }
}
/***************************************************
**Ѱ�����ж��ŵ�λ�ã���������
**����GPS���ݵĻ�����
**����ȫ�ֱ��������еĶ���������ԭGPS�����еĶ��Ž��ᱻ0x00�����\0
****************************************************/
void Creat_DH_Index( char* buffer )
{
	unsigned short i, len;
	unsigned char idj;

	memset( DH_id_sep, 0, sizeof( DH_id_sep ) );
	len = strlen( buffer );
	for ( i = 0, idj = 0; i < len; i++ ){
		if ( buffer[i] == ',' ){
			DH_id_sep[idj] = i;
			idj++;
			buffer[i] = 0x00;
		}
	}
}
/**************************************************
**����GPS���ݵ�N��������ƫ��
**����������Ľ���GPS���ݻ�����
**���ص�N��������֮�����Ϣ����Ҫ*buffer��Ч������������ſ���ִ��
***************************************************/
char* Real_Process_DH( char* buffer, unsigned char num )
{
	if ( num < 1 )
		return  &buffer[0];
	return  &buffer[ DH_id_sep[num - 1] + 1];
}
/***************************************************
**�������ò������������ݲ�����GPS���ݽṹ��
**
**
***************************************************/
void Real_GPS_Command_Process( void )
{
  char* temp;
  unsigned char i, j, zhen;

	if ( strstr( GPS_Real_buf.data, "GGA" ) ){//$GPGGA,112118.000,3743.5044,N,11540.5393,E,1,06,1.6,15.3,M,-9.1,M,,0000*7E
		GPS_Information.Use_EPH_Sum = atof( Real_Process_DH( GPS_Real_buf.data, 7 ) ); //��7������Ϊ����ʹ������
		GPS_Information.MSL_Altitude = atof( Real_Process_DH( GPS_Real_buf.data, 9 ) ); //��9������Ϊ���θ߶�
		
//		printf("Use_EPH_Sum=%d\r\n", GPS_Information.Use_EPH_Sum);
//		printf("MSL_Altitude=%lf\r\n", GPS_Information.MSL_Altitude);
		return;
	}

	if ( strstr( GPS_Real_buf.data, "GSA" ) ) { //$GPGSA,A,3,28,17,11,09,08,07,,,,,,,3.4,1.6,3.0*3B
		temp = Real_Process_DH( GPS_Real_buf.data, 2 ); //��2������Ϊ��λģʽ
		if ( ( *temp == '2' ) || ( *temp == '3' ) )
			GPS_Information.Locate_Mode = *temp;
		else
			GPS_Information.Locate_Mode = Invalid;
		for ( i = 0; i < 12; i++ ){ //�ܹ����Ϊ12������
			GPS_Information.User_EPH[i] = atof( Real_Process_DH( GPS_Real_buf.data, i + 3 ) ); //�ӵ�3��������ʼΪ��ʹ�õ����Ǳ��
		}
		GPS_Information.PDOP = atof( Real_Process_DH( GPS_Real_buf.data, 15 ) ); //��15������Ϊλ�þ���
		GPS_Information.HDOP = atof( Real_Process_DH( GPS_Real_buf.data, 16 ) ); //��16������Ϊˮƽ����
		GPS_Information.VDOP = atof( Real_Process_DH( GPS_Real_buf.data, 17 ) ); //��17������Ϊ��ֱ����
		
//		printf("PDOP=%lf\r\n", GPS_Information.PDOP);
//		printf("HDOP=%lf\r\n", GPS_Information.HDOP);
//		printf("VDOP=%lf\r\n", GPS_Information.VDOP);
		return;
	}

  if ( strstr( GPS_Real_buf.data, "RMC" ) ){//$GPRMC,112118.000,A,3743.5044,N,11540.5393,E,0.25,198.81,130613,,,A*67
    temp = Real_Process_DH( GPS_Real_buf.data, 1 ); //��һ������Ϊʱ��
    if ( *temp != 0 )
      memcpy( GPS_Information.UTC_Time, temp, 6 );
    if ( *( Real_Process_DH( GPS_Real_buf.data, 2 ) ) == 'A' ) {//�ڶ�������
      GPS_Information.Real_Locate = Valid; //ʵʱ������Ч
      GPS_Information.Located = Valid;
    }
    else{
      GPS_Information.Real_Locate = Invalid;  //ʵʱ������Ч
    }
    temp = Real_Process_DH( GPS_Real_buf.data, 3 ); //����������Ϊγ��
    if ( ( *temp >= 0x31 ) && ( *temp <= 0x39 ) ){
      memcpy( GPS_Information.Latitude, temp, 9 );
      GPS_Information.Latitude[9] = 0;
    }
    else{
      GPS_Information.Latitude[0] = '0';
      GPS_Information.Latitude[1] = 0;
    }
    GPS_Information.NS_Indicator = *( Real_Process_DH( GPS_Real_buf.data, 4 ) ); //���ĸ�����Ϊ�ϱ�
		
//		printf("NS_Indicator=%c\r\n", GPS_Information.NS_Indicator);
		
    temp = Real_Process_DH( GPS_Real_buf.data, 5 ); //����
    if ( ( *temp >= 0x31 ) && ( *temp <= 0x39 ) ){
      memcpy( GPS_Information.Longitude, temp, 10 );
      GPS_Information.Longitude[10] = 0;
    }
    else{
      GPS_Information.Longitude[0] = '0';
      GPS_Information.Longitude[1] = 0;
    }
    GPS_Information.EW_Indicator = *( Real_Process_DH( GPS_Real_buf.data, 6 ) ); //?6����
    GPS_Information.Speed = atof( Real_Process_DH( GPS_Real_buf.data, 7 ) ); //?7�ٶ�
    GPS_Information.Course = atof( Real_Process_DH( GPS_Real_buf.data, 8 ) ); //?8����
		
//		printf("EW_Indicator=%c\r\n", GPS_Information.EW_Indicator);
//		printf("Speed=%lf\r\n", GPS_Information.Speed);
//		printf("Course=%lf\r\n", GPS_Information.Course);
    temp = Real_Process_DH( GPS_Real_buf.data, 9 ); //?9����
    if ( *temp != 0 ){
      memcpy( GPS_Information.UTC_Date, temp, 6 );
    }
    return;
  }
	
	if ( strstr( GPS_Real_buf.data, "GSV" ) ){//$GPGSV,3,1,11,28,73,321,32,17,39,289,43,11,38,053,17,09,37,250,41*78
		zhen = atof( Real_Process_DH( GPS_Real_buf.data, 2 ) ); //��ǰ֡��
		if ( ( zhen <= 3 ) && ( zhen != 0 ) ){
			for ( i = ( zhen - 1 ) * 4, j = 4; i < zhen * 4; i++ ){
				GPS_Information.EPH_State[i][0] = atof( Real_Process_DH( GPS_Real_buf.data, j++ ) ); //�Ǻ�
				GPS_Information.EPH_State[i][1] = atof( Real_Process_DH( GPS_Real_buf.data, j++ ) ); //����
				GPS_Information.EPH_State[i][2] = atof( Real_Process_DH( GPS_Real_buf.data, j++ ) ); //��λ��
				GPS_Information.EPH_State[i][3] = atof( Real_Process_DH( GPS_Real_buf.data, j++ ) ); //��ɤ��
			}
		}
		return;
	}
}
/******************************************************
**����GPS��У���
**������ɵ�GPS����
**
******************************************************/
unsigned char Calc_GPS_Sum( const char* Buffer )
{
  unsigned char i, j, k, sum;
  sum = 0;

  for ( i = 1; i < 255; i++ ) {//i?1��1��ʼ��S��ʼ��
    if ( ( Buffer[i] != '*' ) && ( Buffer[i] != 0x00 ) ) {//������
      sum ^= Buffer[i];//GPSУ��
    }
    else{
      break;
    }
  }
  j = Buffer[i + 1];//����������λ
  k = Buffer[i + 2];
  if ( isalpha( j ) ) {
    if ( isupper( j ) ) {
      j -= 0x37;
    }
    else{
      j -= 0x57;
    }
  }
  else{
    if ( ( j >= 0x30 ) && ( j <= 0x39 ) ){
      j -= 0x30;
    }
  }

  if ( isalpha( k ) ) {
    if ( isupper( k ) ) {
      k -= 0x37;
    }
    else{
      k -= 0x57;
    }
  }
  else{
    if ( ( k >= 0x30 ) && ( k <= 0x39 ) ){
      k -= 0x30;
    }
  }

  j = ( j << 4 ) + k; 
  gps_sum = j;

  if ( sum == j ){
		return Valid; 
  }
  else{
    return Invalid; 
  }
}
