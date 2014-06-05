
function initCommandes() {
	$("#clearlog").on("click",function(){
		logger.clear();
	});

	$("#startstop").on("click",function(){
		var $this = $(this),
		status = $this.data("status");
		if(status === "undefined" || !status) {
			socket.emit('commande', { id: 'strat',data: "start"});
			$this.data("status",true);
			//$(this).text("Pause");
			$this.removeClass("start reprendre");
			$this.addClass("pause");
			
			logger.log("Reprise stratégie",logger.etat.info);
		} else {
			socket.emit('commande', { id: "strat",data:"stop" });
			$this.data("status",false);
			//$(this).text("Reprendre");
			$this.removeClass("pause");
			$this.addClass("reprendre");			
			logger.log("Pause stratégie",logger.etat.info);
		}
		
		
	});	
	
	
	var pagemain = $("#pagemain");
	
	$("#onglets a").on("click",function() {
		var elm = $(this); 
		
		if( elm.attr("href") != pagemain.data("onglet")) {
			if(elm.attr("href") == "#carte") {
				
				$("#asservissement").hide();
				gestionAsserv(false);
				$("#carte").show();
				
			} else {
			
				$("#carte").hide();			
				$("#asservissement").show(function() {
					if(typeof(graphDistance) ==="undefined") {
						initGraphes();
					}
					gestionAsserv(true);
				});			
				
			}
				$("#onglets a").removeClass("actif");
				elm.addClass("actif");
			
		} else {
		
		}

		return false;
	});
	
	$("#reset").on("click",function(){
		socket.emit('commande', { id: 'reset' });
	});

	$("#stop").on("click",function(){
		socket.emit('commande', { id: 'test',data:true});
		logger.log("Arret d'urgence !",logger.etat.warning);
	});

	
	
	/** ASSERVISSEMENT *
	********************/
	$("#startstopasserv").on("click",function(){
		var $this = $(this),
		status = $this.data("status");
		if(status === "undefined" || !status) {
			gestionAsserv(true);
		} else {
			gestionAsserv(false);
		}
		
		
	});	
	
	function gestionAsserv(etat) {
		var $elm = $("#startstopasserv");
		if(etat) {
			socket.emit('commande', { id: 'asserv',data:true});
			$elm.data("status",true);
			$elm.removeClass("start reprendre");
			$elm.addClass("pause");		
		
		} else {
			socket.emit('commande', { id: 'asserv',data:false});
			$elm.data("status",false);
			$elm.removeClass("pause");
			$elm.addClass("reprendre");		
		}
	
	}
	
	
	$(".commandes button.appliquer").on("click",function() {
	
		socket.emit('commande',
		{ "id": 'comasserv',"data":{
			"g":$("#gain").val(),
			"vmin":$("#vmin").val(),
			"vmax":$("#vmax").val()
		}});
		

	
	});

	
	
}