var database = require('./database');

exports.getMap = function (req, res) {
    var sql = 'SELECT * FROM map';
    database.query(sql, function(err, items, fields){
                   if (err)
                   res.status(400).json(err);
                   else
                   res.status(200).json(items);
                   });
};

