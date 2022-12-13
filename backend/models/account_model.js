const db = require('../database');

const account = {
    getById: function(id, callback) {
      return db.query('select * from account where id_account = ?', [id], callback);
    },
    getAll: function(callback) {
      return db.query('select * from account', callback);
    },


    add: function(account, callback) {
      return db.query(
        'insert into account (account_number, credit_limit, debit_balance, used_credit) values(?,?,?,?)',
        [account.account_number, account.credit_limit, account.debit_balance, account.used_credit],
        callback
      );
    },


    delete: function(id, callback) {
      return db.query('delete from account where id_account = ?', [id], callback);
    },

    
    update: function(id, account, callback) {
        return db.query('update account set credit_limit = ?,debit_balance = ?,used_credit = ? where id_account = ?',
        [account.credit_limit,account.debit_balance, account.used_credit,id], callback);
    }

  };
  module.exports = account;