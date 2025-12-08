exports.getAllDeliveries = (req, res) => {
  res.status(200).json({ message: 'Liste des livraisons' });
};

exports.createDelivery = (req, res) => {
  const delivery = req.body;
  res.status(201).json({ message: 'Livraison créée', data: delivery });
};
