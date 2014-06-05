
logger.init("log");
initCommandes() ;



var plateau = new Carte("#carte canvas");
var socket = io.connect('http://localhost');


socket.on('connect', function () {
	logger.log("Serveur connecté",logger.etat.notice);
	
});
socket.on('disconnect', function () {
	logger.log("Serveur deconnecté",logger.etat.erreur);
});


socket.on('asserv',function(message){
	if(message.src === "D") {
		graphDistance.update(message.data);
	} else {
		graphAngle.update(message.data);
	}
});





socket.on('test',function(data){
	console.log("test>>>",data);
});




socket.on('log',function(data){
	logger.log("["+data.src+"] "+data.msg,data.lvl);
});





socket.on('position',function(data){
	console.log("position>>>",data);
	plateau.drawPositions(data.robot1,data.robot2);
});






