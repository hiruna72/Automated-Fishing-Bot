var mongoose =  require('mongoose');

var userSchema = new mongoose.Schema({
    username:{
        type:String,
        unique:true,
        required:true
    },
    firstname:{
        type:String
    },
    lastname:{
        type:String
    },
    password:{
        type:String,
        required:true
    },
    locationID:{
        type:String,
        required:true
    }
});

mongoose.model('User',userSchema);