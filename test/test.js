'use strict';

const chai = require('chai');
const expect = require('chai').expect;

chai.use(require('chai-http'));

const app = require('../server.js');

describe('API endpoints', function() {
  this.timeout(5000); // How long to wait for a response (ms)

  before(function() {

  });

  after(function() {

  });

  // GET - List all bots
  it('should return all bots', function() {
    return chai.request(app)
      .get('/api/bots')
      .then(function(res) {
        expect(res).to.have.status(200);
        expect(res).to.be.json;
        expect(res.body).to.be.an('array');
      });
  });

  // GET - Invalid path
  it('should return Not Found', function() {
    return chai.request(app)
      .get('/api/INVALID_PATH')
      .then(function(res) {
       // console.log(res);
        //throw new Error('Path exists!');
        expect(res).to.have.status(404);
      })
      .catch(function(err) {
        //console.log(err);
        expect(err).to.have.status(404);
      });
  });

  // POST - Add new user
  it('should add new user', function() {
      var user = {
        firstname:'TEST',
        lastname:'TEST',
        username:'TEST',
        password:'TEST'}
    return chai.request(app)
      .post('/api/users/register')
      .send(user)
      .then(function(res) {
        expect(res).to.have.status(201);
        expect(res).to.be.json;
        expect(res.body).to.be.an('object');
        expect(res.body).to.be.an('object').that.includes({firstname:'TEST'});
      });
  });

  // POST - Bad Request
  it('should return Bad Request', function() {
       var user = {
        firstname:'TEST',
        lastname:'TEST',
        username:'TEST',
        password:'TEST'}
    return chai.request(app)
      .post('/api/users/register')
      .type('form')
      .send(user)
      .then(function(res) {
        //throw new Error('Invalid content type!');
        expect(res).to.have.status(400);
      })
      .catch(function(err) {
        console.log('ERROR: '+err.status);
        expect(err).to.have.status(400);
      });
  });
    
    // POST - Try succesful login
    it('should login', function() {
        var data = {
            username:'TEST',
            password:'TEST'  
        }
    return chai.request(app)
      .post('/api/users/login')
      .send(data)
      .then(function(res) {
        expect(res).to.have.status(200)
      })
      .catch(function(err) {
        expect(err).to.have.status(400);
      });
  });
});