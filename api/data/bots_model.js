var mongoose = require('mongoose');

//var nodeSchema = new mongoose.Schema({
//    numberOfBots:Number,
//    relayNodeID:{
//        type:String,
//        required:true
//    },
//    performance:Number,
//    activeHours:Number,
//    businessClientID:String,
//    locationID:String
//})
//
//var botSchema = new mongoose.Schema({
//    locationClientID:nodeSchema
//});

//var triggerSchema = new mongoose.Schema({
//    TimeStamp:Number,
//    Triggers:Number
//})
//
//var botSchema = new mongoose.Schema({
//    botID:{
//        type:String,
//        required:true
//    },
//    ActiveHours:Number,
//    TriggerData:[triggerSchema]
//});


var botSchema = new mongoose.Schema({
    ClientID:{
        type:String,
        required:true
    },
    Performance:Number,
    No_bots:Number
});

mongoose.model('Bot',botSchema,'botdata');