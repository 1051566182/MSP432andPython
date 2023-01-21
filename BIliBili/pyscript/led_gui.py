# 这个代码是从网上直接抄过来的
import serial
import time
import tkinter as tk

window = tk.Tk()
window.configure(background="gray14")
window.title("MSP432 - LED CTRL")
msp432 = serial.Serial('COM1', 9600)


def led_control():
    print(">>> LED CTRL - PROGRAM <<<\n")

    def green():
        print(">Green<")
        msp432.write(b'g')
        time.sleep(0.1)
        msp432.write(b'r')
        time.sleep(0.1)
        msp432.write(b'b')
        time.sleep(0.1)
        msp432.write(b'm')

    def red():
        print(">Red<")
        # msp432.write('This is data for test')
        msp432.write('200'.encode())

    def blue():
        print(">Blue<")
        msp432.write(b'b')

    def multiblink():
        print(">Multi<")
        msp432.write(b'm')

    def quit():
        print("\n** END OF PROGRAM **")
        msp432.write(b'q')
        msp432.close()
        window.destroy()

    b1 = tk.Button(window, text="GREEN", command=green, bg="lime green", fg="gray7", font=("Comic Sans MS", 20))
    b2 = tk.Button(window, text="RED", command=red, bg="firebrick2", fg="ghost white", font=("Comic Sans MS", 20))
    b3 = tk.Button(window, text="BLUE", command=blue, bg="dodger blue", fg="ghost white", font=("Comic Sans MS", 20))
    b4 = tk.Button(window, text="MULTI", command=multiblink, bg="MediumOrchid3", fg="ghost white",
                   font=("Comic Sans MS", 20))
    b5 = tk.Button(window, text="EXIT", command=quit, bg="gold", fg="gray7", font=("Comic Sans MS", 20))
    b1.grid(row=1, column=1, padx=5, pady=10)
    b2.grid(row=1, column=2, padx=5, pady=10)
    b3.grid(row=1, column=3, padx=5, pady=10)
    b4.grid(row=1, column=4, padx=5, pady=10)
    b5.grid(row=1, column=5, padx=5, pady=10)
    window.mainloop()


time.sleep(2)
led_control()
