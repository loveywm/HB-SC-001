//更新记录：



2014-12-10：
1：完成基本版本，基本根据需求完成了载重，显示，按键的功能。 loveywm
********************************************************************************
以下是基础版本：

代码编写风格说明：
代码编写的风格，从根本上讲，应该要简单、易懂、无歧义为目的，此外因为编写大量代码是一个相对枯燥的工作，整天对着屏幕，一句一句的分析，很容易疲劳，所以代码编写还要重视优雅，符合自然审美，不为了过份追求严谨而破坏审美、扭曲人性为上，一些需要严谨的地方，可以通过编辑器，或者第三方检测手段实现。

msOS以C#命名规范为标准，以长命名为主，符合简单、易懂、优雅、无歧义的特点，以下是C#的三条命名规范
Pascal 规则，每个单词开头的字母大写(如 TestCounter)。
Camel 规则，除了第一个单词外的其他单词的开头字母大写(如 testCounter)
Upper 规则，仅用于一、两个字符长的常量的缩写命名，超过三个字符长度应该应用Pascal规则(如E,IO,PI,SN)

约定俗称，非常通用，英文字典中可以查到的一些缩写，比如Max,Min,Pwm,IO,SN,ID等，可以采用缩写字。

应用层定义的App结构体内的数据库，因为数据意思要表达清晰，所以尽可能长命名，除非十分通用缩写名(如Pwm)。为了方便其它地方使用，可以在数据库定义时，注明缩写方式，如下：
uint Frequency;                 //Frq
uint SerialNumber;              //SN
uint ProductionDate;            //PD
uint IdentifyNumber0;           //ID0
uint IdentifyNumber1;           //ID1
uint IdentifyNumber2;           //ID2
一些太长的全命名，确实不利于书写，可以在不影响阅读的情况下缩写(SetFrequency -> SetFrq),Frequency的缩写为Frq，在数据库中声明过。

一些常用的局部变量，比如TestCounter，可以缩写成TestCnt，这类在编程时常用，确实不影响阅读的。需要在readme中声明，这类词有
Counter				//Cnt
Receive				//Rcv

菜单界面的各类控件定义对象时，C#有一套比较规范的缩写标准，类似匈牙利命名法，如下：
static Label PowerLabel;		static Label lblPower;
static TextBox FrequencyTextBox;	static TextBox txtFrequency;	
msOS不是很推荐采用，因为msOS的菜单控件不会很多，并且一次性定义好了，全命名看上去优雅很多，但不排斥按C#风格的缩写方式。

指针类型，后缀加Pointer,确实有些长，如何处理还未想好。

********************************************************************************
*/