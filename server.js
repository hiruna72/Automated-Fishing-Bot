require('./api/data/dbconnection.js');
var express = require('express');
var app = express();
var path = require('path');
var bodyParser = require('body-parser');

var routes = require('./api/routes');

app.set('port','3000');

app.use(function(req,res,next){
    console.log(req.method,req.url);
    if(req.url==='/'){
//        res.render('index');
    }
    next();
});

app.use(express.static(path.join(__dirname,'public')));
app.use('/bower_components',express.static(__dirname +'/bower_components'));
app.use('/webapp', express.static(__dirname + '/webapp'));
app.use(bodyParser.urlencoded({extended:false}));
app.use(bodyParser.json());
app.use('/api',routes);

//app.get('/api/bots', function(req, res) {
//    res.render('index');
//    
//    console.log('Check 1');
//});



var server = app.listen(app.get('port'), function(){
    var port = server.address().port;
    console.log('Web server started at port '+ port);
});

module.exports = server;