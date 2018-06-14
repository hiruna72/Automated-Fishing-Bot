var express = require('express');
var router = express.Router();

var botcntrl = require('../controllers/bot_controller.js');
var userctrl = require('../controllers/user_controller.js');

// Bot routes
router
    .route('/bots')
    .get(botcntrl.botsGetAll);

router
    .route('/bots/:botID')
    .get(botcntrl.botsGetOne);

// Authentication routes
router
    .route('/users/register')
    .post(userctrl.register);

router
    .route('/users/login')
    .post(userctrl.login);

router
    .route('/users/profile')
    .post(userctrl.finduser)
    .delete(userctrl.deleteUser);

module.exports = router;
