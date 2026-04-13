document.addEventListener('DOMContentLoaded', () => {
    let curSlide = 0;
    const slides = document.querySelectorAll('.slide');
    const dots = document.querySelectorAll('.dot');
    const track = document.getElementById('track');
    if(!track || slides.length === 0) return;
    
    const slideCount = slides.length;

    function updateSlider() {
        track.style.transform = `translateX(-${curSlide * 100}%)`;
        slides.forEach((s, i) => s.classList.toggle('active', i === curSlide));
        dots.forEach((d, i) => d.classList.toggle('active', i === curSlide));
    }

    document.getElementById('prevSlide')?.addEventListener('click', () => {
        curSlide = (curSlide - 1 + slideCount) % slideCount;
        updateSlider();
        resetInterval();
    });

    document.getElementById('nextSlide')?.addEventListener('click', () => {
        curSlide = (curSlide + 1) % slideCount;
        updateSlider();
        resetInterval();
    });

    dots.forEach(d => d.addEventListener('click', (e) => {
        curSlide = parseInt(e.target.dataset.index);
        updateSlider();
        resetInterval();
    }));

    // Touch swipe support
    let startX = 0;
    const heroSection = document.getElementById('hero');
    if(heroSection) {
        heroSection.addEventListener('touchstart', e => startX = e.changedTouches[0].screenX, {passive: true});
        heroSection.addEventListener('touchend', e => {
            let endX = e.changedTouches[0].screenX;
            if (startX - endX > 50) { 
                curSlide = (curSlide + 1) % slideCount; updateSlider(); resetInterval(); 
            } else if (endX - startX > 50) { 
                curSlide = (curSlide - 1 + slideCount) % slideCount; updateSlider(); resetInterval(); 
            }
        }, {passive: true});
    }

    let autoSlide = setInterval(() => { curSlide = (curSlide + 1) % slideCount; updateSlider(); }, 5000);
    function resetInterval() {
        clearInterval(autoSlide);
        autoSlide = setInterval(() => { curSlide = (curSlide + 1) % slideCount; updateSlider(); }, 5000);
    }
});
