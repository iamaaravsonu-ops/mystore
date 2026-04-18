document.addEventListener('DOMContentLoaded', () => {
    const productGrid = document.getElementById('productGrid');
    const tabs = document.querySelectorAll('.category-tab');
    let allProducts = [];

    // Fetch JSON data
    fetch('data/products.json')
        .then(response => response.json())
        .then(data => {
            allProducts = data;
            renderProducts('carpet'); // Default active tab
        })
        .catch(err => {
            console.error('Error fetching products:', err);
            if(productGrid) {
                productGrid.innerHTML = `<p style="grid-column: 1/-1; text-align: center;">Error loading products. Ensure you are running a local server to fetch JSON.</p>`;
            }
        });

    function renderProducts(category) {
        if(!productGrid) return;
        productGrid.innerHTML = '';
        const filtered = allProducts.filter(p => p.category === category);
        
        filtered.forEach(p => {
            const card = document.createElement('div');
            card.className = 'product-card';
            // Custom data attribute to pull all data quickly inside modal.js
            card.setAttribute('data-id', p.id);
            
            card.innerHTML = `
                <div class="discount-badge">${p.discount}% OFF</div>
                <div class="img-wrapper">
                    <img src="${p.image}" alt="${p.name}" class="product-img" loading="lazy">
                    <button class="quick-add">Quick View</button>
                </div>
                <div class="product-info">
                    <div class="product-name">${p.name}</div>
                    <div class="product-pricing">
                        <span class="price-current">₹${p.price.toLocaleString('en-IN')}</span>
                        <span class="price-old">₹${p.originalPrice.toLocaleString('en-IN')}</span>
                    </div>
                </div>
            `;

            // Setup click listener for the specific modal function
            card.addEventListener('click', () => {
                if(typeof window.openProductModal === 'function') {
                    window.openProductModal(p);
                }
            });

            productGrid.appendChild(card);
        });
    }

    // Category Tabs Logic
    tabs.forEach(tab => {
        tab.addEventListener('click', (e) => {
            // Remove active classes
            tabs.forEach(t => t.classList.remove('active'));
            // Add to clicked
            e.target.classList.add('active');
            
            // Render new
            const category = e.target.dataset.category;
            renderProducts(category);
        });
    });
});
