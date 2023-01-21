
import json
import serial
import time
import urllib.request

COM = "COM1"
UID = "527952382"
STAT_URL = "https://api.bilibili.com/x/relation/stat?vmid=%s&jsonp=jsonp" % UID
# 验证是否将UID填入
print(STAT_URL)

if __name__ == "__main__":
    msp432 = serial.Serial()  # 创建一个串口的对象
    msp432.port = COM  # 设置串口号
    msp432.baudrate = 9600  # 设置波特率
    msp432.bytesize = 8  # 设置数据位
    msp432.parity = 'N'  # 设置校验位
    msp432.stopbits = 1  # 设置停止位
    msp432.timeout = 1  # 超时
    msp432.open()
    # 是否打开串口
    if not msp432.isOpen():
        exit(-1)
    else:
        print("Serial: %s open successs..." % COM)

    # 获取B站粉丝数
    while True:
        # 首先模拟HTTP请求访问B站获得相关数据
        stat_response = urllib.request.urlopen(STAT_URL)
        d = json.loads(stat_response.read())
        print(d)
        datalist = [d['data']['following'], d['data']['follower']]
        print(datalist)
        #将列表的数据转化为字符串方便后续写入，并且以','作为分隔符，分开两组数据
        str_data = str(datalist[0]) + ',' + str(datalist[1])
        print(str_data)
        #将数据逐个写入到msp432的缓冲器中
        for datas in str_data:
            msp432.write(datas.encode())
            time.sleep(0.1)
        msp432.write(b'S') #写入一个停止标志
        time.sleep(3)
