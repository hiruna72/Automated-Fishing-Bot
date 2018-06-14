var mongoose = require('mongoose');
var User = mongoose.model('User');
var bcrypt = require('bcrypt-nodejs');
var jwt = require('jsonwebtoken');
var session = require('express-session');

module.exports.register = function(req,res){
    console.log('registering user');
    
    var username = req.body.username;
    var firstname = req.body.firstname || null;
    var lastname = req.body.lastname || null;
    var password = req.body.password;
    var locationID = req.body.locationID;
    
    User.create({
        username:username,
        firstname:firstname,
        lastname:lastname,
        locationID: locationID,
        password:bcrypt.hashSync(password,bcrypt.genSaltSync(10))
    }, function(err,user){
        if(err){            
            res.status(400).json(err);
        }else{
            console.log('user created',user);
            res.status(201).json(user);
        }
    });
};

module.exports.login = function(req,res){
    console.log('logging in user');
    var username = req.body.username;
    var password = req.body.password;
    
    User.findOne({
        username:username
    }).exec(function(err,user){
        if(err){
            console.log(err);
        }
        else{
            if(!!(user)){
                if(bcrypt.compareSync(password,user.password)){
                console.log('user found',user);
                var token = jwt.sign({username:user,username},'s3cr3t',{expiresIn:3600});
                res.status(200).json({success:true,token:token});
                }
                else{
                    res.status(401).json('Unauthorized');
                }                
            }
            else{
                res.status(401).json('Password is null');
                console.log('Password is null');
            }
        }
    });
};

module.exports.finduser = function(req,res){
    console.log('searching for user');
    
    var username = req.body.username;
    var password = req.body.password;
    
    User.findOne({
        username:username
    }).exec(function(err,user){
        if(err){
            console.log(err);
            res.status(400).json(err);
        }
        else{
            if(!!password){
                if(bcrypt.compareSync(password,user.password)){
                console.log('user found',user);
                res.status(200).json({firstname:user.firstname,lastname:user.lastname,username:user.username,locationID:user.locationID});
                }
                else{
                    res.status(401).json('No user found');
                }                
            }
            else{
                res.status(401).json('Password is null');
                console.log('Password is null');
            }
        }
    });
};

module.exports.deleteUser = function(req,res){
    var username = req.query.userId;

    User
    .findOneAndRemove({'username':username})
    .exec(function(err,user){
        if(err){
            res
            .status(404)
            .json(err);
        }
        else{
            console.log('user deleted: '+username);
            res.status(200)
            .json({'success':true});
        }
    })
}

module.exports.authenticate = function(req,res,next){
    var headerExists = req.headers.authorization;
    if(headerExists){
        var token = req.headers.authorization.split(' ')[1];
        jwt.verify(token,'s3cr3t',function(error,decoded){
            if(error){
                console.log(error);
                res.status(401).json('Unauthorized');
            }
            else{
                req.user = decoded.username;
                next();
            }
        });
    }else{
        res.status(403).json('No token provided');
    }
};





