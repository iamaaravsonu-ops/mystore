document.addEventListener('DOMContentLoaded', () => {
    // Top Bar, Header, Main App Logic.
    
    // Theme Toggle Logic
    const themeBtn = document.getElementById('themeToggle');
    if(themeBtn) {
        themeBtn.addEventListener('click', () => {
            const currentObj = document.body.getAttribute('data-theme');
            const isDark = currentObj === 'dark';
            document.body.setAttribute('data-theme', isDark ? 'light' : 'dark');
        });
    }

    // Modal closing logic (closing on overlay or X)
    const modals = document.querySelectorAll('.modal');
    modals.forEach(modal => {
        modal.addEventListener('click', (e) => {
            if (e.target === modal) {
                modal.classList.remove('active');
                document.body.style.overflow = '';
            }
        });
    });

    const closeBtns = document.querySelectorAll('.close-modal');
    closeBtns.forEach(btn => {
        btn.addEventListener('click', () => {
            modals.forEach(m => m.classList.remove('active'));
            document.body.style.overflow = '';
        });
    });

});
