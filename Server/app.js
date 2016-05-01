var express = require('express');

var synMapController = require('./synMap');
var updateMapController = require('./updateMap');
var changeItemController = require('./changeItem');
var userDataController = require('./userData');

var port = process.env.PORT || 1337;
var app = express();
var router = express.Router();

//interface for synchronize the map
router.route('/synchronizeMap')
    .get(synchronizeMap.getMap);

//interface for update the map
router.route('/updateMap')
    .get(updateMapController.getMap);

//interface for change item
router.route('/changeItem')
    .post(changeItemController.postItem);

//post user data
router.route('/userData')
    .post(userDataController.postUserData);

app.use('/api', router);

app.listen(port);
