#include <reg52.h>
#include <intrins.h>

#define uchar  unsigned char 
#define uint   unsigned int

typedef unsigned char BYTE;
typedef unsigned int WORD;

/************辅助全局变量块*****************/ 
char t,k,buf[6],buf_cnt;
bit  sec,flag,send_flag;			/*sec：秒信号；flag：串行口接收到命令标志；send_flag:回复数据标志*/
/************辅助全局变量块*****************/ 

/*************舵机变量块*******************/ 

//舵机控制
/*sbit dj1=P1^4;
sbit dj2=P1^5;
sbit dj3=P1^6;
sbit dj4=P1^7;
uint timer1;*/
//uchar num1_dj,num2_dj,num3_dj,unum4_dj;//控制每个舵机角度

/*************舵机变量块*******************/ 

/*************壁障与寻迹变量块*******************/ 

sbit d1= P0^1;//避障
uchar num1_d1,num2_d1;
//sbit d2= P1^7;

/*************壁障与寻迹变量块*******************/ 


/*************DHT11变量块*******************/ 
sbit io=P3^2;    			//DHT11数据端接单片机的P3^2口
typedef bit BOOL;			//此声明一个布尔型变量即真或假
uchar data_byte; 
uchar RH,RL,TH,TL; 	 		//TH保存温度，RH保存湿度

/*************DHT11变量块*******************/  

/*************L298N变量块*******************/  
sbit IN1 =P1^0;//右
sbit IN2 =P1^1;
sbit IN3 =P1^2;//左
sbit IN4 =P1^3;
uchar bufn,n;
/*************L298N变量块*******************/  


//*******************************延时函数******************************************* 
void delay(uchar ms)
{ 
	uchar i; 
	while(ms--)                    
	for(i=0;i<110;i++); 

}
void delayms(uint k)
{
	uint i,j;
	for( i=k;i>0;i--)

		for( j=110;j>0;j--);
}

void delay1()				//一个for循环大概需要8个多机器周期一个机器周期为1us晶振为12MHz也就是说本函数延时8us多此延时函数必须德稍微精确一点 
{ 
	uchar i; 
	for(i=0;i<1;i++); 

}

void delay1ms(char t)
{
	int i,j;
	for(i=0;i<t;i++)
		for(j=0;j<1000;j++);
}
//***************************延时函数*****************************************


//**************************DHT11模块*****************************************
void start()//开始信号
{ 
	io=1; 
	delay1(); 
	io=0; 
	delay(25);			// 主机把总线拉低必须大于18ms保证DHT11能检测到起始信号 
	io=1;   			//发送开始信号结束后拉高电平延时20-40us 
	delay1();			//以下三个延时函数差不多为24us符合要求 
	delay1(); 
	delay1(); 
} 

uchar receive_byte()//接收一个字节// 
{ 
	uchar i,temp; 
	for(i=0;i<8;i++)//接收8bit的数据 
	{ 
		while(!io);			//等待50us的低电平开始信号结束 
		delay1();			//开始信号结束之后延时26us-28us以下三个延时函数 
		delay1(); 
		delay1(); 
		temp=0;				//时间为26us-28us表示接收的为数据'0' 
		if(io==1) 
		temp=1; 			//如果26us-28us之后还为高电平则表示接收的数据为'1' 
		while(io);			//等待数据信号高电平'0'为26us-28us'1'为70us 
		data_byte<<=1;		//接收的数据为高位在前右移 
		data_byte|=temp;
	} 
	return data_byte; 
} 

void receive()	//接收数据
{ 
	uchar T_H,T_L,R_H,R_L,check,num_check,i; 
	start();					//开始信号// 
	io=1; 						//主机设为输入判断从机DHT11响应信号 
	if(!io)						//判断从机是否有低电平响应信号// 
	{  
		while(!io);				//判断从机发出 80us 的低电平响应信号是否结束// 
		while(io);				//判断从机发出 80us 的高电平是否结束如结束则主机进入数据接收状态 
		R_H=receive_byte();		//湿度高位 
		R_L=receive_byte();		//湿度低位 
		T_H=receive_byte();		//温度高位 
		T_L=receive_byte();		//温度低位 
		check=receive_byte();	//校验位 
		io=0; 					//当最后一bit数据接完毕后从机拉低电平50us// 
		for(i=0;i<7;i++)		//差不多50us的延时 
		delay1(); 
		io=1;					//总线由上拉电阻拉高进入空闲状态 
		num_check=R_H+R_L+T_H+T_L; 
		if(num_check==check)	//判断读到的四个数据之和是否与校验位相同 
		{ 
			RH=R_H; 
			RL=R_L; 
			TH=T_H; 
			TL=T_L; 
			check=num_check;
		} 
	} 
} 
//********************************DHT11模块*********************************************//

