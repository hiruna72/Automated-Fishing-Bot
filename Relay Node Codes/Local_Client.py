import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev
import paho.mqtt.client as client
import json
import publisher
from tkinter import *
from time import gmtime, strftime




GPIO.setmode(GPIO.BCM)

pipes = [[0xE8, 0xE8, 0xF0, 0xF0, 0xE1], [0xF0, 0xF0, 0xF0, 0xF0, 0xE1]]

radio = NRF24(GPIO, spidev.SpiDev())
radio.begin(0, 17)

radio.setPayloadSize(32)
radio.setChannel(0x76)
radio.setDataRate(NRF24.BR_1MBPS)
radio.setPALevel(NRF24.PA_MIN)

radio.setAutoAck(True)
radio.enableDynamicPayloads()
radio.enableAckPayload()

radio.openWritingPipe(pipes[0])
radio.openReadingPipe(1, pipes[1])
radio.printDetails()
# radio.startListening()


#  ########################################################################################################################
#  ## GUI ###

top = Tk()

#top.mainloop()

topFrame = Frame(top)
topFrame.pack()
bottomFrame = Frame(top)
bottomFrame.pack(side=BOTTOM)

heading = Label(topFrame, text="Local Client", bg="brown", fg="white", font=(None, 30), width=45)
#heading.pack(fill=X)
heading.grid(row=0)


heading2 = Label(topFrame, text="Local Client ID - ****", bg="brown", fg="white", font=(None, 15), width=86)
#heading2.pack(fill=X)
heading2.grid(row=1)

heading3 = Label(topFrame, text="Business Client ID - ****", bg="brown", fg="white", font=(None, 15), width=86)
#heading3.pack(fill=X)
heading3.grid(row=2)

Label_bot1 = Label(bottomFrame, text="Bot 1", bg="white", fg="black", font=(None, 15), width=21)
Label_bot2 = Label(bottomFrame, text="Bot 2", bg="white", fg="black", font=(None, 15), width=21)
Label_bot3 = Label(bottomFrame, text="Bot 3", bg="white", fg="black", font=(None, 15), width=21)
Label_bot4 = Label(bottomFrame, text="Bot 4", bg="white", fg="black", font=(None, 15), width=21)

#b1.pack(side=LEFT, fill=X)
#b2.pack(side=LEFT, fill=X)
#b3.pack(side=LEFT, fill=X)
#b4.pack(side=LEFT, fill=X)

Label_bot1.grid(row=0, column=0)
Label_bot2.grid(row=0, column=1)
Label_bot3.grid(row=0, column=2)
Label_bot4.grid(row=0, column=3)

Label_bID_1 = Label(bottomFrame, text="Bot ID - 00000001", bg="white", fg="black", font=(None, 10), width=29)
Label_bID_2 = Label(bottomFrame, text="Bot ID - 00000010", bg="white", fg="black", font=(None, 10), width=29)
Label_bID_3 = Label(bottomFrame, text="Bot ID - 00000011", bg="white", fg="black", font=(None, 10), width=29)
Label_bID_4 = Label(bottomFrame, text="Bot ID - 00000100", bg="white", fg="black", font=(None, 10), width=29)

Label_bID_1.grid(row=1, column=0)
Label_bID_2.grid(row=1, column=1)
Label_bID_3.grid(row=1, column=2)
Label_bID_4.grid(row=1, column=3)

Label_performance1 = Label(bottomFrame, text="Performance - **%", bg="white", fg="black", font=(None, 10), width=29)
Label_performance2 = Label(bottomFrame, text="Performance - **%", bg="white", fg="black", font=(None, 10), width=29)
Label_performance3 = Label(bottomFrame, text="Performance - **%", bg="white", fg="black", font=(None, 10), width=29)
Label_performance4 = Label(bottomFrame, text="Performance - **%", bg="white", fg="black", font=(None, 10), width=29)

Label_performance1.grid(row=2, column=0)
Label_performance2.grid(row=2, column=1)
Label_performance3.grid(row=2, column=2)
Label_performance4.grid(row=2, column=3)

Label_botStatus_1 = Label(bottomFrame, text="Bot status - ****", bg="white", fg="black", font=(None, 10), width=29)
Label_botStatus_2 = Label(bottomFrame, text="Bot status - ****", bg="white", fg="black", font=(None, 10), width=29)
Label_botStatus_3 = Label(bottomFrame, text="Bot status - ****", bg="white", fg="black", font=(None, 10), width=29)
Label_botStatus_4 = Label(bottomFrame, text="Bot status - ****", bg="white", fg="black", font=(None, 10), width=29)

