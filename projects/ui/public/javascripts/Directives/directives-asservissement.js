plasteamUI.directive("asservissement",["socket","Graph",function(socket, Graph) {
  return {
    "restrict":"EA",
    "templateUrl":"templates/asservissement.html",
    "controller" : function($scope, $attrs){

      var flagRampes = false,
          flagMoteurs = false,
          flagPID = false,
          flagPosition = false;

          $scope.commande = {
            rampes : {
              vmax_d : 0,
              amax_d : 0,
              vmax_a : 0,
              amax_a : 0
            },
            PID : {
              kp_d   : 0,
              kd_d   : 0,
              alpha_d: 0,
              ki_d   : 0,
              imax_d : 0,
              omax_d : 0,
              kp_a   : 0,
              kd_a   : 0,
              alpha_a: 0,
              ki_a   : 0,
              imax_a : 0,
              omax_a : 0
            },
            moteurs :{
              coef_droit : 0,
              min_droit  : 0,
              coef_gauche: 0,
              min_gauche : 0
            },
            position : {
              coef_droit : 0,
              coef_gauche: 0,
              tick_cm    : 0,
              tick_deg   : 0
            }
          };
        $scope.stream = {
          distance : {
            time : new Date(),
            consigne : 0,
            consigne_filtre : 0,
            retour : 0,
            output : 0
          },
          angle : {
            time : new Date(),
            consigne : 0,
            consigne_filtre : 0,
            retour : 0,
            output : 0
          },
          moteurG : {
            time : new Date(),
            data : 0
          },
          moteurD : {
            time : new Date(),
            data : 0
          }
        };

      $scope.streamAsservOn = false ;



      $scope.$watch("commande.rampe",function(newValue, oldValue) {
        flagRampes = true;
      },true);
      $scope.$watch("commande.moteurs",function(newValue, oldValue) {
        flagMoteurs = true;
      },true);
      $scope.$watch("commande.PID",function(newValue, oldValue) {
        flagPID = true;
      },true);
      $scope.$watch("commande.position",function(newValue, oldValue) {
        flagPosition = true;
      },true);

      function lire() {
        socket.emit("commande",{"id":"asserv","RW":"lecture","type":"rampes","carte":$attrs.robot});
        socket.emit("commande",{"id":"asserv","RW":"lecture","type":"moteurs","carte":$attrs.robot});
        socket.emit("commande",{"id":"asserv","RW":"lecture","type":"position","carte":$attrs.robot});
        socket.emit("commande",{"id":"asserv","RW":"lecture","type":"PID","carte":$attrs.robot});
      }

      function maj_UI(donnees) {
        if(donnees.flag === 0 ) {
          $scope.commande.rampes.vmax_d = donnees.commande.vmax_d;
          $scope.commande.rampes.amax_d = donnees.commande.amax_d;
          $scope.commande.rampes.vmax_a = donnees.commande.vmax_a;
          $scope.commande.rampes.amax_a = donnees.commande.amax_a;
        } else if(donnees.flag === 1) {
          $scope.commande.PID.kp_d = donnees.commande.kp_d;
          $scope.commande.PID.kd_d = donnees.commande.kd_d;
          $scope.commande.PID.alpha_d = donnees.commande.alpha_d;
          $scope.commande.PID.ki_d = donnees.commande.ki_d;
          $scope.commande.PID.imax_d = donnees.commande.imax_d;
          $scope.commande.PID.omax_d = donnees.commande.omax_d;
          $scope.commande.PID.kp_a = donnees.commande.kp_a;
          $scope.commande.PID.kd_a = donnees.commande.kd_a;
          $scope.commande.PID.alpha_a = donnees.commande.alpha_a;
          $scope.commande.PID.ki_a = donnees.commande.ki_a;
          $scope.commande.PID.imax_a = donnees.commande.imax_a;
          $scope.commande.PID.omax_a = donnees.commande.omax_a;
        } else if(donnees.flag === 2) {
          $scope.commande.moteurs.coef_droit = donnees.commande.coef_droit;
          $scope.commande.moteurs.min_droit = donnees.commande.min_droit;
          $scope.commande.moteurs.coef_gauche = donnees.commande.coef_gauche;
          $scope.commande.moteurs.min_gauche = donnees.commande.min_gauche;
        } else if(donnees.flag === 3) {
          $scope.commande.position.coef_droit = donnees.commande.coef_droit;
          $scope.commande.position.coef_gauche = donnees.commande.coef_gauche;
          $scope.commande.position.tick_cm = donnees.commande.tick_cm;
          $scope.commande.position.tick_deg = donnees.commande.tick_deg;
        }
      }

      function plotStream(data) {
        var time = new Date();
        $scope.stream.distance.time = time;
        $scope.stream.distance.consigne = data.d_consigne;
        $scope.stream.distance.consigne_filtre = data.d_consigne_filtre;
        $scope.stream.distance.retour = data.d_retour;
        $scope.stream.distance.output = data.d_output*100;

        $scope.stream.angle.time = time;
        $scope.stream.angle.consigne = data.a_consigne;
        $scope.stream.angle.consigne_filtre = data.a_consigne_filtre;
        $scope.stream.angle.retour = data.a_retour;
        $scope.stream.angle.output = data.a_output;

        $scope.stream.moteurG.time = time;
        $scope.stream.moteurD.time = time;
        $scope.stream.moteurG.data = data.moteur_gauche*100;
        $scope.stream.moteurD.data = data.moteur_droit*100;

        //console.log(data);

      }

      socket.on("asserv.config",function(data){
        if($attrs.robot == "archi" && data.src == "Minerve") {
          maj_UI(data.donnee);
        } else if ($attrs.robot == "duchesse" && data.src == "Chausette") {
          maj_UI(data.donnee);
        }
      });

      var flagBoutonAsserv = false;
      socket.on("asserv.stream",function(data){

        if(!flagBoutonAsserv) {
          $scope.streamAsservOn = true;
        }

        if($attrs.robot == "archi" && data.src == "Minerve") {
          plotStream(data.donnee);
        } else if ($attrs.robot == "duchesse" && data.src == "Chausette") {
          plotStream(data.donnee);
        }
      });

      $scope.lire = function(argument) {
        lire();
      };

      $scope.appliquer = function(){

        if(flagRampes){
          socket.emit("commande",{"id":"asserv","RW":"ecriture","carte":$attrs.robot,"type":"rampes","data":$scope.commande.rampes});
          flagRampes = false;
        }
        if(flagMoteurs){
          socket.emit("commande",{"id":"asserv","RW":"ecriture","carte":$attrs.robot,"type":"moteurs","data":$scope.commande.moteurs});
          flagMoteurs = false;
        }
        if(flagPID){
          socket.emit("commande",{"id":"asserv","RW":"ecriture","carte":$attrs.robot,"type":"PID","data":$scope.commande.PID});
          flagPID = false;
        }
        if(flagPosition){
          socket.emit("commande",{"id":"asserv","RW":"ecriture","carte":$attrs.robot,"type":"position","data":$scope.commande.position});
          flagPosition = false;
        }
      };

      $scope.clearGraph = function() {
        Graph.clear();
      };

      $scope.startStopAsserv = function () {

        socket.emit("commande",{"id":"asserv.stream","carte":$attrs.robot,"etat":!$scope.streamAsservOn});

        $scope.streamAsservOn = !$scope.streamAsservOn;

        if(!$scope.streamAsservOn) {
          flagBoutonAsserv = true;
          setTimeout(function(){
            flagBoutonAsserv = false;
          },500);
        }

      };

      $scope.$watch("autoClear",function(newValue){
        Graph.option.autoClear = newValue;
      });

      //  Initialisation

      lire();

    }
  };
}]);
