// Attach a global function so products.js can call it easily.
window.openProductModal = function(product) {
    const modal = document.getElementById('prodModal');
    if(!modal) return;

    // Populate data
    document.getElementById('mImg').src = product.image;
    document.getElementById('mTitle').textContent = product.name;
    document.getElementById('mPrice').textContent = `₹${product.price.toLocaleString('en-IN')}`;
    document.getElementById('mOldPrice').textContent = `₹${product.originalPrice.toLocaleString('en-IN')}`;
    document.getElementById('mDesc').textContent = product.description;

    // Set WhatsApp link correctly based on prompt requested format
    const waNumber = '919060681818';
    const message = `Hi, I'm interested in ${product.name} priced at ₹${product.price}. Please share more details.`;
    document.getElementById('mWaBtn').href = `https://wa.me/${waNumber}?text=${encodeURIComponent(message)}`;

    // Show
    modal.classList.add('active');
    document.body.style.overflow = 'hidden';
};
