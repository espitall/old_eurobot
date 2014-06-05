plasteamUI.directive("logger", ["socket",function(socket) {
  return {
    restrict :"EA",
    link : function(scope, elm, attrs){

      var container = elm[0];
      var element = document.createElement("div");
      container.appendChild(element);
      container.id = "log";

      var etat = {
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

      var compteur = 0;

      function clear() {
        element.innerHTML = "";
      }

      function log(message,type) {

        var item = document.createElement("p");

        switch (type) {
          case etat.debug:
            item.className="debug";
            if(filtre.debug){
              item.className+=" hide";
            }
            break;
          case etat.info:
            item.className="info";
            if(filtre.info){
              item.className+=" hide";
            }
            break;
          case etat.warning:
            item.className="warning";
            if(filtre.warning){
              item.className+=" hide";
            }
            break;
          case etat.erreur:
            item.className="error";
            if(filtre.error){
              item.className+=" hide";
            }
            break;
          default:
            item.className="";
            break;
        }


        compteur++;

        item.innerHTML = message ;

        element.appendChild(item);

        if(compteur===1000){
          clear();
          compteur=0;
        }

        if(autoScroll && compteur%2){
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

        if(classe == "tous") {
          $(".debug").css("display",display);
          $(".notice").css("display",display);
          $(".info").css("display",display);
          $(".warning").css("display",display);
          $(".error").css("display",display);
          filtre = {
            debug : false,
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
.directive("tabs",function(){
    return {
    restrict: 'EA',
    transclude: true,
    scope: {},
    controller: 'TabsCtrl',
    template: '<div class="tabbable"><ul class="nav nav-tabs"><li ng-repeat="pane in panes" ng-class="{active:pane.selected}"><a href="" ng-click="select(pane)">{{pane.heading}}</a></li></ul><div class="tab-content" ng-transclude></div></div>',
    replace: true
  };
})
.directive("pane",function(){
  return {
    require: '^tabs',
    restrict: 'EA',
    transclude: true,
    scope:{
      heading:'@'
    },
    link: function link(scope, element, attrs, tabsCtrl) {
      var getSelected, setSelected;
      scope.selected = false;
      if (attrs.active) {
        getSelected = $parse(attrs.active);
        setSelected = getSelected.assign;
        scope.$watch(
          function watchSelected() {return getSelected(scope.$parent);},
          function updateSelected(value) {scope.selected = value;}
        );
        scope.selected = getSelected ? getSelected(scope.$parent) : false;
      }
      scope.$watch('selected', function(selected) {
        if(selected) {
          tabsCtrl.select(scope);
        }
        if(setSelected) {
          setSelected(scope.$parent, selected);
        }
      });

      tabsCtrl.addPane(scope);
      scope.$on('$destroy', function() {
        tabsCtrl.removePane(scope);
      });
    },
    template: '<div class="tab-pane" ng-class="{active: selected}" ng-show="selected" ng-transclude></div>',
    replace: true
  };
})
.directive('restrict', function($parse) {
    return {
        restrict: 'A',
        require: 'ngModel',
        link: function(scope, iElement, iAttrs, controller) {

          function isNumber(n) {
            return !isNaN(parseFloat(n)) && isFinite(n);
          }

          scope.$watch(iAttrs.ngModel, function(value) {


              if (!value ) {
                  return;
              } else {
                $parse(iAttrs.ngModel).assign(scope,parseFloat(value));
                //$parse(iAttrs.ngModel).assign(scope,parseFloat(parseFloat(value).toFixed(4)));
                return;
              } /*else {
                $parse(iAttrs.ngModel).assign(scope, value.replace(/[^0-9\.]/g,''));
              }*/
          });
        }
    };
})
.directive("icarussvg",["Icarus",function(Icarus) {
  return {
    restrict:"EA",
    replace:true,
    templateUrl:"templates/icarus_simul.html",
    link : function link(scope, element, attrs) {

      scope.$on("icarusEvent",function(e,data){
        setPosition("haut",data.cote,data.position);
        setPosition("bas",data.cote,data.position);
      });

      var
      HAUTEUR_ROBOT = 100,
      LARGEUR_ROBOT = 90,

      OFFSET_BRAS_HAUT = 5,
      OFFSET_BRAS_BAS = 40,
      LONGUEUR_BRAS_HAUT = 80,
      LONGUEUR_BRAS_BAS = 45,
      LARGEUR_BRAS_HAUT = 5;

      scope.bras_haut_width = LONGUEUR_BRAS_HAUT;
      scope.bras_bas_width = LONGUEUR_BRAS_BAS;

      scope.offset_bras_haut = OFFSET_BRAS_HAUT ;
      scope.offset_bras_bas = OFFSET_BRAS_BAS ;

      scope.container_height = HAUTEUR_ROBOT;
      scope.container_width = LARGEUR_ROBOT + LONGUEUR_BRAS_HAUT*2;

      scope.robot_height = HAUTEUR_ROBOT;
      scope.robot_width = LARGEUR_ROBOT;

      scope.x_bras_gauche = 0;
      scope.x_bras_droit = LARGEUR_ROBOT + LONGUEUR_BRAS_HAUT;

      function setPosition(bras,cote,position) {
          var angle = 90;

          if(position !== 0) {
            angle = 0;
          }

          if(bras == "haut") {
            if(cote =="droit") {
              scope.angle_bras_haut_droit = angle;
            } else {
              scope.angle_bras_haut_gauche = -angle;
            }
          } else {
            if(cote =="droit") {
              scope.angle_bras_bas_droit = angle;
            } else {
              scope.angle_bras_bas_gauche = -angle;
            }
          }
      }



    }
  };
}])
.directive("knob",function(){
    return {
    restrict:"EA",
    replace:true,
    transclude: false,
    scope: {
      value:"=",
      onChange:"&"
    },
    template: "<input class='knob' ng-model='value' type='text'>",
    link : function link(scope, element , attrs) {

      $(element)//.val(attrs.value)
      .knob({
        'width':80,
        'height':80,
        'min':600,
        'max':2400,
        'thickness':0.5,
        'fgColor':attrs.color,
        'inputColor':attrs.color,
        'release' : function (v) {
          scope.value = v;
          scope.onChange();
        }
      });


      scope.$watch("value",function(v){
        $(element).val(v).trigger('change');
      });
    }
  };
})
.directive("popupable",function(){

  return {
    restrict:"A",
    replace:true,
    //template:"<div class=''></div>",
    link : function link(scope,element,attrs) {

      element.addClass("draggable");
      //element.addClass("popup");
      attrs.$set('draggable', 'true');


      var divvide = document.createElement("div");
      var dropDiv = document.createElement("div");
      dropDiv.className = "dropDiv hidden";
      dropDiv.draggable = true;

      dropDiv.innerHTML = "Emplacement des commandes";

      element.parent()[0].insertBefore(dropDiv,element[0]);

      var dragFlag = false;

      element.bind('dragstart', function(evt) {
        //Supprime l'image par defaut pour un drag
        evt.dataTransfer.setDragImage(divvide,0,0);

        element.addClass("popup");
        dropDiv.className = "dropDiv";
      });

      // Permet d'activer l'evenement drop sur chrome
      dropDiv.addEventListener("dragover",function(evt) {
        dropDiv.className = "dropDiv dragOver";
        evt.preventDefault();
        return false;
      },false);


      dropDiv.addEventListener("drop",function(evt) {
        element.removeClass("popup");
        dropDiv.className = "dropDiv hidden";

      },false);

      dropDiv.addEventListener('dragleave', function(evt) {
        dropDiv.className = "dropDiv";
        element.addClass("popup");
      });

      element.bind('drag', function(e) {
        if(e.x !== 0) {
          element.css({
            "top": e.y+"px",
            "left":e.x+"px"
          });
        }
      });
    }

  };
});
