const db = require('../database');
const bcrypt = require("bcryptjs");
const dotenv = require('dotenv');
const jwt = require('jsonwebtoken');

const saltRounds = 10;
let tries = 3;


const card = {

    resetTries: function (callback) {
      tries = 3,callback;
    },
    countTries: function(card_number) {
      tries = tries -1;
      console.log("Tries left: "+tries);
      if (tries == 0) {
        console.log("failed 3 tries, locking card.");
        tries = 3;
        db.query("select card_owner from card where card_number = ?",[card_number],function(err,dbResult){
          return db.query("update card set card_owner = 'LOCKED' where card_number = ?",[card_number]);
        });
      }
    },

    checkAdminLogin: function(card_number,pin_code) {
      if (card_number == "admin") {
        if (pin_code == "root") {
          console.log("ADMIN LOGIN SUCCESS\n");
          const adminToken = generateAdminAccessToken({admin: card_number});
          return "ADMIN TOKEN:\n" + adminToken;
        } else {
          console.log("wrong pincode for admin login");
          return false;
        }
    } else {
      console.log("card does not exists");
      return false;
    }
  },

    checkCard: function(card_number,callback){
      return db.query("select card_owner from card where card_number = ?",[card_number],callback);
    },
    
    checkPin: function(cardNumber,checkCard) {
      return db.query('SELECT pin_code FROM card WHERE card_number = ?',[cardNumber],checkCard); 
    },

    getById: function(id, callback) {
      return db.query('select * from card where card_number = ?', [id], callback);
    },

    getAll: function(callback) {
      return db.query('select * from card', callback);
    },

    add: function(card, callback) {
      bcrypt.hash(card.pin_code,saltRounds,function(err,hash){
      return db.query(
        'insert into card (card_number, pin_code, debit_credit, id_account, card_owner) values(?,?,?,?,?)',
        [card.card_number, hash, card.debit_credit, card.id_account, card.card_owner],
        callback)
      });
    },

    delete: function(id, callback) {
      return db.query('delete from card where card_number = ?', [id], callback);
    },
    
    update: function(id,card,callback) {
      bcrypt.hash(card.pin_code,saltRounds,function(err, hashed) {
        return db.query("update card set card_number = ?, pin_code = ?,card_owner = ? where card_number = ?",[card.card_number,hashed,card.card_owner,id],callback)
        })
    },
}

function generateAdminAccessToken(admin) {
    dotenv.config();
    return jwt.sign(admin, process.env.MY_TOKEN, { expiresIn: '24h' });
}

module.exports = card;