var WebSocket = require('ws'),
    SerialPort = require("serialport");

var wss = new WebSocket.Server({ port: 8086, perMessageDeflate: false });
var SocketReady = false;
var portArg;

wss.on("connection", function(ws) {
    console.log("Client connection to serial port service");
    SocketReady = true;
    ws.on("message", function(event) {
        var obj = JSON.parse(event);
        var lol = obj.servo + "\n"; // find your websocket "Data to Send" port...
        lol = lol + "\n";
        port.write(lol, function(err) {
            //console.log("writing:",lol, "to Arduino" );
            if (err) {
                return console.log('Error on write: ', err.message);
            }
        });
    });
});

wss.broadcast = function broadcast(data) {
  wss.clients.forEach(function each(client) {
    //console.log("wss broadcast:", data);
    if ( client.readyState === WebSocket.OPEN)  {
      client.send(data);
    }
  });
};


process.argv.forEach(function (val, index, array) {
    if (index >= 2) 
        portArg = val;
    else {
        portArg = "";
       
    }
});

if (portArg.length == 0) {
 SerialPort.list(function (err, ports) {
    console.log("Here are the serial ports on your system. Find your Arduino and pass that as cmd line argument to this app");
    ports.forEach(function(port) {
        console.log(port.comName, " - ", port.manufacturer);
    });
    process.exit();
 });
 return;
}

var port = new SerialPort(portArg, function (err) {
    parser: SerialPort.parsers.byteDelimiter([10,13]);
    if (err) {
        console.log('Error: ', err.message);
        process.exit();
    } else {
        console.log("Opened port, now waiting for client connection.")
        console.log("Use CTRL+C to stop.");
    } 
});

var acc = "";
port.on('data', function(data) {
    acc += data.toString();
    var l = acc.split("\r\n");

    if(l.length > 1) {
        var d = l[0];
        acc = l[1];
        if(acc === undefined) acc = "";
        //console.log("d:", d);
        //wss.broadcast(JSON.stringify({
        //    out:d
        //}));
        wss.broadcast(d);
    }   
});
    

