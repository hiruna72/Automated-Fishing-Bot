//Using mongoose

var mongoose = require('mongoose');
var dburl = 'mongodb://localhost:27017/BotDB';

mongoose.connect(dburl);

mongoose.connection.on('connected',function(){
    console.log('Mongoose connected to: ',dburl);
});

mongoose.connection.on('disconnected',function(){
    console.log('Mongoose disconnected');
});

mongoose.connection.on('error',function(err){
    console.log('Mongoose connection error: ',err);
});


// Bringing in schemas and models
require('./bots_model.js');
require('./users_model.js');


//Using Mongodb native drivers

/*var MongoClient = require('mongodb').MongoClient;
var dburl = 'mongodb://localhost:27017/MEANdb';

var dbConnection = null;

var open = function(){
    MongoClient.connect(dburl,function(err,client){
        if(err){
            console.log('dbconnection failed');
            return;
        }
         var db = client.db('MEANdb');
        dbConnection = db;
        console.log('Open db connection',db);
    });    
};

var get = function(){
    return dbConnection;
}

module.exports = {
    open,
    get
};*/