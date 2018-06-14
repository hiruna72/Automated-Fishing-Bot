angular.module('fishingbot').controller('RegisterController',RegisterController);

function RegisterController($http,$scope,$window){
    var vm = this;
    
    vm.register = function(){
        var user = {
            firstname: vm.firstname,
            lastname: vm.lastname,
            username: vm.username,
            password: vm.password,
            locationID: vm.locationID
        };
        
        if(!vm.firstname && !vm.lastname){
            vm.error = 'Please add a firstname or a lastname.'
            err = 'Please add a firstname or a lastname.';
        }
        else if(!vm.username || !vm.password){
            vm.error = 'Please add a username and a password'
            err = 'Please add a username and a password';
        }
        else{
            if(vm.password!==vm.passwordRepeat){
                vm.error = 'Please make sure the passwords match'
                err = 'Please make sure the passwords match';
            }
            else{       
                if(vm.password.length>=4){
                    $http({
                        method: 'POST',
                        url: '/api/users/register',
                        data: user,
                        headers: {
                            'Content-Type': "application/json"
                        }       
                    }).then(function(result){
                        console.log(result);
                        vm.message = 'Successful registration, please login';
                        vm.error = '';
                        msg = 'Successful registration, please login';
                    }).catch(function(error){
                        console.log(error.data.code);
                        if(error.data.code===11000){
                            vm.error = 'The username is already used, Try another';
                            vm.message = '';                        
                        }
                    });                    
                }
                else{
                    vm.error = "Password length should be more than 4 characters"
                }
            }    
            $scope.vm.firstname = '';
            $scope.vm.lastname = '';
            $scope.vm.username = '';
            $scope.vm.locationID = '';
            $scope.vm.password = '';
            $scope.vm.passwordRepeat = '';
        } 
    }
};