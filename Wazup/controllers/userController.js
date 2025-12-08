// User controller
exports.getAllUsers = (req, res) => {
  res.status(200).json({ message: 'Liste des utilisateurs' });
};

exports.getUserById = (req, res) => {
  const { id } = req.params;
  res.status(200).json({ message: `Utilisateur ${id}` });
};

exports.updateUser = (req, res) => {
  const { id } = req.params;
  res.status(200).json({ message: `Utilisateur ${id} mis à jour` });
};
