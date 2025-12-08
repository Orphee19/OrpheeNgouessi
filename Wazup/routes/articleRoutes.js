// articleRoutes.js content placeholder
const express = require('express');
const router = express.Router();
const articlesController = require('../controllers/articlesController');

router.get('/', articlesController.getAllArticles);
router.get('/:id', articlesController.getArticleById);
router.post('/', articlesController.createArticle);
router.put('/:id', articlesController.updateArticle);
router.delete('/:id', articlesController.deleteArticle);

module.exports = router;
