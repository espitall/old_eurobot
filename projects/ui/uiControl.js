var events = require('events'),
    sys = require('sys');

var ppp = require("./pppPayloads");


function UiControl() {
    events.EventEmitter.call(this);
}

sys.inherits(UiControl, events.EventEmitter);

UiControl.prototype.receive = function(message) {
    switch (message.id) {

        case "reset":
            this.sendToBot({
                "type":ppp.PAYLOAD_TYPE.SYSTEM,
                "destination":ppp.PAYLOAD_ADDRESS.BROADCAST,
                "payloadData":{
                    "identifiant" : ppp.PAYLOAD_SYSTEM_ID.reset,
                    "sens":0,
                    "donnee" : ""
                }
            });
        break;

        case "stop":
            this.sendToBot({
                "type":ppp.PAYLOAD_TYPE.SYSTEM,
                "destination":ppp.PAYLOAD_ADDRESS.BROADCAST,
                "payloadData":{
                    "identifiant" : ppp.PAYLOAD_SYSTEM_ID.stop,
                    "sens":0,
                    "donnee" : ""
                }
            });
        break;


        /*
        case "name":
            this.sendToBot({"type":"bonjour"});
        break;
        case "asserv":
            this.gestionCommandes(message);
        break;
        case "asserv.stream":
            this.gestionStream(message.carte,message.etat);
        break;
        case "strat":
            this.gestionStrat(message.data);
        break;
        case "commandes":
            this.gestionCommandesPosition(message.data);
        break;
        case "icarus":
            this.gestionIcarus(message.data);
        break;
        case "test":
            this.sendToBot({"type":"bonjour"});
        break;
        */
        default:
        break;

    }
};

UiControl.prototype.gestionCommandesPosition = function(message){

    var donnee;

    switch (message.flag) {
        case 0 :
            donnee = new Buffer(5);

            donnee.writeFloatLE(parseFloat(message.data.d) || 0,1);
        break;
        case 1 :
            donnee = new Buffer(5);
            donnee.writeFloatLE(parseFloat(message.data.a) || 0,1);
        break;
        case 2 :
            donnee = new Buffer(9);
            donnee.writeFloatLE(parseFloat(message.data.x) || 0,1);
            donnee.writeFloatLE(parseFloat(message.data.y) || 0,3);
        break;
        case 3 :
            donnee = new Buffer(13);
            donnee.writeFloatLE(parseFloat(message.data.x) || 0,1);
            donnee.writeFloatLE(parseFloat(message.data.y) || 0,3);
            donnee.writeFloatLE(parseFloat(message.data.a) || 0,5);
        break;

    }

    donnee[0] = message.flag;

    this.sendToBot({
        "type":ppp.PAYLOAD_ADDANC.COMMANDES.ID_PAYLOAD,
        "destination":(message.carte == "archi") ? ppp.PAYLOAD_ADDRESS.Minerve : ppp.PAYLOAD_ADDRESS.Chaussette ,
        "payloadData":{
            "donnee" :donnee
        }
    });




};

UiControl.prototype.gestionStream = function(carte,etat){
    this.sendToBot({
        "type":ppp.PAYLOAD_ADDANC.ASSERV.ID_PAYLOAD,
        "destination":(carte == "archi") ? ppp.PAYLOAD_ADDRESS.Minerve : ppp.PAYLOAD_ADDRESS.Chaussette ,
        "payloadData":{
            "id" : ppp.PAYLOAD_ADDANC.ASSERV.ID_GRAPH,
            "flag" : (etat) ? 1 : 0,
            "donnee" :""
        }
    });
};

UiControl.prototype.gestionCommandes = function(message){
    var donnee = "",
    flag;
    switch (message.type) {
        case "rampes":
            flag = 0x00;
            if(message.RW == "ecriture" ) {
                donnee = new Buffer(16);
                donnee.writeFloatLE((parseFloat(message.data.vmax_d) || 0), 0);
                donnee.writeFloatLE((parseFloat(message.data.amax_d) || 0), 4);
                donnee.writeFloatLE((parseFloat(message.data.vmax_a) || 0), 8);
                donnee.writeFloatLE((parseFloat(message.data.amax_a) || 0), 12);
            }
            break;
        case "PID":
            flag = 0x01;
            if(message.RW == "ecriture" ) {
                donnee = new Buffer(48);
                donnee.writeFloatLE((parseFloat(message.data.kp_d   ) || 0), 0);
                donnee.writeFloatLE((parseFloat(message.data.kd_d   ) || 0), 4);
                donnee.writeFloatLE((parseFloat(message.data.alpha_d) || 0), 8);
                donnee.writeFloatLE((parseFloat(message.data.ki_d   ) || 0), 12);
                donnee.writeFloatLE((parseFloat(message.data.imax_d ) || 0), 16);
                donnee.writeFloatLE((parseFloat(message.data.omax_d ) || 0), 20);
                donnee.writeFloatLE((parseFloat(message.data.kp_a   ) || 0), 24);
                donnee.writeFloatLE((parseFloat(message.data.kd_a   ) || 0), 28);
                donnee.writeFloatLE((parseFloat(message.data.alpha_a) || 0), 32);
                donnee.writeFloatLE((parseFloat(message.data.ki_a   ) || 0), 36);
                donnee.writeFloatLE((parseFloat(message.data.imax_a ) || 0), 40);
                donnee.writeFloatLE((parseFloat(message.data.omax_a ) || 0), 44);
            }
            break;
        case "moteurs":
            flag = 0x02;
            if(message.RW == "ecriture" ) {
                donnee = new Buffer(16);
                donnee.writeFloatLE((parseFloat(message.data.coef_droit ) || 0), 0);
                donnee.writeUInt32LE((parseFloat(message.data.min_droit  )|| 0), 4);
                donnee.writeFloatLE((parseFloat(message.data.coef_gauche) || 0), 8);
                donnee.writeUInt32LE((parseFloat(message.data.min_gauche )|| 0), 12);

            }
            break;
        case "position":
            flag = 0x03;
            if(message.RW == "ecriture" ) {
                donnee = new Buffer(16);
                donnee.writeFloatLE((parseFloat(message.data.coef_droit ) || 0), 0);
                donnee.writeFloatLE((parseFloat(message.data.coef_gauche) || 0), 4);
                donnee.writeFloatLE((parseFloat(message.data.tick_cm    ) || 0), 8);
                donnee.writeFloatLE((parseFloat(message.data.tick_deg   ) || 0), 12);
            }
            break;
    }

    if(message.RW == "ecriture") {
        flag = flag | 0x80 ;
    }

    this.sendToBot({
        "type":ppp.PAYLOAD_ADDANC.ASSERV.ID_PAYLOAD,
        "destination":(message.carte == "archi") ? ppp.PAYLOAD_ADDRESS.Minerve : ppp.PAYLOAD_ADDRESS.Chaussette ,
        "payloadData":{
            "id" : ppp.PAYLOAD_ADDANC.ASSERV.ID_CONFIG,
            "flag" : flag,
            "donnee" : donnee
        }
    });
};

