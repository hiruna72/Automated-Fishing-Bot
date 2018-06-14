angular.module('fishingbot').factory('userDataFactory',userDataFactory);

function userDataFactory($http){    
    
    return{
        findUser:findUser,        
    };
    
    function findUser(){
        return $http.get('/api/profile').then(complete).catch(failed);
    }
    
    function complete(response){
        console.log(response);
        return response;
    }
    
    function failed(error){
        console.log(error.stateText);
    }
}