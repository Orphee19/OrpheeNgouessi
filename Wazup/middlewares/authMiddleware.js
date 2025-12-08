// TODO: Implement authMiddleware.jsconst admin = require('../utils/firebase');

async function authenticateUser(req, res, next) {
  const authHeader = req.headers.authorization;
  if (!authHeader || !authHeader.startsWith('Bearer ')) {
    return res.status(401).json({ message: 'Token manquant' });
  }

  const token = authHeader.split(' ')[1];

  try {
    const decodedToken = await admin.auth().verifyIdToken(token);
    req.user = decodedToken;
    next();
  } catch (error) {
    return res.status(401).json({ message: 'Token invalide', error });
  }
}

// Middleware pour vérifier que l'utilisateur est admin
function isAdmin(req, res, next) {
  if (req.user && req.user.role === 'admin') {
    return next();
  }
  return res.status(403).json({ message: 'Accès refusé : administrateur uniquement' });
}

// Middleware pour vérifier que l'utilisateur est vendeur
function isSeller(req, res, next) {
  if (req.user && req.user.role === 'vendor') {
    return next();
  }
  return res.status(403).json({ message: 'Accès refusé : vendeur uniquement' });
}

// Middleware pour vérifier que l'utilisateur est client
function isClient(req, res, next) {
  if (req.user && req.user.role === 'client') {
    return next();
  }
  return res.status(403).json({ message: 'Accès refusé : client uniquement' });
}

module.exports = {
  authenticateUser,
  isAdmin,
  isSeller,
  isClient,
};
