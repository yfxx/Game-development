
var mysql = require('mysql');
var conn = mysql.createConnection({
                                  host: 'localhost',
                                  user: 'nodejs',
                                  password: 'nodejs',
                                  database:'nodejs',
                                  port: 3306
                                  });

exports.query = function(sql, function(err,rows,fields){
                         conn.connect();
                         conn.query(sql,function(_err, _rows, _fields){
                                    if(_err) throw(_err);
                                    err = _err;
                                    rows = _rows;
                                    fields = _fields;
                                    })
                         conn.end();
                         })