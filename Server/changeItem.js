
var database = require('./database');

exports.postItem = function (req, res) {
    var sql = 'INSERT INTO map (tag, xPos, yPos, z) VALUES (' + req.body.tag + ', ' + req.body.xPos + ', ' + req.body.yPos + ', ' + req.body.z + ')';
    database.query(sql, function(err, items, fields){
                   if (err)
                   res.status(400).json(err);
                   else
                   res.status(200).json(items);
                   });
};



