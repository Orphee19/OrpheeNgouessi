// articleController.js content placeholder
// Contrôleur des articles
exports.getAllArticles = (req, res) => {
  res.status(200).json({ message: 'Liste de tous les articles' });
};

exports.createArticle = (req, res) => {
  const articleData = req.body;
  res.status(201).json({ message: 'Article créé', data: articleData });
};

exports.getArticleById = (req, res) => {
  const { id } = req.params;
  res.status(200).json({ message: `Détails de l'article ${id}` });
};

exports.updateArticle = (req, res) => {
  const { id } = req.params;
  res.status(200).json({ message: `Article ${id} mis à jour` });
};

exports.deleteArticle = (req, res) => {
  const { id } = req.params;
  res.status(200).json({ message: `Article ${id} supprimé` });
};
