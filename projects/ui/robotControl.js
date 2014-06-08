
var events = require('events'),
    sys = require('sys');
var com = require("./com");
var binary = require('binary');



var fs = require("fs");

/*
 Pour log
*/
var red, blue, reset;
red   = "\033[31m";
blue  = "\033[34m";
reset = "\033[0m";

function RobotControl() {

    events.EventEmitter.call(this);
    this.packet = {
      header : {
        valid: false,
      },
      data : new Buffer(0),
      raw : new Buffer(0),
    };
}

sys.inherits(RobotControl, events.EventEmitter);




RobotControl.prototype.receive = function(msg) {

  var payload = null;

  for(var p in com) {
    if(com[p].type == msg.type) {
      payload = com[p];
      break;
    }
  }

  if(payload != null) {
    payload.parse(this, msg);
  }
  else {
    com.log.send(this, "ui", "error", "Unknown payload type: " + msg.type);
  }
};


function crc_ccittupdate(crc,data) {
  data ^= crc&0xff;
  data ^= data<<4;
  data &= 0xff;
  return (((data << 8) | ((crc >> 8) & 0xff)) ^ ((data >> 4) & 0xff) ^ (data << 3))&0xffff;
}


RobotControl.prototype.calculCrc = function(data) {

  var crc = 0xffff;

  for(var i = 0,t = data.length; i<t; i++) {
    crc = crc_ccittupdate(crc,data[i]);
  }

  return crc;
};

/**
* data : data à controler -> Buffer
* crc : Code Crc de data -> int
*/
RobotControl.prototype.valideCRC = function(data,crcCode) {

  //console.log("CRC : ",crcCode,
  //  "compute CRC : ",this.calculCrc(data),
  //  "data len : ", data.length,
  //  "data : ",data,"\n");

  return this.calculCrc(data) == crcCode ;

};



RobotControl.prototype.decode = function(paquet) {

  //append new data to raw buffer
  this.packet.raw = Buffer.concat([this.packet.raw, paquet]);
  if(this.packet.header.valid) {
    if(this.packet.raw.length >= this.packet.header.data.payloadSize) {
      this.packet.data = this.packet.raw.slice(0, this.packet.header.data.payloadSize);
      if(this.valideCRC(this.packet.data,this.packet.header.data.payloadCRC)) {

        this.receive({
          "type":this.packet.header.data.type,
          "data":this.packet.data,
          "source":this.packet.header.data.source
        });

        //parse remaning bytes
        this.packet.raw = this.packet.raw.slice(this.packet.header.data.payloadSize);
        this.packet.header.valid = false;
        this.decode(new Buffer(0));
      }
      else {        
        console.log('bad crc data'); 
        //slice and retry
        this.packet.raw = this.packet.raw.slice(1);
        this.packet.header.valid = false;
        this.decode(new Buffer(0));
      }
    }
  }
  else {
    if(this.packet.raw.length >= 9) {
      //try decode
      var decodedHeader = binary.parse(this.packet.raw)
        .word8("magicStart")
        .word8("payloadSize")
        .word8("source")
        .word8("destination")
        .word8("type")
        .word16lu("headerCRC")
        .word16lu("payloadCRC")
        .vars
        ;

      if(String.fromCharCode(decodedHeader.magicStart) == 'P') {
        //check header crc
        if(this.valideCRC(this.packet.raw.slice(0,5),decodedHeader.headerCRC)) {
          this.packet.header.valid = true;
          this.packet.header.data = decodedHeader;
          this.packet.raw = this.packet.raw.slice(9);
          this.decode(new Buffer(0));
        }
        else {
          console.log('bad crc header'); 
          //slice and retry
          this.packet.raw = this.packet.raw.slice(1);
          this.decode(new Buffer(0));
        }
      }
      else {
        //slice and retry
        this.packet.raw = this.packet.raw.slice(1);
        this.decode(new Buffer(0));
      }
    }
  }
};

RobotControl.prototype.debugFileOpen = function() {

};

RobotControl.prototype.debugFileWrite = function() {

};

/**
* Envoie d'un payload systeme de nom
*/
RobotControl.prototype.bonjour = function() {


  this.emit("sendToBot",this.encode({
    "destination" : ppp.PAYLOAD_ADDRESS.BROADCAST,
    "type":ppp.PAYLOAD_TYPE.SYSTEM,
    "payloadData":{
      "identifiant":ppp.PAYLOAD_SYSTEM_ID.name,
      "donnee":"AddancUi",
      "sens":1
    }
  }));

};
/**
*
* | identifiant (7b)| sens (1b) | donnée |
* sens (bit de poids fort du 1er octet) : 0 pour une requête, 1 pour une réponse
* identifiant (bits de poids faible du 1er octet) : identifiant du message (128 valeurs possibles)
* donnée : donnée du message, dépend de l'identifiant
*/

RobotControl.prototype.encodePayloadSystem = function(identifiant,sens,donnee) {

  var payload = new Buffer(1);

  payload.writeUInt8((sens<<7)|(identifiant),0);

  //console.log("payload system--->",payload,identifiant,donnee);

  if(donnee) {
    return Buffer.concat([payload,new Buffer(donnee)]);
  } else {
    return payload;
  }
};

RobotControl.prototype.encodePayloadAddanc = function(identifiant,donnee) {

  var payload = new Buffer(3);
  payload[0]=1;
  payload[1]=identifiant;
  payload[2]=donnee;
  //console.log("payload addanc--->",identifiant,donnee,payload);
  return payload;
};

RobotControl.prototype.encode = function (packet) {

  // P (1)
  // size (2)
  // src (1)
  // dst (1)
  // type (1)
  // headerCRC (2)
  // data (?)
  // data CRC (2)
  var packetHeader = new Buffer(9);
  console.log(packet);

  packetHeader.write("P",0);
  packetHeader.writeUInt8(packet.payload.length,1);
  packetHeader.writeUInt8(packet.source,2);
  packetHeader.writeUInt8(packet.destination,3);
  packetHeader.writeUInt8(packet.type,4);
  packetHeader.writeUInt16LE(this.calculCrc(packetHeader.slice(0,5)),5);
  packetHeader.writeUInt16LE(this.calculCrc(packet.payload),7);
  return  Buffer.concat([packetHeader, packet.payload]);
};

RobotControl.prototype.sendToBot = function(data) {
  var message = this.encode(data);
  if(message) {
    this.emit("sendToBot",message);
  }
};

RobotControl.prototype.send_ui = function(type,message) {
  this.emit("send_ui",type,message);
};


exports.RobotControl = RobotControl;
