
var database = require('./database');

exports.postUserData = function (req, res) {
    var sql = 'INSERT INTO userData (dataType, resource) VALUES (' + req.body.dataType + ', ' + req.body.resource + ')';
    database.query(sql, function(err, res, fields){
                   if (err)
                   res.status(400).json(res);
                   else
                   res.status(200).json(items);
                   });
};



