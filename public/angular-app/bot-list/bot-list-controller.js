angular.module('fishingbot').controller('BotsController',BotsController);

function BotsController(botDataFactory){
    var vm = this;
    vm.title = 'Automated Fishing Bot';
    botDataFactory.botList().then(function(response){
        //console.log(response);
        vm.bots = response.data;
    });
}