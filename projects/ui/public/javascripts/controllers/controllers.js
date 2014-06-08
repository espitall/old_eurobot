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
		socket.emit("command",{payload: "system", func: "reset", destination: "broadcast"});
	};

	$scope.stop = function(){
		socket.emit("command",{payload: "system", func: "stop", destination: "broadcast"});
	};
}

function TabsCtrl($scope, $element) {

  var panes = $scope.panes = [];

  this.select = $scope.select = function selectPane(pane) {
    angular.forEach(panes, function(pane) {
      pane.selected = false;
    });
    pane.selected = true;
  };

  this.addPane = function addPane(pane) {
    if (!panes.length) {
      $scope.select(pane);
    }
    panes.push(pane);
  };

  this.removePane = function removePane(pane) {
    var index = panes.indexOf(pane);
        panes.splice(index, 1);
    //Select a new pane if removed pane was selected
    if (pane.selected && panes.length > 0) {
      $scope.select(panes[index < panes.length ? index : index-1]);
    }
  };
}

TabsCtrl.$inject = ['$scope', '$element'];
