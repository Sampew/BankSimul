const db = require('../database');


const card_privileges = {
    getById: function(id, callback) {
      return db.query('select * from card_privileges where id_cardprivileges = ?', [id], callback);
    },


    getAll: function(callback) {
      return db.query('select * from card_privileges', callback);
    },


    add: function(card_privileges, callback) {
      return db.query(
        'insert into card_privileges (id_account, card_number) values(?,?)',
        [card_privileges.id_account, card_privileges.card_number],
        callback
      );
    },


    delete: function(id, callback) {
      return db.query('delete from card_privileges where id_cardprivileges = ?', [id], callback);
    },

    
    update: function(id, card_privileges, callback) {
      return db.query(
        'update card_privileges set id_account = ? where id_cardprivileges = ?',
        [card_privileges.id_account,card_privileges.card_number, id],
        callback
      );
    }
  };
  module.exports = card_privileges;