//************************串行口字符（字符串）发送块******************************************//	
void putchar(unsigned char n)
{ 
   SBUF=n;
   while(!TI);
   TI=0;
} 
void puts(unsigned char *q)
{
   while(*q)
      putchar(*q++);
}						  
void Enter()	  //换行函数
{
   putchar(0x0d);
   putchar(0x0a); 
}
void DispNum(unsigned char n)
{
	unsigned char t[8]={0};
	t[0]=n/10+'0';
	t[1]=n%10+'0';
	t[2]=0; 
	puts(t); 
	Enter();
}
//************************串行口字符（字符串）发送块******************************************	//


//************************定时器初始化******************************************	//
void init(){		
	
    SCON=0x50;           //设定串口工作方式
    PCON=0x00;           //波特率不倍增
			
	//TH0=(65536-92)/256;//定时器0装入初始值精确0.1ms为92.16
	//TL0=(65536-92)%256;

    TMOD=0x21;           //定时器1工作于8位自动重载模式, 工作方式2，用于产生波特率，定时器0为16位计时器，工作方式1
    EA=1;//全局中断允许
    ES = 1;              //允许串口中断

   // ET0=1;//定时器0中断，用于舵机控制

    TL1=0xfd;
    TH1=0xfd;             //波特率9600

   // TR0=1;  //启动定时器0，用于舵机控制

    TR1=1;	//启动定时器1，用于温湿度

}

void Time0Config()
{
	//TMOD|= 0x01;   //设置定时计数器工作方式1为定时器合并的init()了

	//--定时器赋初始值，11.0592MHZ下定时0.1ms--//  
	TH0=(65536-92)/256;//定时器0装入初始值精确0.1ms为92.16
	TL0=(65536-92)%256;

    ET0=1;//定时器0中断，用于舵机控制
    TR0=1;  //启动定时器0，用于舵机控制
}

//************************定时器初始化******************************************	//


//************************车轮转向控制******************************************	//
void go()
{
	IN1 = 0;
	IN2 = 1;
	IN3 = 0;
	IN4 = 1;
}
void back()
{
	IN1 = 1;//右
	IN2 = 0;
	IN3 = 1;//左
	IN4 = 0;
}
void left()
{
	IN1 = 0;//右
	IN2 = 1;
	IN3 = 0;//左
	IN4 = 0;
}
void right()
{
	IN1 = 0;//右
	IN2 = 0;
	IN3 = 0;//左
	IN4 = 1;
}
void stop()
{
	IN1 = 0;//右
	IN2 = 0;
	IN3 = 0;//左
	IN4 = 0;
}
void biz(uchar num_d1){//避障函数
	if (num_d1)
	{
		
		go();
		delayms(300);//没有障碍，走1s以后，去检测是否停止避障
	}
	else
	{
		back();
		delayms(400);//先后退一段再转身，防止转身时撞上前面，因车速不同，后退距离待测试
		left();
		delayms(500);//左转90度，因车速不同，后退距离待测试
	}

}
void lz_control(uchar num2)
{
	if (num2==0x65)//停
	{
		stop();
	}
	if (num2==0x61)//a,前
    {
    	if (d1==1)
    	{
    		 go();
    	}
    	else
		{
			stop();
		}
    }
    if (num2==0x62)//b，后
    {
    	 back();
    }
    if (num2==0x63)//c，左
    {
    	 left();
    }
	if (num2==0x64)//d，右
    {
  	    right();
    }
	
}
//************************车轮转向控制******************************************	//

