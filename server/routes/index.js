const temperatureController = require('../controllers').temperature;

module.exports = (app) => {
  app.get('/api', (req, res) => res.status(200).send({
    message: 'Welcome to the AMG Performance Media Service!',
  }));

  app.post('/api/temperature', temperatureController.create);
  app.get('/api/temperature', temperatureController.retrieve);

  // For any other request method on a custom query or queries, return "Method Not Allowed"
  app.all('/api/temperature', (req, res) =>
    res.status(405).send({ message: 'Method Not Allowed' })
  );
  app.all('/api/temperature/:temperatureId', (req, res) =>
    res.status(405).send({ message: 'Method Not Allowed' })
  );
};