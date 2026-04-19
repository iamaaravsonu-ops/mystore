document.addEventListener('DOMContentLoaded', () => {
    const productGrid = document.getElementById('productGrid');
    const tabs = document.querySelectorAll('.category-tab');
    
    // Hardcoded product data to avoid CORS errors when opening locally
    const allProducts = [
        {
            "id": 101,
            "name": "Luxe Living Silver Wood-Plank Area Rug",
            "category": "carpet",
            "price": 300,
            "originalPrice": 500,
            "discount": 40,
            "image": "images/carpets/silver_carpet.jpg",
            "description": "Premium silver wood-plank design, priced per meter."
        },
        {
            "id": 1,
            "name": "Persian Heritage Red Carpet",
            "category": "carpet",
            "price": 4500,
            "originalPrice": 6000,
            "discount": 25,
            "image": "images/carpets/carpet1.jpg",
            "description": "Premium quality carpet crafted with attention to detail."
        },
        {
            "id": 2,
            "name": "Modern Geometric Area Rug",
            "category": "carpet",
            "price": 3200,
            "originalPrice": 4000,
            "discount": 20,
            "image": "images/carpets/carpet2.jpg",
            "description": "A beautiful geometric area rug that enhances any modern living room."
        },
        {
            "id": 3,
            "name": "Luxury Shaggy White Carpet",
            "category": "carpet",
            "price": 2800,
            "originalPrice": 3500,
            "discount": 20,
            "image": "images/carpets/carpet3.jpg",
            "description": "Incredibly soft luxury shaggy white carpet perfect for cozy interior spaces."
        },
        {
            "id": 103,
            "name": "Premium Velvet Blackout Door Curtain",
            "category": "curtain",
            "price": 300,
            "originalPrice": 600,
            "discount": 50,
            "image": "images/curtains/door curtain.jpg",
            "description": "100% blackout velvet curtains for absolute privacy."
        },
        {
            "id": 104,
            "name": "Sheer Linen Patterned Window Curtain",
            "category": "curtain",
            "price": 400,
            "originalPrice": 650,
            "discount": 38,
            "image": "images/curtains/brown printed curtian.jpg",
            "description": "Elegant sheer linen curtains with beautiful patterns."
        },
        {
            "id": 5,
            "name": "Premium Velvet Blackout Curtain",
            "category": "curtain",
            "price": 1800,
            "originalPrice": 2400,
            "discount": 25,
            "image": "images/curtains/curtain1.jpg",
            "description": "100% blackout velvet curtains for absolute privacy and light control."
        },
        {
            "id": 6,
            "name": "Sheer Elegant Window Drapes",
            "category": "curtain",
            "price": 1200,
            "originalPrice": 1600,
            "discount": 25,
            "image": "images/curtains/curtain2.jpg",
            "description": "Lightweight, sheer drapes that let in natural light while maintaining elegance."
        },
        {
            "id": 102,
            "name": "Natural Jute Fiber Eco-Friendly Doormat",
            "category": "doormat",
            "price": 250,
            "originalPrice": 400,
            "discount": 37,
            "image": "images/doormats/jute doormate.jpg",
            "description": "Eco-friendly natural jute fiber doormat, durable and stylish."
        },
        {
            "id": 9,
            "name": "Welcome Coir Mat",
            "category": "doormat",
            "price": 500,
            "originalPrice": 750,
            "discount": 33,
            "image": "images/doormats/doormat1.jpg",
            "description": "Sturdy welcome coir mat, perfect for removing dirt at your doorstep."
        },
        {
            "id": 10,
            "name": "Anti-Slip Rubber Doormat",
            "category": "doormat",
            "price": 450,
            "originalPrice": 600,
            "discount": 25,
            "image": "images/doormats/doormat2.jpg",
            "description": "A completely anti-slip rubber doormat that ensures safety on wet floors."
        },
        {
            "id": 105,
            "name": "Premium Jhol Cleaning Brush",
            "category": "more",
            "price": 180,
            "originalPrice": 300,
            "discount": 40,
            "image": "images/more/jhol cleanwe.jpg",
            "description": "High-quality cleaning brush for hard-to-reach areas."
        },
        {
            "id": 106,
            "name": "Heavy-Duty Durable Dust Pan",
            "category": "more",
            "price": 80,
            "originalPrice": 150,
            "discount": 46,
            "image": "images/more/dust pan.jpg",
            "description": "Extremely durable heavy-duty dust pan for all-day cleaning."
        },
        {
            "id": 13,
            "name": "Ceramic Table Vase Set",
            "category": "more",
            "price": 1400,
            "originalPrice": 2000,
            "discount": 30,
            "image": "images/more/more1.jpg",
            "description": "Elegant ceramic table vases to complement any room's decor."
        },
        {
            "id": 14,
            "name": "Decorative Wall Mirror",
            "category": "more",
            "price": 2500,
            "originalPrice": 3200,
            "discount": 21,
            "image": "images/more/more2.jpg",
            "description": "A beautifully framed decorative wall mirror to enlarge any space."
        }
    ];

    // Initial render
    renderProducts('carpet'); // Default active tab

    function renderProducts(category) {
        if(!productGrid) return;
        productGrid.innerHTML = '';
        const filtered = allProducts.filter(p => p.category === category);
        
        filtered.forEach(p => {
            const card = document.createElement('div');
            card.className = 'product-card';
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
            tabs.forEach(t => t.classList.remove('active'));
            e.target.classList.add('active');
            const category = e.target.dataset.category;
            renderProducts(category);
        });
    });
});
