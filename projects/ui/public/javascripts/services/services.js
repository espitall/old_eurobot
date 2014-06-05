'use strict';

plasteamUI

.factory("socket", ["$rootScope",function($rootScope) {

  var socket = io.connect();
  var $scope = $rootScope;
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

