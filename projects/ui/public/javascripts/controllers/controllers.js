'use strict';
/* Controllers */


function LogCtrl($scope,Logger) {
  $scope.clearlog = function() {
    Logger.clear();
  };

  $scope.error = true;
  $scope.warning = true;
  $scope.info = true;
  $scope.debug = true;

  $scope.config = {
    autoScroll : true
  };

  $scope.$watch("config",function(oldV){
    Logger.config($scope.config);
  },true);
  $scope.$watch("error",function(oldV){
    Logger.filtre("error",oldV);
  });
  $scope.$watch("warning",function(oldV){
    Logger.filtre("warning",oldV);
  });
  $scope.$watch("info",function(oldV){
    Logger.filtre("info",oldV);
  });
  $scope.$watch("debug",function(oldV){
    Logger.filtre("debug",oldV);
  });
}


function HeadCtrl($scope,socket) {
	$scope.reset = function(){
		socket.emit("command",{"id":"reset"});
	};

	$scope.stop = function(){
		socket.emit("command",{"id":"stop"});
	};
}
