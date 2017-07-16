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
LOCAL os_timer_t timer;//声明定时器timer
//LOCAL os_timer_t timer1;//声明定时器timer1

//char IOZT=1;//定义脸变量 用来存储IO执行的状态 假设0=亮 1=灭
//char IOZT1=1;//定义脸变量 用来存储IO执行的状态 假设0=亮 1=灭

struct espconn PhoneConn;
esp_udp PhoneConnUdp;

void user_rf_pre_init(void)
{

}

void timer_callback()
{
//	if(IOZT==1)
//	{
//		GPIO_OUTPUT_SET(GPIO_ID_PIN(14), 0);//GPIO14输出低电平
//		IOZT=0;
//	}
//	else
//	{
//		GPIO_OUTPUT_SET(GPIO_ID_PIN(14), 1);//GPIO14输出高电平
//		IOZT=1;
//	}
//	if(GPIO_INPUT_GET(GPIO_ID_PIN(0))==0)
//	{
//		GPIO_OUTPUT_SET(GPIO_ID_PIN(14), 0);//GPIO14输出低电平
//	}
//	else
//	{
//		GPIO_OUTPUT_SET(GPIO_ID_PIN(14), 1);//GPIO14输出高电平
//	}

	if(GPIO_INPUT_GET(GPIO_ID_PIN(0))==0)
	{
		espconn_sent(&PhoneConn, "按键发送测试", 12);//发送数据
	}

}

//void timer_callback1()
//{
//	if(IOZT1==1)
//	{
//		GPIO_OUTPUT_SET(GPIO_ID_PIN(4), 0);//GPIO14输出低电平
//		IOZT1=0;
//	}
//	else
//	{
//		GPIO_OUTPUT_SET(GPIO_ID_PIN(4), 1);//GPIO14输出高电平
//		IOZT1=1;
//	}
//}
void ICACHE_FLASH_ATTR
user_set_softap_config(void)//设置AP的WIFI名称及密码
{
   struct softap_config config;
   os_memset(config.ssid, 0, 32);
   os_memset(config.password, 0, 64);

   os_memcpy(config.ssid, "ESPESP234", 9);// SSID帐号
   os_memcpy(config.password, "88888888", 8);//密码
   config.authmode = AUTH_WPA_WPA2_PSK;
   config.ssid_len = 0;// or its actual length
   config.max_connection = 5; // 允许接入点数量

   wifi_softap_set_config(&config);
}

void ICACHE_FLASH_ATTR
udpclient_recv(void *arg, char *pdata, unsigned short len)
{
	if(pdata[0]=='L'&&pdata[1]=='E'&&pdata[2]=='D'&&pdata[3]=='='&&pdata[4]=='0')
	{
		GPIO_OUTPUT_SET(GPIO_ID_PIN(14), 0);//GPIO14输出低电平
		espconn_sent(&PhoneConn, "打开LED成功！", 13);//发送数据
	}

	if(pdata[0]=='L'&&pdata[1]=='E'&&pdata[2]=='D'&&pdata[3]=='='&&pdata[4]=='1')
	{
		GPIO_OUTPUT_SET(GPIO_ID_PIN(14), 1);//GPIO14输出高电平
		espconn_sent(&PhoneConn, "关闭LED成功！", 13);//发送数据
	}
}

void user_init(void)
{
	wifi_set_opmode(0x02);//设置模块为AP模式
	user_set_softap_config();//设置AP的WIFI名称及密码

	PhoneConn.type = ESPCONN_UDP;
	PhoneConn.proto.udp = &PhoneConnUdp;
	PhoneConn.proto.udp->local_port = 8888;//本地端口
	PhoneConn.proto.udp->remote_port = 8888;//远程端口
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

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);

	os_timer_disarm(&timer);//关闭定时器，相当于清零计时器计数
	os_timer_setfn(&timer, (os_timer_func_t *)timer_callback, NULL);//初始化定时器
	os_timer_arm(&timer, 100, 1);//开始定时器计数,1000毫秒后，会调用前面的callback函数 （后面的0表示只运行一次 为1表示循环运行）

//	os_timer_disarm(&timer1);//关闭定时器，相当于清零计时器计数
//	os_timer_setfn(&timer1, (os_timer_func_t *)timer_callback1, NULL);//初始化定时器
//	os_timer_arm(&timer1, 500, 1);//开始定时器计数,1000毫秒后，会调用前面的callback函数 （后面的0表示只运行一次 为1表示循环运行）

}
