function initGraphes() {
	graphDistance = new graphiqueF("#distance .divgraph");
	graphAngle = new graphiqueF("#angle .divgraph");
}

var graphiqueF = function(element) {


	var container =  document.querySelector(element),
	x =[],
	brut=[],
	filtre=[],
	retour=[],
	sortie=[],
	options = {
		shadowSize:1,
		xaxis : {
		  mode : 'time'
		},
		selection : {
		  mode : 'x'
		}

	},
	tmax=10000; // nombre de ms max pour un graph

  // Data Format:
  /*this.data = [
    [x, brut],
    [x, filtre],
    [x, retour],
    [x, sortie]
  ];*/
	this.data = [brut,filtre,retour,sortie];

	this.graph = Flotr.draw(container, this.data,options);

	this.update = function(data) {
        console.log(data);
		if(data.t < tmax ) {
			//x.push(data.t);
			brut.push([data.t,data.b]);
			filtre.push([data.t,data.f]);
			retour.push([data.t,data.r]);
			sortie.push([data.t,data.s]);
		} else {
			//x=[];
			brut=[];
			filtre=[];
			retour=[];
			sortie=[];
			this.data = [brut,filtre,retour,sortie];
			tmax+=10000;
		}


//		console.log(this.data,data.t,tmax);
		Flotr.draw(container,this.data,options);
	};
};


var graphiqueE = function(element) {

	//this.element = document.querySelector(element);
	var x =[],
	brut=[],
	filtre=[],
	retour=[],
	sortie=[],
	data,options,
	tmax=10000; // nombre de ms max pour un graph

  // Data Format:
  data = [
    [x, brut],
    [x, filtre],
    [x, retour],
    [x, sortie]
  ];
        // Configuration for detail:
        var detailOptions = {
          name : 'detail',
          data : data,
          height : 150,
          // Flotr Configuration
          config : {
			grid: {
			   verticalLines : true,
			   horizontalLines : true
			}
          }
        };

        // Configuration for summary:
      var summaryOptions = {
          name : 'summary',
          data : data,
          height : 150,
          // Flotr Configuration
          config : {
            yaxis : {
              min : -1.1,
              max : 1.1
            },
			grid: {
			   verticalLines : true,
			   horizontalLines : true
			},
            // Enable selection in the x-axis
            selection : {
              mode : 'x'
            }
          }
        };

        // Building a custom vis:
        var vis = new envision.Visualization(),
        detail = new envision.Component(detailOptions),
        summary = new envision.Component(summaryOptions),
        interaction = new envision.Interaction();

        // Render Visualization
        vis.add(detail)
        .add(summary)
        .render(document.querySelector(element));

        // Wireup Interaction
        interaction
          .leader(summary)
          .follower(detail)
          .add(envision.actions.selection);

		  //this.graph = detail
	/*options = {
		container : document.querySelector(element),
		data : {
			detail : data,
			summary : data
		},
		selection :{
			data : {
				x : {
					min:0,
					max:1000
				}
			}
		}
	}*/

	//this.graph = new envision.templates.TimeSeries(options);

	this.update = function(data) {
	// data > t  , [b,f,r,s]
		if(data.t < tmax ) {
			x.push(data.t);
			brut.push(data.b);
			filtre.push(data.f);
			retour.push(data.r);
			sortie.push(data.s);
		} else {
			x=[];
			brut=[];
			filtre=[];
			retour=[];
			sortie=[];
			tmax+=10000;
		}



      detail.draw(null, {
        xaxis : {
          min : 0,
          max : data.t
        }
      });

     /* this.graph.summary.trigger('select', {
        data : {
          x : {
            min : 0 ,
            max : data.t
          }
        }
      });*/

	};
};


var graphique = function(element) {

	var palette = new Rickshaw.Color.Palette();

	this.graph = new Rickshaw.Graph({
			element: document.querySelector(element),
			renderer: 'line',
			series: [
				{	name:"brut",
					data: [ { x: 0, y: 40 }, { x: 1, y: 49 }, { x: 2, y: 30 }, { x: 3, y: 25 }],
					color: palette.color()
				},
				{	name:"filtre",
					data: [ { x: 0, y: 40 }, { x: 1, y: 45 }, { x: 2, y: 39 }, { x: 3, y: 30 }],
					color: palette.color()
				},
				{	name:"retour",
					data: [ { x: 0, y: 30 }, { x: 1, y: 31 }, { x: 2, y: 39 }, { x: 3, y: 32 }],
					color: palette.color()
				},
				{	name:"sortie",
					data: [ { x: 0, y: 35 }, { x: 1, y: 37 }, { x: 2, y: 39 }, { x: 3, y: 27 }],
					color: palette.color()
				}
			]
	});

	var _this = this;

	var xAxis = new Rickshaw.Graph.Axis.Time({
		graph: _this.graph
	});

	xAxis.render();

	this.update = function(data) {

		if(data) {
			this.graph.series.addData(data);
		}
		this.graph.render();
	}

	this.clear = function() {
		for(var i=0,t=this.graph.series.length;i<t;i++) {
			this.graph.series[i].data = [{ x: 0, y: 0 }];
		}
		this.graph.render();
	}
	this.test = function() {
		var data = {
			"brut":4,
			"filtre":4,
			"retour":4,
			"sortie":44
		}
		this.update(data);
	}

};