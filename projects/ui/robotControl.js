
var events = require('events'),
    sys = require('sys');
var ppp = require("./pppPayloads");
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




RobotControl.prototype.receive = function(message) {
 // console.log(message.data.toString());
  //if(message.type) {
    var payload = {};
    switch (message.type) {

      case ppp.PAYLOAD_TYPE.SYSTEM:
        payload = this.decodePayloadSystem(message.data);
        //console.log("payload System ["+payload.type+"] : ",payload.data);
        //console.log("payload System ["+payload.type+"] : ",payload.data.toString());
      break;

      case ppp.PAYLOAD_TYPE.LOG :
        payload = this.decodePayloadLog(message.data);
        this.sendToUi("log",{"src":this.decodeAddresse(message.source),"lvl":payload.niveau,"msg":payload.message});
        break;

      case ppp.PAYLOAD_ADDANC.ICARUS.ID_PAYLOAD :

        payload = this.decodePayloadIcarus(message.data);

        this.sendToUi(payload.msg_type,{"src":this.decodeAddresse(message.source),"donnee":payload.donnee});

      break;
      case ppp.PAYLOAD_ADDANC.ASSERV.ID_PAYLOAD :

      payload = this.decodePayloadAsserv(message.data);

        this.sendToUi(payload.msg_type,{"src":this.decodeAddresse(message.source),"donnee":payload.donnee});

      break;
      default:
        console.log("type ->" ,(message.type).toString(16));
      break;


    }

  //}
};

RobotControl.prototype.decodePayloadSystem = function(payload) {

  var donnee = "";
  var sens = payload[0]&0x1;
  var identifiant = payload[0]&0x7;


    switch(identifiant) {

      case ppp.PAYLOAD_SYSTEM_ID.ack:
        donnee = "ack";
        break;
      case ppp.PAYLOAD_SYSTEM_ID.nak:
        donnee = "nak";
        break;
      case ppp.PAYLOAD_SYSTEM_ID.ping:
        donnee = {"type":"ping","data":payload.slice(2,3)} ;
        break;
      case ppp.PAYLOAD_SYSTEM_ID.traceroute:
        break;
      case ppp.PAYLOAD_SYSTEM_ID.name:
        donnee = {"type":"Name","data":payload.slice(1,payload.length)} ;
        break;
      case ppp.PAYLOAD_SYSTEM_ID.stop:
        break;
      case ppp.PAYLOAD_SYSTEM_ID.reset:
        donnee = {"type":"reset"};
        break;
      case ppp.PAYLOAD_SYSTEM_ID.supported:
        break;
      default:
        break;
    }

  return donnee;
};


RobotControl.prototype.decodePayloadIcarus = function(payload) {
  var decodePayload = binary.parse(payload)
    .word8("id")
    .tap(function(vars){
      this.buffer("data",payload.length );
    })
    .vars;

    var msg_type = "icarus.",
    donnee = {};

    if(decodePayload.id === ppp.PAYLOAD_ADDANC.ICARUS.ID_CONFIG) {
      msg_type += "config";
      donnee = {
        conf : decodePayload.data.readUInt8(0)& 0x03,
        angle : decodePayload.data.readUInt16LE(1)
      };
    } else if(decodePayload.id === ppp.PAYLOAD_ADDANC.ICARUS.ID_STATUS) {
      msg_type += "position";
      donnee = decodePayload.data.readUInt8(0);
    }

    return {"msg_type":msg_type,"donnee":donnee};

};

