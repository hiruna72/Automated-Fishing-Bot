angular.module('fishingbot',['ngRoute','ng-fusioncharts']).config(config).run(run);

function config($httpProvider,$routeProvider)
{
    $httpProvider.interceptors.push('AuthInterceptor');
    $routeProvider
    .when('/',{
        templateUrl:"angular-app/login/login2.html",
        access:{
            restricted: false
        }
    })
    .when('/bots',{
        templateUrl:"angular-app/bot-list/allBots.html",
        controller:BotsController,
        controllerAs:'vm',
        access:{
            restricted: false
        }
    })
    .when('/bot/:id',{
        templateUrl:"angular-app/bot-display/singleBot.html",
        controller:BotController,
        controllerAs:'vm',
        access:{
            restricted: false
        }
    })
    .when('/register',{
        templateUrl:"angular-app/register/register.html",
        controller:RegisterController,
        controllerAs:'vm',
        access:{
            restricted: false
        }
    })
    .when('/profile',{
        templateUrl:"angular-app/profile/profile.html",
        controller:LoginController,
        controllerAs: 'vm',
        access:{
            restricted: true
        }
    })
    .when('/contact',{
        templateUrl:"angular-app/contact/contact.html",
        access:{
            restricted: false
        }
    })
    .otherwise({
        redirectTo:'/'
    });
};

function run($rootScope,$location,$window,AuthFactory){
    $rootScope.$on('$routeChangeStart',function(event,nextRoute,currentRoute){
        if(nextRoute.access !== undefined && nextRoute.access.restricted && !$window.sessionStorage.token && !AuthFactory.isLoggedIn){
            event.preventDefault();
            $location.path('/');
        }
    })
};