const generateToken = require("../utils/generateToken");

const login = async (req, res) => {
  const { email, password } = req.body;

  try {
    // Ici, vérifier avec Firebase si l’utilisateur existe
    // Supposons que tu récupères userId et role depuis Firebase
    const userId = "12345";
    const role = "client";

    // Générer le token
    const token = generateToken({ id: userId, email, role });

    res.json({
      message: "Connexion réussie",
      token,
      user: { id: userId, email, role },
    });
  } catch (error) {
    res.status(500).json({ message: "Erreur serveur", error: error.message });
  }
};
