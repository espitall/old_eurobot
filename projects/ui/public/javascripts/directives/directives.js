'use strict'

plasteamUI

.directive("logger", ["socket",function(socket) {
  return {
    restrict :"EA",
    link : function(scope, elm, attrs){

      var container = elm[0];
      var element = document.createElement("div");
      container.appendChild(element);
      container.id = "log";

      var filtre = {
        debug : false,
        info : false,
        warning : false,
        erreur : false
      };

      var autoScroll = true ;

      var count = 0;

      function clear() {
        element.innerHTML = "";
      }

      function log(level, source, msg) {
        var item = document.createElement("p");
        item.className = level;
        if(filtre[level]) {
          item.className += " hide";
        }
        
        count++;

        item.innerHTML = "[" + source + "] " + msg;
        element.appendChild(item);

        if(count === 1000) {
          clear();
          count = 0;
        }

        if(autoScroll) {
          container.scrollTop = container.scrollHeight;
        }

      }

      socket.on("log", function (data) {
        log(data.level, data.src, data.msg);
      });

      scope.$on('clearLogEvent', function() {
          clear();
      });

      scope.$on('configLogEvent', function(e,data) {
          autoScroll = data.autoScroll;
      });

      scope.$on('filtreLogEvent',function(e,classe,etat) {

        var display = (etat) ? "block" : "none" ;

        if(classe == "all") {
          $(".debug").css("display",display);
          $(".info").css("display",display);
          $(".warning").css("display",display);
          $(".error").css("display",display);
          filtre = {
            debug : false,
            info : false,
            warning : false,
            erreur : false
          };
        } else {
          $("."+classe).css("display",display);
          filtre[classe] = !etat ;
        }

      });

    }
  };

}])
