angular.module('fishingbot').controller('LoginController',LoginController);

function LoginController($http,$scope,$location,$window,AuthFactory){
    var vm = this;
    
    vm.isLoggedIn = function(){
        if(AuthFactory.isLoggedIn){
            return true;
        } else {
            return false;
        }
    };
    
    vm.login = function(req,res){
        if(vm.username && vm.password){
            var user = {
                username: vm.username,
                password: vm.password
            };
            $http.post('/api/users/login',user).then(function(response){
                if(response.data){                    
                    $window.sessionStorage.token = response.data.token;
                    AuthFactory.isLoggedIn = true; 
                    AuthFactory.username = user.username;
                    AuthFactory.password = user.password;
                    //AuthFactory.userdata = response.data;
                    $location.path('/bots');
                }
                console.log(response);
            }).catch(function(error){
                console.log(error);
                vm.error = "The user name or password is incorrect, try again."
            })
        }
        $scope.vm.username = '';
        $scope.vm.password = '';
    };
    
    vm.logout = function(){
        AuthFactory.isLoggedIn = false;
        delete $window.sessionStorage.token;
        $location.path('/');
    };
    
    vm.isActiveTab = function(url){
        var currentPath = $location.path().split('/')[1];
        return (url === currentPath ? 'active': '');
    }
    
    vm.finduser = function(){
        var user = {
            username: AuthFactory.username,
            password: AuthFactory.password
        }
        $http.post('/api/users/profile',user).then(function(response){
            if(response.data){                    
                console.log("Profile: "+response.data.username);
                vm.username = response.data.username;
                AuthFactory.userdata = response.data;
            }
            }).catch(function(error){
                console.log(error);
                vm.error = "User does not exists."
            })
    }
    
    vm.deleteuser = function(){
        console.log(AuthFactory.username)
        var user = {
            username: AuthFactory.username,
            password: AuthFactory.password
        }
        $http.delete('/api/users/profile',{params: {userId: user.username}}).then(function(response){
            console.log('RES'+response)
            if(response.data.success){                    
                console.log("User Deleted"); 
            }
            }).catch(function(error){
                console.log(error);
            })
    }
    
    $scope.getdata = function(){
        return AuthFactory.userdata;
    }
};