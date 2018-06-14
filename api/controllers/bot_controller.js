var dbconn = require('../data/dbconnection.js');
var ObjectId = require('mongodb').ObjectId;
var bots = require('../data/bot-data.json');
var mongoose = require('mongoose');
var Bot = mongoose.model('Bot');

module.exports.botsGetAll = function(req,res){
    
//    var db = dbconn.get();
//    var dbcollection = db.collection('hotels');
    
    var offset = 0;
    var count = 0;
    var maxCount = 15;
    var location = req.body.location;
    
    if(req.query && req.query.offset){
        offset = parseInt(req.query.offset,10);
    }
    if(req.query && req.query.count){
        count = parseInt(req.query.count,10);
    }
    
    if(isNaN(offset) || isNaN(count)){
    res
      .status(400)
      .json({
        "message" : "If supplied in querystring, count and offset must both be numbers"
      });
    return;
    }

    if(count > maxCount){
    res
      .status(400)
      .json({
        "message" : "Count limit of " + maxCount + " exceeded"
      });
    return;
    }

    Bot
    .find()
    .skip(offset)
    .limit(count)
    .exec(function(err, data) {
      console.log(err);
      console.log(data);
      if (err) {
        console.log("Error finding bots");
        res
          .status(500)
          .json(err);
      } else {
        console.log("Found bots", data.length);
        res
          .json(data);
      }
    });
};

module.exports.botsGetOne = function(req,res){
    
    var botID = req.params.botID;
    var bot = bots[botID];
    
     //console.log('GET hotelId', id);

  Bot
    .findById(botID)
    .exec(function(err, doc) {
      var response = {
        status : 200,
        message : doc
      };
      if(err) {
        console.log("Error finding bot");
        response.status = 500;
        response.message = err;
      } else if(!doc) {
        console.log("botID not found in database", id);
        response.status = 404;
        response.message = {
          "message" : "botID not found " + id
        };
      }
      res
        .status(response.status)
        .json(response.message);
    });
};

var _splitArray = function(input) {
  var output;
  if (input && input.length > 0) {
    output = input.split(";");
  } else {
    output = [];
  }
  return output;
};