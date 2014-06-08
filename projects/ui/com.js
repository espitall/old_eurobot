'use strict'

var binary = require('binary');

var node_addr = {
  sbrain: 0x00,
  pbrain: 0x01,
  ui: 0x02,
  broadcast: 0xff,
}

var addrstr = function(addr) {
  for(var i  in node_addr) {
    if(node_addr[i] == addr) {
      return i;
    }
  }
}

var payloads = {
  system: {
    type: 0x00,

    stop: function(msg) {
      var packet = {}
      packet.payload = new Buffer(1);
      packet.type = this.type;
      packet.source = node_addr.ui;
      packet.destination = node_addr[msg.destination];

      packet.payload[0] = 0x05;
      return packet;
    },

    reset: function(msg) {
      var packet = {}
      packet.payload = new Buffer(1);
      packet.type = this.type;
      packet.source = node_addr.ui;
      packet.destination = node_addr[msg.destination];

      packet.payload[0] = 0x06;
      return packet;
    }
  },

  log: {
    type: 0x01,

    level: {
      debug: 0,
      info: 1,
      warning: 2,
      error: 3,
    },

    parse: function(rctrl, packet) {
      var payload = binary.parse(packet.data)
        .word8("level")
        .tap(function(vars){
          this.buffer("msg",packet.data.length - 1 );
        })
      .vars;

      var lvl = this.level.debug;
      for(var i  in this.level) {
        if(this.level[i] == payload.level) {
          lvl = i;
          break;
        }
      }

      this.send(rctrl, addrstr(packet.source), lvl, ''+payload.msg);
      return true;
    },

    send: function(rctrl, source, level, msg) {
      rctrl.send_ui("log", {
        "src": source,
        "level": level,
        "msg": msg,
      });

      console.log("[" + level.toUpperCase() + "] [" + source + "] " + msg);
    },

  },

  asserv: {
    type: 0x02,

    identifier: {
      debug_stream:   0x00,
      read_d_params:  0x01,
      read_a_params:  0x02,
      write_d_params: 0x03,
      write_a_params: 0x04,
    },

    read_d_params: function(msg) {
      var packet = {}
      packet.payload = new Buffer(1);
      packet.type = this.type;
      packet.source = node_addr.ui;
      packet.destination = node_addr[msg.destination];

      packet.payload[0] = this.identifier.read_d_params;
      return packet;
    },

    read_a_params: function(msg) {
      var packet = {}
      packet.payload = new Buffer(1);
      packet.type = this.type;
      packet.source = node_addr.ui;
      packet.destination = node_addr[msg.destination];

      packet.payload[0] = this.identifier.read_a_params;
      return packet;
    },

    parse: function(rctrl, packet) {
      switch(packet.data[0]) {
        case this.identifier.debug_stream:
          var payload = binary.parse(packet.data)
            .word8("identifier")
            .word32lu("timestamp_ms")
            .word32ls("dist_set_point")
            .word32ls("dist_feedback")
            .word32ls("dist_p")
            .word32ls("dist_d")
            .word32ls("dist_output")
            .word32ls("angu_set_point")
            .word32ls("angu_feedback")
            .word32ls("angu_p")
            .word32ls("angu_d")
            .word32ls("angu_output")
            .tap(function(vars){
              this.buffer("msg",packet.data.length - 1 );
            })
          .vars;

          rctrl.send_ui("asserv_stream", payload);
          return true;

        case this.identifier.read_d_params:
          var payload = binary.parse(packet.data)
            .word8("identifier")
            .word32ls("kp")
            .word32ls("kd")
            .word32ls("shift")
            .word32ls("amax")
            .word32ls("vmax")
            .vars;

          payload.type = "dist";
          payload.source = addrstr(packet.source);

          rctrl.send_ui("asserv_params", payload);
          return true;

        case this.identifier.read_a_params:
          var payload = binary.parse(packet.data)
            .word8("identifier")
            .word32ls("kp")
            .word32ls("kd")
            .word32ls("shift")
            .word32ls("amax")
            .word32ls("vmax")
            .vars;

          payload.type = "angu";
          payload.source = addrstr(packet.source);

          rctrl.send_ui("asserv_params", payload);
          return true;

      }

      return false;
    }
  },
};

module.exports = payloads;