Label_botStatus_1.grid(row=3, column=0)
Label_botStatus_2.grid(row=3, column=1)
Label_botStatus_3.grid(row=3, column=2)
Label_botStatus_4.grid(row=3, column=3)

Label_lastCall_1 = Label(bottomFrame, text="Last call at - ****", bg="white", fg="black", font=(None, 10), width=29)
Label_lastCall_2 = Label(bottomFrame, text="Last call at - ****", bg="white", fg="black", font=(None, 10), width=29)
Label_lastCall_3 = Label(bottomFrame, text="Last call at - ****", bg="white", fg="black", font=(None, 10), width=29)
Label_lastCall_4 = Label(bottomFrame, text="Last call at - ****", bg="white", fg="black", font=(None, 10), width=29)

Label_lastCall_1.grid(row=4, column=0)
Label_lastCall_2.grid(row=4, column=1)
Label_lastCall_3.grid(row=4, column=2)
Label_lastCall_4.grid(row=4, column=3)

Label_battery_1 = Label(bottomFrame, text="Battery - **** hrs left", bg="white", fg="black", font=(None, 10), width=29)
Label_battery_2 = Label(bottomFrame, text="Battery - **** hrs left", bg="white", fg="black", font=(None, 10), width=29)
Label_battery_3 = Label(bottomFrame, text="Battery - **** hrs left", bg="white", fg="black", font=(None, 10), width=29)
Label_battery_4 = Label(bottomFrame, text="Battery - **** hrs left", bg="white", fg="black", font=(None, 10), width=29)

Label_battery_1.grid(row=5, column=0)
Label_battery_2.grid(row=5, column=1)
Label_battery_3.grid(row=5, column=2)
Label_battery_4.grid(row=5, column=3)

#  ####################################################################################################################


class Bot:  # Bot class

    def __init__(self, botID, botPerformance, noOfMessages, noOfFishDetected, batteryLeft, botStatus, lastStatus, lastCallAt):
        self.botID = botID
        self.botPerformance = botPerformance
        self.noOfMessages = noOfMessages
        self.noOfFishDetected = noOfFishDetected
        self.batteryLeft = batteryLeft
        self.botStatus = botStatus
        self.lastStatus = lastStatus
        self.lastCallAt = lastCallAt
        self.lastCallAtN = time.time()

    def batteryLeft(self):
        pass

    def setStatus(self, inputChar):
        if inputChar == '1':
            self.botStatus = 'Status - Critical'
        elif inputChar == '0':
            self.botStatus = 'Status - Normal'
        else:
            self.botStatus = 'Status - Network Failure'

    def setPerformance(self, isDetected):
        self.noOfMessages += 1.0
        if isDetected == '1':
            self.noOfFishDetected += 1.0
        self.botPerformance = 'Performance - '+str(float((self.noOfFishDetected / self.noOfMessages) * 100)) + '%'
# end of bot class


bot_01 = Bot('00000001', '**%', 0.0, 0.0, 'Battery left - 12.00hrs', '****', '****', '****')  # creating four bots
bot_02 = Bot('00000010', '**%', 0.0, 0.0, 'Battery left - 12.00hrs', '****', '****', '****')
bot_03 = Bot('00000011', '**%', 0.0, 0.0, 'Battery left - 12.00hrs', '****', '****', '****')
bot_04 = Bot('00000100', '**%', 0.0, 0.0, 'Battery left - 12.00hrs', '****', '****', '****')

bots = [bot_01, bot_02, bot_03, bot_04]  # inserting bots into bot array

#message = list("ACK")
#while len(message) < 32:
#   message.append(0)

def setBotStatus(inputString, bot):
    # setting bot's performance
    bot.setPerformance(inputString[8]) # 1 = fish detected , 0 = not detected

    # setting bot's danger status # 1 = danger , 0 = normal
    bot.setStatus(inputString[30])

    # setting bot's lastcall
    bot.lastCallAt = strftime("%Y-%m-%d %H:%M:%S", gmtime())
    bot.lastCallAtN = time.time()

