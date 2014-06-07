var pppPayloads = {

    PAYLOAD_TYPE : {
        SYSTEM : 0x00,
        LOG : 0x01
    },
    PAYLOAD_SYSTEM_ID :{
        ack : 0,
        nak : 1,
        ping : 2,
        traceroute : 3,
        name : 4,
        stop   : 5,
        reset : 6,
        supported : 7
    },

    PAYLOAD_ADDRESS : {
        sbrain : 0x00,
        pbrain : 0x01,
        UI: 0x80,
        BROADCAST : 0xff
    },


      /*
    PAYLOAD_ADDANC : {
        position : 0x80,
        COMMANDES : {
            ID_PAYLOAD : 0x85
        },
        ASSERV : {
            ID_PAYLOAD : 0x83,
            ID_CONFIG : 0x00,
            ID_GRAPH :0x01,
            DATA_TYPE : {
                RAMPES : 0,
                PID : 1,
                MOTEURS : 2,
                POSITION : 3
            }
        },
        SENSOR: {
            ID_PAYLOAD:0x81,
            TIRETTE:{
                ID:0,
                UNPLUGGED:0,
                PLUGGED:1
            }
        },
        ICARUS : {
            ID_PAYLOAD : 0x82,
            ID_ACTION : 0x00,
            ID_CONFIG : 0x01,
            ID_STATUS : 0x02
        }
    }

*/

} ;


module.exports = pppPayloads ;
