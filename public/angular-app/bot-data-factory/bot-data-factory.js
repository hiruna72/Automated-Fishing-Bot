angular.module('fishingbot').factory('botDataFactory',botDataFactory);

function botDataFactory($http){    
    
    return{
        botList:botList,
        botDisplay:botDisplay,
        findUser:findUser
        
    };
    
    function botList(){
        return $http.get('/api/bots').then(complete).catch(failed);
    }
    
    function botDisplay(id){
        return $http.get('/api/bots/'+id).then(complete).catch(failed);
    }
    
    function findUser(){
        return $http.post('/api/users/profile').then(complete).catch(failed);
    }
    
    function complete(response){
        //console.log(response);
        return response;
    }
    
    function failed(error){
        console.log(error.stateText);
    }
}