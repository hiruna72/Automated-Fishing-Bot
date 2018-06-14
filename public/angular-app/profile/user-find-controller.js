angular.module('fishingbot').controller('UserFindController',UserFindController);

function UserFindController(botDataFactory){
    var vm = this;
    botDataFactory.findUser().then(function(response){
        vm.user = response;
    });
}