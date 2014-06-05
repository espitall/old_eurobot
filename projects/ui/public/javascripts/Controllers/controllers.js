/* Controllers */

function HeadControl($scope,socket) {

	$scope.reset = function(){
		socket.emit("commande",{"id":"reset"});
	};

	$scope.name = function(){
		socket.emit("commande",{"id":"name"});
	};

}

HeadControl.$inject = ['$scope','socket'];

function LogCtrl($scope,Logger) {

  $scope.clearlog = function() {
    Logger.clear();
  };

  $scope.error = true ;
  $scope.warning = true ;
  $scope.info = true ;
  $scope.debug = true ;

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

function StratControl($scope,socket) {

	$scope.start = function(){
		socket.emit("commande",{"id":"strat","data":"start"});
	};
}

StratControl.$inject = ['$scope','socket'];


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

function IcarusActionneursCtrl($scope, Icarus, socket){

  $scope.icarusGauche = Icarus.getDefautValues();
  /*$scope.icarusGauche = {
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
  };*/
  $scope.icarusDroit = Icarus.getDefautValues();

  socket.on("icarus.position",function(data){
    if(data.src == "Lug/left") {
      Icarus.setUIPosition({"cote":"gauche","position":data.donnee});
      $scope.icarusGauche.position = data.donnee;
    } else {
      Icarus.setUIPosition({"cote":"droit","position":data.donnee});
      $scope.icarusDroit.position = data.donnee;
    }
    //console.log(data);
  });

  socket.on("icarus.config",function(data){

    if(data.src == "Lug/left") {
      switch(data.donnee.conf) {
        case 0:
          $scope.icarusGauche.angle.bas.vertical = data.donnee.angle;
          break;
        case 1:
          $scope.icarusGauche.angle.bas.horizontal = data.donnee.angle;
          break;
        case 2:
          $scope.icarusGauche.angle.haut.vertical = data.donnee.angle;
          break;
        case 3:
          $scope.icarusGauche.angle.haut.horizontal = data.donnee.angle;
          break;
      }
    } else {

    }
  });


  Icarus.getRobotValues();


  $scope.changeAngle = function(icarus,element){
    Icarus.setAngle(icarus,element,$scope.icarusGauche.angle);
  };

  $scope.setEtatIcarusGauche = function(){
    Icarus.setPosition("gauche",!$scope.icarusGauche.position);
  };

  $scope.setEtatIcarusDroit = function(){
    Icarus.setPosition("droit",!$scope.icarusDroit.position);
  };
}



function CarteCtrl($scope,socket) {

  socket.on("position",function(data){
    console.log(data);
  });

  $scope.robot1 = {
    x : 100,
    y : 100,
    a : 10
  };

  $scope.robot2 = {
    x : 200,
    y : 200,
    a : 0
  };
}

function CommandesCtrl($scope, Commandes){


$scope.setD = function(carte) {
  Commandes.go({
    "carte":carte,
    "flag":0,
    "data": {
      "d" : $scope[carte].d
    }
  });
};

$scope.setA = function(carte) {
  Commandes.go({
    "carte":carte,
    "flag":1,
    "data": {
      "a" : $scope[carte].a
    }
  });
};

$scope.setXY = function(carte) {
  Commandes.go({
    "carte":carte,
    "flag":2,
    "data": {
      "x" : $scope[carte].x,
      "y" : $scope[carte].y
    }
  });
};

$scope.setXYA = function(carte) {
  Commandes.go({
    "carte":carte,
    "flag":3,
    "data": {
      "x" : $scope[carte].x,
      "y" : $scope[carte].y,
      "a" : $scope[carte].a
    }
  });
};


}
