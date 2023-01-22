# 【Python与MSP432的爱恨情仇】Python获取B站粉丝并且显示到OLED屏幕上

> 作者：翔丶哈哈
>
> 没怎么写过文档，这是第一次写文档，写的有点混乱，文档可能会再整理一下，也可能就这样子了
>
> 有什么问题可以B站私信或者qq联系。
>
> qq：1051566182



> 这个是跟着这个up主的视频做的
>
> [硬件开发笔记】使用Python获取Bilibili粉丝数并通过OLED屏幕显示](https://www.bilibili.com/video/BV1GR4y127nx/?share_source=copy_web&vd_source=d4379e2c9cb0ed957521900d8a64e469)

代码演示视频及原理讲解视频
[【Python与MSP432的爱恨情仇】Python获取B站粉丝并且显示到OLED屏幕上](https://www.bilibili.com/video/BV1id4y157nw/?share_source=copy_web&vd_source=d4379e2c9cb0ed957521900d8a64e469)

## 技术难点

- 单片机的UART通讯配置
- Python获取粉丝数的代码的编写
- 通讯中的数据如何处理

### 发送与接收数据

经过实验，当比如写入一个数字1000的时候，会进入21次中断

也就是说，接收数据时和发送数据时都是一个字符一个字符传递的！

I get it !

而且当超过限度的时候，这个buffer就会归0，这个限度大概在一百多

- python的serial模块中的write怎么发送字符串？

solution：能由于python3和python2的编码格式不同，python2可以实现字符串的发送，但是在python3下会报错 **格式应该为b’----'，而不是一个字符串**。故寻找其他方法来解决，大概率是需要重新写一个函数。不对，发送字符串需要加一个encode，就是编码格式，

### UART通讯配置

如果是新手不知道怎么配置的话，可以用官方给的例程代码，直接用他们写好的一些配置。也可以自己看寄存器，看技术手册来配置寄存器

### 数据处理

通过Python获取到的数据是一个json格式的，但是我们只需要里面的一些内容即可，再仔细看，其实这个格式很像python里的字典的数据类型

