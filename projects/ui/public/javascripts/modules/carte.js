var Carte = function(element) {
	var TO_RADIANS = Math.PI/180; 
	var element = document.querySelector(element);

	this.ctx = element.getContext('2d');

	this.robot1 = {
		img:document.getElementById("robot1"),
		x:20,
		y:10,
		a:90
	};
	this.robot2 = {
		img:document.getElementById("robot2"),
		x:20,
		y:10,
		a:90
	};
	this.drawRotatedImage = function(element) { 
		var image = element.img,
		x = element.x,
		y = element.y,
		angle = element.a ;
	 
		this.ctx.save(); 

		this.ctx.translate(x, y);
	 
		this.ctx.rotate(angle * TO_RADIANS);

		this.ctx.drawImage(image, -(image.width/2), -(image.height/2));

		this.ctx.restore(); 
	};
	
	this.update = function() {
		this.ctx.clearRect ( 0 , 0 , 500 , 500 );
		this.drawRotatedImage(this.robot1);
		this.drawRotatedImage(this.robot2);
	
	};
	
	
	this.drawPositions = function(robot1,robot2) {

		if(robot1) {
			this.robot1.x = robot1.x;
			this.robot1.y = robot1.y;
			this.robot1.a = robot1.a;
		}
		if(robot2) {
			this.robot2.x = robot2.x;
			this.robot2.y = robot2.y;
			this.robot2.a = robot2.a;
		}

		this.update();
	
	};




}


