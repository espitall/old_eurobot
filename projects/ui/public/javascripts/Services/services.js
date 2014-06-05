'use strict';

plasteamUI
.factory("socket", ["$rootScope",function($rootScope) {

  var socket = io.connect();
  var $scope = $rootScope;
  //var $scope = angular.element(document).injector().get('$rootScope');
  return {
      on: function (eventName, callback) {
        socket.on(eventName, function () {
          var args = arguments;
          $scope.$apply(function () {
            callback.apply(socket, args);
          });
        });
      },
      emit: function (eventName, data, callback) {
        socket.emit(eventName, data, function () {
          var args = arguments;
          $scope.$apply(function () {
            if (callback) {
              callback.apply(socket, args);
            }
          });
        });
      }
    };
}])
.factory("Logger", ["$rootScope",function($rootScope) {
  return {
    clear : function(){
      $rootScope.$broadcast("clearLogEvent");
    },
    filtre : function (classe,etat) {
      $rootScope.$broadcast("filtreLogEvent",classe,etat);
    },
    config : function (config) {
      $rootScope.$broadcast("configLogEvent",config);
    }
  };
}])
.factory("Graph",["$rootScope",function($rootScope) {
  return {
    option : {
      autoClear : true
    },
    clear : function(){
      $rootScope.$broadcast("clearGraphEvent");
    }
  };
}])
.factory("Icarus",["$rootScope","socket",function($rootScope,socket) {

  var ICARUS_DEFAULT_VALUES = {
    "position" : 2,
    "angle" : {
      "haut": {
        "horizontal":1000,
        "vertical":1000
      },
      "bas": {
        "horizontal":1000,
        "vertical":1000
      }
    }
  };

  return {
    getDefautValues : function(){
      return angular.copy(ICARUS_DEFAULT_VALUES);
    },
    getRobotValues : function(){
      socket.emit("commande",{
        id:"icarus",
        data:{"type":"init","icarus":"gauche"}
      });
      socket.emit("commande",{
        id:"icarus",
        data:{"type":"init","icarus":"droit"}
      });
    },
    setAngle : function(icarus,element,angle) {
      switch (element) {
        case "bas.vertical":
            angle = angle.bas.vertical;
            break;
        case "haut.vertical":
            angle = angle.haut.vertical;
            break;
        case "bas.horizontal":
            angle = angle.bas.horizontal;
            break;
        case "haut.horizontal":
            angle = angle.haut.horizontal;
            break;
      }
      socket.emit("commande",{
        id:"icarus",
        data:{
          "type":"config",
          "element":element,
          "W":true,
          "angle":angle,
          "icarus":icarus
        }
      });
    },
    setUIPosition : function(data) {
      $rootScope.$broadcast("icarusEvent",data);
    },
    setPosition : function(cote,position) {
      socket.emit("commande",{
        id:"icarus",
        data:{
          "icarus":cote,
          "type":"action",
          "position":(position) ? 0 : 1
        }
      });
    }
  };
}])
.factory("Commandes",["socket",function(socket) {
  return {
    go : function(data) {
      socket.emit("commande", {
        "id":"commandes",
        "data": data
      });
    }
  };

}]);
