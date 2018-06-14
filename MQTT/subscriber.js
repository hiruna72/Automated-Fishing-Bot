var mqtt = require('mqtt'); //includes mqtt server 
var mongodb = require('mongodb'); // includes mongoDB 
var mongodbClient = mongodb.MongoClient; //initialises the mongoDB client
var mongodbURI = 'mongodb://localhost:27017'; //activating the MongoDB port 27017
var topic = "payload"; // MQTT topic
var collection,clientMQTT; //initialise collection and client

mongodbClient.connect(mongodbURI, (err,client) => { //connect the database with collecion
   //if(err) throw err;
   var db=client.db('BotDB')
   collection=db.collection('botdata'); //name of the collection in the database
   clientMQTT=mqtt.connect({ host: 'localhost', port: 1883 }); //connecting the mqtt server with the MongoDB database
   clientMQTT.subscribe(topic); //subscribing to the topic name 
   clientMQTT.on('message', insertEvent); //inserting the event
   console.log("H1");
}); 
     
//function that displays the data in the MongoDataBase
function insertEvent(topic,message) 
{
   console.log("H2");
   console.log(`Received message: '${message}'`);
   var obj = JSON.parse(message);
   var jsonObject = {"ClientID":obj.ClientID,"No_bots":obj.No_bots,"Performance":obj.Performance};
   collection.insertOne(jsonObject,function(err,res){
      if(err) 
         console.log("Databse Insertion Failed");
      console.log("Record Inserted");
   });
}