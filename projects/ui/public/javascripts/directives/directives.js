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

      var state = {
        debug : 0,
        info : 1,
        warning : 2,
        erreur : 3
      };

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

      function log(message,type) {
        var item = document.createElement("p");

        switch (type) {
          case state.debug:
            item.className="debug";
            if(filtre.debug){
              item.className+=" hide";
            }
            break;

          case state.info:
            item.className="info";
            if(filtre.info){
              item.className+=" hide";
            }
            break;

          case state.warning:
            item.className="warning";
            if(filtre.warning){
              item.className+=" hide";
            }
            break;

          case state.erreur:
            item.className="error";
            if(filtre.error){
              item.className+=" hide";
            }
            break;

          default:
            item.className="";
            break;
        }

        count++;

        item.innerHTML = message ;
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
        log("["+data.src+"] "+data.msg,data.lvl);
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