RobotControl.prototype.decodePayloadAsserv = function(payload) {
  var decodePayload = binary.parse(payload)
    .word8("id")
    .tap(function(vars){
      this.buffer("data",payload.length - 1);
    })
    .vars;


    var msg_type = "",
    donnee = {};

    if(decodePayload.id === ppp.PAYLOAD_ADDANC.ASSERV.ID_CONFIG) {
      msg_type = "asserv.config";
      donnee = {
        flag : decodePayload.data.readUInt8(0),
        commande : {}
      };


      if(donnee.flag === ppp.PAYLOAD_ADDANC.ASSERV.DATA_TYPE.RAMPES) {
        donnee.commande = {
          "vmax_d":decodePayload.data.readFloatLE(1),
          "amax_d":decodePayload.data.readFloatLE(5),
          "vmax_a":decodePayload.data.readFloatLE(9),
          "amax_a":decodePayload.data.readFloatLE(13)
        };
        //console.log("---------------->",decodePayload.data.length,decodePayload.data);
      } else if (donnee.flag === ppp.PAYLOAD_ADDANC.ASSERV.DATA_TYPE.PID) {
        donnee.commande = {
          "kp_d":decodePayload.data.readFloatLE(1),
          "kd_d":decodePayload.data.readFloatLE(5),
          "alpha_d":decodePayload.data.readFloatLE(9),
          "ki_d":decodePayload.data.readFloatLE(13),
          "imax_d":decodePayload.data.readFloatLE(17),
          "omax_d":decodePayload.data.readFloatLE(21),
          "kp_a":decodePayload.data.readFloatLE(25),
          "kd_a":decodePayload.data.readFloatLE(29),
          "alpha_a":decodePayload.data.readFloatLE(33),
          "ki_a":decodePayload.data.readFloatLE(37),
          "imax_a":decodePayload.data.readFloatLE(41),
          "omax_a":decodePayload.data.readFloatLE(45)
        };
      } else if (donnee.flag === ppp.PAYLOAD_ADDANC.ASSERV.DATA_TYPE.MOTEURS) {
        donnee.commande = {
          "coef_droit":decodePayload.data.readFloatLE(1),
          "min_droit":decodePayload.data.readUInt32LE(5),
          "coef_gauche":decodePayload.data.readFloatLE(9),
          "min_gauche":decodePayload.data.readUInt32LE(13)
        };
      } else if (donnee.flag === ppp.PAYLOAD_ADDANC.ASSERV.DATA_TYPE.POSITION) {

        donnee.commande = {
          "coef_droit":decodePayload.data.readFloatLE(1),
          "coef_gauche":decodePayload.data.readFloatLE(5),
          "tick_cm":decodePayload.data.readFloatLE(9),
          "tick_deg":decodePayload.data.readFloatLE(13)
        };

      }
    } else if(decodePayload.id === ppp.PAYLOAD_ADDANC.ASSERV.ID_GRAPH) {
      msg_type = "asserv.stream";
      donnee = {
        "d_consigne":decodePayload.data.readFloatLE(0),
        "d_consigne_filtre":decodePayload.data.readFloatLE(4),
        "d_retour":decodePayload.data.readFloatLE(8),
        "d_output":decodePayload.data.readFloatLE(12),
        "d_i":decodePayload.data.readFloatLE(16),
        "a_consigne":decodePayload.data.readFloatLE(20),
        "a_consigne_filtre":decodePayload.data.readFloatLE(24),
        "a_retour":decodePayload.data.readFloatLE(28),
        "a_output":decodePayload.data.readFloatLE(32),
        "a_i":decodePayload.data.readFloatLE(36),
        "moteur_droit":decodePayload.data.readUInt32LE(40),
        "moteur_gauche":decodePayload.data.readUInt32LE(44)
      };

    } else if(decodePayload.id === 2 ) {
      msg_type = "asserv.2";
    }
  return {"msg_type":msg_type,"donnee":donnee};
  //return {"id":decodePayload.idPayload,"donnee":decodePayload.donnee};
};


RobotControl.prototype.decodePayloadLog = function(payload) {

  var decodePayload = binary.parse(payload)
    .word8("ackniveau")
    .tap(function(vars){
      this.buffer("message",payload.length - 1 );
    })
    .vars;

  //console.log("\n",payload,"\n",decodePayload.ackniveau,"\n",decodePayload.message.toString());

  return {"niveau":decodePayload.ackniveau&0x7,"message":decodePayload.message.toString()} ;
};

