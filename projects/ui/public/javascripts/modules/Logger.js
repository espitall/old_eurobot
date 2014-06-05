define([],function(){

	var _this = this;
	function init(element) {

		_this.container = document.getElementById(element);
		_this.element = document.createElement("div");
		_this.container.appendChild(_this.element) ;
	}
	

	var etat = {
		debug : 0,
		info : 1,
		warning : 2,
		erreur : 3
	};
	
	var compteur = 0;
	
	
	function log(message,type) {

		var item = document.createElement("p");

		switch (type) {
			case etat.debug:
				item.className="debug";
				break;
			case etat.info:
				item.className="info";
				break;
			case etat.warning:
				item.className="warning";
				break;
			case etat.erreur:
				item.className="error";
				break;
			default:
				item.className="";
				break;
		}

		item.innerHTML = message ;
		_this.element.appendChild(item);
		
		compteur++;
		if(compteur%2){
			_this.container.scrollTop = _this.container.scrollHeight;
		}
		/*if(compteur==1500){
			compteur = 0 ;
			clear();
		}*/
	}

	function clear() {
		_this.element.innerHTML = "";
	}

	function test() {
		log("debug verbeux",0);
		log("Une info quelconque ",1);
		log("debug verbeux",0);
		log("debug verbeux",0);
		log("debug verbeux",0);
		log("debug verbeux",0);
		log("Ceci est un warning",2);
		log("Ceci est un message d'erreur",3);
	}
    function performanceTest(){
        console.time("Test de performance Logger");
        for(var i = 0;i<1000;i++){
            log("Une info quelconque ",1);
        }
        for(i = 0;i<1000;i++){
        log("debug verbeux",0);
        }
        clear();
        console.timeEnd("Test de performance Logger");
    }
    return {
		init : init ,
		log : log,
		clear : clear
    };

});