/*void dj_qian()//P1^4口控制右侧舵机，机械臂前后动
{
	Time0Config();
	while(1)
	{ 
		if(timer1>200)  //PWM周期为200*0.1ms=20ms
		{
			timer1=0;
		}
		if(timer1<10)//产生周期为20ms，高电平为1ms，舵机会转到45度  15=num1_dj,可以控制角度，15为90度，20为135度，线性关系，角度暂定
		{
			dj1=1;//如果想反转方向，就需把第一个p10=0，第二个p10=-1

		} 
		else
		{
			dj1=0;
		}
	}
}
void dj_hou()//P1^4口控制右侧舵机，机械臂前后动
{
	Time0Config();
	while(1)
	{ 
		if(timer1>200)  //PWM周期为200*0.1ms=20ms
		{
			timer1=0;
		}
		if (timer1<10)		{
			dj1=0;

		} 
		else
		{
			dj1=-1;
		}
	}
}

void dj_sheng()//P1^5口控制左侧舵机，机械臂升（顺时针）
{
	Time0Config();
	while(1)
	{ 
		if(timer1>200)  //PWM周期为200*0.1ms=20ms
		{
			timer1=0;
		}
		if (timer1<10)		{
			dj2=1;
		}
		else
		{
			dj2=0;
		}
	}
}
void dj_jiang()//P1^5口控制左侧舵机，机械臂降（逆时针）
{
	Time0Config();
	while(1)
	{ 
		if(timer1>200)  //PWM周期为200*0.1ms=20ms
		{
			timer1=0;
		}
		if (timer1<10)		{
			dj2=0;
		}
		else
		{
			dj2=-1;
		}
	}

}
void dj_he()//P1^6口控制前面舵机，机械臂抓手合上（顺时针）
{
	Time0Config();
	while(1)
	{ 
		if(timer1>200)  //PWM周期为200*0.1ms=20ms
		{
			timer1=0;
		}
		if (timer1<10)		{
			dj3=1;
		}
		else
		{
			dj3=0;
		}
	}
}
void dj_zhang()//P1^6口控制前面舵机，机械臂抓手张开（逆时针）
{
	Time0Config();
	while(1)
	{ 
		if(timer1>200)  //PWM周期为200*0.1ms=20ms
		{
			timer1=0;
		}
		if (timer1<10)
		{
			dj3=0;
		}
		else
		{
			dj3=-1;
		}
	}
}
void dj_you()//P1^7口控制下面舵机，机械臂右转（顺时针）
{
	Time0Config();
	while(1)
	{ 
		if(timer1>200)  //PWM周期为200*0.1ms=20ms
		{
			timer1=0;
		}
		if (timer1<10)
		{
			dj4=1;
		}
		else
		{
			dj4=0;
		}
	}
}
void dj_zuo()//P1^7口控制下面舵机，机械臂左转（逆时针）
{
	Time0Config();
	while(1)
	{ 
		if(timer1>200)  //PWM周期为200*0.1ms=20ms
		{
			timer1=0;
		}
		if (timer1<10)
		{
			dj4=0;
		}
		else
		{
			dj4=-1;
		}
	}
}
void dj_control (char num_dj)//具体方向待测试
{

	if (num_dj==0x69)//i,前动
    {
    	 dj_qian();
    }
    if (num_dj==0x6A)//j，后动
    {
    	 dj_hou();
    }
    if (num_dj==0x6B)//k，升
    {
    	 dj_sheng();
    }
	if (num_dj==0x6C)//l，降
    {
  	    dj_jiang();
    }

	if (num_dj==0x6D)//m,张开
    {
    	 dj_zhang();
    }
    if (num_dj==0x6E)//n，合上
    {
    	 dj_he();
    }
    if (num_dj==0x6F)//o，左转
    {
    	 dj_zuo();
    }
	if (num_dj==0x70)//p，右转
    {
  	    dj_you();
    }
}*/

//************************舵机控制******************************************	//

void main()
{
	init();

	//dj1=0;
	num2_d1=1;

	while(1)//一直循环检测是否有数据发过来
	{
	//	biz(d1);//测试用
		
		if(n==1)	//有数据时，即n在串口处被设置为1	
		{
			n=0;//重新设置为0，方便下次判断
			//小车转向人操控
	   	    lz_control(bufn);
	   	    //舵机机械臂控制
	   	   // dj_control(bufn);
	   	    //温湿度
	   	    if (bufn==0x66)//f
	   	    {
	   	    	receive();
	   	    	DispNum(TH);
	   	    	DispNum(RH);
				delay(10);
				break;//是否去掉待测试，不去，有可能温度数据发送间，影响避障期
	   	    }
	   	    if(bufn==0x67)//接收g;则自动避障运行
	   	    {
	   	    	biz(d1);//避障函数
	   	    	
	   	    }
	   	     if (bufn==0x68)//h;不再避障	，直接停止
	   	    {
	   	    	stop();

	   	    }
		}
		else//发完避障数据g以后，不再发送数据，即n=0，仍然可以避障，且不影响人为控制时，不发送数据的间隔
		{
		   if (bufn==0x67)//人为控制时，不发送数据的间隔，bufn不等于g，重新循环，不执行避障
   	         {
   	    		biz(d1);//每次前进1s后，重新检测是否需要停止避障
   	   		 }
		}
	}
}

void  serial() interrupt 4 
{
   ES = 0;                //关闭串行中断
   RI = 0;                //清除串行接受标志位
   bufn = SBUF;  		  //从串口缓冲区取得数据
   n=1;          		  //接收到数据以后，给个标记位
   ES = 1;   			  //允许串口中断
}
/*void Time0(void) interrupt 1    //3 为定时器1的中断号  1 定时器0的中断号 0 外部中断1 2 外部中断2  4 串口中断
{
	TH0 = (65536-92)/256; //重新赋初值
	TL0 = (65536-92)%256;
	timer1++;    
}*/


