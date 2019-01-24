import json
import serial
import datetime
import time
import pika
import sys

s=serial.Serial('/dev/ttyUSB0',9600)
#s=serial.Serial('/dev/ttyACM0',9600)
connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost'))
channel = connection.channel()

#channel.exchange_declare(exchange='solar',exchange_type='topic')


time.sleep(5)

print("programm started")

try:
        while True:
            
            response = s.readline()
         
            
            if "Puffer1" in response:
                rec = response.split('&&')
                print(rec)
                
                routingkey = 'buffer1.temp.up'
                message = rec[0]
                channel.basic_publish(exchange='solar',routing_key=routingkey,body=message)

                routingkey = 'buffer1.temp.down'
                message = rec[1]
                channel.basic_publish(exchange='solar',routing_key=routingkey,body=message)
                
        
            
            elif "Puffer2" in response:
                rec = response.split('&&')
                print(rec)
                
                routingkey = 'buffer2.temp.up'
                message = rec[0]
                channel.basic_publish(exchange='solar',routing_key=routingkey,body=message)

                routingkey = 'buffer2.temp.down'
                message = rec[1]
                channel.basic_publish(exchange='solar',routing_key=routingkey,body=message)
                

            elif "Brauchwasser" in response:
                rec = response.split('&&')
                print(rec)
                
                routingkey = 'freshwater.temp.up'
                message = rec[0]
                channel.basic_publish(exchange='solar',routing_key=routingkey,body=message)

                routingkey = 'freshwater.temp.down'
                message = rec[1]
                channel.basic_publish(exchange='solar',routing_key=routingkey,body=message)
                
                    

            elif "Dach" in response:
                rec = response.split('&&')
                print(rec)
               
                routingkey = 'roof.temp'
                message = rec[0]
                channel.basic_publish(exchange='solar',routing_key=routingkey,body=message)

                
                

            elif "Status" in response:
                rec = response.split('&&')
                print(rec)
                
                routingkey = 'status'
                message = rec[0]
                channel.basic_publish(exchange='solar',routing_key=routingkey,body=message)
                


            else:

                print('Could not identify Message')
        
except KeyboardInterrupt:
    s.close()
