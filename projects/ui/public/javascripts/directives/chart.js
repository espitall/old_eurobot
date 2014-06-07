plasteamUI.directive("chart",["socket", function(socket, Graph) {
  var uniqueId = 1;
  var baseHeight = 300;

  return {
    restrict: "EA",
    template: "<div></div>",
    link : function(scope, elm, attrs) {
      var div  = elm.find('div')[0];
      div.id = 'chart_' + (uniqueId++);

      var data = JSON.parse(attrs.series);
      var series = attrs.series.split(";");
      for(var i in data) {
        data[i].dataPoints = [];
        data[i].showInLegend = true;
        data[i].markerType = 'none';
      }

      var chart = new CanvasJS.Chart(div.id, {
        legend: {
          horizontalAlign: "right",
          verticalAlign: "center",
        },
        title :{
          text: attrs.title,
          fontSize: 15,
        },      
        toolTip: {
          shared: true,
        },
        axisX: JSON.parse(attrs.axisx || '{}'),
        axisY: JSON.parse(attrs.axisy || '{}'),
        axisY2: JSON.parse(attrs.axisy2 || '{}'),
        data: data,
        height: baseHeight,
      });

      chart.render();
      div.style.height = baseHeight + 'px';

      console.log(chart.width);

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
            y: msg[data[i].source],
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


