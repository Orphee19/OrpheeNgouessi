// Rental controller
exports.getAllRentals = (req, res) => {
  res.status(200).json({ message: 'Tous les véhicules à louer' });
};

exports.createRental = (req, res) => {
  const rental = req.body;
  res.status(201).json({ message: 'Véhicule ajouté à la location', data: rental });
};

exports.reserveRental = (req, res) => {
  const { id } = req.params;
  res.status(200).json({ message: `Véhicule ${id} réservé` });
};
