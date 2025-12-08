// Store controller
exports.getAllStores = (req, res) => {
  res.status(200).json({ message: 'Liste des boutiques' });
};

exports.createStore = (req, res) => {
  const store = req.body;
  res.status(201).json({ message: 'Boutique créée', data: store });
};

exports.getStoreById = (req, res) => {
  const { id } = req.params;
  res.status(200).json({ message: `Boutique ${id}` });
};
