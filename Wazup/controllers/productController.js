// Product controller
exports.getAllProducts = (req, res) => {
  res.status(200).json({ message: 'Tous les produits' });
};

exports.createProduct = (req, res) => {
  const product = req.body;
  res.status(201).json({ message: 'Produit créé', data: product });
};

exports.getProductById = (req, res) => {
  const { id } = req.params;
  res.status(200).json({ message: `Produit ${id}` });
};
