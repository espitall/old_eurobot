
/**
 * Module dependencies.
 */

var express = require('express'),
	io = require("socket.io"),
  dgram = require("dgram"),
	http = require('http'),
	path = require('path'),
  robotControlMod = require("./robotControl");
  //uiControl = require("./uiControl.js");


var app = express();

var HOST = "127.0.0.1",
PORT = "5400";


var robotServeur = dgram.createSocket('udp4');

robotServeur.on('listening', function () {
    var address = robotServeur.address();
    console.log('Robot Server listening on ' + address.address + ":" + address.port);
});

var robotControl = new robotControlMod.RobotControl();


robotServeur.on('message', function (message, remote) {

  robotControl.receiv(message);

});

robotServeur.bind(PORT, HOST);

robotControl.on("sendToBot",function(message){
  console.log("message vers le robot : ",message);

});



app.configure(function(){
  app.set('port', process.env.PORT || 3000);
  app.use(express.logger('dev'));
  app.use("/", express.static(__dirname + '/public'));
});


var serveur = http.createServer(app).listen(app.get('port'), function(){
  console.log("Ui server listening on port " + app.get('port'));
});

io = io.listen(serveur);

io.sockets.on('connection', function (socket) {
  robotControl.on("sendToUi",function(message){
    socket.emit("message",message);

  });
 // uiControl.Control(socket);

});


