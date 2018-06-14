angular.module('fishingbot').factory('AuthFactory',AuthFactory);

function AuthFactory(){
    return{
        auth: auth
    };
    
    var auth = {
        isLoggedIn: false,
        username: '',
        password: '',
        userdata: ''
    };
}