angular.module('fishingbot').controller('chartController',chartController);

function chartController($scope,$http,$interval,AuthFactory){
    var getdata;
    var location;
    console.log('Check 2');
    getData = function(url,callback) {
        $http({
            method: 'GET',
            dataType: 'json',
            url: url,
            headers:{
                "Content-Type": "application/json"
            }
        }).then(callback);
    };
    
    //var userdata = AuthFactory.userdata;
    //console.log(userdata);
    //var location = userdata.locationID;
    $scope.reload = function() {
        getData('/api/bots',function(response) {
        console.log('Check 3');
        console.log(response);
        var datasource,
            doc,
            data = [],
            linkeddata = [],
            chart;

        chart = {
            caption: "Performance",
            showValues: 0,
            theme: "zune"
        };
        console.log(response.data.length);
        console.log('Check 4');
        for (var _i = 0; _i < response.data.length; _i++) {
            doc = response.data[_i];
            data.push({
                label: doc.ClientID,
                value: doc.No_bots,
                value: doc.Performance,
                link: 'newchart-json-' + _i.toString()
            });
            console.log('Check 5');
            linkeddata.push({
                id: _i.toString(),
                linkedchart: {
                    chart: {
                        caption: 'Triggers of a Fishing bot  - '+ doc.BotID,
                        showValues: 0,
                        theme: "carbon"
                    },
//                    data: doc.TriggerData.map(function(ele) {
//                        return {
//                            label: ele.TimeStamp,
//                            value: ele.Triggers
//                        };
//                    })
                }
            });
        }

        datasource = {
            chart: chart,
            data: data,
            linkeddata: linkeddata
        };
        $scope.data = datasource;
    });
    };
    $scope.reload();
    $interval($scope.reload,20000);
};

