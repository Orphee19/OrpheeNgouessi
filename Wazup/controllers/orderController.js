// Order controller
exports.getAllOrders = (req, res) => {
  res.status(200).json({ message: 'Liste des commandes' });
};

exports.createOrder = (req, res) => {
  const order = req.body;
  res.status(201).json({ message: 'Commande créée', data: order });
};

exports.getOrderById = (req, res) => {
  const { id } = req.params;
  res.status(200).json({ message: `Commande ${id}` });
};
