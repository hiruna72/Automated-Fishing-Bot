angular.module('fishingbot').controller('BotController',BotController);

function BotController(botDataFactory,$routeParams){
    var vm = this;
    var id = $routeParams.id;
    botDataFactory.botDisplay(id).then(function(response){
        vm.hotel = response;
        vm.stars = _getStarRating(response.stars);
    });
}

function _getStarRating(stars){
    return new Array(stars);
}