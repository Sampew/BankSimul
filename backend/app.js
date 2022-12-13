var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');
const jwt = require('jsonwebtoken');


var cardsRouter = require('./routes/cards');
var loginRouter = require('./routes/login');
var accountsRouter = require('./routes/accounts');
var customersRouter = require('./routes/customers');
var transactionsRouter = require('./routes/transactions');
var privilegesRouter = require('./routes/privileges');
var card_privilegesRouter = require('./routes/card_privileges');
var app = express();


app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));


//NOT PROTECTED
app.use('/login', loginRouter);
app.use(authenticateToken);
//PROTECTED
app.use('/cards', cardsRouter);
app.use('/accounts', accountsRouter);
app.use('/customers', customersRouter);
app.use('/transactions', transactionsRouter);
app.use('/privileges', privilegesRouter);
app.use('/card_privileges', card_privilegesRouter);





function authenticateToken(req, res, next) {
  const authHeader = req.headers['authorization']
  const userToken = authHeader && authHeader.split(' ')[1]
  console.log("token = "+userToken);
  //if (userToken == null) return res.sendStatus(401)
  jwt.verify(userToken, process.env.MY_TOKEN, (err, user) => {
    //console.log(err)
    //if (err) return res.sendStatus(403)
    req.user = user
    next()
  })
}
module.exports = app;