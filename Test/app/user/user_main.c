#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#include "driver/gpio16.h"
#include "gpio.h"
#include "driver/uart.h"
#include "user_interface.h"
#include "spi_flash.h"
#include "espconn.h"
#include "user_config.h"
#include "smartconfig.h"


////uint8 *buf : 数据缓存  uint16 len : 数据⻓长度
//void uart0_tx_buffer(uint8 *buf, uint16 len);//通过 UART0 输出⽤用户数据
//
//
//////void *para : 指向数据结构 RcvMsgBuff 的指针
////void uart0_rx_intr_handler(void *para)//UART0 中断处理函数，⽤用户可在该函数内添加对接收到数据包的处理。
//


//LOCAL os_timer_t timer;//声明定时器timer
struct espconn PhoneConn;
esp_udp PhoneConnUdp;
bool temp ;
void user_rf_pre_init(void)
{

}

//void timer_callback()
//{
//	if(GPIO_INPUT_GET(GPIO_ID_PIN(0))==0)
//	{
//		espconn_sent(&PhoneConn, "按键发送测试", 12);//发送数据
//	}
//
//}

void ICACHE_FLASH_ATTR
user_set_softap_config(void)//设置AP的WIFI名称及密码
{

   struct softap_config config;
   os_memset(config.ssid, 0, 32);
   os_memset(config.password, 0, 64);

   os_memcpy(config.ssid, "Test123", 8);// SSID帐号
   os_memcpy(config.password, "123456789", 9);//密码
   config.authmode = AUTH_WPA_WPA2_PSK;
   config.ssid_len = 0;// or its actual length
   config.max_connection = 5; // 允许接入点数量

   wifi_softap_set_config(&config);
}

void output(int io){
	if(temp){
					GPIO_OUTPUT_SET(GPIO_ID_PIN(io), 0);//GPIO5输出低电平
					temp = false;
					}
					else{
					GPIO_OUTPUT_SET(GPIO_ID_PIN(io), 1);//GPIO5输出高电平
					temp =true;
					}
}

void ICACHE_FLASH_ATTR
udpclient_recv(void *arg, char *pdata, unsigned short len)
{

	if(pdata[0]=='0')
				{
			         output(0);
					uart0_sendStr("0");//发送数据
					espconn_sent(&PhoneConn,"0", 1);

				}
	if(pdata[0]=='1')
			{
		         output(1);
				uart0_sendStr("1");//发送数据
				espconn_sent(&PhoneConn,"1", 1);

			}
	if(pdata[0]=='2')
	  {
		 output(2);
		uart0_sendStr("2");//发送数据
		espconn_sent(&PhoneConn, "2", 1);
	   }
	if(pdata[0]=='3')
		{
		    output(3);
			//GPIO_OUTPUT_SET(GPIO_ID_PIN(14), 1);//GPIO14输出高电平
			uart0_sendStr("3");//发送数据
			espconn_sent(&PhoneConn, "3", 1);
		}
	if(pdata[0]=='4')
			{
		        output(4);
				//GPIO_OUTPUT_SET(GPIO_ID_PIN(14), 1);//GPIO14输出高电平
				uart0_sendStr("4");//发送数据
				espconn_sent(&PhoneConn, "4", 1);
			}
	if(pdata[0]=='5')
			{
		        output(5);
				//GPIO_OUTPUT_SET(GPIO_ID_PIN(14), 1);//GPIO14输出高电平
				uart0_sendStr("5");//发送数据
				espconn_sent(&PhoneConn, "5", 1);
			}
	if(pdata[0]=='6')
			{
		       output(6);
				//GPIO_OUTPUT_SET(GPIO_ID_PIN(14), 1);//GPIO14输出高电平
				uart0_sendStr("6");//发送数据
				espconn_sent(&PhoneConn, "6", 1);
			}
	if(pdata[0]=='7')
				{
		           output(7);
					//GPIO_OUTPUT_SET(GPIO_ID_PIN(14), 1);//GPIO14输出高电平
					uart0_sendStr("7");//发送数据
					espconn_sent(&PhoneConn, "7", 1);
				}
	if(pdata[0]=='8')
					{
		                output(8);
						//GPIO_OUTPUT_SET(GPIO_ID_PIN(14), 1);//GPIO14输出高电平
						uart0_sendStr("8");//发送数据
						espconn_sent(&PhoneConn, "8", 1);
					}
	if(pdata[0]=='9')
					{
		               output(9);
						//GPIO_OUTPUT_SET(GPIO_ID_PIN(14), 1);//GPIO14输出高电平
						uart0_sendStr("9");//发送数据
						espconn_sent(&PhoneConn, "9", 1);
					}

}

void user_init(void)
{
	uart_init( BIT_RATE_115200, BIT_RATE_115200 );///*UART 波特率初始化*/
	wifi_set_opmode(0x02);//设置模块为AP模式
	user_set_softap_config();//设置AP的WIFI名称及密码

	PhoneConn.type = ESPCONN_UDP;
	PhoneConn.proto.udp = &PhoneConnUdp;
	PhoneConn.proto.udp->local_port = 8000;//本地端口
	PhoneConn.proto.udp->remote_port = 8000;//远程端口
	PhoneConn.proto.udp->local_ip[0] = 255;
	PhoneConn.proto.udp->local_ip[1] = 255;
	PhoneConn.proto.udp->local_ip[2] = 255;
	PhoneConn.proto.udp->local_ip[3] = 255;

	PhoneConn.proto.udp->remote_ip[0] = 255;
	PhoneConn.proto.udp->remote_ip[1] = 255;
	PhoneConn.proto.udp->remote_ip[2] = 255;
	PhoneConn.proto.udp->remote_ip[3] = 255;

	espconn_regist_recvcb(&PhoneConn, udpclient_recv); // 注册一个UDP数据包接收回调
	espconn_create(&PhoneConn);//建立 UDP 传输
//	os_timer_disarm(&timer);//关闭定时器，相当于清零计时器计数
//	os_timer_setfn(&timer, (os_timer_func_t *)timer_callback, NULL);//初始化定时器
//	os_timer_arm(&timer, 100, 1);//开始定时器计数,1000毫秒后，会调用前面的callback函数 （后面的0表示只运行一次 为1表示循环运行）
}
