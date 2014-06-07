'use strict'

var binary = require('binary');

var node_addr = {
  sbrain: 0x00,
  pbrain: 0x01,
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
    },

    send: function(rctrl, source, level, msg) {
      rctrl.send_ui("log", {
        "src": source,
        "level": level,
        "msg": msg,
      });

      console.log("[" + level.toUpperCase() + "] [" + source + "] " + msg);
    },

  }
};

module.exports = payloads;
