const db = require('../database');


const transactions = {
    getById: function(id, callback) {
      return db.query('select * from transactions where id_transactions = ?', [id], callback);
    },


    getAll: function(callback) {
      return db.query('select * from transactions', callback);
    },


    add: function(transactions, callback) {
      return db.query(
        'insert into transactions (card_number, date, description, sum) values(?,?,?,?)',
        [transactions.card_number, transactions.date, transactions.description, transactions.sum],
        callback
      );
    },


    delete: function(id, callback) {
      return db.query('delete from transactions where id_transactions = ?', [id], callback);
    },

    
    update: function(id, transactions, callback) {
      return db.query(
        'update transactions set description = ? where id_transactions = ?',
        [transactions.description, id],
        callback
      );
    }
  };
  module.exports = transactions;