RobotControl.prototype.decodeAddresse = function(adresseCode) {
  var adresse = "" ;
  switch(adresseCode) {
    case ppp.PAYLOAD_ADDRESS.sbrain:
      adresse = "sbrain";
      break;
    case ppp.PAYLOAD_ADDRESS.pbrain:
      adresse = "pbrain";
      break;
    case ppp.PAYLOAD_ADDRESS.BROADCAST:
      adresse = "broadcast";
      break;
    default:
      adresse = "code "+adresseCode+" inconnu";
      break;
  }

  return adresse;

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
  /*
  if(this.paquet.Hcomplet) {

    this.paquet.data = Buffer.concat([this.paquet.data,paquet]);

    if(this.paquet.data.length -1  >= this.paquet.header.payloadSize) {

      var _this = this ;
      var decodePaquet = binary.parse(this.paquet.data)
        .tap(function(vars){
          this.buffer("payloadData",_this.paquet.header.payloadSize);
        })
        .vars
        ;

      if(this.valideCRC(this.paquet.data,this.paquet.header.payloadCRC)) {


        this.paquet.Hcomplet = false;


        if(this.paquet.data.length - 2 > this.paquet.header.payloadSize) {

          var paquetSuivant = this.paquet.data.slice(this.paquet.header.payloadSize+2);

          this.decode(paquetSuivant);
        }

      } else {
        console.log("mauvais crc trame");
        this.paquet.Hcomplet = false;
      }

    }
  } else {

    if(paquet.length > 7) {
      if(this.paquet.headerData.length>0) {
        paquet = Buffer.concat([this.paquet.headerData,paquet]);
      }
      var headerTemp = paquet.toString();
      var positionDebutPaquet = headerTemp.indexOf("P");

      if(positionDebutPaquet != -1) {

        paquet = paquet.slice(positionDebutPaquet);

        this.paquet.header = binary.parse(paquet)
        .word8("P")
        .word8("payloadSize")
        .word8("source")
        .word8("destination")
        .word8("type")
        .word16lu("headerCRC")
        .word16lu("payloadCRC")
        .vars
        ;
        this.paquet.headerData = new Buffer(0);

        if(this.valideCRC(paquet.slice(0,4),this.paquet.header.headerCRC)) {

          this.paquet.Hcomplet=true;
          this.paquet.data = new Buffer(0);
          if(paquet.length>8) {
            this.decode(paquet.slice(8));
          }
        } else {
          console.log(red,"mauvais CRC header",reset);
          this.paquet.Hcomplet=false;
        }
      } else {
        this.paquet.Hcomplet=false;
        this.paquet.headerData = new Buffer(0);
      }
    } else {
      this.paquet.headerData = Buffer.concat([this.paquet.headerData,paquet]);

      if(this.paquet.headerData.length  > 7) {
        this.decode(this.paquet.headerData);
      }
    }
  }
*/

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

RobotControl.prototype.encode = function (data) {
  //console.log(data);
  var paquet = {
    source : ppp.PAYLOAD_ADDRESS.UI,
    destination : data.destination,
    type : data.type
  };

  switch (data.type) {
    case ppp.PAYLOAD_TYPE.SYSTEM :
      paquet.data = this.encodePayloadSystem(
        data.payloadData.identifiant,
        data.payloadData.sens,
        data.payloadData.donnee
      );
    break;


    case ppp.PAYLOAD_ADDANC.SENSOR.ID_PAYLOAD:
      paquet.data = this.encodePayloadAddanc(
        data.payloadData.id,
        data.payloadData.donnee
      );

    break;

    case ppp.PAYLOAD_ADDANC.ICARUS.ID_PAYLOAD:
      paquet.data=new Buffer(1 + data.payloadData.donnee.length);
      paquet.data[0]=data.payloadData.id;

      if(data.payloadData.donnee.length > 0){
        data.payloadData.donnee.copy(paquet.data,1);
      }
      //console.log("==========*",paquet.data,data.payloadData.donnee.length);
    break;

    case ppp.PAYLOAD_ADDANC.ASSERV.ID_PAYLOAD :
      paquet.data=new Buffer(2 + data.payloadData.donnee.length);
      paquet.data[0]=data.payloadData.id;
      paquet.data[1]=data.payloadData.flag;

      if(data.payloadData.donnee.length > 0){
        data.payloadData.donnee.copy(paquet.data,2);
      }

    break;
    case ppp.PAYLOAD_ADDANC.COMMANDES.ID_PAYLOAD :
      paquet.data=data.payloadData.donnee;
    break;

    case "bonjour":

      this.bonjour();
    break;

    default:
    break;

  }

  // P (1)
  // size (2)
  // src (1)
  // dst (1)
  // type (1)
  // headerCRC (2)
  // data (?)
  // data CRC (2)
  if(typeof paquet.data !== "undefined") {
    var paquetHeader = new Buffer(9);

    paquetHeader.write("P",0);
    paquetHeader.writeUInt8(paquet.data.length,1);
    paquetHeader.writeUInt8(paquet.source,2);
    paquetHeader.writeUInt8(paquet.destination,3);
    paquetHeader.writeUInt8(paquet.type,4);
    paquetHeader.writeUInt16LE(this.calculCrc(paquetHeader.slice(0,5)),5);
    paquetHeader.writeUInt16LE(this.calculCrc(paquet.data),7);
    return  Buffer.concat([paquetHeader,paquet.data]);

  } else {
    return ;
  }
};

RobotControl.prototype.sendToBot = function(data) {
  var message = this.encode(data);
  if(message) {
    this.emit("sendToBot",message);
  }
};

RobotControl.prototype.sendToUi = function(type,message) {
  this.emit("sendToUi",type,message);
};


exports.RobotControl = RobotControl;
