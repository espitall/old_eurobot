'use strict'
/**
 * Module dependencies.
 */

var express = require('express'),
    io = require("socket.io"),
    net = require("net"),
    http = require('http'),
    path = require('path'),
    robotControlMod = require("./robotControl"),
    uiControlMod = require("./uiControl"),
    binary = require('binary'),
    SerialPort = require("serialport").SerialPort;

var app = express();

var robotControl = new robotControlMod.RobotControl();
var uiControl = new uiControlMod.UiControl();

process.on('uncaughtException', function(err) {
  console.error(err.stack);
  robotControl.sendToUi("log",{src:"Serveur",lvl:4,msg:"err.stack"});
});


if(process.argv[2] && process.argv[2][0] == '/') {
  //use serial
  var baudrate = process.argv[3] || 19200;
  var serialPort = new SerialPort(process.argv[2], {
    baudrate: 19200
  }, false);

  serialPort.open(function () {
    console.log('serial port open');
    robotControl.sendToUi("log",{src:"Serveur",lvl:2,msg:"Serial port " + process.argv[2] + ":" + baudrate + " opened"});
    serialPort.on('data', function(data) {
      robotControl.decode(data);
    });
    //serialPort.write("ls\n", function(err, results) {
    //  console.log('err ' + err);
    //  console.log('results ' + results);
    //});
  });

  robotControl.on("sendToBot",function (message){
    console.log("message vers le robot : ",message);
    serialPort.write(message);
  });

  uiControl.on("sendToBot",function (data){
    robotControl.sendToBot(data);
  });

}
else {
  //use TCP
  var HOST = process.argv[2] || '127.0.0.1';
  var PORT = process.argv[3] || '8787';
  var TIMETORECO = 2000; // Tempo avant essai de reconnexion


  var robotServeur = new net.Socket();
  robotServeur.setNoDelay(true)
    robotServeur.connect(PORT, HOST);


  robotServeur.on("connect",function() {
    console.log('CONNECTED TO: ' + HOST + ':' + PORT);
    robotControl.sendToUi("log",{src:"Serveur",lvl:2,msg:"Connexion avec le robot Ok"});
    robotControl.bonjour();
  });

  robotServeur.on("data",function (data) {
    robotControl.decode(data);
  });

  robotServeur.on('close', function() {
    console.log('Connexion vers le robot fermée');
    robotControl.sendToUi("log",{src:"Node",lvl:4,msg:"Connexion vers le robot fermée"});

    setTimeout(function() {
      console.log("Tentative de reconnexion...");
      robotControl.sendToUi("log",{src:"Node",lvl:2,msg:"Tentative de reconnexion..."});
      robotServeur.connect(PORT, HOST);
    },TIMETORECO);

  });

  robotServeur.on('error', function(e) {
    console.log('Erreur de connexion vers le robot',e.code);
    robotControl.sendToUi("log",{src:"Node",lvl:4,msg:"Erreur de connexion vers le robot"});
  });

  robotControl.on("sendToBot",function (message){
    console.log("message vers le robot : ",message);
    robotServeur.write(message);
  });

  uiControl.on("sendToBot",function (data){
    robotControl.sendToBot(data);
  });
}

app.configure(function(){
  app.set('port', process.env.PORT || 3000);
  //app.use(express.logger('dev'));
  app.use("/", express.static(__dirname + '/public'));
});


var serveur = http.createServer(app).listen(app.get('port'), function(){
  console.log("Ui server listening on http://127.0.0.1:" + app.get('port'));
});

io = io.listen(serveur);
io.set('log level', 1);

robotControl.on("sendToUi",function (type,message){
  io.sockets.emit(type,message);
});

io.sockets.on('connection', function (socket) {
  console.log("Client connecté");

  socket.on('command', function (socket) {
    uiControl.receive(socket);
  });
});
