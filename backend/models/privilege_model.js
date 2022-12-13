const db = require('../database');


const privileges = {
    getById: function(id, callback) {
      return db.query('select * from privileges where id_privileges = ?', [id], callback);
    },


    getAll: function(callback) {
      return db.query('select * from privileges', callback);
    },


    add: function(privileges, callback) {
      return db.query(
        'insert into privileges (id_customer, id_account, account_owner) values(?,?,?)',
        [privileges.id_customer, privileges.id_account, privileges.account_owner],
        callback
      );
    },


    delete: function(id, callback) {
      return db.query('delete from privileges where id_privileges = ?', [id], callback);
    },

    
    update: function(id, privileges, callback) {
      return db.query(
        'update privileges set account_owner = ? where id_privileges = ?',
        [privileges.account_owner, id],
        callback
      );
    }
  };
  module.exports = privileges;