/**
* gestionIcarus : s'occupe de traiter les demandes pour les bras icarus droite et gauche
* message -> le message provenant de l'ihm
* -- --
* autoStatus -> permet une lecture auto si le message n'est pas une demande de status
*/
UiControl.prototype.gestionIcarus = function(message,autoStatus) {

    var donnee="",
    identifiant;

    if(message.type == "action") {
        donnee = new Buffer(1);
        donnee[0] = message.position;

        identifiant = ppp.PAYLOAD_ADDANC.ICARUS.ID_ACTION;
    } else if(message.type == "config") {

        if(message.W) {
            donnee = new Buffer(4);
        } else {
            donnee = new Buffer(1);
        }

        identifiant = ppp.PAYLOAD_ADDANC.ICARUS.ID_CONFIG;

        switch (message.element) {
            case "bas.vertical":
                donnee[0] = 0x00;
                break;
            case "haut.vertical":
                donnee[0] = 0x01;
                break;
            case "bas.horizontal":
                donnee[0] = 0x02;
                break;
            case "haut.horizontal":
                donnee[0] = 0x03;
                break;
        }


        if(message.W) {
           donnee[1] = donnee[1] | 0x80 ;
           donnee.writeUInt16LE(message.angle,2);
        }

    } else if(message.type == "status") {
        identifiant = ppp.PAYLOAD_ADDANC.ICARUS.ID_ACTION;
        donnee = new Buffer(1);
        donnee[0] = 0x55;
    } else if(message.type == "init") {

        message.type = "config";
        message.element = "bas.vertical";
        this.gestionIcarus(message);
        message.element = "haut.vertical";
        this.gestionIcarus(message);
        message.element = "bas.horizontal";
        this.gestionIcarus(message);
        message.element = "haut.horizontal";
        this.gestionIcarus(message);

        message.type = "status";
        this.gestionIcarus(message);
    } else {
        return ;
    }


    this.sendToBot({
        "type":ppp.PAYLOAD_ADDANC.ICARUS.ID_PAYLOAD,
        "destination":(message.icarus == "gauche") ? ppp.PAYLOAD_ADDRESS.Lug_left : ppp.PAYLOAD_ADDRESS.Lug_right ,
        "payloadData":{
            "id" : identifiant,
            "donnee" : donnee
        }
    });

    if(typeof(autoStatus) !=="undefined" && autoStatus) {
        this.gestionIcarus(message,false);
    }
};



/**
* TODO : Systeme de strat
*
* Paquet TIRETTE
*
**/
UiControl.prototype.gestionStrat =function(message) {

    switch(message) {
        case "stop":
            this.sendToBot({
                "type":ppp.PAYLOAD_ADDANC.SENSOR.ID_PAYLOAD,
                "destination":ppp.PAYLOAD_ADDRESS.Hreidmarr_strategie,
                "payloadData":{
                    "id":ppp.PAYLOAD_ADDANC.SENSOR.TIRETTE.ID,
                    "donnee":ppp.PAYLOAD_ADDANC.SENSOR.TIRETTE.PLUGGED
                }
            });
            break;
        case "start":
            this.sendToBot({
                "type":ppp.PAYLOAD_ADDANC.SENSOR.ID_PAYLOAD,
                "destination":ppp.PAYLOAD_ADDRESS.Hreidmarr_strategie,
                "payloadData":{
                    "id":ppp.PAYLOAD_ADDANC.SENSOR.TIRETTE.ID,
                    "donnee":ppp.PAYLOAD_ADDANC.SENSOR.TIRETTE.UNPLUGGED
                }
            });
            break;
        default:
        break;


    }

};


UiControl.prototype.sendToBot = function(data) {
  this.emit("sendToBot",data);
};






exports.UiControl = UiControl ;