def networkFailureSet(bots):
     delay = 30
     if  time.time() - bots[0].lastCallAtN >= delay:
         bots[0].botStatus = 'Status - Network Failure'
         Label_botStatus_1['text'] = bots[0].botStatus
         Label_botStatus_1['bg'] = getLabelColour(bots[0])
     if  time.time() - bots[1].lastCallAtN >= delay:
         bots[1].botStatus = 'Status - Network Failure'
         Label_botStatus_2['text'] = bots[1].botStatus
         Label_botStatus_2['bg'] = getLabelColour(bots[1])
     if  time.time() - bots[2].lastCallAtN >= delay:
         bots[2].botStatus = 'Status - Network Failure'
         Label_botStatus_3['text'] = bots[2].botStatus
         Label_botStatus_3['bg'] = getLabelColour(bots[2])
     if  time.time() - bots[3].lastCallAtN >= delay:
         bots[3].botStatus = 'Status - Network Failure'
         Label_botStatus_4['text'] = bots[3].botStatus
         Label_botStatus_4['bg'] = getLabelColour(bots[3]) 

def getLabelColour(bot):
    if bot.botStatus == 'Status - Critical':
        return 'red'
    elif bot.botStatus == 'Status - Normal':
        return 'green'
    elif bot.botStatus == 'Status - Network Failure':
        return 'orange'

def setBotStatusGui(bot):
    if bot.botID == '00000001':
        Label_performance1['text'] = bot.botPerformance
        Label_botStatus_1['text'] = bot.botStatus
        Label_botStatus_1['bg'] = getLabelColour(bot)
        Label_lastCall_1['text'] = bot.lastCallAt
        Label_battery_1['text'] = bot.batteryLeft
    elif bot.botID == '00000010':
        Label_performance2['text'] = bot.botPerformance
        Label_botStatus_2['text'] = bot.botStatus
        Label_botStatus_2['bg'] = getLabelColour(bot)
        Label_lastCall_2['text'] = bot.lastCallAt
        Label_battery_2['text'] = bot.batteryLeft
    elif bot.botID == '00000011':
        Label_performance3['text'] = bot.botPerformance
        Label_botStatus_3['text'] = bot.botStatus
        Label_botStatus_3['bg'] = getLabelColour(bot)
        Label_lastCall_3['text'] = bot.lastCallAt
        Label_battery_3['text'] = bot.batteryLeft
    elif bot.botID == '00000100':
        Label_performance4['text'] = bot.botPerformance
        Label_botStatus_4['text'] = bot.botStatus
        Label_botStatus_4['bg'] = getLabelColour(bot)
        Label_lastCall_4['text'] = bot.lastCallAt
        Label_battery_4['text'] = bot.batteryLeft


while(1):
    top.update()

    start = time.time()
    radio.startListening()

    while not radio.available(0):
        time.sleep(1 / 100)
        if time.time() - start > 2:
            print("Timed out.")
            break

    receivedMessage = []
    radio.read(receivedMessage, radio.getDynamicPayloadSize())
    print("Received: {}".format(receivedMessage))

    print("Translating the receivedMessage into unicode characters")
    string = ""
    for n in receivedMessage:
        # Decode into standard unicode set
        if (n >= 32 and n <= 126):
            string += chr(n)

    print("Out received message decodes to: {}".format(string))

    if string[0:8] == '00000001':
        setBotStatus(string, bots[0])
        jsonObject = json.dumps({"BotID": "bot12","ActiveHours": 2,"TriggerData":[{"TimeStamp":"1400","Triggers": 2}]})
        publisher.sendToServer(jsonObject)
    elif string[0:8] == '00000010':
        setBotStatus(string, bots[1])
        jsonObject = json.dumps({"BotID": "bot12","ActiveHours": 2,"TriggerData":[{"TimeStamp":"1400","Triggers": 2}]})
        publisher.sendToServer(jsonObject)
    elif string[0:8] == '00000011':
        setBotStatus(string, bots[2])
        jsonObject = json.dumps({"BotID": "bot12","ActiveHours": 2,"TriggerData":[{"TimeStamp":"1400","Triggers": 2}]})
        publisher.sendToServer(jsonObject)
    elif string[0:8] == '00000100':
        setBotStatus(string, bots[3])
        jsonObject = json.dumps({"BotID": "bot12","ActiveHours": 2,"TriggerData":[{"TimeStamp":"1400","Triggers": 2}]})
        publisher.sendToServer(jsonObject)

    networkFailureSet(bots)

    #Setting GUI
    for b in bots:
        setBotStatusGui(b)



    radio.stopListening()

    











