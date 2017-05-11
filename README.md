

In brief:

* _arduino-imu_ is a script to upload to your Arduino 101
* _node-serial-bridge_ is a Nodejs script that runs on your computer to pipe data between the Arduino and the wider world
* _noodl-ws_ is a Noodl project that demonstrates using data sent from _arduino-imu_

# arduino-imu

This folder contains an Arduino script. Out of the box it will send data from the 101's IMU to the serial port in MQTT format. There is also a demo function for how to write a value from an analog pin as well.

Write this to your Arduino, and test that it works by opening the serial monitor. You should see a stream of IMU values as you move the board. Close the serial monitor once you're happy, so that the Node script can connect.

_arduino-connected_ contains an alternative sample Arduino script.

# node-serial-bridge

This is a small script that runs on your computer and pipes data between the Arduino and Noodl. For it to work, you must have a serial connection to your Arduino. For the 101, this means having it connected via USB. Unfortunately this form of communication is not possible over Bluetooth for the 101.

To set this script up, you first have to run `npm install` in its directory, to fetch the necessary packages.

The script will run continuously, piping data back and forth. So you must start it when you begin your hacking session, and probably want to close it when you're done.

You can start it with:

```
node app.js
```

You'll get a list of serial ports on your system. Most likely you'll see one that you recognise from the Arduino IDE. On my system, it's `/dev/cu.usbmodem1411`. If you're on a Windows machine it will be something like `COM2`.

Now that you know the serial port, start the script telling it what port to use. In my case, this is:

```
node app.js /dev/cu.usbmodem1411
```

Note that serial only allows for one connection at a time. If you have the serial port monitor open in the Arduino IDE it won't work, and if you want to upload new code to your Arduino, you'll also need to restart node-serial-bridge.

Once started, leave the script running. It won't print anything out until a connection is made from Noodl.

# noodl-ws

This is the Noodl project which demonstrates using data from the Arduino via the Nodejs script (quite the journey!). Add the folder as an external project to Noodl. We assume you have some familiarity with Noodl already.

If you've changed the keys they get sent from the Arduino script, you'll need to make the appropriate modifications to the websocket block in the project.

You also should start up _node-serial-bridge_ in the background so the data flows into Noodl.

You can also see how to send data back to the Arduino, although the demo script Arduino script doesn't use this.

# Credits

Created by Mathias Lewin @ Topp. Small changes made by Clint Heyer @ Malmö University.
