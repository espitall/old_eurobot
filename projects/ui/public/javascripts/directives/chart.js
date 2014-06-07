plasteamUI.directive("chart",["socket", function(socket, Graph) {
  var uniqueId = 1;
  var baseWidth = 600;
  var baseHeight = 300;

  return {
    restrict: "EA",
    template: "<div></div>",
    link : function(scope, elm, attrs) {
      var div  = elm.find('div')[0];
      div.id = 'chart_' + (uniqueId++);
      console.log('id:' + div.id);

      var options = {
        width:  attrs.width  || baseWidth,
        height: attrs.height || baseHeight
      };

      var data = [];
      var series = attrs.series.split(";");
      for(var i in series) {
        var str  = series[i].trim();
        data.push({
          type: attrs.type,
          dataPoints: [],
          name: str,
          index: str,
          showInLegend: true,
        });
      }

      div.width = options.width;
      div.height = options.height;
      var chart = new CanvasJS.Chart(div.id, {
        legend: {
          horizontalAlign: "right",
          verticalAlign: "center",
        },
        title :{
          text: attrs.title
        },      
        data: data,
      });

      chart.render();

      socket.on("asserv_stream", function (msg) {
        var dps = data[0].dataPoints;

        while((dps.length > 0) && (dps[dps.length - 1].x > msg.timestamp_ms)) {
          for(var i = 0, l = data.length; i < l; i += 1) {
            data[i].dataPoints.shift();
          }
        }

        for(var i = 0, l = data.length; i < l; i += 1) {
          data[i].dataPoints.push({
            x: msg.timestamp_ms,
            y: msg[data[i].index],
          });
        }

        dps = data[0].dataPoints;
        while ((dps.length > 0) && ((msg.timestamp_ms - dps[0].x) > 5000))
        {
          for(var i = 0, l = data.length; i < l; i += 1) {
            data[i].dataPoints.shift();
          }
        }

        chart.render();
      });
    }
  }
}]);


