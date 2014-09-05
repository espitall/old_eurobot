plasteamUI.directive("asserv",["socket",function(socket, Graph) {
  return {
    restrict:"EA",
    templateUrl:"templates/asserv.html",
    "controller" : function($scope, $attrs){

      $scope.commands = {
        dist: {
          vmax: undefined,
          amax: undefined,
          kp:   undefined,
          kd:   undefined,
          shift: undefined,
        },
        angu: {
          vmax: undefined,
          amax: undefined,
          kp:   undefined,
          kd:   undefined,
          shift: undefined,
        },
      };
      $scope.board = $attrs.board;
      $scope.chart_enable = false;

      $scope.read_d_params = function(argument) {
        socket.emit("command",{payload: "asserv", func: "read_d_params", destination: $attrs.board});
      }

      $scope.write_d_params = function(argument) {
        socket.emit("command",{payload: "asserv", func: "write_d_params", destination: $attrs.board, args: $scope.commands.dist});
        $scope.read_d_params(null);
      }

      $scope.read_a_params = function(argument) {
        socket.emit("command",{payload: "asserv", func: "read_a_params", destination: $attrs.board});
      }

      $scope.write_a_params = function(argument) {
        socket.emit("command",{payload: "asserv", func: "write_a_params", destination: $attrs.board, args: $scope.commands.angu});
        $scope.read_a_params(null);
      }

      $scope.read_d_params(null);
      $scope.read_a_params(null);

      socket.on("asserv_params", function(data) {
        if($attrs.board == data.source) {
          $scope.commands[data.type].vmax = data.vmax;
          $scope.commands[data.type].amax = data.amax;
          $scope.commands[data.type].kp = data.kp;
          $scope.commands[data.type].kd = data.kd;
          $scope.commands[data.type].shift = data.shift;
        }
      });
    },
  };
}]);

