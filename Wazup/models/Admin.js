// TODO: Implement Admin.jsconst db = require('../firebase');

const Admin = {
  create: async (adminData) => {
    return await db.collection('admins').add(adminData);
  },
  getAll: async () => {
    const snapshot = await db.collection('admins').get();
    return snapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));
  },
  update: async (id, data) => {
    return await db.collection('admins').doc(id).update(data);
  },
  delete: async (id) => {
    return await db.collection('admins').doc(id).delete();
  }
};

module.exports = Admin;
