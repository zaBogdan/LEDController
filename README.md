# zaHome - LED Strip Controller

This script was wrote for an ESP8266 chip in order to control the LED SMD5050 strips. The way it works is by recieving UDP packets, interpret them and write the output to the pins. 

### How to wire things up
TBD

### How to send the server code work

When powered on the chip, set all three pins to OUTPUT mode, than tries to connect to the internal WiFi network, if succeded it will startup an UDP server that listen on port `1337`. After all that if everything is right the entire LED strip must go full white for 5 seconds, than go back to black/shutdown.

Now you have a server that listens for UDP packets, and decodes them like `atribute:value`. The whole payload looks like: `r:xxx:g:xxx:b:xxx:cmd:xxxxxxx`.You can pass from 1-4 arguments, those being:
- `r` which obviously stands for RED 
- `g` which is for GREEN pin
- `b` which is for BLUE pin
- `cmd` which can only be `shutdown` or `start`

> Note: The order in which you send this payload doesn't matter. It can be `g:xxx:r:xxx:b:xxx:cmd:::::` or `g:xxx` or `cmd:xxx` or anything. 

If the server is started and the command was processed as it should, it will respond you back with `0xTrue`. Otherwise, it will return back `0xFalse`. Please note that while the server is set to shutdown mode, it will always respond to `0xFalse` because it will never get processed. 

### Values that can/must be changed

This can be changed acording to your own prefernce
```cpp
//Internet credentials
const char* ssid     = "demoWifiNetwork"; //Router SSID         
const char* password = "demoPassword"; //Router password

//RGB pins
const int REDPIN = 4; //GPIO4
const int GREENPIN = 16;//GPIO16
const int BLUEPIN = 5;//GPIO5

//Maximum trashold for newtork connection
const int MaximumWaitingTime = 60; //How many seconds do you want to wait before going offline

/**
 * Configuration stuff
 */
unsigned int localUdpPort = 1337; //UDP Port
char replyPacket[] = "0xTrue"; //Default response
```

### Default output
This is a default output for debugging purpose only.
```
-> Initialization started.
-> 1...2...3...4...5...6...The local ip is 192.168.0.191
-> UDP server started at port 1701
-> Testing the RGB strip (all white)
-> Test finished. If it didn t work please check your connections!
-> Server now accepts packets
-> Received 23 bytes from 192.168.0.101, port 42548
-> The packet is: `r:162:g:56:b:0:cmd:None` 
-> R: 648 , G: 224 B: 0 
-> 
-> Sending a packate to 192.168.0.101 using port 42548
-> Received 24 bytes from 192.168.0.101, port 36882
-> The packet is: `r:0:g:0:b:0:cmd:shutdown` 
-> R: 0 , G: 0 B: 0 
-> 
-> Sending a packate to 192.168.0.101 using port 36882
```

### Demo script to test

For the purpose of development and fast debugging, I wrote a python script that takes 4 arguments and it sends them over the network to the ESP controller. to use it you can type this commands
```zsh
./main.py --r 256 --g 50 --b 30 --cmd start
./main.py  --cmd shutdown //Here rgb tags are optional. 
./main.py --r 256 --g 50 --b 30
./main.py --r 256 --g 256 --b 256
./main.py --r 256 --g 0 --b 0
./main.py --r 0 --g 256 --b 0
./main.py --r 0 --g 0 --b 256
./main.py --r 100 --g 50 --b 256
``` 

### Mobile Application (React-Native)

TBD