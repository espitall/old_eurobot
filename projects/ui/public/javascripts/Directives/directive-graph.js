plasteamUI.directive("graph",["Graph",function(Graph) {
    return {
        restrict: 'E',
        replace : true,
        template : "<div class='divgraph'></div>",
        link: function(scope, elem, attrs) {
           var graph = null,
               options= {
                "shadowSize":0,
                "xaxis" : {
                  "mode" : 'time',
                  "labelsAngle" : 45
                },
                "selection" : {
                  "mode" : 'x'
                },
                "HtmlText" : false
               },
               data_consigne = [],
               data_consigne_filtre = [],
               data_retour = [],
               data_output = [],
               tend=0, // timestamp max du graph
               TMAX=10000; // nombre de ms max pour un graph

              function clearGraph(){
                data_consigne = [];
                data_consigne_filtre = [];
                data_retour = [];
                data_output = [];
              }

              scope.$on("clearGraphEvent",function(){
                clearGraph();
              });

              Flotr.EventAdapter.observe(elem[0], 'flotr:select', function (area) {

                var data = [];

                if(attrs.simple) {
                   data = [data_output] ;
                } else {
                  data = [data_consigne,data_consigne_filtre,data_retour,data_output];
                }

                graph = drawGraph(data,{
                  xaxis: {min:area.x1, max:area.x2,"mode" : 'time'},
                  yaxis: {min:area.y1, max:area.y2}
                });
              });


              Flotr.EventAdapter.observe(elem[0], 'flotr:click', function () {

                if(attrs.simple) {
                   drawGraph([data_output]);
                } else {
                  drawGraph([data_consigne,data_consigne_filtre,data_retour,data_output]);
                }

              });

              function drawGraph (data,opts) {

                var o = Flotr._.extend(Flotr._.clone(options), opts || {});

                return Flotr.draw(elem[0], data, o);
              }

            if(attrs.simple) {
              scope.$watch(attrs.ngModel, function(v){
                data_output.push([v.time,v.data]);
                graph = drawGraph([data_output]);

              },true);
            } else {
              scope.$watch(attrs.ngModel, function(v){
                if(Graph.option.autoClear && (tend === 0 || v.time.getTime() > tend)) {
                 clearGraph();
                 tend= v.time.getTime() + 10000;
                }

                data_consigne.push([v.time,v.consigne]);
                data_consigne_filtre.push([v.time,v.consigne_filtre]);
                data_retour.push([v.time,v.retour]);
                data_output.push([v.time,v.output]);

                graph = drawGraph([data_consigne,data_consigne_filtre,data_retour,data_output]);

              },true);
            }

        }
    };
}]);
